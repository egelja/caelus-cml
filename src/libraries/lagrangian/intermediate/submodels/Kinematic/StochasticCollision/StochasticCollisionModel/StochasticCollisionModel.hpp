/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2017 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::StochasticCollisionModel

Description
    Templated stochastic collision model class


\*---------------------------------------------------------------------------*/

#ifndef StochasticCollisionModel_HPP
#define StochasticCollisionModel_HPP

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "CloudSubModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class StochasticCollisionModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class StochasticCollisionModel
:
    public CloudSubModelBase<CloudType>
{
protected:

    //- Main collision routine
    virtual void collide
    (
        typename CloudType::parcelType::trackingData& td,
        const scalar dt
    ) = 0;


public:

    //- Runtime type information
    TypeName("collisionModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        StochasticCollisionModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        StochasticCollisionModel(CloudType& owner);

        //- Construct from dictionary
        StochasticCollisionModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        StochasticCollisionModel(const StochasticCollisionModel<CloudType>& cm);

        //- Construct and return a clone
        virtual autoPtr<StochasticCollisionModel<CloudType>> clone() const = 0;


    //- Destructor
    virtual ~StochasticCollisionModel();


    //- Selector
    static autoPtr<StochasticCollisionModel<CloudType>> New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Member Functions

        //- Update the model
        void update
        (
            typename CloudType::parcelType::trackingData& td,
            const scalar dt
        );
};


} // End namespace CML


#define makeStochasticCollisionModel(CloudType)                                \
                                                                               \
    typedef CloudType::kinematicCloudType kinematicCloudType;                  \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        StochasticCollisionModel<kinematicCloudType>,                          \
        0                                                                      \
    );                                                                         \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        StochasticCollisionModel<kinematicCloudType>,                          \
        dictionary                                                             \
    );


#define makeStochasticCollisionModelType(SS, CloudType)                        \
                                                                               \
    typedef CloudType::kinematicCloudType kinematicCloudType;                  \
    defineNamedTemplateTypeNameAndDebug(SS<kinematicCloudType>, 0);            \
                                                                               \
    StochasticCollisionModel<kinematicCloudType>::                             \
        adddictionaryConstructorToTable<SS<kinematicCloudType>>                \
            add##SS##CloudType##kinematicCloudType##ConstructorToTable_;



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::StochasticCollisionModel<CloudType>::StochasticCollisionModel
(
    CloudType& owner
)
:
    CloudSubModelBase<CloudType>(owner)
{}


template<class CloudType>
CML::StochasticCollisionModel<CloudType>::StochasticCollisionModel
(
    const StochasticCollisionModel<CloudType>& cm
)
:
    CloudSubModelBase<CloudType>(cm)
{}


template<class CloudType>
CML::StochasticCollisionModel<CloudType>::StochasticCollisionModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    CloudSubModelBase<CloudType>(owner, dict, typeName, type)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::StochasticCollisionModel<CloudType>::~StochasticCollisionModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::StochasticCollisionModel<CloudType>::update
(
    typename CloudType::parcelType::trackingData& td,
    const scalar dt
)
{
    if (this->active())
    {
        this->collide(td, dt);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
template<class CloudType>
CML::autoPtr<CML::StochasticCollisionModel<CloudType>>
CML::StochasticCollisionModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    word modelType(dict.lookup("stochasticCollisionModel"));

    Info<< "Selecting stochastic collision model " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown model type type "
            << modelType << ", constructor not in hash table" << nl << nl
            << "    Valid model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
    }

    return autoPtr<StochasticCollisionModel<CloudType>>
    (
        cstrIter()(dict, owner)
    );
}


#endif
