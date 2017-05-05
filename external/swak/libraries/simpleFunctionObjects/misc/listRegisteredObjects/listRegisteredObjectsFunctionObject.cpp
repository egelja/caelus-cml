/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
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

Contributors/Copyright:
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "listRegisteredObjectsFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(listRegisteredObjectsFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        listRegisteredObjectsFunctionObject,
        dictionary
    );

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

listRegisteredObjectsFunctionObject::listRegisteredObjectsFunctionObject
(
    const word &name,
    const Time& t,
    const dictionary& dict
)
:
    simpleFunctionObject(name,t,dict)
{
}

bool listRegisteredObjectsFunctionObject::start()
{
    Info << "Content of object registry " << obr().name()
        << " at start" << endl;

    dumpObr();

    return true;
}

void listRegisteredObjectsFunctionObject::write()
{
    Info << "Content of object registry " << obr().name()
        << endl;

    dumpObr();
}

void listRegisteredObjectsFunctionObject::dumpObr()
{
    wordList toc(obr().toc());
    sort(toc);

    const string nameConst("Name");
    unsigned int maxNameLen(nameConst.size());
    const string typeConst("Type");
    unsigned int maxTypeLen(typeConst.size());
    const string autoWriteConst("Autowrite");

    forAll(toc,i) {
        const regIOobject &o=*(obr()[toc[i]]);
        if(o.name().size()>maxNameLen) {
            maxNameLen=o.name().size();
        }
        if(o.headerClassName().size()>maxTypeLen) {
            maxTypeLen=o.headerClassName().size();
        }
    }

    Info().width(maxNameLen);
    Info << nameConst.c_str() << " ";
    Info().width(maxTypeLen);
    Info << typeConst.c_str() << " " << autoWriteConst.c_str() << endl;
    for(unsigned int i=0;i<maxNameLen;i++) Info << "=";
    Info << " ";
    for(unsigned int i=0;i<maxTypeLen;i++) Info << "=";
    Info << " ";
    for(unsigned int i=0;i<autoWriteConst.size();i++) Info << "=";
    Info << endl;
    forAll(toc,i) {
        const regIOobject &o=*(obr()[toc[i]]);
        Info().width(maxNameLen);
        Info << o.name().c_str() << " ";
        Info().width(maxTypeLen);
        Info << o.headerClassName().c_str() << " ";
        if(o.writeOpt()==IOobject::AUTO_WRITE) {
            Info << "Yes";
        } else {
            Info << "No";
        }
        Info << endl;
    }
    Info << endl;
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

} // namespace CML

// ************************************************************************* //
