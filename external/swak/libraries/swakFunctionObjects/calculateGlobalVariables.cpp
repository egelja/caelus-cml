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

#include "calculateGlobalVariables.hpp"

#include "GlobalVariablesRepository.hpp"

namespace CML {
    defineTypeNameAndDebug(calculateGlobalVariables,0);
}

CML::calculateGlobalVariables::calculateGlobalVariables
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
    :
    obr_(obr),
    driver_(
        CommonValueExpressionDriver::New(
            dict,
            refCast<const fvMesh>(obr)
        )
    ),
    toGlobalNamespace_(dict.lookup("toGlobalNamespace")),
    toGlobalVariables_(dict.lookup("toGlobalVariables")),
    noReset_(dict.lookupOrDefault<bool>("noReset",false))
{
    if(debug) {
        Info << "calculateGlobalVariables " << name << " created" << endl;
    }

    if(!dict.found("noReset")) {
        WarningInFunction
            << "No entry 'noReset' in " << dict.name()
            << ". Assumig 'false'"<< endl;
    }

    driver_->createWriterAndRead(name+"_"+type());

    executeAndWriteToGlobal();
}

CML::calculateGlobalVariables::~calculateGlobalVariables()
{}

void CML::calculateGlobalVariables::executeAndWriteToGlobal()
{
    // this also sets the variables
    driver_->clearVariables();

    forAll(toGlobalVariables_,i) {
        const word &name=toGlobalVariables_[i];
        if(debug) {
            Info << "Getting variable " << name << endl;
        }

        ExpressionResult &res=GlobalVariablesRepository::getGlobalVariables(
            obr_
        ).addValue(
            name,
            toGlobalNamespace_,
            const_cast<const CommonValueExpressionDriver&>(
                driver_()
            ).variable(name)
        );

        if(noReset_) {
            res.noReset();
        }

        if(debug) {
            Pout << "Has value "
                << const_cast<const CommonValueExpressionDriver&>(
                    driver_()
                ).variable(name) << endl;
        }
    }
}

void CML::calculateGlobalVariables::timeSet()
{
    // Do nothing
}

void CML::calculateGlobalVariables::read(const dictionary& dict)
{
    WarningInFunction
        << "This function object does not honor changes during runtime"
        << endl;
}

void CML::calculateGlobalVariables::write()
{
    executeAndWriteToGlobal();

    // make sure that the stored Variables are consistently written
    driver_->tryWrite();
}


void CML::calculateGlobalVariables::end()
{
}

void CML::calculateGlobalVariables::execute()
{
}

void CML::calculateGlobalVariables::clearData()
{
}

// ************************************************************************* //
