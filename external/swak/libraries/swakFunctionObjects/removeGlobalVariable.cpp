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
    2011-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
    2013 Bruno Santos <wyldckat@gmail.com>

\*---------------------------------------------------------------------------*/

#include "removeGlobalVariable.hpp"

#include "GlobalVariablesRepository.hpp"

namespace CML {
    defineTypeNameAndDebug(removeGlobalVariable,0);
}

CML::removeGlobalVariable::removeGlobalVariable
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
    :
    obr_(obr)
{
    read(dict);
    execute();
}

CML::removeGlobalVariable::~removeGlobalVariable()
{}

void CML::removeGlobalVariable::timeSet()
{
    // Do nothing
}

void CML::removeGlobalVariable::read(const dictionary& dict)
{
    names_=wordList(dict.lookup("globalVariables"));
    scope_=word(dict.lookup("globalScope"));
}

void CML::removeGlobalVariable::execute()
{
   forAll(names_,i) {
        const word &name=names_[i];

        bool removed=GlobalVariablesRepository::getGlobalVariables(
            obr_
        ).removeValue(
            name,
            scope_
        );
        if(!removed) {
            WarningInFunction
                << "Variable " << name << " in scope " << scope_
                << " not removed" << endl;
        }
   }
}


void CML::removeGlobalVariable::end()
{
}

void CML::removeGlobalVariable::write()
{
}

void CML::removeGlobalVariable::clearData()
{
}

// ************************************************************************* //
