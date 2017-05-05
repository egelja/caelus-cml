/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

#include "solidBodyMotionFvMesh.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "volFields.hpp"
#include "transformField.hpp"
#include "cellZoneMesh.hpp"
#include "boolList.hpp"
#include "syncTools.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(solidBodyMotionFvMesh, 0);
    addToRunTimeSelectionTable(dynamicFvMesh, solidBodyMotionFvMesh, IOobject);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidBodyMotionFvMesh::solidBodyMotionFvMesh(const IOobject& io)
:
    dynamicFvMesh(io),
    dynamicMeshCoeffs_
    (
        IOdictionary
        (
            IOobject
            (
                "dynamicMeshDict",
                io.time().constant(),
                *this,
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE,
                false
            )
        ).subDict(typeName + "Coeffs")
    ),
    SBMFPtr_(solidBodyMotionFunction::New(dynamicMeshCoeffs_, io.time())),
    undisplacedPoints_
    (
        IOobject
        (
            "points",
            io.time().constant(),
            meshSubDir,
            *this,
            IOobject::MUST_READ,
            IOobject::NO_WRITE,
            false
        )
    ),
    zoneID_(-1),
    pointIDs_()
{
    if (undisplacedPoints_.size() != nPoints())
    {
        FatalIOErrorIn
        (
            "solidBodyMotionFvMesh::solidBodyMotionFvMesh(const IOobject&)",
            dynamicMeshCoeffs_
        )   << "Read " << undisplacedPoints_.size()
            << " undisplaced points from " << undisplacedPoints_.objectPath()
            << " but the current mesh has " << nPoints()
            << exit(FatalIOError);
    }

    word cellZoneName =
        dynamicMeshCoeffs_.lookupOrDefault<word>("cellZone", "none");

    if (cellZoneName != "none")
    {
        zoneID_ = cellZones().findZoneID(cellZoneName);
        Info<< "Applying solid body motion to cellZone " << cellZoneName
            << endl;

        const cellZone& cz = cellZones()[zoneID_];


        // collect point IDs of points in cell zone

        boolList movePts(nPoints(), false);

        forAll(cz, i)
        {
            label cellI = cz[i];
            const cell& c = cells()[cellI];
            forAll(c, j)
            {
                const face& f = faces()[c[j]];
                forAll(f, k)
                {
                    label pointI = f[k];
                    movePts[pointI] = true;
                }
            }
        }

        syncTools::syncPointList(*this, movePts, orEqOp<bool>(), false);

        DynamicList<label> ptIDs(nPoints());
        forAll(movePts, i)
        {
            if (movePts[i])
            {
                ptIDs.append(i);
            }
        }

        pointIDs_.transfer(ptIDs);
    }
    else
    {
        Info<< "Applying solid body motion to entire mesh" << endl;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::solidBodyMotionFvMesh::~solidBodyMotionFvMesh()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::solidBodyMotionFvMesh::update()
{
    static bool hasWarned = false;

    if (zoneID_ != -1)
    {
        pointField transformedPts(undisplacedPoints_);

        UIndirectList<point>(transformedPts, pointIDs_) =
            transform
            (
                SBMFPtr_().transformation(),
                pointField(transformedPts, pointIDs_)
            );

        fvMesh::movePoints(transformedPts);
    }
    else
    {
        fvMesh::movePoints
        (
            transform
            (
                SBMFPtr_().transformation(),
                undisplacedPoints_
            )
        );
    }


    if (foundObject<volVectorField>("U"))
    {
        const_cast<volVectorField&>(lookupObject<volVectorField>("U"))
            .correctBoundaryConditions();
    }
    else if (!hasWarned)
    {
        hasWarned = true;

        WarningIn("solidBodyMotionFvMesh::update()")
            << "Did not find volVectorField U."
            << " Not updating U boundary conditions." << endl;
    }

    return true;
}


// ************************************************************************* //
