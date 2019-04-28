/*---------------------------------------------------------------------------*\
 Copyright (C) 2011-2017 OpenFOAM Foundation
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
#include "CloudSubModelBase.hpp"

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
    public CloudSubModelBase<CloudType>
{
    // Private data

        //- Output path
        fileName outputDir_;


    // Private Member Functions

        //- Inherite write from CloudSubModelBase
        using CloudSubModelBase<CloudType>::write;

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
            CloudType& owner,
            const word& modelName
        ),
        (dict, owner, modelName)
    );


    // Constructors

        //- Construct null from owner
        CloudFunctionObject(CloudType& owner);

        //- Construct from dictionary
        CloudFunctionObject
        (
            const dictionary& dict,
            CloudType& owner,
            const word& objectType,
            const word& modelName
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
        const word& objectType,
        const word& modelName
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
                const scalar dt,
                const point& position0,
                bool& keepParticle
            );

            //- Post-patch hook
            virtual void postPatch
            (
                const typename CloudType::parcelType& p,
                const polyPatch& pp,
                bool& keepParticle
            );

            //- Post-face hook
            virtual void postFace
            (
                const typename CloudType::parcelType& p,
                bool& keepParticle
            );


        // Input/output

            //- Return the output path
            const fileName& outputDir() const;

            //- Return the output time path
            fileName outputTimeDir() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeCloudFunctionObject(CloudType)                                     \
                                                                               \
    typedef CloudType::kinematicCloudType kinematicCloudType;                  \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        CloudFunctionObject<kinematicCloudType>,                               \
        0                                                                      \
    );                                                                         \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        CloudFunctionObject<kinematicCloudType>,                               \
        dictionary                                                             \
    );


#define makeCloudFunctionObjectType(SS, CloudType)                             \
                                                                               \
    typedef CloudType::kinematicCloudType kinematicCloudType;                  \
    defineNamedTemplateTypeNameAndDebug(SS<kinematicCloudType>, 0);            \
                                                                               \
    CloudFunctionObject<kinematicCloudType>::                                  \
        adddictionaryConstructorToTable<SS<kinematicCloudType> >               \
            add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //

template<class CloudType>
void CML::CloudFunctionObject<CloudType>::write()
{
    NotImplemented;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::CloudFunctionObject<CloudType>::CloudFunctionObject(CloudType& owner)
:
    CloudSubModelBase<CloudType>(owner),
    outputDir_()
{}


template<class CloudType>
CML::CloudFunctionObject<CloudType>::CloudFunctionObject
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName,
    const word& objectType
)
:
    CloudSubModelBase<CloudType>(modelName, owner, dict, typeName, objectType),
    outputDir_(owner.mesh().time().path())
{
    const fileName relPath =
        "postProcessing"/cloud::prefix/owner.name()/this->modelName();


    if (Pstream::parRun())
    {
        // Put in undecomposed case (Note: gives problems for
        // distributed data running)
        outputDir_ = outputDir_/".."/relPath;
    }
    else
    {
        outputDir_ = outputDir_/relPath;
    }
    outputDir_.clean();
}


template<class CloudType>
CML::CloudFunctionObject<CloudType>::CloudFunctionObject
(
    const CloudFunctionObject<CloudType>& ppm
)
:
    CloudSubModelBase<CloudType>(ppm),
    outputDir_(ppm.outputDir_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::CloudFunctionObject<CloudType>::~CloudFunctionObject()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::CloudFunctionObject<CloudType>::preEvolve()
{}


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
    const scalar,
    const point&,
    bool&
)
{}


template<class CloudType>
void CML::CloudFunctionObject<CloudType>::postPatch
(
    const typename CloudType::parcelType&,
    const polyPatch&,
    bool&
)
{}


template<class CloudType>
void CML::CloudFunctionObject<CloudType>::postFace
(
    const typename CloudType::parcelType&,
    bool&
)
{}


template<class CloudType>
const CML::fileName& CML::CloudFunctionObject<CloudType>::outputDir() const
{
    return outputDir_;
}


template<class CloudType>
CML::fileName CML::CloudFunctionObject<CloudType>::outputTimeDir() const
{
    return outputDir_/this->owner().time().timeName();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::CloudFunctionObject<CloudType> >
CML::CloudFunctionObject<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner,
    const word& objectType,
    const word& modelName
)
{
    Info<< "    Selecting cloud function " << modelName << " of type "
        << objectType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(objectType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown cloud function type "
            << objectType << nl << nl
            << "Valid cloud function types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<CloudFunctionObject<CloudType> >
    (
        cstrIter()
        (
            dict,
            owner,
            modelName
        )
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
