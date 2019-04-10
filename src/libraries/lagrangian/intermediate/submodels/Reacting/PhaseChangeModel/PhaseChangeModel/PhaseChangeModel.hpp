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
    CML::PhaseChangeModel

Description
    Templated phase change model class


\*---------------------------------------------------------------------------*/

#ifndef PhaseChangeModel_H
#define PhaseChangeModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "CloudSubModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class PhaseChangeModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PhaseChangeModel
:
    public CloudSubModelBase<CloudType>
{
public:

    // Public enumerations

        //- Enthalpy transfer type
        enum enthalpyTransferType
        {
            etLatentHeat,
            etEnthalpyDifference
        };

        //- Name representations of enthalpy transfer types
        static const CML::wordList enthalpyTransferTypeNames;


protected:

    // Protected data

        //- Enthalpy transfer type enumeration
        enthalpyTransferType enthalpyTransfer_;


        // Counters

            //- Mass of lagrangian phase converted
            scalar dMass_;


    // Protected Member Functions

        //- Convert word to enthalpy transfer type
        enthalpyTransferType wordToEnthalpyTransfer(const word& etName) const;

        //- Sherwood number
        scalar Sh() const;


public:

    //- Runtime type information
    TypeName("phaseChangeModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        PhaseChangeModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        PhaseChangeModel(CloudType& owner);

        //- Construct from dictionary
        PhaseChangeModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        PhaseChangeModel(const PhaseChangeModel<CloudType>& pcm);

        //- Construct and return a clone
        virtual autoPtr<PhaseChangeModel<CloudType>> clone() const = 0;


    //- Destructor
    virtual ~PhaseChangeModel();


    //- Selector
    static autoPtr<PhaseChangeModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Access

        //- Return the enthalpy transfer type enumeration
        const enthalpyTransferType& enthalpyTransfer() const;


    // Member Functions

        //- Update model
        virtual void calculate
        (
            const scalar dt,
            const label celli,
            const scalar Re,
            const scalar Pr,
            const scalar d,
            const scalar nu,
            const scalar T,
            const scalar Ts,
            const scalar pc,
            const scalar Tc,
            const scalarField& X,
            scalarField& dMassPC
        ) const = 0;

        //- Return the enthalpy per unit mass
        virtual scalar dh
        (
            const label idc,
            const label idl,
            const scalar p,
            const scalar T
        ) const;

        //- Return vapourisation temperature
        virtual scalar Tvap(const scalarField& X) const;

        //- Return maximum/limiting temperature
        virtual scalar TMax(const scalar p, const scalarField& X) const;

        //- Add to phase change mass
        void addToPhaseChangeMass(const scalar dMass);


        // I-O

            //- Write injection info to stream
            virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makePhaseChangeModel(CloudType)                                        \
                                                                               \
    typedef CloudType::reactingCloudType reactingCloudType;                    \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        PhaseChangeModel<reactingCloudType>,                                   \
        0                                                                      \
    );                                                                         \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        PhaseChangeModel<reactingCloudType>,                                   \
        dictionary                                                             \
    );                                                                         \


#define makePhaseChangeModelType(SS, CloudType)                                \
                                                                               \
    typedef CloudType::reactingCloudType reactingCloudType;                    \
    defineNamedTemplateTypeNameAndDebug(SS<reactingCloudType>, 0);             \
                                                                               \
    PhaseChangeModel<reactingCloudType>::                                      \
        adddictionaryConstructorToTable<SS<reactingCloudType> >                \
            add##SS##CloudType##reactingCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class CloudType>
const CML::wordList CML::PhaseChangeModel<CloudType>::
enthalpyTransferTypeNames
(
    IStringStream
    (
        "("
            "latentHeat "
            "enthalpyDifference"
        ")"
    )()
);


// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //

template<class CloudType>
typename CML::PhaseChangeModel<CloudType>::enthalpyTransferType
CML::PhaseChangeModel<CloudType>::wordToEnthalpyTransfer(const word& etName)
const
{
    forAll(enthalpyTransferTypeNames, i)
    {
        if (etName == enthalpyTransferTypeNames[i])
        {
            return enthalpyTransferType(i);
        }
    }

    FatalErrorInFunction
        << "Unknown enthalpyType " << etName << ". Valid selections are:" << nl
        << enthalpyTransferTypeNames << exit(FatalError);

    return enthalpyTransferType(0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::PhaseChangeModel<CloudType>::PhaseChangeModel
(
    CloudType& owner
)
:
    CloudSubModelBase<CloudType>(owner),
    enthalpyTransfer_(etLatentHeat),
    dMass_(0.0)
{}


template<class CloudType>
CML::PhaseChangeModel<CloudType>::PhaseChangeModel
(
    const PhaseChangeModel<CloudType>& pcm
)
:
    CloudSubModelBase<CloudType>(pcm),
    enthalpyTransfer_(pcm.enthalpyTransfer_),
    dMass_(pcm.dMass_)
{}


template<class CloudType>
CML::PhaseChangeModel<CloudType>::PhaseChangeModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    CloudSubModelBase<CloudType>(owner, dict, typeName, type),
    enthalpyTransfer_
    (
        wordToEnthalpyTransfer(this->coeffDict().lookup("enthalpyTransfer"))
    ),
    dMass_(0.0)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::PhaseChangeModel<CloudType>::~PhaseChangeModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const typename CML::PhaseChangeModel<CloudType>::enthalpyTransferType&
CML::PhaseChangeModel<CloudType>::enthalpyTransfer() const
{
    return enthalpyTransfer_;
}


template<class CloudType>
CML::scalar CML::PhaseChangeModel<CloudType>::dh
(
    const label idc,
    const label idl,
    const scalar p,
    const scalar T
) const
{
    return 0.0;
}


template<class CloudType>
CML::scalar CML::PhaseChangeModel<CloudType>::TMax
(
    const scalar p,
    const scalarField& X
) const
{
    return GREAT;
}


template<class CloudType>
CML::scalar CML::PhaseChangeModel<CloudType>::Tvap(const scalarField& X) const
{
    return -GREAT;
}


template<class CloudType>
void CML::PhaseChangeModel<CloudType>::addToPhaseChangeMass(const scalar dMass)
{
    dMass_ += dMass;
}


template<class CloudType>
void CML::PhaseChangeModel<CloudType>::info(Ostream& os)
{
    const scalar mass0 = this->template getBaseProperty<scalar>("mass");
    const scalar massTotal = mass0 + returnReduce(dMass_, sumOp<scalar>());

    Info<< "    Mass transfer phase change      = " << massTotal << nl;

    if (this->writeTime())
    {
        this->setBaseProperty("mass", massTotal);
        dMass_ = 0.0;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


template<class CloudType>
CML::autoPtr<CML::PhaseChangeModel<CloudType> >
CML::PhaseChangeModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    const word modelType(dict.lookup("phaseChangeModel"));

    Info<< "Selecting phase change model " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown phase change model type "
            << modelType << nl << nl
            << "Valid phase change model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<PhaseChangeModel<CloudType> >(cstrIter()(dict, owner));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
