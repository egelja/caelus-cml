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
    funkySetFields

Description

Contributors/Copyright:
    2010, 2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"

#include "PatchValueExpressionDriver.hpp"

#include "timeSelector.hpp"

#include "OFstream.hpp"

#include "printSwakVersion.hpp"

#include "RepositoryBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{

    CML::timeSelector::addOptions(false);

#   include "addRegionOption.hpp"

    argList::validOptions.insert("dict","<dictionary to use>");
    argList::validOptions.insert("cacheFields","");

#   include "setRootCase.hpp"

    printSwakVersion();

    word dictName="funkySetBoundaryDict";
    if(args.options().found("dict")) {
        dictName=args.options()["dict"];
    }

    bool cacheFields=args.options().found("cacheFields");
    if(cacheFields) {
        WarningInFunction
            << "The current behaviour is to cache fields that were read from disc. "
            << "This may lead to unexpected behaviour as previous modifications "
            << "will not be visible."
            << endl;
    }

#   include "createTime.hpp"
    CML::instantList timeDirs = CML::timeSelector::select0(runTime, args);

#   include "createNamedMesh.hpp"

    IOdictionary funkyDict
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

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        CML::Info<< "Time = " << runTime.timeName() << CML::endl;

        RepositoryBase::updateRepos();

        mesh.readUpdate();

        forAllIter(dictionary,funkyDict,it) {
            const dictionary &part=(*it).dict();

            word fieldName=part["field"];

            Info << "\n\nPart: " << (*it).keyword()
                << " working on field " << fieldName << endl;

            IOdictionary field(
                IOobject
                (
                    fieldName,
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                )
            );
            // deregister the dictionary so that the field can work on itself
            field.checkOut();
            {
                // this way it doesn't matter that the file is not of the right class
                IFstream inStream(field.filePath());
                field.readHeader(inStream);
                field.readData(inStream);
            }

            List<dictionary> expressions(part.lookup("expressions"));

            forAll(expressions,expressionI) {
                const dictionary &expression=expressions[expressionI];

                word target(expression["target"]);
                word patchName(expression["patchName"]);
                exprString expr(
                    expression["expression"],
                    expression
                );
                Info << "Setting " << target << " on " << patchName
                    << " the expression " << expr << endl;

                PatchValueExpressionDriver driver(expression,mesh);
                driver.setSearchBehaviour(
                    cacheFields,
                    false,
                    true             // search on disc
                );

                driver.clearVariables();
                driver.parse(expr);

                dictionary &patchDict=field.subDict("boundaryField").subDict(patchName);

                if(patchDict.found(target)) {
                    // Does not work (memory problem)
                    //                    patchDict.changeKeyword(keyType(target),keyType(word(target+"Old")),true);
                    if(patchDict.found(target+"Old")) {
                        patchDict.remove(target+"Old");
                    }
                    patchDict.changeKeyword(keyType(target),keyType(word(target+"Old")));
                }
                OStringStream result;
                string newEntry=driver.outputEntry();
                patchDict.set(target,newEntry.c_str());
            }

            {
                // this way the class is not overwritten
                word actualClass=field.headerClassName();

                OStringStream headerStream;
                field.writeHeader(headerStream);
                string newHeader=headerStream.str().replace("dictionary",actualClass);

                OFstream outStream(field.filePath());
                outStream << newHeader.c_str();
                field.writeData(outStream);
            }
        }
    }

    Info << "End\n" << endl;

    return 0;
}


// ************************************************************************* //
