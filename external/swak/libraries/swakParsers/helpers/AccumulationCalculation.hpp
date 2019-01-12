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

Class
    CML::AccumulationCalculation

Description
    Calculates the accumulations for a field. The purpose of this utilitiy
    class is to cache important intermediate results and thus make sure that
    - they are not calculated twice
    - parallel communication is minimized
    Also should it collect the implementation of all possible accumulations
    in one place

SourceFiles
    AccumulationCalculation.cpp

Contributors/Copyright:
    2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef AccumulationCalculation_H
#define AccumulationCalculation_H

#include "NumericAccumulationNamedEnum.hpp"
#include "CommonValueExpressionDriver.hpp"
#include "SimpleDistribution.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class AccumulationCalculation Declaration
\*---------------------------------------------------------------------------*/

template <typename Type>
class AccumulationCalculation
{
    // Private data

    // Constructors

    //- Disallow default bitwise assignment
    void operator=(const AccumulationCalculation&);

    //- Construct null
    AccumulationCalculation(const IOobject &);

    //- the data
    const Field<Type> &data_;

    //- mask that selects what part of the data to use
    const Field<bool> mask_;

    //- the data that is actually used
    autoPtr<Field<Type> > usedData_;

    //- is this point data?
    const bool isPoint_;

    //- the driver
    const CommonValueExpressionDriver &driver_;

    //- weights on demand
    autoPtr<scalarField> weights_;

    //- on demand distribution
    autoPtr<SimpleDistribution<Type> > distribution_;

    //- on demand weighted distribution
    autoPtr<SimpleDistribution<Type> > weightedDistribution_;

    bool hasWeightSum_;
    scalar weightSum_;

    bool hasSize_;
    label size_;

    bool hasMaximum_;
    Type maximum_;

    bool hasMinimum_;
    Type minimum_;

    bool hasAverage_;
    Type average_;

    bool hasWeightedAverage_;
    Type weightedAverage_;

    bool hasSum_;
    Type sum_;

    bool hasWeightedSum_;
    Type weightedSum_;

    bool hasSumMag_;
    Type sumMag_;

    static const label numberOfBins=100;

    label maskSize() const;

protected:

    const scalarField &weights();

    const Field<Type> &data();

public:
     //- Runtime type information
    //    TypeName("AccumulationCalculation");

    //- Constructor
    AccumulationCalculation(
        const Field<Type> &data,
        bool isPoint,
        CommonValueExpressionDriver &driver
    );

     //- Constructor with a mask
    AccumulationCalculation(
        const Field<Type> &data,
        bool isPoint,
        CommonValueExpressionDriver &driver,
        const Field<bool> &mask
    );

    //- Destructor

    ~AccumulationCalculation();

    const SimpleDistribution<Type> &distribution();

    const SimpleDistribution<Type> &weightedDistribution();

    label size();
    scalar weightSum();

    Type maximum();
    Type minimum();
    Type average();
    Type weightedAverage();
    Type sum();
    Type sumMag();
    Type weightedSum();

    //- do an accumulation according to a specification
    Type operator()(const NumericAccumulationNamedEnum::accuSpecification &);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML {

    // defineTemplateTypeNameAndDebug(AccumulationCalculation, 0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template <typename Type>
AccumulationCalculation<Type>::AccumulationCalculation(
    const Field<Type> &data,
    bool isPoint,
    CommonValueExpressionDriver &driver
):
    data_(data),
    mask_(data_.size(),true),
    isPoint_(isPoint),
    driver_(driver),
    hasWeightSum_(false),
    hasSize_(false),
    hasMaximum_(false),
    hasMinimum_(false),
    hasAverage_(false),
    hasWeightedAverage_(false),
    hasSum_(false),
    hasWeightedSum_(false),
    hasSumMag_(false)
{
}

template <typename Type>
AccumulationCalculation<Type>::AccumulationCalculation(
    const Field<Type> &data,
    bool isPoint,
    CommonValueExpressionDriver &driver,
    const Field<bool> &mask
):
    data_(data),
    mask_(mask),
    isPoint_(isPoint),
    driver_(driver),
    hasWeightSum_(false),
    hasSize_(false),
    hasMaximum_(false),
    hasMinimum_(false),
    hasAverage_(false),
    hasWeightedAverage_(false),
    hasSum_(false),
    hasWeightedSum_(false),
    hasSumMag_(false)
{
    if(data_.size()!=mask_.size()) {
        FatalErrorInFunction
            << "Sizes of data " << data_.size()
            << " and specified maks " << mask_.size()
            << " differ"
            << endl
            << exit(FatalError);

    }
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template <typename Type>
AccumulationCalculation<Type>::~AccumulationCalculation()
{
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


template <typename Type>
label AccumulationCalculation<Type>::maskSize() const
{
    label size=0;
    forAll(mask_,i) {
        if(mask_[i]) {
            size++;
        }
    }
    return size;
}

template <typename Type>
const scalarField &AccumulationCalculation<Type>::weights()
{
    if(!weights_.valid()) {
        scalarField rawWeight(
            driver_.weights(
                data_.size(),
                isPoint_
            )
        );

        weights_.set(
            new scalarField(
                maskSize(),
                0
            )
        );

        label cnt=0;
        forAll(mask_,i) {
            if(mask_[i]) {
                weights_()[cnt]=rawWeight[i];
                cnt++;
            }
        }

        assert(cnt==maskSize());
    }
    return weights_();
}

template <typename Type>
const Field<Type> &AccumulationCalculation<Type>::data()
{
    if(!usedData_.valid()) {
        usedData_.set(
            new Field<Type>(
                maskSize(),
                pTraits<Type>::zero
            )
        );

        label cnt=0;
        forAll(mask_,i) {
            if(mask_[i]) {
                usedData_()[cnt]=data_[i];
                cnt++;
            }
        }

        assert(cnt==maskSize());
     }

    return usedData_();
}

template <typename Type>
const SimpleDistribution<Type> &AccumulationCalculation<Type>::distribution()
{
    if(!distribution_.valid()) {
        distribution_.set(
            new SimpleDistribution<Type>(
                this->minimum(),
                this->maximum(),
                numberOfBins
            )
        );
        Field<scalar> oneWeight(data().size(),1);
        distribution_().calcScalarWeight(
            data(),
            oneWeight
        );
    }

    return distribution_();
}

template <typename Type>
const SimpleDistribution<Type> &AccumulationCalculation<Type>::weightedDistribution()
{
    if(!weightedDistribution_.valid()) {
        weightedDistribution_.set(
            new SimpleDistribution<Type>(
                this->minimum(),
                this->maximum(),
                numberOfBins
            )
        );
        weightedDistribution_().calcScalarWeight(
            data(),
            this->weights()
        );
    }

    return weightedDistribution_();
}

template <typename Type>
scalar AccumulationCalculation<Type>::weightSum()
{
    if(!hasWeightSum_) {
        weightSum_=gSum(weights());
        hasWeightSum_=true;
    }
    return weightSum_;
}

template <typename Type>
label AccumulationCalculation<Type>::size()
{
    if(!hasSize_) {
        size_=data().size();
        reduce(size_,plusOp<label>());
        hasSize_=true;
    }
    return size_;
}

template <typename Type>
Type AccumulationCalculation<Type>::maximum()
{
    if(!hasMaximum_) {
        maximum_=gMax(data());
        hasMaximum_=true;
    }
    return maximum_;
}

template <typename Type>
Type AccumulationCalculation<Type>::minimum()
{
    if(!hasMinimum_) {
        minimum_=gMin(data());
        hasMinimum_=true;
    }
    return minimum_;
}

template <typename Type>
Type AccumulationCalculation<Type>::average()
{
    if(!hasAverage_) {
        average_=gAverage(data());
        hasAverage_=true;
    }
    return average_;
}

template <typename Type>
Type AccumulationCalculation<Type>::weightedAverage()
{
    if(!hasWeightedAverage_) {
        const scalar wSum=gSum(weights());
        const Type tSum=gSum(weights()*data());

        weightedAverage_=tSum/wSum;
        hasWeightedAverage_=true;
    }
    return weightedAverage_;
}

template <typename Type>
Type AccumulationCalculation<Type>::sum()
{
    if(!hasSum_) {
        sum_=gSum(data());
        hasSum_=true;
    }
    return sum_;
}

template <typename Type>
Type AccumulationCalculation<Type>::weightedSum()
{
    if(!hasWeightedSum_) {
        weightedSum_=gSum(data()*weights());
        hasWeightedSum_=true;
    }
    return weightedSum_;
}

template <typename Type>
Type AccumulationCalculation<Type>::sumMag()
{
    if(!hasSumMag_) {
        for(direction i=0;i<pTraits<Type>::nComponents;i++) {
            setComponent(sumMag_,i)=gSumMag(data().component(i));
        }
        hasSumMag_=true;
    }
    return sumMag_;
}

template <typename Type>
Type AccumulationCalculation<Type>::operator()(
    const NumericAccumulationNamedEnum::accuSpecification &accu
) {
    switch(accu.first()) {
        case NumericAccumulationNamedEnum::numQuantile:
        case NumericAccumulationNamedEnum::numWeightedQuantile:
        case NumericAccumulationNamedEnum::numRange:
        case NumericAccumulationNamedEnum::numWeightedRange:
            if(
                accu.second()<0
                ||
                accu.second()>1
            ) {
                WarningInFunction
                    << "Accumulation " << accu << " does not have parameter "
                    << "in the range [0,1]"
                    << endl;
            }
            break;
        default:
            // others need no range checking
            ;
    }

    switch(accu.first()) {
        case NumericAccumulationNamedEnum::numSize:
            return size()*pTraits<Type>::one;
            break;
        case NumericAccumulationNamedEnum::numWeightSum:
            return weightSum()*pTraits<Type>::one;
            break;
        case NumericAccumulationNamedEnum::numMin:
            return minimum();
            break;
        case NumericAccumulationNamedEnum::numMax:
            return maximum();
            break;
        case NumericAccumulationNamedEnum::numSum:
            return sum();
            break;
        case NumericAccumulationNamedEnum::numWeightedSum:
        case NumericAccumulationNamedEnum::numIntegrate:
            return weightedSum();
            break;
        case NumericAccumulationNamedEnum::numAverage:
            return average();
            break;
        case NumericAccumulationNamedEnum::numSumMag:
            return sumMag();
            break;
        case NumericAccumulationNamedEnum::numWeightedAverage:
            return weightedAverage();
            break;
        case NumericAccumulationNamedEnum::numMedian:
            return distribution().median();
            break;
        case NumericAccumulationNamedEnum::numWeightedMedian:
            return weightedDistribution().median();
            break;
        case NumericAccumulationNamedEnum::numQuantile:
            return distribution().quantile(accu.second());
            break;
        case NumericAccumulationNamedEnum::numWeightedQuantile:
            return weightedDistribution().quantile(accu.second());
            break;
        case NumericAccumulationNamedEnum::numRange:
            return distribution().quantile(0.5*(1+accu.second()))
                -
                distribution().quantile(0.5*(1-accu.second()));
            break;
        case NumericAccumulationNamedEnum::numWeightedRange:
            return weightedDistribution().quantile(0.5*(1+accu.second()))
                -
                weightedDistribution().quantile(0.5*(1-accu.second()));
            break;
        case NumericAccumulationNamedEnum::numSmaller:
            return distribution().smaller(accu.second());
            break;
        case NumericAccumulationNamedEnum::numBigger:
            return pTraits<Type>::one-distribution().smaller(accu.second());
            break;
        case NumericAccumulationNamedEnum::numWeightedSmaller:
            return weightedDistribution().smaller(accu.second());
            break;
        case NumericAccumulationNamedEnum::numWeightedBigger:
            return pTraits<Type>::one-weightedDistribution().smaller(accu.second());
            break;
        default:
            WarningInFunction
                << "Unimplemented accumultation type "
                << NumericAccumulationNamedEnum::names[accu.first()]
                // << ". Currently only 'min', 'max', 'sum', 'weightedAverage' and 'average' are supported"
                    << endl;
            return pTraits<Type>::zero;
    }
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
