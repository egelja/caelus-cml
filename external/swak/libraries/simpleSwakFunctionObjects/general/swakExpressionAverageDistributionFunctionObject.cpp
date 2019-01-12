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
    2008-2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "swakExpressionAverageDistributionFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "pointFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(swakExpressionAverageDistributionFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        swakExpressionAverageDistributionFunctionObject,
        dictionary
    );



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

swakExpressionAverageDistributionFunctionObject::swakExpressionAverageDistributionFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    timelineFunctionObject(name,t,dict),
    expression_(
        dict.lookup("expression"),
        dict
    ),
    weightExpression_(
        dict.lookup("weight"),
        dict
    ),
    maskExpression_(
        dict.lookup("mask"),
        dict
    ),
    abscissaExpression_(
        dict.lookup("abscissa"),
        dict
    ),
    driver_(
        CommonValueExpressionDriver::New(
            dict,
            refCast<const fvMesh>(obr_)
        )
    )
{
    driver_->createWriterAndRead(name+"_"+type());
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void swakExpressionAverageDistributionFunctionObject::clearDistributions() {
    distScalar_.clear();
    distVector_.clear();
    distTensor_.clear();
    distSymmTensor_.clear();
    distSphericalTensor_.clear();
    componentNames_=wordList(0);
}

bool swakExpressionAverageDistributionFunctionObject::start() {
    // startup_=true;
    // startTime_=time().timeName();

    return true;
}

stringList swakExpressionAverageDistributionFunctionObject::columnNames()
{
    return stringList();
}

wordList swakExpressionAverageDistributionFunctionObject::fileNames()
{
    return wordList();
}

word swakExpressionAverageDistributionFunctionObject::dirName()
{
    return typeName;
}

word swakExpressionAverageDistributionFunctionObject::baseName()
{
    return "expression";
}


void swakExpressionAverageDistributionFunctionObject::write()
{
    if(verbose()) {
        Info << "AverageDistributionExpression " << name() << " : ";
    }

    clearDistributions();

    driver_->clearVariables();

    driver_->parse(weightExpression_);
    if(
        driver_->CommonValueExpressionDriver::getResultType()
        ==
        pTraits<scalar>::typeName
    ) {
        weightValues_.reset(
            driver_->getResult<scalar>().ptr()
        );
    } else {
        FatalErrorInFunction
            << "Don't know how to handle weight expressions of type "
            << driver_->CommonValueExpressionDriver::getResultType()
            << endl
            << exit(FatalError);

    }

    driver_->parse(maskExpression_);
    if(
        driver_->CommonValueExpressionDriver::getResultType()
        ==
        pTraits<bool>::typeName
    ) {
        maskValues_.reset(
            driver_->getResult<bool>().ptr()
        );
    } else if(
        driver_->CommonValueExpressionDriver::getResultType()
        ==
        pTraits<scalar>::typeName
    ) {
        scalarField rawMask(driver_->getResult<scalar>());
        maskValues_.reset(
            new Field<bool>(rawMask.size(),false)
        );
        forAll(rawMask,i) {
            if(rawMask[i]>SMALL) {
                maskValues_()[i]=true;
            }
        }
    } else {
        FatalErrorInFunction
            << "Don't know how to handle logical expressions of type "
            << driver_->CommonValueExpressionDriver::getResultType()
            << endl
            << exit(FatalError);

    }

    driver_->parse(abscissaExpression_);
    word rType=driver_->CommonValueExpressionDriver::getResultType();
    if(rType==pTraits<scalar>::typeName) {
        getDistributionInternal(distScalar_);
    } else if(rType==pTraits<vector>::typeName) {
        getDistributionInternal(distVector_);
    } else if(rType==pTraits<tensor>::typeName) {
        getDistributionInternal(distTensor_);
    } else if(rType==pTraits<symmTensor>::typeName) {
        getDistributionInternal(distSymmTensor_);
    } else if(rType==pTraits<sphericalTensor>::typeName) {
        getDistributionInternal(distSphericalTensor_);
    } else {
        WarningInFunction
            << "Don't know how to handle type " << rType
            << " of expression " << abscissaExpression_
            << endl;
    }

    bool zeroDistribution=false;
    if(
        distScalar_.size()>0
        &&
        distScalar_[0].maxNrBins()<=0
    ) {
        zeroDistribution=true;
    }
    if(
        distVector_.size()>0
        &&
        distVector_[0].maxNrBins()<=0
    ) {
        zeroDistribution=true;
    }
    if(
        distTensor_.size()>0
        &&
        distTensor_[0].maxNrBins()<=0
    ) {
        zeroDistribution=true;
    }
    if(
        distSymmTensor_.size()>0
        &&
        distSymmTensor_[0].maxNrBins()<=0
    ) {
        zeroDistribution=true;
    }
    if(
        distSphericalTensor_.size()>0
        &&
        distSphericalTensor_[0].maxNrBins()<=0
    ) {
        zeroDistribution=true;
    }

    if(verbose()) {
        Info << endl;
    }

    if(zeroDistribution) {
        WarningInFunction
            << "Distribution for " << name() << " has size 0. "
            << "Doing nothing"
            << endl;
    } else if(Pstream::master()) {
        writeADistribution(distScalar_);
        writeADistribution(distVector_);
        writeADistribution(distTensor_);
        writeADistribution(distSymmTensor_);
        writeADistribution(distSphericalTensor_);
        if(verbose()) {
            reportADistribution(distScalar_);
            reportADistribution(distVector_);
            reportADistribution(distTensor_);
            reportADistribution(distSymmTensor_);
            reportADistribution(distSphericalTensor_);
        }
    }

    // make sure that the stored Variables are consistently written
    driver_->tryWrite();
}

} // namespace CML

// ************************************************************************* //
