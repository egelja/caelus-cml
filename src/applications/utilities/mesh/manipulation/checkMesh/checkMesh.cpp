/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
Copyright (C) 2015-2017 OpenCFD Ltd.
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
    Checks validity of a mesh.

Usage
    \b checkMesh [OPTION]

    Options:
      - \par -allGeometry
        Checks all (including non finite-volume specific) geometry

      - \par -allTopology
        Checks all (including non finite-volume specific) addressing

      - \par -meshQuality
        Checks against user defined (in \a system/meshQualityDict) quality
        settings

      - \par -region \<name\>
        Specify an alternative mesh region.

      - \par -writeSets \<surfaceFormat\>
        Reconstruct all cellSets and faceSets geometry and write to
        postProcessing directory according to surfaceFormat
        (e.g. vtk or ensight). Additionally reconstructs all pointSets and
        writes as vtk format.

      - \par -writeAllFields \n
        Writes all mesh quality measures as fields.

      - \par -writeFields '(\<fieldName\>)' \n
        Writes selected mesh quality measures as fields.

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "timeSelector.hpp"
#include "Time.hpp"

#include "fvMesh.hpp"
#include "globalMeshData.hpp"
#include "surfaceWriter.hpp"
#include "vtkSetWriter.hpp"

#include "checkTools.hpp"
#include "checkTopology.hpp"
#include "checkGeometry.hpp"
#include "checkMeshQuality.hpp"
#include "writeFields.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();
    #include "addRegionOption.hpp"
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
        "writeAllFields",
        "write volFields with mesh quality parameters"
    );
    argList::addOption
    (
        "writeFields",
        "wordList",
        "write volFields with selected mesh quality parameters"
    );
    argList::addBoolOption
    (
        "meshQuality",
        "read user-defined mesh quality criterions from system/meshQualityDict"
    );
    argList::addOption
    (
        "writeSets",
        "surfaceFormat",
        "reconstruct and write all faceSets and cellSets in selected format"
    );
    argList::addBoolOption
    (
        "meshBandwidth",
        "calculate resulting matrix bandwidth"
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



    #include "setRootCase.hpp"
    #include "createTime.hpp"
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
// createNamedMesh.hpp
// ~~~~~~~~~~~~~~~~~~~~~

    CML::word regionName;

    if (args.optionReadIfPresent("region", regionName))
    {
        CML::Info
            << "Create mesh " << regionName << " for time = "
            << runTime.timeName() << CML::nl << CML::endl;
    }
    else
    {
        regionName = CML::fvMesh::defaultRegion;
        CML::Info
            << "Create mesh for time = "
            << runTime.timeName() << CML::nl << CML::endl;
    }

    CML::fvMesh mesh
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
    const bool meshQuality = args.optionFound("meshQuality");
    const bool meshBandwidth = args.optionFound("meshBandwidth");

    word surfaceFormat;
    const bool writeSets = args.optionReadIfPresent("writeSets", surfaceFormat);
    HashSet<word> selectedFields;
    bool writeFields = args.optionReadIfPresent
    (
        "writeFields",
        selectedFields
    );
    if (!writeFields && args.optionFound("writeAllFields"))
    {
        selectedFields.insert("nonOrthoAngle");
        selectedFields.insert("faceWeight");
        selectedFields.insert("skewness");
        selectedFields.insert("cellDeterminant");
        selectedFields.insert("aspectRatio");
        selectedFields.insert("cellShapes");
        selectedFields.insert("cellVolume");
        selectedFields.insert("cellVolumeRatio");
    }


    if (noTopology)
    {
        Info<< "Disabling all topology checks." << nl << endl;
    }
    if (allTopology)
    {
        Info<< "Enabling all (cell, face, edge, point) topology checks."
            << nl << endl;
    }
    if (allGeometry)
    {
        Info<< "Enabling all geometry checks." << nl << endl;
    }
    if (meshQuality)
    {
        Info<< "Enabling user-defined geometry checks." << nl << endl;
    }
    if (meshBandwidth)
    {
        Info<< "Enabling calculation of matrix bandwidth." << nl << endl;
    }
    if (writeSets)
    {
        Info<< "Reconstructing and writing " << surfaceFormat
            << " representation"
            << " of all faceSets and cellSets." << nl << endl;
    }
    if (selectedFields.size())
    {
        Info<< "Writing mesh quality as fields " << selectedFields << nl
            << endl;
    }


    autoPtr<IOdictionary> qualDict;
    if (meshQuality)
    {
        qualDict.reset
        (
            new IOdictionary
            (
                IOobject
                (
                    "meshQualityDict",
                    mesh.time().system(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
           )
        );
    }


    autoPtr<surfaceWriter> surfWriter;
    autoPtr<writer<scalar> > setWriter;
    if (writeSets)
    {
        surfWriter = surfaceWriter::New(surfaceFormat);
        setWriter = writer<scalar>::New(vtkSetWriter<scalar>::typeName);
    }


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

            printMeshStats(mesh, allTopology, meshBandwidth);

            label nFailedChecks = 0;

            if (!noTopology)
            {
                nFailedChecks += checkTopology
                (
                    mesh,
                    allTopology,
                    allGeometry,
                    surfWriter,
                    setWriter
                );
            }

            nFailedChecks += checkGeometry
            (
                mesh,
                allGeometry,
                surfWriter,
                setWriter
            );

            if (meshQuality)
            {
                nFailedChecks += checkMeshQuality(mesh, qualDict(), surfWriter);
            }


            // Note: no reduction in nFailedChecks necessary since is
            //       counter of checks, not counter of failed cells,faces etc.

            if (nFailedChecks == 0)
            {
                Info<< "\nMesh OK.\n" << endl;
            }
            else
            {
                Info<< "\nFailed " << nFailedChecks << " mesh checks.\n"
                    << endl;
            }


            // Write selected fields
            CML::writeFields(mesh, selectedFields);
        }
        else if (state == polyMesh::POINTS_MOVED)
        {
            Info<< "Time = " << runTime.timeName() << nl << endl;

            label nFailedChecks = checkGeometry
            (
                mesh,
                allGeometry,
                surfWriter,
                setWriter
            );

            if (meshQuality)
            {
                nFailedChecks += checkMeshQuality(mesh, qualDict(), surfWriter);
            }


            if (nFailedChecks)
            {
                Info<< "\nFailed " << nFailedChecks << " mesh checks.\n"
                    << endl;
            }
            else
            {
                Info<< "\nMesh OK.\n" << endl;
            }


            // Write selected fields
            CML::writeFields(mesh, selectedFields);
        }
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
