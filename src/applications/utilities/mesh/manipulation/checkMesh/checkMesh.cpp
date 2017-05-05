/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

Application
    checkMesh

Description
    Checks validity of a mesh

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "timeSelector.hpp"
#include "Time.hpp"

#include "polyMesh.hpp"
#include "globalMeshData.hpp"

#include "printMeshStats.hpp"
#include "checkTopology.hpp"
#include "checkGeometry.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();
#   include "addRegionOption.hpp"
    argList::addBoolOption
    (
        "noTopology",
        "skip checking the mesh topology"
    );
    argList::addBoolOption
    (
        "allGeometry",
        "include bounding box checks"
    );
    argList::addBoolOption
    (
        "allTopology",
        "include extra topology checks"
    );

    argList::addBoolOption
    (
        "defectCorrection",
        "include deffect correction"
    );

    argList::addOption
    (
        "areaSwitch",
        "scalar",
        "face area below which the centroid method switches to simple nodal average"
    );


#   include "setRootCase.hpp"
#   include "createTime.hpp"
    instantList timeDirs = timeSelector::select0(runTime, args);
    const bool defectCorrection = args.optionFound("defectCorrection");

    scalar areaSwitch;

    if (args.optionReadIfPresent("areaSwitch", areaSwitch))
    {
        // Do nothing
    }
    else
    {
        areaSwitch = 1e-8;
    }

//
// createNamedPolyMesh.H
// ~~~~~~~~~~~~~~~~~~~~~

    CML::word regionName;

    if (args.optionReadIfPresent("region", regionName))
    {
        CML::Info
            << "Create polyMesh " << regionName << " for time = "
            << runTime.timeName() << CML::nl << CML::endl;
    }
    else
    {
        regionName = CML::polyMesh::defaultRegion;
        CML::Info
            << "Create polyMesh for time = "
            << runTime.timeName() << CML::nl << CML::endl;
    }

    CML::polyMesh mesh
    (
        CML::IOobject
        (
            regionName,
            runTime.timeName(),
            runTime,
            CML::IOobject::MUST_READ
        ),
        defectCorrection,
        areaSwitch
    );

    const bool noTopology  = args.optionFound("noTopology");
    const bool allGeometry = args.optionFound("allGeometry");
    const bool allTopology = args.optionFound("allTopology");

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        polyMesh::readUpdateState state = mesh.readUpdate();

        if
        (
            !timeI
         || state == polyMesh::TOPO_CHANGE
         || state == polyMesh::TOPO_PATCH_CHANGE
        )
        {
            Info<< "Time = " << runTime.timeName() << nl << endl;

            // Clear mesh before checking
            mesh.clearOut();

            // Reconstruct globalMeshData
            mesh.globalData();

            printMeshStats(mesh, allTopology);

            label noFailedChecks = 0;

            if (!noTopology)
            {
                noFailedChecks += checkTopology(mesh, allTopology, allGeometry);
            }

            noFailedChecks += checkGeometry(mesh, allGeometry);

            // Note: no reduction in noFailedChecks necessary since is
            //       counter of checks, not counter of failed cells,faces etc.

            if (noFailedChecks == 0)
            {
                Info<< "\nMesh OK.\n" << endl;
            }
            else
            {
                Info<< "\nFailed " << noFailedChecks << " mesh checks.\n"
                    << endl;
            }
        }
        else if (state == polyMesh::POINTS_MOVED)
        {
            Info<< "Time = " << runTime.timeName() << nl << endl;

            label nFailedChecks = checkGeometry(mesh, allGeometry);

            if (nFailedChecks)
            {
                Info<< "\nFailed " << nFailedChecks << " mesh checks.\n"
                    << endl;
            }
            else
            {
                Info<< "\nMesh OK.\n" << endl;
            }
        }
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
