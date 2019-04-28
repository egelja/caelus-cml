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
    CML::TomiyamaLiftForce

Description
    Tomiyama particle lift force model applicable to deformable bubbles.


\*---------------------------------------------------------------------------*/

#ifndef TomiyamaLiftForce_H
#define TomiyamaLiftForce_H

#include "LiftForce.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class TomiyamaLiftForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class TomiyamaLiftForce
:
    public LiftForce<CloudType>
{
protected:

    // Protected data

        //- Surface tension
        scalar sigma_;


    // Protected Member Functions

        //- Calculate the lift coefficient
        virtual scalar Cl
        (
            const typename CloudType::parcelType& p,
            const typename CloudType::parcelType::trackingData& td,
            const vector& curlUc,
            const scalar Re,
            const scalar muc
        ) const;


public:

    //- Runtime type information
    TypeName("TomiyamaLift");


    // Constructors

        //- Construct from mesh
        TomiyamaLiftForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict,
            const word& forceType = typeName
        );

        //- Construct copy
        TomiyamaLiftForce(const TomiyamaLiftForce& lf);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType> > clone() const
        {
            return autoPtr<ParticleForce<CloudType> >
            (
                new TomiyamaLiftForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~TomiyamaLiftForce();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::TomiyamaLiftForce<CloudType>::TomiyamaLiftForce::Cl
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const vector& curlUc,
    const scalar Re,
    const scalar muc
) const
{
    const vector& g = this->owner().g().value();

    scalar Eo = p.Eo(td, sigma_);
    scalar dH = p.d()*cbrt(1.0 + 0.163*pow(Eo, 0.757));
    scalar Eod = p.Eo(g, p.rho(), td.rhoc(), p.U(), dH, sigma_);
    scalar f = 0.00105*pow3(Eod) - 0.0159*sqr(Eod) - 0.0204*Eod + 0.474;

    if (Eod <= 4)
    {
        return min(0.288*tanh(0.121*Re), f);
    }
    else if ((Eod > 4) && (Eod <= 10))
    {
        return f;
    }
    else
    {
        return -0.27;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::TomiyamaLiftForce<CloudType>::TomiyamaLiftForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& forceType
)
:
    LiftForce<CloudType>(owner, mesh, dict, forceType),
    sigma_(readScalar(this->coeffs().lookup("sigma")))
{}


template<class CloudType>
CML::TomiyamaLiftForce<CloudType>::TomiyamaLiftForce
(
    const TomiyamaLiftForce& lf
)
:
    LiftForce<CloudType>(lf),
    sigma_(lf.sigma_)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
CML::TomiyamaLiftForce<CloudType>::~TomiyamaLiftForce()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
