/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
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

#include "randomExponentialPluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"
#include "FieldValuePluginFunction.hpp"
#include <Random.hpp>

#include "addToRunTimeSelectionTable.hpp"

namespace CML {

typedef randomExponentialPluginFunction<FieldValuePluginFunction,FieldValueExpressionDriver> randomExponentialPluginFunctionField;
defineTemplateTypeNameAndDebug(randomExponentialPluginFunctionField,0);

addNamedToRunTimeSelectionTable(FieldValuePluginFunction, randomExponentialPluginFunctionField , name, randomExponential);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template <typename FType,typename DType>
randomExponentialPluginFunction<FType,DType>::randomExponentialPluginFunction(
    const DType &parentDriver,
    const word &name
):
    FType(
        parentDriver,
        name,
        word("volScalarField"),
        string("seed primitive label,halfLife primitive scalar")
    ),
    halfLife_(1),
    seed_(666)
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <typename FType,typename DType>
void randomExponentialPluginFunction<FType,DType>::doEvaluationInternal(
    scalarField &f
) {
    label seed=seed_;

    if(seed<=0) {
        seed=this->mesh().time().timeIndex()-seed;
    }

    Random rand(seed);

    forAll(f,i) {
        f[i]=-log(1-rand.scalar01())*halfLife_;
    }
}

template <typename FType,typename DType>
void randomExponentialPluginFunction<FType,DType>::doEvaluation()
{
    autoPtr<volScalarField> pRandom(
        new volScalarField(
            IOobject(
                "exponentialRandom",
                this->mesh().time().timeName(),
                this->mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            this->mesh(),
            dimensionedScalar("randomExp",dimless,0),
            "zeroGradient"
        )
    );

    doEvaluationInternal(pRandom->internalField());

    this->result().setObjectResult(pRandom);
}

template <typename FType,typename DType>
void randomExponentialPluginFunction<FType,DType>::setArgument(
    label index,
    const label &val
)
{
    assert(index==0);
    seed_=val;
}

template <typename FType,typename DType>
void randomExponentialPluginFunction<FType,DType>::setArgument(
    label index,
    const scalar &val
)
{
    assert(index==1);
    halfLife_=val;
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
