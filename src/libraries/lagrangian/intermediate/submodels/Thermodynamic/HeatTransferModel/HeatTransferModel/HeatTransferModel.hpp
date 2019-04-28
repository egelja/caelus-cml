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
    CML::HeatTransferModel

Description
    Templated heat transfer model class

SourceFiles
    HeatTransferModel.cpp
    HeatTransferModelNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef HeatTransferModel_H
#define HeatTransferModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "CloudSubModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class HeatTransferModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class HeatTransferModel
:
    public CloudSubModelBase<CloudType>
{
    // Private data

        //- Apply Bird's correction to the htc
        const Switch BirdCorrection_;


public:

    //- Runtime type information
    TypeName("heatTransferModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        HeatTransferModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        HeatTransferModel(CloudType& owner);

        //- Construct from dictionary
        HeatTransferModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        HeatTransferModel(const HeatTransferModel<CloudType>& htm);

        //- Construct and return a clone
        virtual autoPtr<HeatTransferModel<CloudType> > clone() const = 0;


    //- Destructor
    virtual ~HeatTransferModel();


    //- Selector
    static autoPtr<HeatTransferModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Member Functions

        // Access

            //- Return the Bird htc correction flag
            const Switch& BirdCorrection() const;


        // Evaluation

            //- Nusselt number
            virtual scalar Nu
            (
                const scalar Re,
                const scalar Pr
            ) const = 0;

            //- Return heat transfer coefficient
            virtual scalar htc
            (
                const scalar dp,
                const scalar Re,
                const scalar Pr,
                const scalar kappa,
                const scalar NCpW
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeHeatTransferModel(CloudType)                                       \
                                                                               \
    typedef CloudType::thermoCloudType thermoCloudType;                        \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        HeatTransferModel<thermoCloudType>,                                    \
        0                                                                      \
    );                                                                         \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        HeatTransferModel<thermoCloudType>,                                    \
        dictionary                                                             \
    );


#define makeHeatTransferModelType(SS, CloudType)                               \
                                                                               \
    typedef CloudType::thermoCloudType thermoCloudType;                        \
    defineNamedTemplateTypeNameAndDebug(SS<thermoCloudType>, 0);               \
                                                                               \
    HeatTransferModel<thermoCloudType>::                                       \
        adddictionaryConstructorToTable<SS<thermoCloudType> >                  \
            add##SS##CloudType##thermoCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::HeatTransferModel<CloudType>::HeatTransferModel(CloudType& owner)
:
    CloudSubModelBase<CloudType>(owner),
    BirdCorrection_(false)
{}


template<class CloudType>
CML::HeatTransferModel<CloudType>::HeatTransferModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    CloudSubModelBase<CloudType>(owner, dict, typeName, type),
    BirdCorrection_(this->coeffDict().lookup("BirdCorrection"))
{}


template<class CloudType>
CML::HeatTransferModel<CloudType>::HeatTransferModel
(
    const HeatTransferModel<CloudType>& htm
)
:
    CloudSubModelBase<CloudType>(htm),
    BirdCorrection_(htm.BirdCorrection_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::HeatTransferModel<CloudType>::~HeatTransferModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const CML::Switch& CML::HeatTransferModel<CloudType>::BirdCorrection() const
{
    return BirdCorrection_;
}


template<class CloudType>
CML::scalar CML::HeatTransferModel<CloudType>::htc
(
    const scalar dp,
    const scalar Re,
    const scalar Pr,
    const scalar kappa,
    const scalar NCpW
) const
{
    const scalar Nu = this->Nu(Re, Pr);

    scalar htc = Nu*kappa/dp;

    if (BirdCorrection_ && (mag(htc) > ROOTVSMALL) && (mag(NCpW) > ROOTVSMALL))
    {
        const scalar phit = min(NCpW/htc, 50);
        if (phit > 0.001)
        {
            htc *= phit/(exp(phit) - 1.0);
        }
    }

    return htc;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::HeatTransferModel<CloudType> >
CML::HeatTransferModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    const word modelType(dict.lookup("heatTransferModel"));

    Info<< "Selecting heat transfer model " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown heat transfer model type "
            << modelType << nl << nl
            << "Valid heat transfer model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<HeatTransferModel<CloudType> >(cstrIter()(dict, owner));
}


#endif

// ************************************************************************* //
