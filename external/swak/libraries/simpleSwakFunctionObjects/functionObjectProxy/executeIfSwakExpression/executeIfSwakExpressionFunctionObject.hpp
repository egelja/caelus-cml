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

Class
    executeIfSwakExpressionFunctionObject

Description
    This proxy-functionObject only executs its childrens if a
    swak-expression evaluates to true

SourceFiles
    executeIfSwakExpressionFunctionObject.cpp

\*---------------------------------------------------------------------------*/

#ifndef executeIfSwakExpressionFunctionObject_H
#define executeIfSwakExpressionFunctionObject_H

#include "conditionalFunctionObjectListProxy.hpp"

#include "CommonValueExpressionDriver.hpp"

#include "LogicalAccumulationNamedEnum.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class executeIfSwakExpressionFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class executeIfSwakExpressionFunctionObject
:
    public conditionalFunctionObjectListProxy
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    executeIfSwakExpressionFunctionObject(const executeIfSwakExpressionFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const executeIfSwakExpressionFunctionObject&);

    //- the expression driver
    autoPtr<CommonValueExpressionDriver> driver_;

    //- the logical expression
    exprString logicalExpression_;

    void readParameters(const dictionary &);

private:
    LogicalAccumulationNamedEnum::value logicalAccumulation_;

protected:

    //- the actual conition that determines whether the FO should be executed
    virtual bool condition();

public:

    //- Runtime type information
    TypeName("executeIfSwakExpression");


    // Constructors

    //- Construct from components
    executeIfSwakExpressionFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool read(const dictionary& dict);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
