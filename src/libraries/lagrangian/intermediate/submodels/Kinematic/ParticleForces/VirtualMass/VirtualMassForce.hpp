/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2017 OpenFOAM Foundation
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
    CML::VirtualMassForce

Description
    Calculates particle virtual mass force

\*---------------------------------------------------------------------------*/

#ifndef VirtualMassForce_H
#define VirtualMassForce_H

#include "PressureGradientForce.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class VirtualMassForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class VirtualMassForce
:
    public PressureGradientForce<CloudType>
{
    // Private data

        //- Virtual mass coefficient - typically 0.5
        scalar Cvm_;


public:

    //- Runtime type information
    TypeName("virtualMass");


    // Constructors

        //- Construct from mesh
        VirtualMassForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict,
            const word& forceType = typeName
        );

        //- Construct copy
        VirtualMassForce(const VirtualMassForce& pgf);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType> > clone() const
        {
            return autoPtr<ParticleForce<CloudType> >
            (
                new VirtualMassForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~VirtualMassForce();


    // Member Functions

        // Evaluation

            //- Cache fields
            virtual void cacheFields(const bool store);

            //- Calculate the non-coupled force
            virtual forceSuSp calcCoupled
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


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::VirtualMassForce<CloudType>::VirtualMassForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& forceType
)
:
    PressureGradientForce<CloudType>(owner, mesh, dict, forceType),
    Cvm_(readScalar(this->coeffs().lookup("Cvm")))
{}


template<class CloudType>
CML::VirtualMassForce<CloudType>::VirtualMassForce
(
    const VirtualMassForce& vmf
)
:
    PressureGradientForce<CloudType>(vmf),
    Cvm_(vmf.Cvm_)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
CML::VirtualMassForce<CloudType>::~VirtualMassForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::VirtualMassForce<CloudType>::cacheFields(const bool store)
{
    PressureGradientForce<CloudType>::cacheFields(store);
}


template<class CloudType>
CML::forceSuSp CML::VirtualMassForce<CloudType>::calcCoupled
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    forceSuSp value =
        PressureGradientForce<CloudType>::calcCoupled(p, td, dt, mass, Re, muc);

    value.Su() *= Cvm_;

    return value;
}


template<class CloudType>
CML::scalar CML::VirtualMassForce<CloudType>::massAdd
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar mass
) const
{
    return mass*td.rhoc()/p.rho()*Cvm_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
