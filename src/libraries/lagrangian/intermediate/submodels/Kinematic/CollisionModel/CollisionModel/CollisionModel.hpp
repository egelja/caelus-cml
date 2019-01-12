/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::CollisionModel

Description
    Templated collision model class.

SourceFiles
    CollisionModel.cpp
    CollisionModelNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef CollisionModel_H
#define CollisionModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "CloudSubModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class CollisionModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class CollisionModel
:
    public CloudSubModelBase<CloudType>
{
    // Protected data

        //- Convenience typedef for parcel type
        typedef typename CloudType::parcelType parcelType;


public:

    //- Runtime type information
    TypeName("collisionModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        CollisionModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        CollisionModel(CloudType& owner);

        //- Construct from components
        CollisionModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        CollisionModel(const CollisionModel<CloudType>& cm);

        //- Construct and return a clone
        virtual autoPtr<CollisionModel<CloudType>> clone() const = 0;


    //- Destructor
    virtual ~CollisionModel();


    //- Selector
    static autoPtr<CollisionModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Member Functions

        //- Return the number of times to subcycle the current
        //  timestep to meet the criteria of the collision model
        virtual label nSubCycles() const = 0;

        // Collision function
        virtual void collide() = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeCollisionModel(CloudType)                                          \
                                                                               \
    typedef CloudType::collidingCloudType collidingCloudType;                  \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        CollisionModel<collidingCloudType>,                                    \
        0                                                                      \
    );                                                                         \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        CollisionModel<collidingCloudType>,                                    \
        dictionary                                                             \
    );


#define makeCollisionModelType(SS, CloudType)                                  \
                                                                               \
    typedef CloudType::collidingCloudType collidingCloudType;                  \
    defineNamedTemplateTypeNameAndDebug(SS<collidingCloudType>, 0);            \
                                                                               \
    CollisionModel<collidingCloudType>::                                       \
        adddictionaryConstructorToTable<SS<collidingCloudType> >               \
            add##SS##CloudType##collidingCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::CollisionModel<CloudType>::CollisionModel(CloudType& owner)
:
    CloudSubModelBase<CloudType>(owner)
{}


template<class CloudType>
CML::CollisionModel<CloudType>::CollisionModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    CloudSubModelBase<CloudType>(owner, dict, typeName, type)
{}


template<class CloudType>
CML::CollisionModel<CloudType>::CollisionModel
(
    const CollisionModel<CloudType>& cm
)
:
    CloudSubModelBase<CloudType>(cm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::CollisionModel<CloudType>::~CollisionModel()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::CollisionModel<CloudType> >
CML::CollisionModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    word modelType(dict.lookup("collisionModel"));

    Info<< "Selecting collision model " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown collision model type " << modelType
            << ", constructor not in hash table" << nl << nl
            << "    Valid collision model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
    }

    return autoPtr<CollisionModel<CloudType> >(cstrIter()(dict, owner));
}




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
