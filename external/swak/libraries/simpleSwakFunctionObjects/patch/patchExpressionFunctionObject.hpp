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
    patchExpressionFunctionObject

Description
    Evaluates an expression of the fields on the patches

SourceFiles
    patchExpressionFunctionObject.cpp

Contributors/Copyright:
    2010-2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef patchExpressionFunctionObject_H
#define patchExpressionFunctionObject_H

#include "patchFunctionObject.hpp"

#include "PatchValueExpressionDriver.hpp"

#include "NumericAccumulationNamedEnum.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class patchExpressionFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class patchExpressionFunctionObject
:
    public patchFunctionObject
{

    // Private Member Functions

    //- Disallow default bitwise copy construct
    patchExpressionFunctionObject(const patchExpressionFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const patchExpressionFunctionObject&);

    //- integrate single field on all patches
    template <class T>
    Field<T> evaluate(const word& fieldName,T preset) const;

    //- the expression that is to be evaluated
    exprString expression_;

    //- the specification
    dictionary data_;

    //- list with the accumulations of evaluated expression
    List<NumericAccumulationNamedEnum::accuSpecification> accumulations_;

    PtrList<PatchValueExpressionDriver> drivers_;

protected:

    word dirName();

    stringList columnNames();

    wordList fileNames();

    template<class T>
    void writeTheData(const word &pName,PatchValueExpressionDriver &driver);
public:

    //- Runtime type information
    TypeName("patchExpression");


    // Constructors

    //- Construct from components
    patchExpressionFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    //- Do the writing
    virtual void write();

    //- read the stuff
    virtual bool start();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "volFields.hpp"
#include "IOmanip.hpp"
#include "fvMesh.hpp"
#include "fvCFD.hpp"

#include "AccumulationCalculation.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class T>
void patchExpressionFunctionObject::writeTheData(const word &pName,PatchValueExpressionDriver &driver)
{
    Field<T> result(driver.getResult<T>());

    AccumulationCalculation<T> calculator(
        result,
        driver.result().isPoint(),
        driver
    );

    Field<T> results(accumulations_.size());

    forAll(accumulations_,i) {
        const NumericAccumulationNamedEnum::accuSpecification accu=
            accumulations_[i];
        T val=calculator(accu);

        results[i]=val;
        if(verbose()) {
            Info << " " << accu
                << "=" << val;
        }
    }

    if (Pstream::master()) {
        writeTime(pName,time().value());
        writeData(pName,results);
        endData(pName);
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
