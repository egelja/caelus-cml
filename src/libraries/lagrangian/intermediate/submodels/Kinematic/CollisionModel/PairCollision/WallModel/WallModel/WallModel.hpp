/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::WallModel

Description
    Templated wall interaction class

SourceFiles
    WallModel.cpp
    WallModelNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef WallModel_H
#define WallModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "WallSiteData.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                          Class WallModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class WallModel
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
    TypeName("wallModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        WallModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct from components
        WallModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );


    //- Destructor
    virtual ~WallModel();


    //- Selector
    static autoPtr<WallModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Access

        //- Return the owner cloud object
        const CloudType& owner() const;

        //- Return non-const access to the owner cloud object
        CloudType& owner();

        //- Return the dictionary
        const dictionary& dict() const;

        //- Return the coefficients dictionary
        const dictionary& coeffDict() const;


    // Member Functions

        //- Return the effective radius for a particle for the model
        virtual scalar pREff(const typename CloudType::parcelType& p) const = 0;

        //- Whether the WallModel has a timestep limit that will
        //  require subCycling
        virtual bool controlsTimestep() const = 0;

        //- For WallModels that control the timestep, calculate the
        //  number of subCycles needed to satisfy the minimum
        //  allowable timestep
        virtual label nSubCycles() const = 0;

        //- Calculate the wall interaction for a parcel
        virtual void evaluateWall
        (
            typename CloudType::parcelType& p,
            const List<point>& flatSitePoints,
            const List<WallSiteData<vector> >& flatSiteData,
            const List<point>& sharpSitePoints,
            const List<WallSiteData<vector> >& sharpSiteData
        ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeWallModel(CloudType)                                               \
                                                                               \
    defineNamedTemplateTypeNameAndDebug(WallModel<CloudType>, 0);              \
                                                                               \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        WallModel<CloudType>,                                                  \
        dictionary                                                             \
    );


#define makeWallModelType(SS, CloudType)                                       \
                                                                               \
    defineNamedTemplateTypeNameAndDebug(SS<CloudType>, 0);                     \
                                                                               \
    WallModel<CloudType>::                                                     \
        adddictionaryConstructorToTable<SS<CloudType> >                        \
            add##SS##CloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::WallModel<CloudType>::WallModel
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
CML::WallModel<CloudType>::~WallModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const CloudType&
CML::WallModel<CloudType>::owner() const
{
    return owner_;
}


template<class CloudType>
CloudType&
CML::WallModel<CloudType>::owner()
{
    return owner_;
}


template<class CloudType>
const CML::dictionary& CML::WallModel<CloudType>::dict() const
{
    return dict_;
}


template<class CloudType>
const CML::dictionary&
CML::WallModel<CloudType>::coeffDict() const
{
    return coeffDict_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::WallModel<CloudType> >
CML::WallModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    word WallModelType(dict.lookup("wallModel"));

    Info<< "Selecting wall model " << WallModelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(WallModelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown wall model type type " << WallModelType
            << ", constructor not in hash table" << nl << nl
            << "    Valid wall model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
    }

    return autoPtr<WallModel<CloudType> >(cstrIter()(dict, owner));
}


#endif

// ************************************************************************* //
