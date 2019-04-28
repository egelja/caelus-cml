/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2010 OpenCFD Ltd.
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
    CML::CloudProxyForParticle

Description

SourceFiles
    CloudProxyForParticle.cpp

Contributors/Copyright:
    2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef CloudProxyForParticle_H
#define CloudProxyForParticle_H

#include "CloudProxy.hpp"

#include "HashPtrTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class CloudProxyForParticle Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class CloudProxyForParticle
:
    public CloudProxy
{
    const CloudType &cloud_;

    //- Disallow default bitwise assignment
    void operator=(const CloudProxyForParticle&);

    //- Disallow copy construct
    CloudProxyForParticle(const CloudProxyForParticle &);

protected:
    template<class RType,class Func>
    tmp<Field<RType> > mapToParticles(
        const Func &f
    ) const;

    //- abstract class to store pointers to member functions of the particle class
    template<class RType>
    class ParticleMethodWrapper
    {
    public:
        typedef typename CloudType::particleType particleType;

        ParticleMethodWrapper()
            {}
        virtual ~ParticleMethodWrapper()
            {}

        virtual RType operator()(const particleType &) const = 0;

        RType operator()(const particleType *p) const
            { return operator()(*p); }
    };

    template<class RType>
    class ParticleMethodWrapperReference
    :
    public ParticleMethodWrapper<RType>
    {

    public:
        typedef typename ParticleMethodWrapper<RType>::particleType particleType;

        typedef const RType& (particleType::*FSig)() const;

        FSig fPtr_;

        ParticleMethodWrapperReference(FSig f):
            ParticleMethodWrapper<RType>(),
            fPtr_(f)
            {}

        virtual ~ParticleMethodWrapperReference()
            {}

        virtual RType
        operator()(const particleType &p) const
            {
                return ((p).*(fPtr_))();
            }
    };

    template<class RType,class FType=RType>
    class ParticleMethodWrapperValue
    :
    public ParticleMethodWrapper<RType>
    {

    public:
        typedef typename ParticleMethodWrapper<RType>::particleType particleType;

        typedef FType (particleType::*FSig)() const;

        FSig fPtr_;

        ParticleMethodWrapperValue(FSig f):
            ParticleMethodWrapper<RType>(),
            fPtr_(f)
            {}

        virtual ~ParticleMethodWrapperValue()
            {}

        virtual RType
        operator()(const particleType &p) const
            {
                return ((p).*(fPtr_))();
            }
    };

    template<class RType>
    class ParticleMethodWrapperConstant
    :
    public ParticleMethodWrapper<RType>
    {
        RType value_;

    public:
        typedef typename ParticleMethodWrapper<RType>::particleType particleType;

        ParticleMethodWrapperConstant(RType value):
            ParticleMethodWrapper<RType>(),
            value_(value)
            {}

        virtual ~ParticleMethodWrapperConstant()
            {}

        virtual RType
        operator()(const particleType &p) const
            {
                return value_;
            }
    };

    template<class RType>
    class ParticleMethodWrapperFieldElement
    :
    public ParticleMethodWrapper<RType>
    {

    public:
        typedef typename ParticleMethodWrapper<RType>::particleType particleType;

        typedef const Field<RType>& (particleType::*FSig)() const;

        FSig fPtr_;
        label index_;

        ParticleMethodWrapperFieldElement(
            FSig f,
            label index
        ):
            ParticleMethodWrapper<RType>(),
            fPtr_(f),
            index_(index)
            {}

        virtual ~ParticleMethodWrapperFieldElement()
            {}

        virtual RType
        operator()(const particleType &p) const
            {
                //                Info << ((p).*(fPtr_))() << " " << index_ << " " << endl;
                return ((p).*(fPtr_))()[index_];
            }
    };

    HashPtrTable<ParticleMethodWrapper<scalar>,word,string::hash>
      scalarFunctions_;

    void addScalarFunction(
        const word &name,
        const string &description,
        ParticleMethodWrapper<scalar> *ptr
    );

    HashPtrTable<ParticleMethodWrapper<bool>,word,string::hash>
      boolFunctions_;

    void addBoolFunction(
        const word &name,
        const string &description,
        ParticleMethodWrapper<bool> *ptr
    );

    HashPtrTable<ParticleMethodWrapper<vector>,word,string::hash>
      vectorFunctions_;

    void addVectorFunction(
        const word &name,
        const string &description,
        ParticleMethodWrapper<vector> *ptr
    );

    HashPtrTable<ParticleMethodWrapper<tensor>,word,string::hash>
      tensorFunctions_;

    void addTensorFunction(
        const word &name,
        const string &description,
        ParticleMethodWrapper<tensor> *ptr
    );

    HashPtrTable<ParticleMethodWrapper<symmTensor>,word,string::hash>
      symmTensorFunctions_;

    void addSymmTensorFunction(
        const word &name,
        const string &description,
        ParticleMethodWrapper<symmTensor> *ptr
    );

    HashPtrTable<ParticleMethodWrapper<sphericalTensor>,word,string::hash>
      sphericalTensorFunctions_;

    void addSphericalTensorFunction(
        const word &name,
        const string &description,
        ParticleMethodWrapper<sphericalTensor> *ptr
    );

public:

    typedef CloudType cloudType;
    typedef typename CloudType::particleType particleType;

    // Constructors

    //- Construct from components
    CloudProxyForParticle
    (
        const cloud &
    );

    // Destructor

    virtual ~CloudProxyForParticle();

    const cloudType &theCloud() const
        { return cloud_; }

    virtual tmp<Field<scalar> > getScalarField(
        const word &name
    ) const;

    virtual tmp<Field<bool> > getBoolField(
        const word &name
    ) const;

     virtual tmp<Field<vector> > getVectorField(
        const word &name
    ) const;

    virtual tmp<Field<tensor> > getTensorField(
        const word &name
    ) const;

    virtual tmp<Field<symmTensor> > getSymmTensorField(
        const word &name
    ) const;

    virtual tmp<Field<sphericalTensor> > getSphericalTensorField(
        const word &name
    ) const;

    tmp<Field<vector> > getPositions() const;

    tmp<Field<label> > getCells() const;

    tmp<Field<scalar> > weights() const;

    label size() const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "DebugOStream.hpp"

#include <functional>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

    // workaround for a template not defined in <functional>
template <typename S,typename T>
inline std::const_mem_fun_t<S,T> const_mem_fun(S (T::*f)() const)
{
    return std::const_mem_fun_t<S,T>(f);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CloudProxyForParticle<CloudType>::CloudProxyForParticle
(
    const cloud& c
)
:
    CloudProxy(c),
    cloud_(
        dynamicCast<const CloudType&>(
            c
        )
    )

{
    addScalarFunction(
        "origProc",
        "Originating processor",
        new ParticleMethodWrapperValue<scalar,label>(
            &particleType::origProc
        )
    );
    addScalarFunction(
        "origId",
        "Original id",
        new ParticleMethodWrapperValue<scalar,label>(
            &particleType::origId
        )
    );
    addScalarFunction(
        "cell",
        "number of the cell",
        new ParticleMethodWrapperValue<scalar,label>(
            &particleType::cell
        )
    );
    addScalarFunction(
        "face",
        "number of the face",
        new ParticleMethodWrapperValue<scalar,label>(
            &particleType::face
        )
    );
    addBoolFunction(
        "onBoundary",
        "is this currently on the boundary",
        new ParticleMethodWrapperValue<bool>(
            &particleType::onBoundaryFace
        )
    );
    addBoolFunction(
        "onBoundaryFace",
        "is this currently on the boundary",
        new ParticleMethodWrapperValue<bool>(
            &particleType::onBoundaryFace
        )
    );
    addScalarFunction(
        "stepFraction",
        "fraction of the time-step completed",
        new ParticleMethodWrapperValue<scalar>(
            &particleType::stepFraction
        )
    );
    addScalarFunction(
        "currentTimeFraction",
        "Current fraction within the time-step",
        new ParticleMethodWrapperValue<scalar>(
            &particleType::currentTimeFraction
        )
    );
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CloudProxyForParticle<CloudType>::~CloudProxyForParticle()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CloudProxyForParticle<CloudType>::addScalarFunction(
    const word &name,
    const string &description,
    ParticleMethodWrapper<scalar> *ptr
)
{
    addField<scalar>(name,description);
    scalarFunctions_.set(
        name,
        ptr
    );
}

template<class CloudType>
void CloudProxyForParticle<CloudType>::addBoolFunction(
    const word &name,
    const string &description,
    ParticleMethodWrapper<bool> *ptr
)
{
    addField<bool>(name,description);
    boolFunctions_.set(
        name,
        ptr
    );
}

template<class CloudType>
void CloudProxyForParticle<CloudType>::addVectorFunction(
    const word &name,
    const string &description,
    ParticleMethodWrapper<vector> *ptr
)
{
    addField<vector>(name,description);
    vectorFunctions_.set(
        name,
        ptr
    );
}

template<class CloudType>
void CloudProxyForParticle<CloudType>::addTensorFunction(
    const word &name,
    const string &description,
    ParticleMethodWrapper<tensor> *ptr
)
{
    addField<tensor>(name,description);
    tensorFunctions_.set(
        name,
        ptr
    );
}

template<class CloudType>
void CloudProxyForParticle<CloudType>::addSymmTensorFunction(
    const word &name,
    const string &description,
    ParticleMethodWrapper<symmTensor> *ptr
)
{
    addField<symmTensor>(name,description);
    symmTensorFunctions_.set(
        name,
        ptr
    );
}

template<class CloudType>
void CloudProxyForParticle<CloudType>::addSphericalTensorFunction(
    const word &name,
    const string &description,
    ParticleMethodWrapper<sphericalTensor> *ptr
)
{
    addField<sphericalTensor>(name,description);
    sphericalTensorFunctions_.set(
        name,
        ptr
    );
}

template<class CloudType>
tmp<Field<scalar> > CloudProxyForParticle<CloudType>::getScalarField(
    const word &name
) const
{
    if(scalarFunctions_.found(name)) {
        Dbug << "Found " << name << " in scalar table" << endl;
        return mapToParticles<scalar>(
            *scalarFunctions_[name]
        );
    }

    FatalErrorInFunction
        << "No scalar field with name " << name << " defined for cloud "
        << cloud_.name() << " of type " // << cloud_.CloudTyptype()
        << endl
        << exit(FatalError);
    return tmp<Field<scalar> >(
        new Field<scalar>(0)
    );
}

template<class CloudType>
tmp<Field<bool> > CloudProxyForParticle<CloudType>::getBoolField(
    const word &name
) const
{
    if(boolFunctions_.found(name)) {
        Dbug << "Found " << name << " in bool table" << endl;
        return mapToParticles<bool>(
            *boolFunctions_[name]
        );
    }

    FatalErrorInFunction
        << "No bool field with name " << name << " defined for cloud "
        << cloud_.name() << " of type " // << cloud_.CloudTyptype()
        << endl
        << exit(FatalError);
    return tmp<Field<bool> >(
        new Field<bool>(0)
    );
}

template<class CloudType>
tmp<Field<vector> > CloudProxyForParticle<CloudType>::getVectorField(
    const word &name
) const
{
    if(vectorFunctions_.found(name)) {
        Dbug << "Found " << name << " in vector table" << endl;
        return mapToParticles<vector>(
            *vectorFunctions_[name]
        );
    }

    FatalErrorInFunction
        << "No vector field with name " << name << " defined for cloud "
        << cloud_.name() << " of type " // << cloud_.CloudTyptype()
        << endl
        << exit(FatalError);
    return tmp<Field<vector> >(
        new Field<vector>(0)
    );
}

template<class CloudType>
tmp<Field<tensor> > CloudProxyForParticle<CloudType>::getTensorField(
    const word &name
) const
{
    if(tensorFunctions_.found(name)) {
        Dbug << "Found " << name << " in tensor table" << endl;
        return mapToParticles<tensor>(
            *tensorFunctions_[name]
        );
    }

    FatalErrorInFunction
        << "No tensor field with name " << name << " defined for cloud "
        << cloud_.name() << " of type " // << cloud_.CloudTyptype()
        << endl
        << exit(FatalError);
    return tmp<Field<tensor> >(
        new Field<tensor>(0)
    );
}

template<class CloudType>
tmp<Field<symmTensor> > CloudProxyForParticle<CloudType>::getSymmTensorField(
    const word &name
) const
{
    if(symmTensorFunctions_.found(name)) {
        Dbug << "Found " << name << " in symmTensor table" << endl;
        return mapToParticles<symmTensor>(
            *symmTensorFunctions_[name]
        );
    }

    FatalErrorInFunction
        << "No symmTensor field with name " << name << " defined for cloud "
        << cloud_.name() << " of type " // << cloud_.CloudTyptype()
        << endl
        << exit(FatalError);
    return tmp<Field<symmTensor> >(
        new Field<symmTensor>(0)
    );
}

template<class CloudType>
tmp<Field<sphericalTensor> > CloudProxyForParticle<CloudType>::getSphericalTensorField(
    const word &name
) const
{
    if(sphericalTensorFunctions_.found(name)) {
        Dbug << "Found " << name << " in sphericalTensor table" << endl;
        return mapToParticles<sphericalTensor>(
            *sphericalTensorFunctions_[name]
        );
    }

    FatalErrorInFunction
        << "No sphericalTensor field with name " << name << " defined for cloud "
        << cloud_.name() << " of type " // << cloud_.CloudTyptype()
        << endl
        << exit(FatalError);
    return tmp<Field<sphericalTensor> >(
        new Field<sphericalTensor>(0)
    );
}

template<class CloudType>
tmp<Field<scalar> > CloudProxyForParticle<CloudType>::weights() const
{
    return tmp<Field<scalar> >(
        new Field<scalar>(theCloud().size(),1.0)
    );
}

template<class CloudType>
template<class RType,class Func>
tmp<Field<RType> > CloudProxyForParticle<CloudType>::mapToParticles(
    const Func &f
) const
{
    tmp<Field<RType> > tResult(
        new Field<RType>(theCloud().size())
    );
    Field<RType> &result=tResult();
    label i=0;
    forAllConstIter(typename CloudType,theCloud(),it)
    {
	const particleType &p=(*it);
        result[i]=f(&p);
        i++;
    }

    return tResult;
}

template<class CloudType>
tmp<Field<vector> > CloudProxyForParticle<CloudType>::getPositions() const
{
    return mapToParticles<vector>(
        const_mem_fun(
            &particleType::position
        )
    );
}

template<class CloudType>
tmp<Field<label> > CloudProxyForParticle<CloudType>::getCells() const
{
    return mapToParticles<label>(
        const_mem_fun(
            &particleType::cell
        )
    );
}

template<class CloudType>
label CloudProxyForParticle<CloudType>::size() const
{
    return cloud_.size();
}


} // namespace end


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
