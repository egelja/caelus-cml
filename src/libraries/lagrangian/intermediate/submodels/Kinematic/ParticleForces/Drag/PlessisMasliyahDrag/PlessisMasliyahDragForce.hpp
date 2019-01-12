/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2017 OpenFOAM Foundation
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
    CML::PlessisMasliyahDragForce

Description
    PlessisMasliyahDragForce drag model for solid spheres.

\*---------------------------------------------------------------------------*/

#ifndef PlessisMasliyahDragForce_HPP
#define PlessisMasliyahDragForce_HPP

#include "ParticleForce.hpp"
#include "volFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class PlessisMasliyahDragForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PlessisMasliyahDragForce
:
    public ParticleForce<CloudType>
{
    // Private Data

        //- Reference to the carrier volume fraction field
        const volScalarField& alphac_;


    // Private Member Functions

        //- Drag coefficient multiplied by Reynolds number
        scalar CdRe(const scalar Re) const;


public:

    //- Runtime type information
    TypeName("PlessisMasliyahDrag");


    // Constructors

        //- Construct from mesh
        PlessisMasliyahDragForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Construct copy
        PlessisMasliyahDragForce(const PlessisMasliyahDragForce<CloudType>& df);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType>> clone() const
        {
            return autoPtr<ParticleForce<CloudType>>
            (
                new PlessisMasliyahDragForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~PlessisMasliyahDragForce();


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
};


} // End namespace CML


#include "volFields.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::PlessisMasliyahDragForce<CloudType>::CdRe
(
    const scalar Re
) const
{
    if (Re > 1000.0)
    {
        return 0.44*Re;
    }
    else
    {
        return 24.0*(1.0 + 0.15*pow(Re, 0.687));
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::PlessisMasliyahDragForce<CloudType>::PlessisMasliyahDragForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    ParticleForce<CloudType>(owner, mesh, dict, typeName, true),
    alphac_
    (
        this->mesh().template lookupObject<volScalarField>
        (
            this->coeffs().lookup("alphac")
        )
    )
{}


template<class CloudType>
CML::PlessisMasliyahDragForce<CloudType>::PlessisMasliyahDragForce
(
    const PlessisMasliyahDragForce<CloudType>& df
)
:
    ParticleForce<CloudType>(df),
    alphac_
    (
        this->mesh().template lookupObject<volScalarField>
        (
            this->coeffs().lookup("alphac")
        )
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::PlessisMasliyahDragForce<CloudType>::~PlessisMasliyahDragForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::forceSuSp CML::PlessisMasliyahDragForce<CloudType>::calcCoupled
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    scalar alphac(alphac_[p.cell()]);

    scalar cbrtAlphap(pow(1.0 - alphac, 1.0/3.0));

    scalar A =
        26.8*pow3(alphac)
       /(
            sqr(cbrtAlphap)
           *(1.0 - cbrtAlphap)
           *sqr(1.0 - sqr(cbrtAlphap))
          + SMALL
        );

    scalar B =
        sqr(alphac)
       /sqr(1.0 - sqr(cbrtAlphap));

    return forceSuSp
    (
        Zero,
        (mass/p.rho())
       *(A*(1.0 - alphac)/alphac + B*Re)*muc/(alphac*sqr(p.d()))
    );
}


#endif
