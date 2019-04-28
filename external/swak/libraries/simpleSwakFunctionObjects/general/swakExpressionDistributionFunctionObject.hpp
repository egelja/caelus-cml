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
    swakExpressionDistributionFunctionObject

Description
    Distribution of a swak-expression

SourceFiles
    swakExpressionDistributionFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef swakExpressionDistributionFunctionObject_H
#define swakExpressionDistributionFunctionObject_H

#include "distributionFunctionObject.hpp"

#include "CommonValueExpressionDriver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class swakExpressionDistributionFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class swakExpressionDistributionFunctionObject
:
    public distributionFunctionObject
{

    // Private Member Functions

    //- Disallow default bitwise copy construct
    swakExpressionDistributionFunctionObject(const swakExpressionDistributionFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const swakExpressionDistributionFunctionObject&);


    //- the expression that is to be evaluated
    exprString expression_;

    //- the expression for the weight
    exprString weightExpression_;

    //- the expression for the mask
    exprString maskExpression_;

    //- the expression driver
    autoPtr<CommonValueExpressionDriver> driver_;

    template <typename T>
    void getDistributionInternal(
        autoPtr<SimpleDistribution<T> > &dist,
        autoPtr<Field<T> > &sameWeight
    );

    autoPtr<Field<scalar> > weightValuesScalar_;
    autoPtr<Field<vector> > weightValuesVector_;
    autoPtr<Field<tensor> > weightValuesTensor_;
    autoPtr<Field<symmTensor> > weightValuesSymmTensor_;
    autoPtr<Field<sphericalTensor> > weightValuesSphericalTensor_;

    autoPtr<Field<bool> > maskValues_;

protected:

    word dirName();

    virtual word baseName();

    virtual void getDistribution();

public:

    //- Runtime type information
    TypeName("swakExpressionDistribution");


    // Constructors

    //- Construct from components
    swakExpressionDistributionFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    void write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "volFields.hpp"
#include "IOmanip.hpp"
#include "fvMesh.hpp"
#include "fvCFD.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <typename T>
void swakExpressionDistributionFunctionObject::getDistributionInternal(
    autoPtr<SimpleDistribution<T> > &dist,
    autoPtr<Field<T> > &sameWeight
) {
    if(sameWeight.valid()) {
        dist=setData(
            driver_->getResult<T>()(),
            sameWeight(),
            maskValues_()
        );
    } else if(weightValuesScalar_.valid()) {
        dist=setDataScalar(
            driver_->getResult<T>()(),
            weightValuesScalar_(),
            maskValues_()
        );
    } else {
        FatalErrorInFunction
            << "Weight neither of type " << pTraits<scalar>::typeName
            << " nor " << pTraits<T>::typeName
            << endl
            << "Set weights are: "
            << pTraits<scalar>::typeName << ":"
            << weightValuesScalar_.valid() << " "
            << pTraits<vector>::typeName << ":"
            << weightValuesVector_.valid() << " "
            << pTraits<tensor>::typeName << ":"
            << weightValuesTensor_.valid() << " "
            << pTraits<symmTensor>::typeName << ":"
            << weightValuesSymmTensor_.valid() << " "
            << pTraits<sphericalTensor>::typeName << ":"
            << weightValuesSphericalTensor_.valid() << " "
            << exit(FatalError);
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
