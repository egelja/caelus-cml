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
    writeAndEndSwakExpressionFunctionObject

Description
    Check whether a swak-condition is met. If yes everything is written to disk
    and the run ends gracefully

SourceFiles
    writeAndEndSwakExpressionFunctionObject.cpp

Contributors/Copyright:
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef writeAndEndSwakExpressionFunctionObject_H
#define writeAndEndSwakExpressionFunctionObject_H

#include "writeAndEndFunctionObject.hpp"

#include "CommonValueExpressionDriver.hpp"

#include "LogicalAccumulationNamedEnum.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class writeAndEndSwakExpressionFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class writeAndEndSwakExpressionFunctionObject
:
    public writeAndEndFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    writeAndEndSwakExpressionFunctionObject(const writeAndEndSwakExpressionFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const writeAndEndSwakExpressionFunctionObject&);

    //- the expression driver
    autoPtr<CommonValueExpressionDriver> driver_;

    //- the logical expression
    exprString logicalExpression_;

    void readParameters(const dictionary &);

private:
    //- possible logical accumulations
    LogicalAccumulationNamedEnum::value logicalAccumulation_;

protected:

    //- do the actual checking
    virtual bool endRunNow();

public:

    //- Runtime type information
    TypeName("writeAndEndSwakExpression");


    // Constructors

    //- Construct from components
    writeAndEndSwakExpressionFunctionObject
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
