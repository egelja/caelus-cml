/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::AtomizationModel

Description
    Templated atomization model class


\*---------------------------------------------------------------------------*/

#ifndef AtomizationModel_H
#define AtomizationModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "CloudSubModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class AtomizationModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class AtomizationModel
:
    public CloudSubModelBase<CloudType>
{
public:

    //- Runtime type information
    TypeName("atomizationModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        AtomizationModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        AtomizationModel(CloudType& owner);

        //- Construct from dictionary
        AtomizationModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        AtomizationModel(const AtomizationModel<CloudType>& am);

        //- Construct and return a clone
        virtual autoPtr<AtomizationModel<CloudType>> clone() const = 0;


    //- Destructor
    virtual ~AtomizationModel();


    //- Selector
    static autoPtr<AtomizationModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Member Functions

        //- Average temperature calculation
        scalar Taverage(const scalar& Tliq, const scalar& Tc) const;

        //- Initial value of liquidCore
        virtual scalar initLiquidCore() const = 0;

        //- Flag to indicate if chi needs to be calculated
        virtual bool calcChi() const = 0;

        virtual void update
        (
            const scalar dt,
            scalar& d,
            scalar& liquidCore,
            scalar& tc,
            const scalar rho,
            const scalar mu,
            const scalar sigma,
            const scalar volFlowRate,
            const scalar rhoAv,
            const scalar Urel,
            const vector& pos,
            const vector& injectionPos,
            const scalar pAmbient,
            const scalar chi,
            Random& rndGen
        ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeAtomizationModel(CloudType)                                        \
                                                                               \
    typedef CloudType::sprayCloudType sprayCloudType;                          \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        AtomizationModel<sprayCloudType>,                                      \
        0                                                                      \
    );                                                                         \
                                                                               \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        AtomizationModel<sprayCloudType>,                                      \
        dictionary                                                             \
    );


#define makeAtomizationModelType(SS, CloudType)                                \
                                                                               \
    typedef CloudType::sprayCloudType sprayCloudType;                          \
    defineNamedTemplateTypeNameAndDebug(SS<sprayCloudType>, 0);                \
                                                                               \
    AtomizationModel<sprayCloudType>::                                         \
        adddictionaryConstructorToTable<SS<sprayCloudType> >                   \
            add##SS##CloudType##sprayCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::AtomizationModel<CloudType>::AtomizationModel
(
    CloudType& owner
)
:
    CloudSubModelBase<CloudType>(owner)
{}


template<class CloudType>
CML::AtomizationModel<CloudType>::AtomizationModel
(
    const AtomizationModel<CloudType>& am
)
:
    CloudSubModelBase<CloudType>(am)
{}


template<class CloudType>
CML::AtomizationModel<CloudType>::AtomizationModel
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
CML::AtomizationModel<CloudType>::~AtomizationModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::AtomizationModel<CloudType>::Taverage
(
    const scalar& Tl,
    const scalar& Tc
) const
{
    return (2.0*Tl + Tc)/3.0;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::AtomizationModel<CloudType> >
CML::AtomizationModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    word AtomizationModelType(dict.lookup("atomizationModel"));

    Info<< "Selecting AtomizationModel " << AtomizationModelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(AtomizationModelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown AtomizationModelType type "
            << AtomizationModelType
            << ", constructor not in hash table" << nl << nl
            << "    Valid AtomizationModel types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
    }

    return autoPtr<AtomizationModel<CloudType> >(cstrIter()(dict, owner));
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
