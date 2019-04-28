/*---------------------------------------------------------------------------*\
Copyright Niels G. Jacobsen Technical University of Denmark
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

#include "overtopping.hpp"
#include "fvMesh.hpp"
#include "surfaceFields.hpp"
#include "volFields.hpp"
#include "IOList.hpp"
#include "emptyPolyPatch.hpp"
#include "processorPolyPatch.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
defineTypeNameAndDebug(overtopping, 0);
}

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //


void CML::overtopping::updateMesh(const mapPolyMesh&)
{
    // Do nothing
}


void CML::overtopping::movePoints(const pointField&)
{
    // Do nothing
}


bool CML::overtopping::timeSet()
{
    // Do nothing
    return true;
}


void CML::overtopping::makeFile()
{
    // Create the overtopping file if not already created
    if (outputFilePtr_.empty())
    {
        if (debug)
        {
            Info<< "Creating output file." << endl;
        }

        // File update
        if (Pstream::master())
        {
            fileName outputDir;
            word startTimeName =
                    mesh_.time().timeName(mesh_.time().startTime().value());

            if (Pstream::parRun())
            {
                // Put in undecomposed case (Note: gives problems for
                // distributed data running)
                outputDir =
                        mesh_.time().path()/".."/this->type()/startTimeName;
            }
            else
            {
                outputDir = mesh_.time().path()/this->type()/startTimeName;
            }

            // Create directory if does not exist
            mkDir(outputDir);

            // Open new file at start up
            outputFilePtr_.reset(new OFstream(outputDir/(type() + ".dat")));

            outputFileForcePtr_.reset
            (
                new OFstream(outputDir/(type() + "_rhoU2A.dat"))
            );

            // Add headers to output data
            writeFileHeader();
        }
    }
}


void CML::overtopping::writeFileHeader()
{
    if (outputFilePtr_.valid())
    {
        outputFilePtr_() << "Time:";

        const faceZoneMesh& faceZones(mesh_.faceZones());

        forAll (faceZones, fzi)
        {
            if (operateOnZone(faceZones[fzi]))
            {
                outputFilePtr_() << "\t" << faceZones[fzi].name();
            }
        }

        outputFilePtr_() << endl;
    }

    if (outputFileForcePtr_.valid())
    {
        outputFileForcePtr_() << "Time:";

        const faceZoneMesh& faceZones(mesh_.faceZones());

        forAll (faceZones, fzi)
        {
            if (operateOnZone(faceZones[fzi]))
            {
                outputFileForcePtr_() << "\t" << faceZones[fzi].name();
            }
        }

        outputFileForcePtr_() << endl;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


CML::overtopping::overtopping
(
        const word& name,
        const objectRegistry& obr,
        const dictionary& dict,
        const bool loadFromFiles
)
:
    phiName_(dict.lookupOrDefault<word>("phiName","phi")),
    rhoPhiName_(dict.lookupOrDefault<word>("rhoPhiName","rho*phi")),
    rho1_
    (
        dimensionedScalar
        (
            obr.lookupObject<dictionary>("transportProperties")
           .subDict("phase1").lookup("rho")
        ).value()
    ),
    rho2_
    (
        dimensionedScalar
        (
            obr.lookupObject<dictionary>("transportProperties")
           .subDict("phase2").lookup("rho")
        ).value()
    ),
    invRhoDiff_(1.0/( rho1_ - rho2_ )),

    mesh_(refCast<const fvMesh>(obr)),

    outputFilePtr_(nullptr)
{
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


CML::overtopping::~overtopping()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


bool CML::overtopping::operateOnZone(const faceZone& fz) const
{
    const string& zoneName(fz.name());
    const string& ownName(this->type());
    const label N = ownName.size();

    if (!zoneName.compare(0, N, ownName))
    {
        return true;
    }
    else
    {
        return false;
    }
}


void CML::overtopping::read(const dictionary& dict)
{
    //    if (active_)
    //    {
    //        initialise(dict);
    //    }
}


void CML::overtopping::computeAndWriteBoundary
(
    const label& faceI,
    const surfaceScalarField& phi,
    const surfaceScalarField& rhoPhi,
    const surfaceScalarField& magSf,
    const surfaceVectorField& Sf,
    vector& q,
    vector& f
) const
{
    label facePatchId = mesh_.boundaryMesh().whichPatch(faceI);
    label faceId(-1);

    q = vector::zero;
    f = vector::zero;

    const polyPatch& pp = mesh_.boundaryMesh()[facePatchId];

    if (isA<processorPolyPatch>(pp))
    {
        if (refCast<const processorPolyPatch>(pp).owner())
        {
            faceId = pp.whichFace(faceI);
        }
        else
        {
            faceId = -1;
        }
    }
    // Overtopping will not be defined on both ends of a cyclic patch,
    // so redundant
//    else if (isA<cyclicPolyPatch>(pp))
//    {
//        label patchFaceI = faceI - pp.start();
//        if (patchFaceI < pp.size()/2)
//        {
//            faceId = patchFaceI;
//        }
//        else
//        {
//            faceId = -1;
//        }
//    }
    else if (!isA<emptyPolyPatch>(pp))
    {
        faceId = faceI - pp.start();
    }
    else
    {
        faceId = -1;
        facePatchId = -1;
    }

    if (faceId > -1)
    {
        const scalarField& phiw(phi.boundaryField()[facePatchId]);
        const scalarField& rhoPhiw(rhoPhi.boundaryField()[facePatchId]);
        const scalarField& magSfw(magSf.boundaryField()[facePatchId]);
        const vectorField& Sfw(Sf.boundaryField()[facePatchId]);

        q = ((rhoPhiw[faceId] - phiw[faceId]*rho2_)*invRhoDiff_)
            *Sfw[faceId]/magSfw[faceId];
        f = rho1_*q*CML::mag(q/magSfw[faceId]);
    }

}


void CML::overtopping::computeAndWrite
(
    const faceZone& fz,
    const surfaceScalarField& phi,
    const surfaceScalarField& rhoPhi
)
{
    const surfaceVectorField& Sf(mesh_.Sf());
    const surfaceScalarField& magSf(mesh_.magSf());

    vectorField q(fz.size(), vector::zero);
    vectorField f(fz.size(), vector::zero);

    forAll (fz, facei)
    {
        label faceI(fz[facei]);

        if (mesh_.isInternalFace( faceI))
        {
            q[facei] = ((rhoPhi[faceI] - phi[faceI]*rho2_)*invRhoDiff_)
                *Sf[faceI]/magSf[faceI];
            f[facei] = rho1_*q[facei]
                *CML::mag(q[facei]/magSf[faceI]);
        }
        else
        {
            computeAndWriteBoundary
            (
                faceI,
                phi,
                rhoPhi,
                magSf,
                Sf,
                q[facei],
                f[facei]
            );
        }
    }

    vector OT(CML::gSum(q));
    vector F (CML::gSum(f));

    if (Pstream::master())
    {
        outputFilePtr_() << "\t" << OT;

        outputFileForcePtr_() << "\t" << F;
    }

}


void CML::overtopping::write()
{
    makeFile();

    const faceZoneMesh& faceZones(mesh_.faceZones());

    if (Pstream::master())
    {
        outputFilePtr_() << mesh_.time().timeName();

        outputFileForcePtr_() << mesh_.time().timeName();
    }

    const surfaceScalarField& phi =
        mesh_.lookupObject<surfaceScalarField>(phiName_);
    const surfaceScalarField& rhoPhi =
        mesh_.lookupObject<surfaceScalarField>(rhoPhiName_);

    forAll (faceZones, fzi)
    {
        if (operateOnZone(faceZones[fzi]))
        {
            computeAndWrite(faceZones[fzi], phi, rhoPhi);
        }
    }

    if (Pstream::master())
    {
        outputFilePtr_() << endl;

        outputFileForcePtr_() << endl;
    }
}

// ************************************************************************* //
