/*---------------------------------------------------------------------------*\
 Copyright (C) 2011-2012 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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
    CML::CloudFunctionObject

Description
    Templated cloud function object base class

SourceFiles
    CloudFunctionObject.cpp
    CloudFunctionObjectNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef CloudFunctionObject_H
#define CloudFunctionObject_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "SubModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class polyPatch;
class tetIndices;

/*---------------------------------------------------------------------------*\
                    Class CloudFunctionObject Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class CloudFunctionObject
:
    public SubModelBase<CloudType>
{
    // Protected Member Functions

        //- Write post-processing info
        virtual void write();


public:

    //- Runtime type information
    TypeName("cloudFunctionObject");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        CloudFunctionObject,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        CloudFunctionObject(CloudType& owner);

        //- Construct from dictionary
        CloudFunctionObject
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelType
        );

        //- Construct copy
        CloudFunctionObject(const CloudFunctionObject<CloudType>& ppm);

        //- Construct and return a clone
        virtual autoPtr<CloudFunctionObject<CloudType> > clone() const
        {
            return autoPtr<CloudFunctionObject<CloudType> >
            (
                new CloudFunctionObject<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~CloudFunctionObject();


    //- Selector
    static autoPtr<CloudFunctionObject<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner,
        const word& modelType
    );


    // Member Functions

        // Evaluation

            //- Pre-evolve hook
            virtual void preEvolve();

            //- Post-evolve hook
            virtual void postEvolve();

            //- Post-move hook
            virtual void postMove
            (
                typename CloudType::parcelType& p,
                const label cellI,
                const scalar dt
            );

            //- Post-patch hook
            virtual void postPatch
            (
                const typename CloudType::parcelType& p,
                const polyPatch& pp,
                const scalar trackFraction,
                const tetIndices& tetIs
            );

            //- Post-face hook
            virtual void postFace
            (
                const typename CloudType::parcelType& p,
                const label faceI
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeCloudFunctionObject(CloudType)                                    \
                                                                              \
    typedef CloudType::kinematicCloudType kinematicCloudType;                 \
    defineNamedTemplateTypeNameAndDebug                                       \
    (                                                                         \
        CloudFunctionObject<kinematicCloudType>,                              \
        0                                                                     \
    );                                                                        \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
        CloudFunctionObject<kinematicCloudType>,                              \
        dictionary                                                            \
    );


#define makeCloudFunctionObjectType(SS, CloudType)                            \
                                                                              \
    typedef CloudType::kinematicCloudType kinematicCloudType;                 \
    defineNamedTemplateTypeNameAndDebug(SS<kinematicCloudType>, 0);           \
                                                                              \
    CloudFunctionObject<kinematicCloudType>::                                 \
        adddictionaryConstructorToTable<SS<kinematicCloudType> >              \
            add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //

template<class CloudType>
void CML::CloudFunctionObject<CloudType>::write()
{
    notImplemented("void CML::CloudFunctionObject<CloudType>::write()");
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::CloudFunctionObject<CloudType>::CloudFunctionObject(CloudType& owner)
:
    SubModelBase<CloudType>(owner)
{}


template<class CloudType>
CML::CloudFunctionObject<CloudType>::CloudFunctionObject
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    SubModelBase<CloudType>(owner, dict, typeName, type, "")
{}


template<class CloudType>
CML::CloudFunctionObject<CloudType>::CloudFunctionObject
(
    const CloudFunctionObject<CloudType>& ppm
)
:
    SubModelBase<CloudType>(ppm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::CloudFunctionObject<CloudType>::~CloudFunctionObject()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::CloudFunctionObject<CloudType>::preEvolve()
{
    // do nothing
}


template<class CloudType>
void CML::CloudFunctionObject<CloudType>::postEvolve()
{
    if (this->owner().time().outputTime())
    {
        this->write();
    }
}


template<class CloudType>
void CML::CloudFunctionObject<CloudType>::postMove
(
    typename CloudType::parcelType&,
    const label,
    const scalar
)
{
    // do nothing
}


template<class CloudType>
void CML::CloudFunctionObject<CloudType>::postPatch
(
    const typename CloudType::parcelType&,
    const polyPatch&,
    const scalar,
    const tetIndices&
)
{
    // do nothing
}


template<class CloudType>
void CML::CloudFunctionObject<CloudType>::postFace
(
    const typename CloudType::parcelType&,
    const label
)
{
    // do nothing
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::CloudFunctionObject<CloudType> >
CML::CloudFunctionObject<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelType
)
{
    Info<< "    Selecting cloud function " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "CloudFunctionObject<CloudType>::New"
            "("
                "const dictionary&, "
                "CloudType&"
            ")"
        )   << "Unknown cloud function type "
            << modelType << nl << nl
            << "Valid cloud function types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<CloudFunctionObject<CloudType> >(cstrIter()(dict, owner));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
