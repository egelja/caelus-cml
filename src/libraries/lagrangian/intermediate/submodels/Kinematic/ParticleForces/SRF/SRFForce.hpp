/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2017 OpenFOAM Foundation
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
    CML::SRFForce

Description
    Calculates particle SRF reference frame force


\*---------------------------------------------------------------------------*/

#ifndef SRFForce_H
#define SRFForce_H

#include "ParticleForce.hpp"
#include "SRFModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;

/*---------------------------------------------------------------------------*\
                       Class SRFForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class SRFForce
:
    public ParticleForce<CloudType>
{
    // Private data

        //- Pointer to the SRF model
        const typename SRF::SRFModel* srfPtr_;


public:

    //- Runtime type information
    TypeName("SRF");


    // Constructors

        //- Construct from mesh
        SRFForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Construct copy
        SRFForce(const SRFForce& srff);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType> > clone() const
        {
            return autoPtr<ParticleForce<CloudType> >
            (
                new ParticleForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~SRFForce();


    // Member Functions

        // Evaluation

            //- Cache fields
            virtual void cacheFields(const bool store);

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
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::SRFForce<CloudType>::SRFForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    ParticleForce<CloudType>(owner, mesh, dict, typeName, false),
    srfPtr_(nullptr)
{}


template<class CloudType>
CML::SRFForce<CloudType>::SRFForce
(
    const SRFForce& srff
)
:
    ParticleForce<CloudType>(srff),
    srfPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
CML::SRFForce<CloudType>::~SRFForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::SRFForce<CloudType>::cacheFields(const bool store)
{
    if (store)
    {
        const typename SRF::SRFModel& model = this->mesh().template
            lookupObject<SRF::SRFModel>("SRFProperties");
        srfPtr_ = &model;
    }
    else
    {
        srfPtr_ = nullptr;
    }
}


template<class CloudType>
CML::forceSuSp CML::SRFForce<CloudType>::calcNonCoupled
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    forceSuSp value(Zero, 0.0);

    const typename SRF::SRFModel& srf = *srfPtr_;

    const vector& omega = srf.omega().value();

    const vector& r = p.position();

    // Coriolis and centrifugal acceleration terms
    value.Su() =
        mass*(1.0 - td.rhoc()/p.rho())
       *(2.0*(p.U() ^ omega) + (omega ^ (r ^ omega)));

    return value;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
