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
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "writeAndEndFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(writeAndEndFunctionObject, 0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

writeAndEndFunctionObject::writeAndEndFunctionObject
(
    const word &name,
    const Time& t,
    const dictionary& dict
)
:
    simpleFunctionObject(name,t,dict),
    isStopped_(false)
{
}

bool writeAndEndFunctionObject::start()
{
    if(debug) {
        Info << name() << "::start() - Entering" << endl;
    }

    simpleFunctionObject::start();

    if(debug) {
        Info << name() << "::start() - Leaving" << endl;
    }

    return true;
}

void writeAndEndFunctionObject::write()
{
    if(debug) {
        Info << name() << "::write() - Entering" << endl;
    }
    if(isStopped()) {
        if(debug) {
            Info << name() << "::write() - isStopped" << endl;
        }
        return;
    }
    if(
        this->endRunNow()
    ) {
        if(debug) {
            Info << name() << "::write() - stopping" << endl;
        }
        isStopped_=true;

        Info << "Ending run because of functionObject " << this->name() << endl;
        const_cast<Time &>(time()).writeAndEnd();
    }
    if(debug) {
        Info << name() << "::write() - Leaving" << endl;
    }
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

} // namespace CML

// ************************************************************************* //
