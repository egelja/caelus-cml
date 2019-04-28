/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2016 Applied CCM
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "fvMesh.hpp"
#include "Time.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "slicedVolFields.hpp"
#include "slicedSurfaceFields.hpp"
#include "SubField.hpp"
#include "cyclicFvPatchFields.hpp"
#include "cyclicAMIFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void fvMesh::makeSf() const
{
    if (debug)
    {
        Info<< "void fvMesh::makeSf() : "
            << "assembling face areas"
            << endl;
    }

    // It is an error to attempt to recalculate
    // if the pointer is already set
    if (SfPtr_)
    {
        FatalErrorInFunction
            << "face areas already exist"
            << abort(FatalError);
    }

    SfPtr_ = new slicedSurfaceVectorField
    (
        IOobject
        (
            "S",
            pointsInstance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        *this,
        dimArea,
        faceAreas()
    );
}


void fvMesh::makeMagSf() const
{
    if (debug)
    {
        Info<< "void fvMesh::makeMagSf() : "
            << "assembling mag face areas"
            << endl;
    }

    // It is an error to attempt to recalculate
    // if the pointer is already set
    if (magSfPtr_)
    {
        FatalErrorInFunction
            << "mag face areas already exist"
            << abort(FatalError);
    }

    // Note: Added stabilisation for faces with exactly zero area.
    // These should be caught on mesh checking but at least this stops
    // the code from producing Nans.
    magSfPtr_ = new surfaceScalarField
    (
        IOobject
        (
            "magSf",
            pointsInstance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        mag(Sf()) + dimensionedScalar("vs", dimArea, VSMALL)
    );
}


void fvMesh::makeC() const
{
    if (debug)
    {
        Info<< "void fvMesh::makeC() : "
            << "assembling cell centres"
            << endl;
    }

    // It is an error to attempt to recalculate
    // if the pointer is already set
    if (CPtr_)
    {
        FatalErrorInFunction
            << "cell centres already exist"
            << abort(FatalError);
    }

    // Construct as slices. Only preserve processor (not e.g. cyclic)

    CPtr_ = new slicedVolVectorField
    (
        IOobject
        (
            "C",
            pointsInstance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        *this,
        dimLength,
        cellCentres(),
        faceCentres(),
        true,               //preserveCouples
        true                //preserveProcOnly
    );
}


void fvMesh::makeCg() const
{
    if (debug)
    {
        Info<< "void fvMesh::makeCg() : "
            << "assembling cell centres"
            << endl;
    }

    // It is an error to attempt to recalculate
    // if the pointer is already set
    if (CgPtr_)
    {
        FatalErrorInFunction
            << "Geometric cell centres already exist"
            << abort(FatalError);
    }

    // Construct as slices. Only preserve processor (not e.g. cyclic)

    CgPtr_ = new slicedVolVectorField
    (
        IOobject
        (
            "Cg",
            pointsInstance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        *this,
        dimLength,
        cellCentresGeometric(),
        faceCentres(),
        true,               //preserveCouples
        true                //preserveProcOnly
    );
}


void fvMesh::makeDefectCorrVecs() const
{
    if (debug)
    {
        Info<< "void fvMesh::makeDefectCorrVecs() : "
            << "assembling defect correction vectors"
            << endl;
    }

    defectCorrVecsPtr_ = new volVectorField
    (
        IOobject
        (
            "defectCorrectionVectors",
            pointsInstance(),
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        *this,
        dimLength
    );
    volVectorField& DefectCorrVecs = *defectCorrVecsPtr_;

    // Set local references to mesh data
    const vectorField& Cg = cellCentresGeometric();
    const vectorField& C = cellCentres();

    forAll(C, celli)
    {
        DefectCorrVecs[celli] = Cg[celli] - C[celli];
    }


    forAll(DefectCorrVecs.boundaryField(), patchI)
    {
        fvPatchVectorField& patchDefectCorrVecs =
            DefectCorrVecs.boundaryField()[patchI];

        patchDefectCorrVecs = vector::zero;
    }
}


void fvMesh::makeCf() const
{
    if (debug)
    {
        Info<< "void fvMesh::makeCf() : "
            << "assembling face centres"
            << endl;
    }

    // It is an error to attempt to recalculate
    // if the pointer is already set
    if (CfPtr_)
    {
        FatalErrorInFunction
            << "face centres already exist"
            << abort(FatalError);
    }

    CfPtr_ = new slicedSurfaceVectorField
    (
        IOobject
        (
            "Cf",
            pointsInstance(),
            meshSubDir,
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        *this,
        dimLength,
        faceCentres()
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const volScalarField::DimensionedInternalField& fvMesh::V() const
{
    if (!VPtr_)
    {
        if (debug)
        {
            Info<< "fvMesh::V() const: "
                << "constructing from primitiveMesh::cellVolumes()"
                << endl;
        }

        VPtr_ = new slicedVolScalarField::DimensionedInternalField
        (
            IOobject
            (
                "V",
                time().timeName(),
                *this,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            *this,
            dimVolume,
            cellVolumes()
        );
    }

    return *static_cast<slicedVolScalarField::DimensionedInternalField*>(VPtr_);
}


const volScalarField::DimensionedInternalField& fvMesh::V0() const
{
    if (!V0Ptr_)
    {
        FatalErrorInFunction
            << "V0 is not available"
            << abort(FatalError);
    }

    return *V0Ptr_;
}


volScalarField::DimensionedInternalField& fvMesh::setV0()
{
    if (!V0Ptr_)
    {
        FatalErrorInFunction
            << "V0 is not available"
            << abort(FatalError);
    }

    return *V0Ptr_;
}


const volScalarField::DimensionedInternalField& fvMesh::V00() const
{
    if (!V00Ptr_)
    {
        if (debug)
        {
            Info<< "fvMesh::V00() const: "
                << "constructing from V0"
                << endl;
        }

        V00Ptr_ = new DimensionedField<scalar, volMesh>
        (
            IOobject
            (
                "V00",
                time().timeName(),
                *this,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            V0()
        );

        // If V00 is used then V0 should be stored for restart
        V0Ptr_->writeOpt() = IOobject::AUTO_WRITE;
    }

    return *V00Ptr_;
}


tmp<volScalarField::DimensionedInternalField> fvMesh::Vsc() const
{
    if (moving() && time().subCycling())
    {
        const TimeState& ts = time();
        const TimeState& ts0 = time().prevTimeState();

        scalar tFrac =
        (
            ts.value() - (ts0.value() - ts0.deltaTValue())
        )/ts0.deltaTValue();

        if (tFrac < (1 - SMALL))
        {
            return V0() + tFrac*(V() - V0());
        }
        else
        {
            return V();
        }
    }
    else
    {
        return V();
    }
}


tmp<volScalarField::DimensionedInternalField> fvMesh::Vsc0() const
{
    if (moving() && time().subCycling())
    {
        const TimeState& ts = time();
        const TimeState& ts0 = time().prevTimeState();

        scalar t0Frac =
        (
            (ts.value() - ts.deltaTValue())
          - (ts0.value() - ts0.deltaTValue())
        )/ts0.deltaTValue();

        if (t0Frac > SMALL)
        {
            return V0() + t0Frac*(V() - V0());
        }
        else
        {
            return V0();
        }
    }
    else
    {
        return V0();
    }
}


const surfaceVectorField& fvMesh::Sf() const
{
    if (!SfPtr_)
    {
        makeSf();
    }

    return *SfPtr_;
}


const surfaceScalarField& fvMesh::magSf() const
{
    if (!magSfPtr_)
    {
        makeMagSf();
    }

    return *magSfPtr_;
}


const volVectorField& fvMesh::C() const
{
    if (!CPtr_)
    {
        makeC();
    }

    return *CPtr_;
}


const volVectorField& fvMesh::Cg() const
{
    if (!CgPtr_)
    {
        makeCg();
    }

    return *CgPtr_;
}


const volVectorField& fvMesh::defectCorrVecs() const
{
    if (!defectCorrVecsPtr_)
    {
        makeDefectCorrVecs();
    }

    return *defectCorrVecsPtr_;
}


const surfaceVectorField& fvMesh::Cf() const
{
    if (!CfPtr_)
    {
        makeCf();
    }

    return *CfPtr_;
}


tmp<surfaceVectorField> fvMesh::delta() const
{
    if (debug)
    {
        Info<< "void fvMesh::delta() : "
            << "calculating face deltas"
            << endl;
    }

    tmp<surfaceVectorField> tdelta
    (
        new surfaceVectorField
        (
            IOobject
            (
                "delta",
                pointsInstance(),
                meshSubDir,
                *this,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            *this,
            dimLength
        )
    );
    surfaceVectorField& delta = tdelta();

    const volVectorField& C = this->C();
    const labelUList& owner = this->owner();
    const labelUList& neighbour = this->neighbour();

    forAll(owner, facei)
    {
        delta[facei] = C[neighbour[facei]] - C[owner[facei]];
    }

    forAll(delta.boundaryField(), patchi)
    {
        delta.boundaryField()[patchi] = boundary()[patchi].delta();
    }

    return tdelta;
}


const surfaceScalarField& fvMesh::phi() const
{
    if (!phiPtr_)
    {
        FatalErrorInFunction
            << "mesh flux field does not exist, is the mesh actually moving?"
            << abort(FatalError);
    }

    // Set zero current time
    // mesh motion fluxes if the time has been incremented
    if (phiPtr_->timeIndex() != time().timeIndex())
    {
        (*phiPtr_) = dimensionedScalar("0", dimVolume/dimTime, 0.0);
    }

    return *phiPtr_;
}


surfaceScalarField& fvMesh::setPhi()
{
    if (!phiPtr_)
    {
        FatalErrorInFunction
            << "mesh flux field does not exist, is the mesh actually moving?"
            << abort(FatalError);
    }

    return *phiPtr_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
