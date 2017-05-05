/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::StochasticCollisionModel

Description
    Templated stochastic collision model class


\*---------------------------------------------------------------------------*/

#ifndef StochasticCollisionModel_H
#define StochasticCollisionModel_H

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
        virtual autoPtr<StochasticCollisionModel<CloudType> > clone() const
        {
            return autoPtr<StochasticCollisionModel<CloudType> >
            (
                new StochasticCollisionModel<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~StochasticCollisionModel();


    //- Selector
    static autoPtr<StochasticCollisionModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Member Functions

        virtual bool update
        (
            const scalar dt,
            cachedRandom& rndGen,
            vector& pos1,
            scalar& m1,
            scalar& d1,
            scalar& N1,
            vector& U1,
            scalar& rho1,
            scalar& T1,
            scalarField& Y1,
            const scalar sigma1,
            const label celli,
            const scalar voli,
            vector& pos2,
            scalar& m2,
            scalar& d2,
            scalar& N2,
            vector& U2,
            scalar& rho2,
            scalar& T2,
            scalarField& Y2,
            const scalar sigma2,
            const label cellj,
            const scalar volj
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeStochasticCollisionModel(CloudType)                               \
                                                                              \
    typedef CloudType::sprayCloudType sprayCloudType;                         \
    defineNamedTemplateTypeNameAndDebug                                       \
    (                                                                         \
        StochasticCollisionModel<sprayCloudType>,                             \
        0                                                                     \
    );                                                                        \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
        StochasticCollisionModel<sprayCloudType>,                             \
        dictionary                                                            \
    );


#define makeStochasticCollisionModelType(SS, CloudType)                       \
                                                                              \
    typedef CloudType::sprayCloudType sprayCloudType;                         \
    defineNamedTemplateTypeNameAndDebug(SS<sprayCloudType>, 0);               \
                                                                              \
    StochasticCollisionModel<sprayCloudType>::                                \
        adddictionaryConstructorToTable<SS<sprayCloudType> >                  \
            add##SS##CloudType##sprayCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

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
bool CML::StochasticCollisionModel<CloudType>::update
(
    const scalar dt,
    cachedRandom& rndGen,
    vector& pos1,
    scalar& m1,
    scalar& d1,
    scalar& N1,
    vector& U1,
    scalar& rho1,
    scalar& T1,
    scalarField& Y1,
    const scalar sigma1,
    const label celli,
    const scalar voli,
    vector& pos2,
    scalar& m2,
    scalar& d2,
    scalar& N2,
    vector& U2,
    scalar& rho2,
    scalar& T2,
    scalarField& Y2,
    const scalar sigma2,
    const label cellj,
    const scalar volj
) const
{
    notImplemented
    (
        "bool CML::StochasticCollisionModel<CloudType>::update"
        "("
            "const scalar, "
            "cachedRandom&, "
            "vector&, "
            "scalar&, "
            "scalar&, "
            "scalar&, "
            "vector&, "
            "scalar&, "
            "scalar&, "
            "scalarField&, "
            "const scalar, "
            "const label, "
            "const scalar, "
            "vector&, "
            "scalar&, "
            "scalar&, "
            "scalar&, "
            "vector&, "
            "scalar&, "
            "scalar&, "
            "scalarField&, "
            "const scalar, "
            "const label, "
            "const scalar"
        ") const"
    );

    return false;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::StochasticCollisionModel<CloudType> >
CML::StochasticCollisionModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    word modelType(dict.lookup("stochasticCollisionModel"));

    Info<< "Selecting StochasticCollisionModel " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "StochasticCollisionModel<CloudType>::New"
            "("
                "const dictionary&, "
                "CloudType&"
            ")"
        )   << "Unknown StochasticCollisionModelType type "
            << modelType << ", constructor not in hash table" << nl << nl
            << "    Valid StochasticCollisionModel types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
    }

    return autoPtr<StochasticCollisionModel<CloudType> >
    (
        cstrIter()(dict, owner)
    );
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
