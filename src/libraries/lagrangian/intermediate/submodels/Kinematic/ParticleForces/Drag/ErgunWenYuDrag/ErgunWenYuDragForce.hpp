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
    CML::ErgunWenYuDragForce

Description
    Ergun-Wen-Yu drag model for solid spheres.

\*---------------------------------------------------------------------------*/

#ifndef ErgunWenYuDragForce_HPP
#define ErgunWenYuDragForce_HPP

#include "ParticleForce.hpp"
#include "volFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class ErgunWenYuDragForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ErgunWenYuDragForce
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
    TypeName("ErgunWenYuDrag");


    // Constructors

        //- Construct from mesh
        ErgunWenYuDragForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Construct copy
        ErgunWenYuDragForce(const ErgunWenYuDragForce<CloudType>& df);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType>> clone() const
        {
            return autoPtr<ParticleForce<CloudType>>
            (
                new ErgunWenYuDragForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ErgunWenYuDragForce();


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
CML::scalar CML::ErgunWenYuDragForce<CloudType>::CdRe
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
CML::ErgunWenYuDragForce<CloudType>::ErgunWenYuDragForce
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
CML::ErgunWenYuDragForce<CloudType>::ErgunWenYuDragForce
(
    const ErgunWenYuDragForce<CloudType>& df
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
CML::ErgunWenYuDragForce<CloudType>::~ErgunWenYuDragForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::forceSuSp CML::ErgunWenYuDragForce<CloudType>::calcCoupled
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

    if (alphac < 0.8)
    {
        return forceSuSp
        (
            Zero,
            (mass/p.rho())
           *(150.0*(1.0 - alphac)/alphac + 1.75*Re)*muc/(alphac*sqr(p.d()))
        );
    }
    else
    {
        return forceSuSp
        (
            Zero,
            (mass/p.rho())
           *0.75*CdRe(alphac*Re)*muc*pow(alphac, -2.65)/(alphac*sqr(p.d()))
        );
    }
}


#endif
