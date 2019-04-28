/*---------------------------------------------------------------------------*\
Copyright (C) 2018 OpenFOAM Foundation
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
    CML::ScaledForce

Description
    Particle force model scaled by a constant value

    particleForces
    {
        // Tuning the simulation by reducing the pressure gradient force

        scaled1
        {
            type scaled;
            factor 0.94;
            forceType pressureGradient;
            U U;
        }

        // Blending two drag models

        scaled2
        {
            type scaled;
            factor 0.2;
            forceType sphereDrag;
        }

        scaled3
        {
            type scaled;
            factor 0.8;
            forceType ErgunWenYuDrag;
            alphac alpha.air;
        }
    }


\*---------------------------------------------------------------------------*/

#ifndef ScaledForce_HPP
#define ScaledForce_HPP

#include "ParticleForce.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                       Class ScaledForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ScaledForce
:
    public ParticleForce<CloudType>
{
    // Private Data

        //- Model to scale
        autoPtr<ParticleForce<CloudType>> model_;

        //- Scale factor
        const scalar factor_;


    // Private member functions

        //- Create the dictionary for the sub-model
        dictionary modelDict(const dictionary& dict) const;


public:

    //- Runtime type information
    TypeName("scaled");


    // Constructors

        //- Construct from mesh
        ScaledForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Construct copy
        ScaledForce(const ScaledForce<CloudType>& df);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType>> clone() const
        {
            return autoPtr<ParticleForce<CloudType>>
            (
                new ScaledForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ScaledForce();


    // Member Functions

        // Evaluation

            //- Calculate the coupled force
            virtual forceSuSp calcCoupled
            (
                const typename CloudType::parcelType& p,
                const typename CloudType::parcelType::trackingData& td,
                const scalar dt,
                const scalar mass,
                const scalar Re,
                const scalar muc
            ) const;

            //- Calculate the non-coupled force
            virtual forceSuSp calcNonCoupled
            (
                const typename CloudType::parcelType& p,
                const typename CloudType::parcelType::trackingData& td,
                const scalar dt,
                const scalar mass,
                const scalar Re,
                const scalar muc
            ) const;

            //- Return the added mass
            virtual scalar massAdd
            (
                const typename CloudType::parcelType& p,
                const typename CloudType::parcelType::trackingData& td,
                const scalar mass
            ) const;
};


} // End namespace CML


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class CloudType>
CML::dictionary CML::ScaledForce<CloudType>::modelDict
(
    const dictionary& dict
) const
{
    dictionary modelDict(dict);
    modelDict.add<word>("type", dict.lookupType<word>("forceType"), true);
    return modelDict;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ScaledForce<CloudType>::ScaledForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    ParticleForce<CloudType>(owner, mesh, dict, typeName, true),
    model_
    (
        ParticleForce<CloudType>::New
        (
            owner,
            mesh,
            modelDict(dict),
            dict.lookupType<word>("forceType")
        )
    ),
    factor_(readScalar(this->coeffs().lookup("factor")))
{}


template<class CloudType>
CML::ScaledForce<CloudType>::ScaledForce
(
    const ScaledForce<CloudType>& df
)
:
    ParticleForce<CloudType>(df),
    model_(nullptr),
    factor_(1)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ScaledForce<CloudType>::~ScaledForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::forceSuSp CML::ScaledForce<CloudType>::calcCoupled
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    return factor_*model_->calcCoupled(p, td, dt, mass, Re, muc);
}


template<class CloudType>
CML::forceSuSp CML::ScaledForce<CloudType>::calcNonCoupled
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    return factor_*model_->calcCoupled(p, td, dt, mass, Re, muc);
}


template<class CloudType>
CML::scalar CML::ScaledForce<CloudType>::massAdd
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar mass
) const
{
    return factor_*model_->massAdd(p, td, mass);
}


#endif
