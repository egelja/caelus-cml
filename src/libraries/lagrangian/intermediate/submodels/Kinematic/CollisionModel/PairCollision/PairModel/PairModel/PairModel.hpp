/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2016 OpenFOAM Foundation
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
    CML::PairModel

Description
    Templated pair interaction class


\*---------------------------------------------------------------------------*/

#ifndef PairModel_H
#define PairModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                          Class PairModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PairModel
{
    // Private data

        //- The CollisionModel dictionary
        const dictionary& dict_;

        //- Reference to the owner cloud class
        CloudType& owner_;

        //- The coefficients dictionary
        const dictionary coeffDict_;


public:

    //- Runtime type information
    TypeName("pairModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        PairModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct from components
        PairModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );


    //- Destructor
    virtual ~PairModel();


    //- Selector
    static autoPtr<PairModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Access

        //- Return the owner cloud object
        const CloudType& owner() const;

        //- Return the dictionary
        const dictionary& dict() const;

        //- Return the coefficients dictionary
        const dictionary& coeffDict() const;


    // Member Functions

        //- Whether the PairModel has a timestep limit that will
        //  require subCycling
        virtual bool controlsTimestep() const = 0;

        //- For PairModels that control the timestep, calculate the
        //  number of subCycles needed to satisfy the minimum
        //  allowable timestep
        virtual label nSubCycles() const = 0;

        //- Calculate the pair interaction between parcels
        virtual void evaluatePair
        (
            typename CloudType::parcelType& pA,
            typename CloudType::parcelType& pB
        ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makePairModel(CloudType)                                               \
                                                                               \
    defineNamedTemplateTypeNameAndDebug(PairModel<CloudType>, 0);              \
                                                                               \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        PairModel<CloudType>,                                                  \
        dictionary                                                             \
    );


#define makePairModelType(SS, CloudType)                                       \
                                                                               \
    defineNamedTemplateTypeNameAndDebug(SS<CloudType>, 0);                     \
                                                                               \
    PairModel<CloudType>::                                                     \
        adddictionaryConstructorToTable<SS<CloudType> >                        \
        add##SS##CloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::PairModel<CloudType>::PairModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    dict_(dict),
    owner_(owner),
    coeffDict_(dict.subDict(type + "Coeffs"))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::PairModel<CloudType>::~PairModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const CloudType&
CML::PairModel<CloudType>::owner() const
{
    return owner_;
}


template<class CloudType>
const CML::dictionary& CML::PairModel<CloudType>::dict() const
{
    return dict_;
}


template<class CloudType>
const CML::dictionary& CML::PairModel<CloudType>::coeffDict() const
{
    return coeffDict_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::PairModel<CloudType> >
CML::PairModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    word PairModelType(dict.lookup("pairModel"));

    Info<< "Selecting pair model " << PairModelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(PairModelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown pair model type "
            << PairModelType
            << ", constructor not in hash table" << nl << nl
            << "    Valid pair model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
    }

    return autoPtr<PairModel<CloudType> >(cstrIter()(dict, owner));
}


#endif

// ************************************************************************* //
