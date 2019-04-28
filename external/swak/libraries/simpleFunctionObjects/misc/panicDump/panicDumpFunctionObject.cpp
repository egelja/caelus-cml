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
    2008-2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "panicDumpFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(panicDumpFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        panicDumpFunctionObject,
        dictionary
    );

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

panicDumpFunctionObject::panicDumpFunctionObject
(
    const word &name,
    const Time& t,
    const dictionary& dict
)
:
    simpleFunctionObject(name,t,dict),
    fieldName_(""),
    maximum_(HUGE_VAL),
    minimum_(-HUGE_VAL),
    storeAndWritePreviousState_(false)
{
}

bool panicDumpFunctionObject::start()
{
    simpleFunctionObject::start();

    fieldName_=word(dict_.lookup("fieldName"));
    minimum_=readScalar(dict_.lookup("minimum"));
    maximum_=readScalar(dict_.lookup("maximum"));

    Info << "Checking for field " << fieldName_ << " in range [ " << minimum_
        << " , " << maximum_ << " ] " << endl;

    if(dict_.found("storeAndWritePreviousState")) {
        storeAndWritePreviousState_=readBool(
            dict_.lookup("storeAndWritePreviousState")
        );
        if(storeAndWritePreviousState_) {
            Info << name() << " stores the previous time-steps" << endl;
            lastTimes_.set(
                new TimeCloneList(
                    dict_
                )
            );
        }
    } else {
        WarningInFunction
            << "storeAndWritePreviousState not set in" << dict_.name() << endl
                << "Assuming 'false'"
                << endl;

    }

    return true;
}

void panicDumpFunctionObject::write()
{
    check<volScalarField>();
    check<volVectorField>();
    check<volSphericalTensorField>();
    check<volSymmTensorField>();
    check<volTensorField>();

    if(lastTimes_.valid()) {
        lastTimes_->copy(time());
    }
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

} // namespace CML

// ************************************************************************* //
