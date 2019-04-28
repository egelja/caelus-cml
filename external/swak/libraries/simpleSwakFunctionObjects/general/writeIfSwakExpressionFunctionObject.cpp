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
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "writeIfSwakExpressionFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(writeIfSwakExpressionFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        writeIfSwakExpressionFunctionObject,
        dictionary
    );

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

writeIfSwakExpressionFunctionObject::writeIfSwakExpressionFunctionObject
(
    const word &name,
    const Time& t,
    const dictionary& dict
)
:
    conditionDrivenWritingFunctionObject(name,t,dict)
{
    readParameters(dict);
}

bool writeIfSwakExpressionFunctionObject::read(const dictionary& dict)
{
    readParameters(dict);
    return conditionDrivenWritingFunctionObject::read(dict);
}

void writeIfSwakExpressionFunctionObject::readParameters(const dictionary &dict)
{
    driver_=CommonValueExpressionDriver::New(
        dict,
        refCast<const fvMesh>(obr_)
    );

    writeExpression_=exprString(
        dict.lookup("writeCondition"),
        dict
    );

    writeAccumulation_=LogicalAccumulationNamedEnum::names[
        dict.lookup("writeConditionAccumulation")
    ];

    if(cooldownMode()==cdmRetrigger) {
        stopCooldownExpression_=exprString(
            dict.lookup("retriggerCondition"),
            dict
        );

        stopCooldownAccumulation_=LogicalAccumulationNamedEnum::names[
            dict.lookup("retriggerConditionAccumulation")
        ];
    }
    if(writeControlMode()==scmWriteUntilSwitch) {
        stopWriteExpression_=exprString(
            dict.lookup("stopWritingCondition"),
            dict
        );

        stopWriteAccumulation_=LogicalAccumulationNamedEnum::names[
            dict.lookup("stopWritingConditionAccumulation")
        ];
    }
}

bool writeIfSwakExpressionFunctionObject::checkStartWriting() {
    bool result=evaluateCondition(writeExpression_,writeAccumulation_);

    return result;
}

bool writeIfSwakExpressionFunctionObject::checkStopWriting() {
    bool result=evaluateCondition(stopWriteExpression_,stopWriteAccumulation_);

    return result;
}

bool writeIfSwakExpressionFunctionObject::checkStopCooldown() {
    bool result=evaluateCondition(stopCooldownExpression_,stopCooldownAccumulation_);

    return result;
}


bool writeIfSwakExpressionFunctionObject::evaluateCondition(
        exprString expression,
        LogicalAccumulationNamedEnum::value accumulation
) {
    driver_->clearVariables();
    driver_->parse(expression);

    if(
        driver_->CommonValueExpressionDriver::getResultType()
        !=
        pTraits<bool>::typeName
    ) {
        FatalErrorInFunction
            << "Logical Expression " << expression
            << " evaluates to type "
            << driver_->CommonValueExpressionDriver::getResultType()
            << " when it should be " << pTraits<bool>::typeName
            << endl
            << exit(FatalError);
    }

    bool result=false;

    switch(accumulation) {
        case LogicalAccumulationNamedEnum::logAnd:
            result=driver_->getReduced(andOp<bool>(),true);
            break;
        case LogicalAccumulationNamedEnum::logOr:
            result=driver_->getReduced(orOp<bool>(),false);
            break;
        default:
            FatalErrorInFunction
                << "Unimplemented logical accumulation "
                << LogicalAccumulationNamedEnum::names[accumulation]
                << endl
                << exit(FatalError);
    }
    if(writeDebug()) {
        Info << "Expression " << expression
            << " evaluates to " << driver_->getResult<bool>() << endl;
        Info << " -> "
            << LogicalAccumulationNamedEnum::names[accumulation]
            << " gives " << result << endl;
    }

    Info << name() << ": " << expression << " evaluated to " << (result ? "true" : "false" ) << endl;

    return result;
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

} // namespace CML

// ************************************************************************* //
