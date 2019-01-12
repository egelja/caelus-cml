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
    distributionFunctionObject

Description
    Basis for function objects that generate distributions

SourceFiles
    distributionFunctionObject.cpp

Contributors/Copyright:
    2008-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef distributionFunctionObject_H
#define distributionFunctionObject_H

#include "timelineFunctionObject.hpp"
#include "NamedEnum.hpp"

#include "SimpleDistribution.hpp"

#include "vector.hpp"
#include "tensor.hpp"
#include "symmTensor.hpp"
#include "sphericalTensor.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class distributionFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class distributionFunctionObject
:
    public timelineFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    distributionFunctionObject(const distributionFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const distributionFunctionObject&);

    bool writeTimeline_;

    bool writeDistribution_;

    template<class Type>
    void writeATimeline(const autoPtr<SimpleDistribution<Type> > &pDist);

    template<class Type>
    void writeADistribution(const autoPtr<SimpleDistribution<Type> > &pDist);

    template<class Type>
    void reportADistribution(const autoPtr<SimpleDistribution<Type> > &pDist);

    bool startup_;
    word startTime_;
protected:

    autoPtr<SimpleDistribution<scalar> > distScalar_;
    autoPtr<SimpleDistribution<vector> > distVector_;
    autoPtr<SimpleDistribution<tensor> > distTensor_;
    autoPtr<SimpleDistribution<symmTensor> > distSymmTensor_;
    autoPtr<SimpleDistribution<sphericalTensor> > distSphericalTensor_;

    template<class Type>
    autoPtr<SimpleDistribution<Type> > setData(
        const Field<Type> &values,
        const Field<Type> &weights,
        const scalar binWidth=-1
    );

    template<class Type>
    autoPtr<SimpleDistribution<Type> > setData(
        const Field<Type> &values,
        const Field<Type> &weights,
        const Field<bool> &mask,
        const scalar binWidth=-1
    );

    template<class Type>
    autoPtr<SimpleDistribution<Type> > setDataScalar(
        const Field<Type> &values,
        const Field<scalar> &weights,
        const scalar binWidth=-1
    );

    template<class Type>
    autoPtr<SimpleDistribution<Type> > setDataScalar(
        const Field<Type> &values,
        const Field<scalar> &weights,
        const Field<bool> &mask,
        const scalar binWidth=-1
    );

    void clearDistributions();

    virtual fileName dataDir();

public:

    //- Runtime type information
    TypeName("distributionFunctionObject");


    // Constructors

    //- Construct from components
    distributionFunctionObject
    (
        const word &name,
        const Time&,
        const dictionary&
    );


    // Member Functions

    bool writeTimeline() const { return writeTimeline_; }

    bool writeDistribution() const { return writeDistribution_; }

    virtual void flush();

    virtual void write();

    virtual bool start();

protected:
    virtual stringList columnNames();

    virtual wordList fileNames();

    virtual word baseName()=0;

    virtual stringList componentNames();

    virtual void getDistribution()=0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IOmanip.hpp"
#include "objectRegistry.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML {
/*---------------------------------------------------------------------------*\
                           Class distributionFunctionObject Declaration
\*---------------------------------------------------------------------------*/

template <typename Type>
autoPtr<SimpleDistribution<Type> > distributionFunctionObject::setData(
    const Field<Type> &values,
    const Field<Type> &weights,
        const scalar binWidth
)
{
    Field<bool> mask(values.size(),true);

    return setData(
        values,
        weights,
        mask,
        binWidth
    );
}

template <typename Type>
autoPtr<SimpleDistribution<Type> > distributionFunctionObject::setDataScalar(
    const Field<Type> &values,
    const Field<scalar> &weights,
        const scalar binWidth
)
{
    Field<bool> mask(values.size(),true);

    return setDataScalar(
        values,
        weights,
        mask,
        binWidth
    );
}

template <typename Type>
autoPtr<SimpleDistribution<Type> > distributionFunctionObject::setData(
    const Field<Type> &values,
    const Field<Type> &weights,
    const Field<bool> &mask,
    const scalar binWidth
)
{
    scalar binSize=binWidth;
    if(binSize<0) {
        binSize=readScalar(dict().lookup("distributionBinWidth"));
    }

    label binNrMax=dict().lookupOrDefault<label>(
        "distributionMaxBinNr",
        1000
    );

    autoPtr<SimpleDistribution<Type> > pDist;

    bool goOn=true;
    while(goOn) {
        goOn=false;

        pDist.reset(
            new SimpleDistribution<Type>(
                binSize
            )
        );

        SimpleDistribution<Type> &dist=pDist();

        dist.calc(values,weights,mask);

        if(dist.maxNrBins()>binNrMax) {
            scalar newBinSize=binSize*(1.1*dist.maxNrBins())/binNrMax;
            WarningInFunction
                << "Number of bin " << dist.maxNrBins()
                    << " exceeds allowed number "
                    << binNrMax << ". Increasing binSize from " << binSize
                    << " to " << newBinSize << endl
                    << "If you want to increase the maximum number of bins "
                    << "set the variable 'distributionMaxBinNr' to a higher value"
                    << endl;
            binSize=newBinSize;
            goOn=true;
        }
    }

    return pDist;
}

template <typename Type>
autoPtr<SimpleDistribution<Type> > distributionFunctionObject::setDataScalar(
    const Field<Type> &values,
    const Field<scalar> &weights,
    const Field<bool> &mask,
    const scalar binWidth
)
{
    scalar binSize=binWidth;
    if(binSize<0) {
        binSize=readScalar(dict().lookup("distributionBinWidth"));
    }

    label binNrMax=dict().lookupOrDefault<label>(
        "distributionMaxBinNr",
        1000
    );

    autoPtr<SimpleDistribution<Type> > pDist;

    bool goOn=true;
    while(goOn) {
        goOn=false;

        pDist.reset(
            new SimpleDistribution<Type>(
                binSize
            )
        );

        SimpleDistribution<Type> &dist=pDist();

        dist.calcScalarWeight(values,weights,mask);

        if(dist.maxNrBins()>binNrMax) {
            scalar newBinSize=binSize*(1.1*dist.maxNrBins())/binNrMax;
            WarningInFunction
                << "Number of bin " << dist.maxNrBins()
                    << " exceeds allowed number "
                    << binNrMax << ". Increasing binSize from " << binSize
                    << " to " << newBinSize << endl
                    << "If you want to increase the maximum number of bins "
                    << "set the variable 'distributionMaxBinNr' to a higher value"
                    << endl;
            binSize=newBinSize;
            goOn=true;
        }
    }

    return pDist;
}

template<class Type>
void distributionFunctionObject::writeATimeline(
    const autoPtr<SimpleDistribution<Type> > &pDist
) {
    if(!pDist.valid()) {
        return;
    }
    const SimpleDistribution<Type> &dist=pDist();

    for(direction i=0;i<pTraits<Type>::nComponents;i++) {
        word name(this->baseName()+"_"+pTraits<Type>::componentNames[i]);
        writeTime(name,this->time().value());

        DynamicList<scalar> data;
        data.append(component(dist.mean(),i));
        data.append(component(dist.median(),i));
        data.append(component(dist.min(),i));
        data.append(component(dist.max(),i));
        data.append(dist.totalWeight(i));

        writeData(
            name,
            data
        );
        endData(name);
    }
}

template<class Type>
void distributionFunctionObject::writeADistribution(
    const autoPtr<SimpleDistribution<Type> > &pDist
) {
    if(!pDist.valid()) {
        return;
    }
    const SimpleDistribution<Type> &dist=pDist();
    fileName theDir(this->baseDir()/"distributions"/obr().time().timeName());

    mkDir(theDir);

    dist.write(
        theDir/(this->baseName()+"_distribution")
    );
}

template<class Type>
void distributionFunctionObject::reportADistribution(
    const autoPtr<SimpleDistribution<Type> > &pDist
) {
    if(!pDist.valid()) {
        return;
    }
    const SimpleDistribution<Type> &dist=pDist();

    Info<< regionString()
        << " Distribution of " << this->baseName()
        << "(" << pTraits<Type>::typeName << "): "
        << dist.maxNrBins() << " bins. Mean: " << dist.mean()
        << " Median: " << dist.median()
        << " Min: " << dist.min() << " Max: " << dist.max() << endl;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


#endif

// ************************************************************************* //
