/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Application
    postProcessWaves

Description
    Utility to carry out a wide range of post-processing operations, which are
    typically related to coastal and offshore engineering topics. The post-
    processing utility can, however, also be used on other data sets, e.g.
    general probing of velocities.

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "Time.hpp"
#include "fvMesh.hpp"

#include "postProcessingWaves.hpp"
#include "uniformDimensionedFields.hpp"

using namespace CML;

int main(int argc, char *argv[])
{

#   include "addTimeOptions.hpp"
#   include "setRootCase.hpp"

#   include "createTime.hpp"

    // Needed by e.g. the reflection analysis
    Info << "\nReading g" << endl;
    uniformDimensionedVectorField g
    (
        IOobject
        (
            "g",
            runTime.constant(),
            runTime,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // Read the dataProcessing dictionary
    IOdictionary postProcProperties
    (
        IOobject
        (
            "postProcessingProperties",
            "constant",
            runTime,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // Potentially clean out prior directory postProcessedWaves2CML
    Switch deleteOutput
        (
            postProcProperties.lookup("deleteParentOutputDirectory")
        );

    if (deleteOutput)
    {
        fileName dirName( "postProcessedWaves" );

        if (isDir(dirName))
        {
            Info << "Deleting 'postProcessedWaves'.\n" << endl;

            CML::rmDir( dirName );
        }
    }

    // Get a list of sub-dictionaries in dictionary DP
    wordList toc( postProcProperties.toc() );

    // Loop over all items in TOC
    forAll (toc, itemi)
    {
        word tocName( toc[itemi] );

        if (postProcProperties.isDict(tocName))
        {
            Info << "Processing: " << tocName << endl;

            const dictionary& subDict( postProcProperties.subDict( tocName ) );

            wordList actionList( subDict.lookup("actionList") );

            // Loop over the action list for each process
            forAll (actionList, actionItem)
            {
                Info << "    Processing sub-action: "
                     << actionList[actionItem] << endl;
                autoPtr<postProcessingWaves> action
                    (
                        postProcessingWaves::New
                        (
                            runTime,
                            subDict,
                            actionList[actionItem]
                        )
                    );

                action->evaluate();
            }
            Info << endl;
        }
    }

    Info << nl << "End" << endl;

    return 0;
}

