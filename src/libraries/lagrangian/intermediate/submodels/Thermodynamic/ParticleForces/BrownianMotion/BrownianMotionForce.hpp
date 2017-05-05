/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    CML::BrownianMotionForce

Description
    Calculates particle Brownian motion force


\*---------------------------------------------------------------------------*/

#ifndef BrownianMotionForce_H
#define BrownianMotionForce_H

#include "ParticleForce.hpp"
#include "cachedRandom.hpp"
#include "mathematicalConstants.hpp"
#include "demandDrivenData.hpp"
#include "incompressible/turbulenceModel/turbulenceModel.hpp"
#include "compressible/turbulenceModel/turbulenceModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class BrownianMotionForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class BrownianMotionForce
:
    public ParticleForce<CloudType>
{
    // Private data

        //- Reference to the cloud random number generator
        cachedRandom& rndGen_;

        //- Molecular free path length [m]
        const scalar lambda_;

        //- Turbulence flag
        bool turbulence_;

        //- Pointer to the turbulence kinetic energy field
        const volScalarField* kPtr_;

        //- Flag that indicates ownership of turbulence k field
        bool ownK_;


    // Private Member Functions

        //- Inverse erf for Gaussian distribution
        scalar erfInv(const scalar y) const;

        //- Return the k field from the turbulence model
        tmp<volScalarField> kModel() const;


public:

    //- Runtime type information
    TypeName("BrownianMotion");


    // Constructors

        //- Construct from mesh
        BrownianMotionForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Construct copy
        BrownianMotionForce(const BrownianMotionForce& bmf);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType> > clone() const
        {
            return autoPtr<ParticleForce<CloudType> >
            (
                new BrownianMotionForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~BrownianMotionForce();


    // Member Functions

        // Access

            //- Return const access to the molecular free path length [m]
            inline scalar lambda() const;

            //- Return const access to the turbulence flag
            inline bool turbulence() const;


        // Evaluation

            //- Cache fields
            virtual void cacheFields(const bool store);

            //- Calculate the non-coupled force
            virtual forceSuSp calcCoupled
            (
                const typename CloudType::parcelType& p,
                const scalar dt,
                const scalar mass,
                const scalar Re,
                const scalar muc
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
inline CML::scalar CML::BrownianMotionForce<CloudType>::lambda() const
{
    return lambda_;
}


template<class CloudType>
inline bool CML::BrownianMotionForce<CloudType>::turbulence() const
{
    return turbulence_;
}


using namespace CML::constant;

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::BrownianMotionForce<CloudType>::erfInv(const scalar y) const
{
    const scalar a = 0.147;
    scalar k = 2.0/(mathematical::pi*a) +  0.5*log(1.0 - y*y);
    scalar h = log(1.0 - y*y)/a;
    scalar x = sqrt(-k + sqrt(k*k - h));

    if (y < 0.0)
    {
        return -x;
    }
    else
    {
        return x;
    }
}


template<class CloudType>
CML::tmp<CML::volScalarField>
CML::BrownianMotionForce<CloudType>::kModel() const
{
    const objectRegistry& obr = this->owner().mesh();
    const word turbName = "turbulenceModel";

    if (obr.foundObject<compressible::turbulenceModel>(turbName))
    {
        const compressible::turbulenceModel& model =
            obr.lookupObject<compressible::turbulenceModel>(turbName);
        return model.k();
    }
    else if (obr.foundObject<incompressible::turbulenceModel>(turbName))
    {
        const incompressible::turbulenceModel& model =
            obr.lookupObject<incompressible::turbulenceModel>(turbName);
        return model.k();
    }
    else
    {
        FatalErrorIn
        (
            "CML::tmp<CML::volScalarField>"
            "CML::BrownianMotionForce<CloudType>::kModel() const"
        )
            << "Turbulence model not found in mesh database" << nl
            << "Database objects include: " << obr.sortedToc()
            << abort(FatalError);

        return tmp<volScalarField>(NULL);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::BrownianMotionForce<CloudType>::BrownianMotionForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    ParticleForce<CloudType>(owner, mesh, dict, typeName, true),
    rndGen_(owner.rndGen()),
    lambda_(readScalar(this->coeffs().lookup("lambda"))),
    turbulence_(readBool(this->coeffs().lookup("turbulence"))),
    kPtr_(NULL),
    ownK_(false)
{}


template<class CloudType>
CML::BrownianMotionForce<CloudType>::BrownianMotionForce
(
    const BrownianMotionForce& bmf
)
:
    ParticleForce<CloudType>(bmf),
    rndGen_(bmf.rndGen_),
    lambda_(bmf.lambda_),
    turbulence_(bmf.turbulence_),
    kPtr_(NULL),
    ownK_(false)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
CML::BrownianMotionForce<CloudType>::~BrownianMotionForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::BrownianMotionForce<CloudType>::cacheFields(const bool store)
{
    if (turbulence_)
    {
        if (store)
        {
            tmp<volScalarField> tk = kModel();
            if (tk.isTmp())
            {
                kPtr_ = tk.ptr();
                ownK_ = true;
            }
            else
            {
                kPtr_ = tk.operator->();
                ownK_ = false;
            }
        }
        else
        {
            if (ownK_ && kPtr_)
            {
                deleteDemandDrivenData(kPtr_);
                ownK_ = false;
            }
        }
    }
}


template<class CloudType>
CML::forceSuSp CML::BrownianMotionForce<CloudType>::calcCoupled
(
    const typename CloudType::parcelType& p,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    forceSuSp value(vector::zero, 0.0);

    const scalar dp = p.d();
    const scalar Tc = p.Tc();

    const scalar eta = rndGen_.sample01<scalar>();
    const scalar alpha = 2.0*lambda_/dp;
    const scalar cc = 1.0 + alpha*(1.257 + 0.4*exp(-1.1/alpha));

    const scalar sigma = physicoChemical::sigma.value();

    scalar f = 0.0;
    if (turbulence_)
    {
        const label cellI = p.cell();
        const volScalarField& k = *kPtr_;
        const scalar kc = k[cellI];
        const scalar Dp = sigma*Tc*cc/(3*mathematical::pi*muc*dp);
        f = eta/mass*sqrt(2.0*sqr(kc)*sqr(Tc)/(Dp*dt));
    }
    else
    {
        const scalar rhoRatio = p.rho()/p.rhoc();
        const scalar s0 =
            216*muc*sigma*Tc/(sqr(mathematical::pi)*pow5(dp)*(rhoRatio)*cc);
        f = eta*sqrt(mathematical::pi*s0/dt);
    }

    const scalar sqrt2 = sqrt(2.0);
    for (label i = 0; i < 3; i++)
    {
        const scalar x = rndGen_.sample01<scalar>();
        const scalar eta = sqrt2*erfInv(2*x - 1.0);
        value.Su()[i] = mass*f*eta;
    }

    return value;
}


#endif

// ************************************************************************* //
