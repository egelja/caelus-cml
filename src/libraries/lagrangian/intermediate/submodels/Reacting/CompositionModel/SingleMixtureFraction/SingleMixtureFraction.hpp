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
    CML::SingleMixtureFraction

Description
    Templated parcel multi-phase, multi-component class


\*---------------------------------------------------------------------------*/

#ifndef SingleMixtureFraction_H
#define SingleMixtureFraction_H

#include "CompositionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class SingleMixtureFraction Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class SingleMixtureFraction
:
    public CompositionModel<CloudType>
{
    // Private data

        // Indices of the phases

            //- Gas
            label idGas_;

            //- Liquid
            label idLiquid_;

            //- Solid
            label idSolid_;


       // Mixture properties

            //- Phase component total fractions
            scalarField YMixture0_;


    // Private Member Functions

        //- Construct the indices and check correct specification of
        //  1 gas, 1 liquid and 1 solid
        void constructIds();


public:

    //- Runtime type information
    TypeName("singleMixtureFraction");


    // Constructors

        //- Construct from dictionary
        SingleMixtureFraction(const dictionary& dict, CloudType& owner);

        //- Construct copy
        SingleMixtureFraction(const SingleMixtureFraction<CloudType>& cm);

        //- Construct and return a clone
        virtual autoPtr<CompositionModel<CloudType> > clone() const
        {
            return autoPtr<CompositionModel<CloudType> >
            (
                new SingleMixtureFraction<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~SingleMixtureFraction();


    // Member Functions

        // Access

            // Mixture properties

                //- Return the list of mixture mass fractions
                virtual const scalarField& YMixture0() const;

                // Indices of gas, liquid and solid phases in phase properties
                // list

                    //- Gas id
                    virtual label idGas() const;

                    //- Liquid id
                    virtual label idLiquid() const;

                    //- Solid id
                    virtual label idSolid() const;
};


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class CloudType>
void CML::SingleMixtureFraction<CloudType>::constructIds()
{
    forAll(this->phaseProps(), phaseI)
    {
        switch (this->phaseProps()[phaseI].phase())
        {
            case phaseProperties::GAS:
            {
                idGas_ = phaseI;
                break;
            }
            case phaseProperties::LIQUID:
            {
                idLiquid_ = phaseI;
                break;
            }
            case phaseProperties::SOLID:
            {
                idSolid_ = phaseI;
                break;
            }
            default:
            {
                FatalErrorInFunction
                    << "Unknown phase enumeration" << nl << abort(FatalError);
            }
        }
    }

    if (idGas_ < 0)
    {
        FatalErrorInFunction
            << "No gas phase found in phase list:" << nl
            << this->phaseTypes() << exit(FatalError);
    }
    if (idLiquid_ < 0)
    {
        FatalErrorInFunction
            << "No liquid phase found in phase list:" << nl
            << this->phaseTypes() << exit(FatalError);
    }
    if (idSolid_ < 0)
    {
        FatalErrorInFunction
            << "No solid phase found in phase list:" << nl
            << this->phaseTypes() << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::SingleMixtureFraction<CloudType>::SingleMixtureFraction
(
    const dictionary& dict,
    CloudType& owner
)
:
    CompositionModel<CloudType>(dict, owner, typeName),

    idGas_(-1),
    idLiquid_(-1),
    idSolid_(-1),

    YMixture0_(3)
{
    constructIds();

    if (this->phaseProps().size() != 3)
    {
        FatalErrorInFunction
            << "Incorrect number of phases: " << nl
            << "    Please specify 1 gas, 1 liquid and 1 solid"
            << exit(FatalError);
    }

    this->coeffDict().lookup("YGasTot0") >> YMixture0_[idGas_];
    this->coeffDict().lookup("YLiquidTot0") >> YMixture0_[idLiquid_];
    this->coeffDict().lookup("YSolidTot0") >> YMixture0_[idSolid_];

    if (mag(sum(YMixture0_) - 1.0) > SMALL)
    {
        FatalErrorInFunction
            << "Sum of phases should be 1. Phase fractions:" << nl
            << YMixture0_ << exit(FatalError);
    }
}


template<class CloudType>
CML::SingleMixtureFraction<CloudType>::SingleMixtureFraction
(
    const SingleMixtureFraction<CloudType>& cm
)
:
    CompositionModel<CloudType>(cm),
    idGas_(cm.idGas_),
    idLiquid_(cm.idLiquid_),
    idSolid_(cm.idSolid_),
    YMixture0_(cm.YMixture0_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::SingleMixtureFraction<CloudType>::~SingleMixtureFraction()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const CML::scalarField&
CML::SingleMixtureFraction<CloudType>::YMixture0() const
{
    return YMixture0_;
}


template<class CloudType>
CML::label CML::SingleMixtureFraction<CloudType>::idGas() const
{
    return idGas_;
}


template<class CloudType>
CML::label CML::SingleMixtureFraction<CloudType>::idLiquid() const
{
    return idLiquid_;
}


template<class CloudType>
CML::label CML::SingleMixtureFraction<CloudType>::idSolid() const
{
    return idSolid_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
