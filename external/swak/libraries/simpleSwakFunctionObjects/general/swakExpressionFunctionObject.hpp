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
    swakExpressionFunctionObject

Description
    Can evaluate any swak-expression that can be produced by runTime-selection

SourceFiles
    swakExpressionFunctionObject.cpp

Contributors/Copyright:
    2010-2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef swakExpressionFunctionObject_H
#define swakExpressionFunctionObject_H

#include "timelineFunctionObject.hpp"

#include "CommonValueExpressionDriver.hpp"

#include "NumericAccumulationNamedEnum.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class swakExpressionFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class swakExpressionFunctionObject
:
    public timelineFunctionObject
{

    // Private Member Functions

    //- Disallow default bitwise copy construct
    swakExpressionFunctionObject(const swakExpressionFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const swakExpressionFunctionObject&);

    //- evaluate the expression
    template <class T>
    Field<T> evaluate(const word& fieldName,T preset) const;

    //- the expression that is to be evaluated
    exprString expression_;

    //- the mask expression
    exprString maskExpression_;

    //- list with the accumulations of evaluated expression
    List<NumericAccumulationNamedEnum::accuSpecification> accumulations_;

    //- the expression driver
    autoPtr<CommonValueExpressionDriver> driver_;
protected:

    word dirName();

    stringList columnNames();

    wordList fileNames();

    template<class T>
    void writeTheData(CommonValueExpressionDriver &driver);
public:

    //- Runtime type information
    TypeName("swakExpression");


    // Constructors

    //- Construct from components
    swakExpressionFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    //- Do the writing
    virtual void write();

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
void swakExpressionFunctionObject::writeTheData(
    CommonValueExpressionDriver &driver
)
{
    Field<T> result(driver.getResult<T>());

    autoPtr<AccumulationCalculation<T> > pCalculator;

    if(maskExpression_=="") {
        pCalculator.set(
            new AccumulationCalculation<T>(
                result,
                driver.result().isPoint(),
                driver
            )
        );
    } else {
        bool isPoint=driver.result().isPoint();

        driver.parse(maskExpression_);

        autoPtr<Field<bool> > maskValues;
        if(
            driver.CommonValueExpressionDriver::getResultType()
            ==
            pTraits<bool>::typeName
        ) {
            maskValues.reset(
                driver.getResult<bool>().ptr()
            );
        } else if(
            driver.CommonValueExpressionDriver::getResultType()
            ==
            pTraits<scalar>::typeName
        ) {
            scalarField rawMask(driver.getResult<scalar>());
            maskValues.reset(
                new Field<bool>(rawMask.size(),false)
            );
            forAll(rawMask,i) {
                if(rawMask[i]>SMALL) {
                    maskValues()[i]=true;
                }
            }
        } else {
            FatalErrorInFunction
                << "Don't know how to handle logical expressions of type "
                << driver.CommonValueExpressionDriver::getResultType()
                << " from " << maskExpression_
                << endl
                << exit(FatalError);
        }
        if(maskValues().size()!=result.size()) {
            FatalErrorInFunction
                << "Size of mask " << maskExpression_ << " not equal to "
                << expression_ << " (" << maskValues().size() << " vs "
                << result.size() << ")"
                << endl
                << exit(FatalError);
        }

        pCalculator.set(
            new AccumulationCalculation<T>(
                result,
                isPoint,
                driver,
                maskValues
            )
        );
    }

    AccumulationCalculation<T> &calculator=pCalculator();

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
        writeTime(name(),time().value());
        writeData(name(),results);
        endData(name());
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
