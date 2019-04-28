/*---------------------------------------------------------------------------*\
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

\*---------------------------------------------------------------------------*/

#include "executeIfSwakExpressionFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"
#include "argList.hpp"

#include "objectRegistry.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(executeIfSwakExpressionFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        executeIfSwakExpressionFunctionObject,
        dictionary
    );

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

executeIfSwakExpressionFunctionObject::executeIfSwakExpressionFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    conditionalFunctionObjectListProxy(
        name,
        t,
        dict
    )
{
    readParameters(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool executeIfSwakExpressionFunctionObject::condition()
{
    driver_->clearVariables();
    driver_->parse(logicalExpression_);

    if(driver_->CommonValueExpressionDriver::getResultType()!=pTraits<bool>::typeName) {
        FatalErrorInFunction
            << "Logical Expression " << logicalExpression_
            << " evaluates to type " << driver_->CommonValueExpressionDriver::getResultType()
            << " when it should be " << pTraits<bool>::typeName
            << endl
            << exit(FatalError);
    }

    bool result=false;

    switch(logicalAccumulation_) {
        case LogicalAccumulationNamedEnum::logAnd:
            result=driver_->getReduced(andOp<bool>(),true);
            break;
        case LogicalAccumulationNamedEnum::logOr:
            result=driver_->getReduced(orOp<bool>(),false);
            break;
        default:
            FatalErrorInFunction
                << "Unimplemented logical accumulation "
                << LogicalAccumulationNamedEnum::names[logicalAccumulation_]
                << endl
                << exit(FatalError);
    }
    if(writeDebug()) {
        Info << "Expression " << logicalExpression_
            << " evaluates to " << driver_->getResult<bool>() << endl;
        Info << " -> " <<  LogicalAccumulationNamedEnum::names[logicalAccumulation_]
            << " gives " << result << endl;
    }

    return result;
}

bool executeIfSwakExpressionFunctionObject::read(const dictionary& dict)
{
    readParameters(dict);
    return conditionalFunctionObjectListProxy::read(dict);
}

void executeIfSwakExpressionFunctionObject::readParameters(const dictionary &dict)
{
    driver_=CommonValueExpressionDriver::New(
        dict,
        refCast<const fvMesh>(obr_)
    );

    logicalExpression_=exprString(
        dict.lookup("logicalExpression"),
        dict
    );

    logicalAccumulation_=LogicalAccumulationNamedEnum::names[dict.lookup("logicalAccumulation")];
}

} // namespace CML

// ************************************************************************* //
