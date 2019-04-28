/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::CompositionModel

Description
    Templated reacting parcel composition model class
    Consists of carrier species (via thermo package), and additional liquids
    and solids

SourceFiles
    CompositionModel.cpp
    CompositionModelNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef CompositionModel_H
#define CompositionModel_H

#include "CloudSubModelBase.hpp"
#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

#include "PtrList.hpp"
#include "SLGThermo.hpp"

#include "phasePropertiesList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class CompositionModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class CompositionModel
:
    public CloudSubModelBase<CloudType>
{
    // Private data

        //- Reference to the thermo database
        const SLGThermo& thermo_;

        //- List of phase properties
        phasePropertiesList phaseProps_;


public:

    //- Runtime type information
    TypeName("compositionModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        CompositionModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        CompositionModel(CloudType& owner);

        //- Construct from dictionary
        CompositionModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        CompositionModel(const CompositionModel<CloudType>& cm);

        //- Construct and return a clone
        virtual autoPtr<CompositionModel<CloudType> > clone() const = 0;


    //- Destructor
    virtual ~CompositionModel();


    //- Selector
    static autoPtr<CompositionModel<CloudType> > New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Member Functions

        // Access

            //- Return the thermo database
            const SLGThermo& thermo() const;


            // Composition lists

                //- Return the carrier components (wrapper function)
                const basicSpecieMixture& carrier() const;

                //- Return the global (additional) liquids
                const liquidMixtureProperties& liquids() const;

                //- Return the global (additional) solids
                const solidMixtureProperties& solids() const;

                //- Return the list of phase properties
                const phasePropertiesList& phaseProps() const;

                //- Return the number of phases
                label nPhase() const;


            // Phase properties

                //- Return the list of phase type names
                //  If only 1 phase, return the component names of that phase
                const wordList& phaseTypes() const;

                //- Return the list of state labels (s), (l), (g) etc.
                const wordList& stateLabels() const;

                //- Return the list of component names for phaseI
                const wordList& componentNames(const label phaseI) const;

                //- Return global id of component cmptName in carrier thermo
                label carrierId
                (
                    const word& cmptName,
                    const bool allowNotFound = false
                ) const;

                //- Return local id of component cmptName in phase phaseI
                label localId
                (
                    const label phaseI,
                    const word& cmptName,
                    const bool allowNotFound = false
                ) const;

                //- Return carrier id of component given local id
                label localToCarrierId
                (
                    const label phaseI,
                    const label id,
                    const bool allowNotFound = false
                ) const;

                //- Return the list of phase phaseI mass fractions
                const scalarField& Y0(const label phaseI) const;

                //- Return the list of phase phaseI volume fractions fractions
                //  based on supplied mass fractions Y
                scalarField X(const label phaseI, const scalarField& Y) const;


            // Mixture properties

                //- Return the list of mixture mass fractions
                //  If only 1 phase, return component fractions of that phase
                virtual const scalarField& YMixture0() const = 0;

                // Indices of gas, liquid and solid phases in phase properties
                // list - returns -1 if not applicable

                    //- Gas id
                    virtual label idGas() const = 0;

                    //- Liquid id
                    virtual label idLiquid() const = 0;

                    //- Solid id
                    virtual label idSolid() const = 0;


        // Evaluation

            //- Return total enthalpy for the phase phaseI
            virtual scalar H
            (
                const label phaseI,
                const scalarField& Y,
                const scalar p,
                const scalar T
            ) const;

            //- Return sensible enthalpy for the phase phaseI
            virtual scalar Hs
            (
                const label phaseI,
                const scalarField& Y,
                const scalar p,
                const scalar T
            ) const;

            //- Return chemical enthalpy for the phase phaseI
            virtual scalar Hc
            (
                const label phaseI,
                const scalarField& Y,
                const scalar p,
                const scalar T
            ) const;

            //- Return specific heat caoacity for the phase phaseI
            virtual scalar Cp
            (
                const label phaseI,
                const scalarField& Y,
                const scalar p,
                const scalar T
            ) const;

            //- Return latent heat for the phase phaseI
            virtual scalar L
            (
                const label phaseI,
                const scalarField& Y,
                const scalar p,
                const scalar T
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeCompositionModel(CloudType)                                        \
                                                                               \
    typedef CloudType::reactingCloudType reactingCloudType;                    \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        CompositionModel<reactingCloudType>,                                   \
        0                                                                      \
    );                                                                         \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        CompositionModel<reactingCloudType>,                                   \
        dictionary                                                             \
    );                                                                         \


#define makeCompositionModelType(SS, CloudType)                                \
                                                                               \
    typedef CloudType::reactingCloudType reactingCloudType;                    \
    defineNamedTemplateTypeNameAndDebug(SS<reactingCloudType>, 0);             \
                                                                               \
    CompositionModel<reactingCloudType>::                                      \
        adddictionaryConstructorToTable<SS<reactingCloudType> >                \
            add##SS##CloudType##reactingCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::CompositionModel<CloudType>::CompositionModel(CloudType& owner)
:
    CloudSubModelBase<CloudType>(owner),
    thermo_(owner.thermo()),
    phaseProps_()
{}


template<class CloudType>
CML::CompositionModel<CloudType>::CompositionModel
(
    const dictionary& dict,
    CloudType& owner,
    const word& type
)
:
    CloudSubModelBase<CloudType>(owner, dict, typeName, type),
    thermo_(owner.thermo()),
    phaseProps_
    (
        this->coeffDict().lookup("phases"),
        thermo_.carrier().species(),
        thermo_.liquids().components(),
        thermo_.solids().components()
    )
{}


template<class CloudType>
CML::CompositionModel<CloudType>::CompositionModel
(
    const CompositionModel<CloudType>& cm
)
:
    CloudSubModelBase<CloudType>(cm),
    thermo_(cm.thermo_),
    phaseProps_(cm.phaseProps_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::CompositionModel<CloudType>::~CompositionModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const CML::SLGThermo& CML::CompositionModel<CloudType>::thermo() const
{
    return thermo_;
}


template<class CloudType>
const CML::basicSpecieMixture&
CML::CompositionModel<CloudType>::carrier() const
{
    return thermo_.carrier();
}


template<class CloudType>
const CML::liquidMixtureProperties&
CML::CompositionModel<CloudType>::liquids() const
{
    return thermo_.liquids();
}


template<class CloudType>
const CML::solidMixtureProperties&
CML::CompositionModel<CloudType>::solids() const
{
    return thermo_.solids();
}


template<class CloudType>
const CML::phasePropertiesList&
CML::CompositionModel<CloudType>::phaseProps() const
{
    return phaseProps_;
}


template<class CloudType>
CML::label CML::CompositionModel<CloudType>::nPhase() const
{
    return phaseProps_.size();
}


template<class CloudType>
const CML::wordList& CML::CompositionModel<CloudType>::phaseTypes() const
{
    // if only 1 phase, return the constituent component names
    if (phaseProps_.size() == 1)
    {
        return phaseProps_[0].names();
    }
    else
    {
        return phaseProps_.phaseTypes();
    }
}


template<class CloudType>
const CML::wordList& CML::CompositionModel<CloudType>::stateLabels() const
{
    return phaseProps_.stateLabels();
}


template<class CloudType>
const CML::wordList&
CML::CompositionModel<CloudType>::componentNames(const label phasei) const
{
    return phaseProps_[phasei].names();
}


template<class CloudType>
CML::label CML::CompositionModel<CloudType>::carrierId
(
    const word& cmptName,
    const bool allowNotFound
) const
{
    label id = thermo_.carrierId(cmptName);

    if (id < 0 && !allowNotFound)
    {
        FatalErrorInFunction
            << "Unable to determine global id for requested component "
            << cmptName << ". Available components are " << nl
            << thermo_.carrier().species()
            << abort(FatalError);
    }

    return id;
}


template<class CloudType>
CML::label CML::CompositionModel<CloudType>::localId
(
    const label phasei,
    const word& cmptName,
    const bool allowNotFound
) const
{
    label id = phaseProps_[phasei].id(cmptName);

    if (id < 0 && !allowNotFound)
    {
        FatalErrorInFunction
            << "Unable to determine local id for component " << cmptName
            << abort(FatalError);
    }

    return id;
}


template<class CloudType>
CML::label CML::CompositionModel<CloudType>::localToCarrierId
(
    const label phasei,
    const label id,
    const bool allowNotFound
) const
{
    label cid = phaseProps_[phasei].carrierIds()[id];

    if (cid < 0 && !allowNotFound)
    {
        FatalErrorInFunction
            << "Unable to determine global carrier id for phase "
            << phasei << " with local id " << id
            << abort(FatalError);
    }

    return cid;
}


template<class CloudType>
const CML::scalarField& CML::CompositionModel<CloudType>::Y0
(
    const label phasei
) const
{
    return phaseProps_[phasei].Y();
}


template<class CloudType>
CML::scalarField CML::CompositionModel<CloudType>::X
(
    const label phasei,
    const scalarField& Y
) const
{
    const phaseProperties& props = phaseProps_[phasei];
    scalarField X(Y.size());
    scalar WInv = 0.0;
    switch (props.phase())
    {
        case phaseProperties::GAS:
        {
            forAll(Y, i)
            {
                label cid = props.carrierIds()[i];
                X[i] = Y[i]/thermo_.carrier().Wi(cid);
                WInv += X[i];
            }
            break;
        }
        case phaseProperties::LIQUID:
        {
            forAll(Y, i)
            {
                X[i] = Y[i]/thermo_.liquids().properties()[i].W();
                WInv += X[i];
            }
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Only possible to convert gas and liquid mass fractions"
                << abort(FatalError);
        }
    }

    X /= WInv;

    return X;
}


template<class CloudType>
CML::scalar CML::CompositionModel<CloudType>::H
(
    const label phasei,
    const scalarField& Y,
    const scalar p,
    const scalar T
) const
{
    const phaseProperties& props = phaseProps_[phasei];
    scalar HMixture = 0.0;
    switch (props.phase())
    {
        case phaseProperties::GAS:
        {
            forAll(Y, i)
            {
                label cid = props.carrierIds()[i];
                HMixture += Y[i]*thermo_.carrier().Ha(cid, p, T);
            }
            break;
        }
        case phaseProperties::LIQUID:
        {
            forAll(Y, i)
            {
                HMixture += Y[i]*thermo_.liquids().properties()[i].h(p, T);
            }
            break;
        }
        case phaseProperties::SOLID:
        {
            forAll(Y, i)
            {
                HMixture +=
                     Y[i]
                    *(
                        thermo_.solids().properties()[i].Hf()
                      + thermo_.solids().properties()[i].Cp()*T
                     );
            }
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown phase enumeration" << abort(FatalError);
        }
    }

    return HMixture;
}


template<class CloudType>
CML::scalar CML::CompositionModel<CloudType>::Hs
(
    const label phasei,
    const scalarField& Y,
    const scalar p,
    const scalar T
) const
{
    const phaseProperties& props = phaseProps_[phasei];
    scalar HsMixture = 0.0;
    switch (props.phase())
    {
        case phaseProperties::GAS:
        {
            forAll(Y, i)
            {
                label cid = props.carrierIds()[i];
                HsMixture += Y[i]*thermo_.carrier().Hs(cid, p, T);
            }
            break;
        }
        case phaseProperties::LIQUID:
        {
            forAll(Y, i)
            {
                HsMixture +=
                    Y[i]
                   *(
                       thermo_.liquids().properties()[i].h(p, T)
                     - thermo_.liquids().properties()[i].h(p, 298.15)
                    );
            }
            break;
        }
        case phaseProperties::SOLID:
        {
            forAll(Y, i)
            {
                HsMixture += Y[i]*thermo_.solids().properties()[i].Cp()*T;
            }
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown phase enumeration"
                << abort(FatalError);
        }
    }

    return HsMixture;
}


template<class CloudType>
CML::scalar CML::CompositionModel<CloudType>::Hc
(
    const label phasei,
    const scalarField& Y,
    const scalar p,
    const scalar T
) const
{
    const phaseProperties& props = phaseProps_[phasei];
    scalar HcMixture = 0.0;
    switch (props.phase())
    {
        case phaseProperties::GAS:
        {
            forAll(Y, i)
            {
                label cid = props.carrierIds()[i];
                HcMixture += Y[i]*thermo_.carrier().Hc(cid);
            }
            break;
        }
        case phaseProperties::LIQUID:
        {
            forAll(Y, i)
            {
                HcMixture +=
                    Y[i]*thermo_.liquids().properties()[i].h(p, 298.15);
            }
            break;
        }
        case phaseProperties::SOLID:
        {
            forAll(Y, i)
            {
                HcMixture += Y[i]*thermo_.solids().properties()[i].Hf();
            }
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown phase enumeration"
                << abort(FatalError);
        }
    }

    return HcMixture;
}


template<class CloudType>
CML::scalar CML::CompositionModel<CloudType>::Cp
(
    const label phasei,
    const scalarField& Y,
    const scalar p,
    const scalar T
) const
{
    const phaseProperties& props = phaseProps_[phasei];
    scalar CpMixture = 0.0;
    switch (props.phase())
    {
        case phaseProperties::GAS:
        {
            forAll(Y, i)
            {
                label cid = props.carrierIds()[i];
                CpMixture += Y[i]*thermo_.carrier().Cp(cid, p, T);
            }
            break;
        }
        case phaseProperties::LIQUID:
        {
            forAll(Y, i)
            {
                CpMixture += Y[i]*thermo_.liquids().properties()[i].Cp(p, T);
            }
            break;
        }
        case phaseProperties::SOLID:
        {
            forAll(Y, i)
            {
                CpMixture += Y[i]*thermo_.solids().properties()[i].Cp();
            }
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown phase enumeration"
                << abort(FatalError);
        }
    }

    return CpMixture;
}


template<class CloudType>
CML::scalar CML::CompositionModel<CloudType>::L
(
    const label phasei,
    const scalarField& Y,
    const scalar p,
    const scalar T
) const
{
    const phaseProperties& props = phaseProps_[phasei];
    scalar LMixture = 0.0;
    switch (props.phase())
    {
        case phaseProperties::GAS:
        {
            if (debug)
            {
                WarningInFunction
                    << "No support for gaseous components" << endl;
            }
            break;
        }
        case phaseProperties::LIQUID:
        {
            forAll(Y, i)
            {
                LMixture += Y[i]*thermo_.liquids().properties()[i].hl(p, T);
            }
            break;
        }
        case phaseProperties::SOLID:
        {
            if (debug)
            {
                WarningInFunction
                    << "No support for solid components" << endl;
            }
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown phase enumeration"
                << abort(FatalError);
        }
    }

    return LMixture;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::autoPtr<CML::CompositionModel<CloudType> >
CML::CompositionModel<CloudType>::New
(
    const dictionary& dict,
    CloudType& owner
)
{
    const word modelType(dict.lookup("compositionModel"));

    Info<< "Selecting composition model " << modelType << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown composition model type "
            << modelType << nl << nl
            << "Valid composition model types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc() << nl
            << exit(FatalError);
    }

    return autoPtr<CompositionModel<CloudType> >(cstrIter()(dict, owner));
}


#endif

// ************************************************************************* //
