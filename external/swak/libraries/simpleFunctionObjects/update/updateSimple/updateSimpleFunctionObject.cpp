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

#include "updateSimpleFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

    defineTypeNameAndDebug(updateSimpleFunctionObject, 0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

updateSimpleFunctionObject::updateSimpleFunctionObject
(
    const word &name,
    const Time& t,
    const dictionary& dict
)
:
    simpleFunctionObject(name,t,dict)
{
}

bool updateSimpleFunctionObject::start()
{
    simpleFunctionObject::start();

    runIfStartTime_=dict_.lookupOrDefault<bool>("runIfStartTime",false);
    onlyAtStartup_=readBool(dict_.lookup("onlyAtStartup"));
    if(onlyAtStartup_) {
        runIfStartTime_=readBool(dict_.lookup("runIfStartTime"));
    }

    if(onlyAtStartup_) {
        if(
            !runIfStartTime_
            ||
            time().timeIndex()==0
        ) {
            recalc();
        }
    }

    return true;
}

void updateSimpleFunctionObject::write()
{
    if(!onlyAtStartup_) {
        recalc();
    }
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

} // namespace CML

// ************************************************************************* //
