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

Class
    writeIfSwakExpressionFunctionObject

Description

    Check whether a swak-condition is met. If yes start writing
    again. Also offers the possibility to stop writing via an
    expression

SourceFiles
    writeIfSwakExpressionFunctionObject.cpp

Contributors/Copyright:
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef writeIfSwakExpressionFunctionObject_H
#define writeIfSwakExpressionFunctionObject_H

#include "conditionDrivenWritingFunctionObject.hpp"

#include "CommonValueExpressionDriver.hpp"

#include "LogicalAccumulationNamedEnum.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class writeIfSwakExpressionFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class writeIfSwakExpressionFunctionObject
:
    public conditionDrivenWritingFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    writeIfSwakExpressionFunctionObject(const writeIfSwakExpressionFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const writeIfSwakExpressionFunctionObject&);

    //- the expression driver
    autoPtr<CommonValueExpressionDriver> driver_;

    //- the logical expression
    exprString writeExpression_;

    //- possible logical accumulations
    LogicalAccumulationNamedEnum::value writeAccumulation_;

    //- the logical expression
    exprString stopWriteExpression_;

    //- possible logical accumulations
    LogicalAccumulationNamedEnum::value stopWriteAccumulation_;

    //- the logical expression
    exprString stopCooldownExpression_;

    //- possible logical accumulations
    LogicalAccumulationNamedEnum::value stopCooldownAccumulation_;

    void readParameters(const dictionary &);

    bool evaluateCondition(
        exprString expression,
        LogicalAccumulationNamedEnum::value accumulation
    );

protected:

    //- should writing start now?
    virtual bool checkStartWriting();

    //- switch off writing?
    virtual bool checkStopWriting();

    //- switch off cooldown?
    virtual bool checkStopCooldown();

public:

    //- Runtime type information
    TypeName("writeIfSwakExpression");


    // Constructors

    //- Construct from components
    writeIfSwakExpressionFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool read(const dictionary &dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
