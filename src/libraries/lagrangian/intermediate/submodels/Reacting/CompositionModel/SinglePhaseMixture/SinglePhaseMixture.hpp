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
    CML::SinglePhaseMixture

Description
    Templated parcel single phase, multi-component class


\*---------------------------------------------------------------------------*/

#ifndef SinglePhaseMixture_H
#define SinglePhaseMixture_H

#include "CompositionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class SinglePhaseMixture Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class SinglePhaseMixture
:
    public CompositionModel<CloudType>
{
    // Private data

        // Indices of the phases - only 1 will be set

            //- Gas
            label idGas_;

            //- Liquid
            label idLiquid_;

            //- Solid
            label idSolid_;


    // Private Member Functions

        //- Construct the indices and check correct specification of
        //  1 gas or 1 liquid or 1 solid
        void constructIds();


public:

    //- Runtime type information
    TypeName("singlePhaseMixture");


    // Constructors

        //- Construct from dictionary
        SinglePhaseMixture(const dictionary& dict, CloudType& owner);

        //- Construct copy
        SinglePhaseMixture(const SinglePhaseMixture<CloudType>& cm);

        //- Construct and return a clone
        virtual autoPtr<CompositionModel<CloudType> > clone() const
        {
            return autoPtr<CompositionModel<CloudType> >
            (
                new SinglePhaseMixture<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~SinglePhaseMixture();


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


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class CloudType>
void CML::SinglePhaseMixture<CloudType>::constructIds()
{
    if (this->phaseProps().size() == 0)
    {
        FatalErrorInFunction
            << "Phase list is empty" << exit(FatalError);
    }
    else if (this->phaseProps().size() > 1)
    {
        FatalErrorInFunction
            << "Only one phase permitted" << exit(FatalError);
    }

    switch (this->phaseProps()[0].phase())
    {
        case phaseProperties::GAS:
        {
            idGas_ = 0;
            break;
        }
        case phaseProperties::LIQUID:
        {
            idLiquid_ = 0;
            break;
        }
        case phaseProperties::SOLID:
        {
            idSolid_ = 0;
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown phase enumeration" << abort(FatalError);
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::SinglePhaseMixture<CloudType>::SinglePhaseMixture
(
    const dictionary& dict,
    CloudType& owner
)
:
    CompositionModel<CloudType>(dict, owner, typeName),
    idGas_(-1),
    idLiquid_(-1),
    idSolid_(-1)
{
    constructIds();
}


template<class CloudType>
CML::SinglePhaseMixture<CloudType>::SinglePhaseMixture
(
    const SinglePhaseMixture<CloudType>& cm
)
:
    CompositionModel<CloudType>(cm),
    idGas_(cm.idGas_),
    idLiquid_(cm.idLiquid_),
    idSolid_(cm.idSolid_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::SinglePhaseMixture<CloudType>::~SinglePhaseMixture()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
const CML::scalarField&
CML::SinglePhaseMixture<CloudType>::YMixture0() const
{
    return this->phaseProps()[0].Y();
}


template<class CloudType>
CML::label CML::SinglePhaseMixture<CloudType>::idGas() const
{
    return idGas_;
}


template<class CloudType>
CML::label CML::SinglePhaseMixture<CloudType>::idLiquid() const
{
    return idLiquid_;
}


template<class CloudType>
CML::label CML::SinglePhaseMixture<CloudType>::idSolid() const
{
    return idSolid_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
