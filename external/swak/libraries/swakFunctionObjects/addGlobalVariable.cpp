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

#include "addGlobalVariable.hpp"

#include "GlobalVariablesRepository.hpp"

namespace CML {
    defineTypeNameAndDebug(addGlobalVariable,0);
}

CML::addGlobalVariable::addGlobalVariable
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

CML::addGlobalVariable::~addGlobalVariable()
{}

void CML::addGlobalVariable::timeSet()
{
    // Do nothing
}

void CML::addGlobalVariable::read(const dictionary& dict)
{
    if(dict.found("globalVariables")) {
        const dictionary variables(dict.subDict("globalVariables"));
        const word scope(dict.lookup("globalScope"));

        wordList names(variables.toc());
        forAll(names,i) {
            const word &name=names[i];
            const dictionary &dict=variables.subDict(name);

            ExpressionResult &res=GlobalVariablesRepository::getGlobalVariables(
                obr_
            ).addValue(
                name,
                scope,
                ExpressionResult(dict,true,true),
                false
            );
            res.noReset();
        }
    } else {
        ExpressionResult &res=GlobalVariablesRepository::getGlobalVariables(
            obr_
        ).addValue(
            dict,
            "",
            false
        );
        res.noReset();
    }
}

void CML::addGlobalVariable::execute()
{
}


void CML::addGlobalVariable::end()
{
}

void CML::addGlobalVariable::write()
{
}

void CML::addGlobalVariable::clearData()
{
}

// ************************************************************************* //
