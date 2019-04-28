/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2005 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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
    funkyWarpMesh

Description
    Warp mesh with new coordinates calculated for the points

Contributors/Copyright:
    2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"

#include "FieldValueExpressionDriver.hpp"

#include "printSwakVersion.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{
#   include "addRegionOption.hpp"
    argList::validOptions.insert("overwrite", "");
    argList::validOptions.insert("expression","expression to write");
    argList::validOptions.insert("dictExt","Extension to the dictionary");
    argList::validOptions.insert("relative", "");

#   include "setRootCase.hpp"

    printSwakVersion();

#   include "createTime.hpp"
    runTime.functionObjects().off();

    CML::instantList timeDirs = CML::timeSelector::select0(runTime, args);

#   include "createNamedMesh.hpp"
    const word oldInstance = mesh.pointsInstance();

    bool overwrite    = args.optionFound("overwrite");
    bool relative     = false;

    pointField newPoints;

    if (!overwrite)
    {
        runTime++;
    }

    if(args.optionFound("expression")) {
        if(args.optionFound("dictExt")) {
            FatalErrorInFunction
                << "Can't specify 'dictExt' and 'expression' at the same time"
                << endl
                << exit(FatalError);

        }
        relative=args.optionFound("relative");
        exprString expression(
            args.options()["expression"],
            dictionary::null
        );
        FieldValueExpressionDriver driver(
            runTime.timeName(),
            runTime,
            mesh
        );
        // no clearVariables needed here
        driver.parse(expression);
        if(!driver.resultIsTyp<pointVectorField>()) {
            FatalErrorInFunction
                << "Expression " << expression
                << " does not evaluate to a pointVectorField but a "
                << driver.typ()
                << endl
                << exit(FatalError);
        }
        newPoints=driver.getResult<pointVectorField>().internalField();
    } else {
        Info << "Dictionary mode" << nl << endl;
        if(args.optionFound("relative")) {
            FatalErrorInFunction
                << "Option 'relative' not allowed in dictionary-mode"
                << endl
                << exit(FatalError);
        }
        word dictName("funkyWarpMeshDict");
        if(args.optionFound("dictExt")) {
            dictName+="."+word(args.options()["dictExt"]);
        }
        IOdictionary warpDict
            (
                IOobject
                (
                    dictName,
                    runTime.system(),
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            );
        const word mode(warpDict.lookup("mode"));
        if(mode=="set") {
            relative=readBool(warpDict.lookup("relative"));
            exprString expression(
                warpDict.lookup("expression"),
                warpDict
            );
            FieldValueExpressionDriver driver(
                runTime.timeName(),
                runTime,
                mesh
            );
            driver.readVariablesAndTables(warpDict);
            driver.clearVariables();

            driver.parse(expression);
            if(!driver.resultIsTyp<pointVectorField>()) {
                FatalErrorInFunction
                    << "Expression " << expression
                    << " does not evaluate to a pointVectorField but a "
                    << driver.typ()
                    << endl
                    << exit(FatalError);
            }
            newPoints=driver.getResult<pointVectorField>().internalField();
        } else if (mode=="move") {
            NotImplemented;
        } else {
            FatalErrorInFunction
                << "Possible values for 'mode' are 'set' or 'move'"
                << endl
                << exit(FatalError);
        }
    }

    if(relative) {
        newPoints += mesh.points();
    }

    mesh.movePoints(newPoints);

    // Write mesh
    if (overwrite)
    {
        mesh.setInstance(oldInstance);
    }
    Info << nl << "Writing polyMesh to time " << runTime.timeName() << endl;

    IOstream::defaultPrecision(15);

    // Bypass runTime write (since only writes at outputTime)
    if
    (
       !runTime.objectRegistry::writeObject
        (
            runTime.writeFormat(),
            IOstream::currentVersion,
            runTime.writeCompression()
        )
    )
    {
        FatalErrorInFunction
            << "Failed writing polyMesh."
            << exit(FatalError);
    }

    // Write points goes here

    // Write fields
    runTime.write();

    Info << "End\n" << endl;

    Info << nl << "Now run 'checkMesh' before you do anything else"
        << nl << endl;

    return 0;
}


// ************************************************************************* //
