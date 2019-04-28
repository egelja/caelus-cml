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
    swakExpressionAverageDistributionFunctionObject

Description
    Averages of a swak-expression in various directions

SourceFiles
    swakExpressionAverageDistributionFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef swakExpressionAverageDistributionFunctionObject_H
#define swakExpressionAverageDistributionFunctionObject_H

#include "timelineFunctionObject.hpp"

#include "CommonValueExpressionDriver.hpp"

#include "SimpleDistribution.hpp"

#include "vector.hpp"
#include "tensor.hpp"
#include "symmTensor.hpp"
#include "sphericalTensor.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class swakExpressionAverageDistributionFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class swakExpressionAverageDistributionFunctionObject
:
    public timelineFunctionObject
{

    // Private Member Functions

    //- Disallow default bitwise copy construct
    swakExpressionAverageDistributionFunctionObject(const swakExpressionAverageDistributionFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const swakExpressionAverageDistributionFunctionObject&);


    //- the expression that is to be evaluated
    exprString expression_;

    //- the expression for the weight
    exprString weightExpression_;

    //- the weight values
    autoPtr<Field<scalar> > weightValues_;

    //- the expression for the mask
    exprString maskExpression_;

    //- the mask values
    autoPtr<Field<bool> > maskValues_;

    //- the expression for the abscissa
    exprString abscissaExpression_;

    //- the expression driver
    autoPtr<CommonValueExpressionDriver> driver_;

    template <typename T>
    void getDistributionInternal(PtrList<SimpleDistribution<T> > &dist);

    template <typename T>
    void writeADistribution(const PtrList<SimpleDistribution<T> > &dist);

    template <typename T>
    void reportADistribution(const PtrList<SimpleDistribution<T> > &dist);

protected:

    word dirName();

    virtual word baseName();

    PtrList<SimpleDistribution<scalar> > distScalar_;
    PtrList<SimpleDistribution<vector> > distVector_;
    PtrList<SimpleDistribution<tensor> > distTensor_;
    PtrList<SimpleDistribution<symmTensor> > distSymmTensor_;
    PtrList<SimpleDistribution<sphericalTensor> > distSphericalTensor_;

    wordList componentNames_;

    template<class Type,class AType>
    autoPtr<PtrList<SimpleDistribution<AType> > > setData(
        const Field<AType> &xValues,
        const Field<Type> &values,
        const Field<scalar> &weights
    );

    template<class Type,class AType>
    autoPtr<PtrList<SimpleDistribution<AType> > > setData(
        const Field<AType> &xValues,
        const Field<Type> &values,
        const Field<scalar> &weights,
        const Field<bool> &mask
    );

    void clearDistributions();

    // virtual void getDistribution();

public:

    //- Runtime type information
    TypeName("swakExpressionAverageDistribution");


    // Constructors

    //- Construct from components
    swakExpressionAverageDistributionFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );


    // Member Functions

    //     virtual void flush();

    virtual void write();

    virtual bool start();

protected:
    virtual stringList columnNames();

    virtual wordList fileNames();

    // virtual stringList componentNames();
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

template <typename Type,typename AType>
autoPtr<PtrList<SimpleDistribution<AType> > >
swakExpressionAverageDistributionFunctionObject::setData(
        const Field<AType> &xValues,
        const Field<Type> &values,
        const Field<scalar> &weights
) {
    Field<bool> mask(values.size(),true);

    return setData(
        xValues,
        values,
        weights,
        mask
    );
}

template <typename Type,typename AType>
autoPtr<PtrList<SimpleDistribution<AType> > >
swakExpressionAverageDistributionFunctionObject::setData(
        const Field<AType> &xValues,
        const Field<Type> &values,
        const Field<scalar> &weights,
        const Field<bool> &mask
) {
    componentNames_=wordList(pTraits<Type>::nComponents);
    for(direction i=0;i<(pTraits<Type>::nComponents);i++) {
        componentNames_[i]=pTraits<Type>::componentNames[i];
    }

    label binNr=readLabel(dict().lookup("binNumber"));

    dict().lookup("valueIfZero"); // provoke an error if unset
    AType valueIfZero=dict().lookupOrDefault(
        "valueIfZero",
        pTraits<AType>::zero
    );
    AType span=gMax(xValues)-gMin(xValues);
    AType binSize=pTraits<AType>::zero;
    for(direction i=0;i<pTraits<AType>::nComponents;i++) {
        setComponent(binSize,i)=max(
            SMALL*1e5,
            component(span,i)/(binNr+1)
        );
    }

    if(debug) {
        Info << "swakExpressionAverageDistributionFunctionObject::setData "
            << "span: " << span << " nrBins: " << binNr
            << " binSize: " << binSize << endl;
    }

    autoPtr<PtrList<SimpleDistribution<AType> > > pDists(
        new PtrList<SimpleDistribution<AType> >(pTraits<Type>::nComponents)
    );
    PtrList<SimpleDistribution<AType> > &dists=pDists();
    PtrList<SimpleDistribution<AType> > wDists(pTraits<Type>::nComponents);

    for(direction i=0;i<pTraits<Type>::nComponents;i++) {
        dists.set(
            i,
            new SimpleDistribution<AType>(
                binSize
            )
        );
        SimpleDistribution<AType> &dist=dists[i];
        wDists.set(
            i,
            new SimpleDistribution<AType>(
                binSize
            )
        );
        SimpleDistribution<AType> &wDist=wDists[i];
        // addition of 2*VSMALL is a workaround for weights that are
        // equal to 0. Needs proper rewrite in SimpleDistribution
        dist.calcScalarWeight(xValues,values.component(i)*weights+2*VSMALL,mask);
        dist.calcMinimumMaximum(xValues,values.component(i),mask);
        wDist.calcScalarWeight(xValues,weights,mask);

        if(debug>1) {
            Info << "Dist: " << dist << endl
                << "Weight: " << wDist << endl;
        }
        reduce(dist,plusOp<SimpleDistribution<AType> >());
        reduce(wDist,plusOp<SimpleDistribution<AType> >());
        dist.divideByDistribution(
            wDist,
            valueIfZero
        );
        if(debug>1) {
            Info << "Dist: " << dist << endl;
        }
    }

    return pDists;
}

template <typename Type>
void swakExpressionAverageDistributionFunctionObject::getDistributionInternal(
    PtrList<SimpleDistribution<Type> > &dist
) {
    Field<Type> values(driver_->getResult<Type>());

    driver_->parse(expression_);
    word rType=driver_->CommonValueExpressionDriver::getResultType();
    if(rType==pTraits<scalar>::typeName) {
        dist.transfer(
            setData(
                values,
                driver_->getResult<scalar>()(),
                weightValues_,
                maskValues_
            )()
        );
    } else if(rType==pTraits<vector>::typeName) {
        dist.transfer(
            setData(
                values,
                driver_->getResult<vector>()(),
                weightValues_,
                maskValues_
            )()
        );
    } else if(rType==pTraits<tensor>::typeName) {
        dist.transfer(
            setData(
                values,
                driver_->getResult<tensor>()(),
                weightValues_,
                maskValues_
            )()
        );
    } else if(rType==pTraits<symmTensor>::typeName) {
        dist.transfer(
            setData(
                values,
                driver_->getResult<symmTensor>()(),
                weightValues_,
                maskValues_
            )()
        );
    } else if(rType==pTraits<sphericalTensor>::typeName) {
        dist.transfer(
            setData(
                values,
                driver_->getResult<sphericalTensor>()(),
                weightValues_,
                maskValues_
            )()
        );
    } else {
        FatalErrorInFunction
            << "Unhandled type " << rType << " for abscissaExpression "
            << abscissaExpression_
            << endl
            << exit(FatalError);
    }
}

template <typename Type>
void swakExpressionAverageDistributionFunctionObject::writeADistribution(
    const PtrList<SimpleDistribution<Type> > &dist
)
{
    if(dist.size()<=0) {
        return;
    }
    if(dist.size()!=componentNames_.size()) {
        FatalErrorInFunction
            << "The size of the abszissa-components " << componentNames_
            << " is not equal to the number of stored distributions "
            << dist.size()
            << endl
            << exit(FatalError);

    }
    fileName theDir(this->baseDir()/obr().time().timeName());
    mkDir(theDir);
    forAll(componentNames_,i) {
        fileName fName=theDir/this->baseName()+"_averageDistribution";
        if(componentNames_.size()>1) {
            fName+="_"+componentNames_[i];
        }
        dist[i].writeRaw(
            fName
        );
    }
}

template <typename Type>
void swakExpressionAverageDistributionFunctionObject::reportADistribution(
    const PtrList<SimpleDistribution<Type> > &dist
)
{
    if(dist.size()<=0) {
        return;
    }

    Info<< regionString()
        << " Distribution of " << this->baseName()
        << "(" << pTraits<Type>::typeName << "): ";
    if(componentNames_.size()>1) {
        Info << endl;
    }

    forAll(componentNames_,i) {
        if(componentNames_.size()>1) {
            Info << componentNames_[i] << ": ";
        }
        Info << dist[i].maxNrBins() << " bins. "
            << " Min: " << dist[i].min() << " Max: " << dist[i].max() << endl;
    }

    Info << endl;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
