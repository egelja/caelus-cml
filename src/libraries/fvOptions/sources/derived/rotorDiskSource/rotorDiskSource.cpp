/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
Copyright (C) 2016 Applied CCM
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "rotorDiskSource.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "mathematicalConstants.hpp"
#include "trimModel.hpp"
#include "unitConversion.hpp"
#include "fvMatrices.hpp"
#include "geometricOneField.hpp"
#include "syncTools.hpp"
#include "DataEntry.hpp"

using namespace CML::constant;

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

namespace CML
{
    namespace fv
    {
        defineTypeNameAndDebug(rotorDiskSource, 0);
        addToRunTimeSelectionTable(option, rotorDiskSource, dictionary);
    }

    template<> const char* NamedEnum<fv::rotorDiskSource::geometryModeType, 2>::
        names[] =
    {
        "auto",
        "specified"
    };

    const NamedEnum<fv::rotorDiskSource::geometryModeType, 2>
        fv::rotorDiskSource::geometryModeTypeNames_;

    template<> const char* NamedEnum<fv::rotorDiskSource::inletFlowType, 3>::
        names[] =
    {
        "fixed",
        "surfaceNormal",
        "local"
    };

    const NamedEnum<fv::rotorDiskSource::inletFlowType, 3>
        fv::rotorDiskSource::inletFlowTypeNames_;
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void CML::fv::rotorDiskSource::checkData()
{
    // Set inflow type
    switch (selectionMode())
    {
        case smCellSet:
        case smCellZone:
        case smAll:
        {
            // Set the profile ID for each blade section
            profiles_.connectBlades(blade_.profileName(), blade_.profileID());
            switch (inletFlow_)
            {
                case ifFixed:
                {
                    coeffs_.lookup("inletVelocity") >> inletVelocity_;
                    break;
                }
                case ifSurfaceNormal:
                {
                    scalar UIn
                    (
                        readScalar(coeffs_.lookup("inletNormalVelocity"))
                    );
                    inletVelocity_ = -coordSys_.R().e3()*UIn;
                    break;
                }
                case ifLocal:
                {
                    // Do nothing
                    break;
                }
                default:
                {
                    FatalErrorInFunction
                        << "Unknown inlet velocity type" << abort(FatalError);
                }
            }


            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Source cannot be used with '"
                << selectionModeTypeNames_[selectionMode()]
                << "' mode.  Please use one of: " << nl
                << selectionModeTypeNames_[smCellSet] << nl
                << selectionModeTypeNames_[smCellZone] << nl
                << selectionModeTypeNames_[smAll]
                << exit(FatalError);
        }
    }
}


void CML::fv::rotorDiskSource::setFaceArea(vector& axis, const bool correct)
{
    area_ = 0.0;

    static const scalar tol = 0.8;

    const label nInternalFaces = mesh_.nInternalFaces();
    const polyBoundaryMesh& pbm = mesh_.boundaryMesh();
    const vectorField& Sf = mesh_.Sf();
    const scalarField& magSf = mesh_.magSf();

    vector n = Zero;

    // Calculate cell addressing for selected cells
    labelList cellAddr(mesh_.nCells(), -1);
    UIndirectList<label>(cellAddr, cells_) = identity(cells_.size());
    labelList nbrFaceCellAddr(mesh_.nFaces() - nInternalFaces, -1);
    forAll(pbm, patchi)
    {
        const polyPatch& pp = pbm[patchi];

        if (pp.coupled())
        {
            forAll(pp, i)
            {
                label facei = pp.start() + i;
                label nbrFacei = facei - nInternalFaces;
                label own = mesh_.faceOwner()[facei];
                nbrFaceCellAddr[nbrFacei] = cellAddr[own];
            }
        }
    }

    // Correct for parallel running
    syncTools::swapBoundaryFaceList(mesh_, nbrFaceCellAddr);

    // Add internal field contributions
    for (label facei = 0; facei < nInternalFaces; facei++)
    {
        const label own = cellAddr[mesh_.faceOwner()[facei]];
        const label nbr = cellAddr[mesh_.faceNeighbour()[facei]];

        if ((own != -1) && (nbr == -1))
        {
            vector nf = Sf[facei]/magSf[facei];

            if ((nf & axis) > tol)
            {
                area_[own] += magSf[facei];
                n += Sf[facei];
            }
        }
        else if ((own == -1) && (nbr != -1))
        {
            vector nf = Sf[facei]/magSf[facei];

            if ((-nf & axis) > tol)
            {
                area_[nbr] += magSf[facei];
                n -= Sf[facei];
            }
        }
    }


    // Add boundary contributions
    forAll(pbm, patchi)
    {
        const polyPatch& pp = pbm[patchi];
        const vectorField& Sfp = mesh_.Sf().boundaryField()[patchi];
        const scalarField& magSfp = mesh_.magSf().boundaryField()[patchi];

        if (pp.coupled())
        {
            forAll(pp, j)
            {
                const label facei = pp.start() + j;
                const label own = cellAddr[mesh_.faceOwner()[facei]];
                const label nbr = nbrFaceCellAddr[facei - nInternalFaces];
                const vector nf = Sfp[j]/magSfp[j];

                if ((own != -1) && (nbr == -1) && ((nf & axis) > tol))
                {
                    area_[own] += magSfp[j];
                    n += Sfp[j];
                }
            }
        }
        else
        {
            forAll(pp, j)
            {
                const label facei = pp.start() + j;
                const label own = cellAddr[mesh_.faceOwner()[facei]];
                const vector nf = Sfp[j]/magSfp[j];

                if ((own != -1) && ((nf & axis) > tol))
                {
                    area_[own] += magSfp[j];
                    n += Sfp[j];
                }
            }
        }
    }

    if (correct)
    {
        reduce(n, sumOp<vector>());
        axis = n/mag(n);
    }

    if (debug)
    {
        volScalarField area
        (
            IOobject
            (
                name_ + ":area",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("0", dimArea, 0)
        );
        UIndirectList<scalar>(area.internalField(), cells_) = area_;

        Info<< type() << ": " << name_ << " writing field " << area.name()
            << endl;

        area.write();
    }
}


void CML::fv::rotorDiskSource::createCoordinateSystem()
{
    // Construct the local rotor co-ordinate system
    vector origin(Zero);
    vector axis(Zero);
    vector refDir(Zero);

    geometryModeType gm =
        geometryModeTypeNames_.read(coeffs_.lookup("geometryMode"));

    switch (gm)
    {
        case gmAuto:
        {
            // Determine rotation origin (cell volume weighted)
            scalar sumV = 0.0;
            const scalarField& V = mesh_.V();
            const vectorField& C = mesh_.C();
            forAll(cells_, i)
            {
                const label celli = cells_[i];
                sumV += V[celli];
                origin += V[celli]*C[celli];
            }
            reduce(origin, sumOp<vector>());
            reduce(sumV, sumOp<scalar>());
            origin /= sumV;

            // Determine first radial vector
            vector dx1(Zero);
            scalar magR = -GREAT;
            forAll(cells_, i)
            {
                const label celli = cells_[i];
                vector test = C[celli] - origin;
                if (mag(test) > magR)
                {
                    dx1 = test;
                    magR = mag(test);
                }
            }
            reduce(dx1, maxMagSqrOp<vector>());
            magR = mag(dx1);

            // Determine second radial vector and cross to determine axis
            forAll(cells_, i)
            {
                const label celli = cells_[i];
                vector dx2 = C[celli] - origin;
                if (mag(dx2) > 0.5*magR)
                {
                    axis = dx1 ^ dx2;
                    if (mag(axis) > SMALL)
                    {
                        break;
                    }
                }
            }
            reduce(axis, maxMagSqrOp<vector>());
            axis /= mag(axis);

            // Correct the axis direction using a point above the rotor
            {
                vector pointAbove(coeffs_.lookup("pointAbove"));
                vector dir = pointAbove - origin;
                dir /= mag(dir);
                if ((dir & axis) < 0)
                {
                    axis *= -1.0;
                }
            }

            coeffs_.lookup("refDirection") >> refDir;

            // Set the face areas and apply correction to calculated axis
            // e.g. if cellZone is more than a single layer in thickness
            setFaceArea(axis, true);

            break;
        }
        case gmSpecified:
        {
            coeffs_.lookup("origin") >> origin;
            coeffs_.lookup("axis") >> axis;
            coeffs_.lookup("refDirection") >> refDir;

            setFaceArea(axis, false);

            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown geometryMode " << geometryModeTypeNames_[gm]
                << ". Available geometry modes include "
                << geometryModeTypeNames_ << exit(FatalError);
        }
    }

    coordSys_ = cylindricalCS("rotorCoordSys", origin, axis, refDir, false);

    // Define global xyz vectors
    const vector xGlobal = vector (1, 0, 0);
    const vector yGlobal = vector (0, 1, 0);
    const vector zGlobal = vector (0, 0, 1);

    // Alternative way of calculating pitch and bank angles
    bankAng_  = atan2(coordSys_.R().e3().y(), coordSys_.R().e3().z());
    pitchAng_ = atan2(coordSys_.R().e3().x(), coordSys_.R().e3().z());

    // Tensor for transforming from Cartesian into Pitch/Bank Plane
    scalar cp = cos(pitchAng_);
    scalar sp = sin(pitchAng_);
    scalar cb = cos(bankAng_);
    scalar sb = sin(bankAng_);

    // Default tensor for transforming from Cartesian into Pitch/Bank Plane
    PB_ = tensor(cp, sp*sb, sp*cb, 0, cb, -sb, -sp, cp*sb, cp*cb);


    // Tensor for transforming from Cartesian into Pitch/Bank Plane
    // If the rotor axis is close to +-Z-Direction, 
    // such as the main rotor, then use the following
    if ( ((axis & zGlobal) > 0.8) || ((axis & zGlobal) < -0.8) )
    {
        PB_ = tensor(cp, sp*sb, sp*cb, 0, cb, -sb, -sp, cp*sb, cp*cb);
    }

    // If the rotor axis is close to +-Y-Direction, 
    // such as the tail rotor, then use the following
    if ( ((axis & yGlobal) > 0.8) || ((axis & yGlobal) < -0.8) )
    {
        PB_ = tensor(cp, -sp*cb, -sp*-sb, sp, cp*cb, cp*-sb, 0, sb, cb);
    }

    // If the rotor axis is close to +-X-Direction, 
    // such as an aircraft propeller, then use the following
    if ( ((axis & xGlobal) > 0.8) || ((axis & xGlobal) < -0.8) )
    {
        PB_ = tensor(cp*cb, -sp, cp*sb, sp*cb, cp, sp*sb, -sb, 0, cb);
    }

    // Tensor for transforming from Pitch/Bank Plane into Cartesian 
    invPB_ = PB_.T();

    const scalar sumArea = gSum(area_);
    const scalar diameter = CML::sqrt(4.0*sumArea/mathematical::pi);
    Info<< "    Rotor gometry:" << nl
        << "    - disk diameter = " << diameter << nl
        << "    - disk area     = " << sumArea << nl
        << "    - origin        = " << coordSys_.origin() << nl
        << "    - r-axis        = " << coordSys_.R().e1() << nl
        << "    - psi-axis      = " << coordSys_.R().e2() << nl
        << "    - z-axis        = " << coordSys_.R().e3() << nl
        << "    - disk pitch angle  = " << radToDeg(pitchAng_) << nl
        << "    - disk bank angle   = " << radToDeg(bankAng_) << endl;
}


void CML::fv::rotorDiskSource::constructGeometry()
{
    const vectorField& C = mesh_.C();

    forAll(cells_, i)
    {
        if (area_[i] > ROOTVSMALL)
        {
            const label celli = cells_[i];

            // Position in (planar) rotor co-ordinate system
            x_[i] = coordSys_.localPosition(C[celli]);

            // Cache max radius
            rMax_ = max(rMax_, x_[i].x());

            // Swept angle relative to rDir axis [radians] in range 0 -> 2*pi
            scalar psi = x_[i].y();

            if (rotorDebug_)
            {
                psiList_[i] = radToDeg(psi);
            }

            // Blade flap angle [radians]
            scalar beta =
                flap_.beta0 - flap_.beta1c*cos(psi) - flap_.beta1s*sin(psi);

            // Blade flapping angular velocity [rad/s]
            scalar betaDOT =
                this->omega()*( flap_.beta1c*sin(psi)-flap_.beta1s*cos(psi) );

            // Determine rotation tensor to convert from planar system into the
            // rotor cone system
            scalar c = cos(beta);
            scalar s = sin(beta);
            R_[i] = tensor(c, 0, -s, 0, 1, 0, s, 0, c);
            invR_[i] = R_[i].T();

            // Blade flapping linear velocity (rBetaDOT)
            rBetaDOT_[i] = x_[i].x() * betaDOT;
        }
    }
    // Reduce rMax_ for parallel running
    reduce(rMax_, maxOp<scalar>());
}


CML::tmp<CML::vectorField> CML::fv::rotorDiskSource::inflowVelocity
(
    const volVectorField& U
) const
{
    switch (inletFlow_)
    {
        case ifFixed:
        case ifSurfaceNormal:
        {
            return tmp<vectorField>
            (
                new vectorField(mesh_.nCells(), inletVelocity_)
            );

            break;
        }
        case ifLocal:
        {
            return U.internalField();

            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown inlet flow specification" << abort(FatalError);
        }
    }

    return tmp<vectorField>(new vectorField(mesh_.nCells(), Zero));
}


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

CML::fv::rotorDiskSource::rotorDiskSource
(
    const word& name,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh

)
:
    option(name, modelType, dict, mesh),
    rhoRef_(1.0),
    rotorDebug_(false),
    rotorURF_(1.0),
    rpm_(DataEntry<scalar>::New("rpm", coeffs_)),
    nBlades_(0),
    inletFlow_(ifLocal),
    inletVelocity_(Zero),
    tipEffect_(1.0),
    flap_(),
    x_(cells_.size(), Zero),
    R_(cells_.size(), I),
    invR_(cells_.size(), I),
    rBetaDOT_(cells_.size(), 0.0),
    area_(cells_.size(), 0.0),
    coordSys_(false),
    rMax_(0.0),
    psiList_(cells_.size(),0.0),
    trim_(trimModel::New(*this, coeffs_)),
    blade_(coeffs_.subDict("blade")),
    profiles_(coeffs_.subDict("profiles"))
{
    read(dict);
    Info<< "    - creating rotor disk zone: "
        << this->name() << endl;

}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::fv::rotorDiskSource::~rotorDiskSource()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::fv::rotorDiskSource::addSup
(
    fvMatrix<vector>& eqn,
    const label fieldi
)
{
    volVectorField force
    (
        IOobject
        (
            name_ + ":rotorForce",
            mesh_.time().timeName(),
            mesh_
        ),
        mesh_,
        dimensionedVector("zero", eqn.dimensions()/dimVolume, Zero)
    );

    static volVectorField oldF
    (
        IOobject
        (
            name_ + ":oldForce",
            mesh_.time().timeName(),
            mesh_
        ),
        mesh_,
        dimensionedVector
        (
            "zero",
            eqn.dimensions()/dimVolume,
            Zero
        )
    );

    // Read the reference density for incompressible flow
    coeffs_.lookup("rhoRef") >> rhoRef_;

    const volVectorField& U = eqn.psi();
    const vectorField Uin(inflowVelocity(U));
    trim_->correct(Uin, force);
    calculate(geometricOneField(), Uin, trim_->thetag(), force);

    // Under relax momentum source 
    force = oldF + rotorURF_ * (force - oldF);
    oldF = force;

    // Add source to rhs of eqn
    eqn -= force;

    if (mesh_.time().outputTime())
    {
        force.write();
    }
}


void CML::fv::rotorDiskSource::addSup
(
    const volScalarField& rho,
    fvMatrix<vector>& eqn,
    const label fieldi
)
{
    volVectorField force
    (
        IOobject
        (
            name_ + ":rotorForce",
            mesh_.time().timeName(),
            mesh_
	),
	mesh_,
	dimensionedVector
	(
	    "zero",
	    eqn.dimensions()/dimVolume,
            Zero
	)
    );

    static volVectorField oldF
    (
        IOobject
        (
            name_ + ":oldForce",
            mesh_.time().timeName(),
            mesh_
        ),
        mesh_,
        dimensionedVector
        (
            "zero",
            eqn.dimensions()/dimVolume,
            Zero
        )
    );

    const volVectorField& U = eqn.psi();
    const vectorField Uin(inflowVelocity(U));
    trim_->correct(rho, Uin, force);
    calculate(rho, Uin, trim_->thetag(), force);

    // Under relax momentum source 
    force = oldF + rotorURF_ * (force - oldF);
    oldF = force;

    // Add source to rhs of eqn
    eqn -= force;

    if (mesh_.time().outputTime())
    {
        force.write();
    }
}


void CML::fv::rotorDiskSource::writeData(Ostream& os) const
{
    os  << indent << name_ << endl;
    dict_.write(os);
}


bool CML::fv::rotorDiskSource::read(const dictionary& dict)
{
    if (option::read(dict))
    {
        coeffs_.lookup("fieldNames") >> fieldNames_;
        applied_.setSize(fieldNames_.size(), false);

        // Read if rotorDebug is active
        coeffs_.lookup("rotorDebugMode") >> rotorDebug_;

        // Read co-ordinate system/geometry invariant properties

        coeffs_.lookup("nBlades") >> nBlades_;

        inletFlow_ = inletFlowTypeNames_.read(coeffs_.lookup("inletFlowType"));

        coeffs_.lookup("tipEffect") >> tipEffect_;
        coeffs_.lookup("rotorURF") >> rotorURF_;

        const dictionary& flapCoeffs(coeffs_.subDict("flapCoeffs"));
        flapCoeffs.lookup("beta0") >> flap_.beta0;
        flapCoeffs.lookup("beta1c") >> flap_.beta1c;
        flapCoeffs.lookup("beta1s") >> flap_.beta1s;
        flap_.beta0 = degToRad(flap_.beta0);
        flap_.beta1c = degToRad(flap_.beta1c);
        flap_.beta1s = degToRad(flap_.beta1s);


        // Create co-ordinate system
        createCoordinateSystem();

        // Read co-odinate system dependent properties
        checkData();

        constructGeometry();

        trim_->read(coeffs_);

        if (debug)
        {
            writeField("thetag", trim_->thetag()(), true);
            writeField("faceArea", area_, true);
        }

        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
