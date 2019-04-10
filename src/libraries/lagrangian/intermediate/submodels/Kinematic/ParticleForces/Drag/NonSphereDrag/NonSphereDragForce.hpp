/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2019 OpenFOAM Foundation
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
    CML::NonSphereDragForce

Description
    Drag model for non-spherical particles.

    The drag coefficient is given by:
    \f[
        Cd = \frac{24}{Re} (1 + a Re^{b}) + Re \frac{c}{Re + d}
    \f]

    Where \f$a\f$, \f$b\f$, \f$c\f$, and \f$d\f$ are coefficients, calculated
    as functions of \f$phi\f$. \f$phi\f$ is the ratio of the surface area of a
    sphere with the same volume as the particle to the actual surface area of
    the particle, and must be between 0 and 1.

    Reference:
    \verbatim
        Haider, A., & Levenspiel, O. (1989).
        Drag coefficient and terminal velocity of spherical and nonspherical
        particles.
        Powder technology, 58(1), 63-70.
    \endverbatim

\*---------------------------------------------------------------------------*/

#ifndef NonSphereDragForce_H
#define NonSphereDragForce_H

#include "ParticleForce.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class NonSphereDragForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NonSphereDragForce
:
    public ParticleForce<CloudType>
{
protected:

    // Protected Data

        //- The ratio of the surface area of a sphere with the same volume as
        //  the particle to the actual surface area of the particle. Between 0
        //  and 1.
        const scalar phi_;

        //- Coefficient
        const scalar a_;

        //- Coefficient
        const scalar b_;

        //- Coefficient
        const scalar c_;

        //- Coefficient
        const scalar d_;


public:

    //- Runtime type information
    TypeName("nonSphereDrag");


    // Constructors

        //- Construct from mesh
        NonSphereDragForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Construct copy
        NonSphereDragForce(const NonSphereDragForce<CloudType>& df);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType> > clone() const
        {
            return autoPtr<ParticleForce<CloudType> >
            (
                new NonSphereDragForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NonSphereDragForce();


    // Member Functions

        // Evaluation

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
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::NonSphereDragForce<CloudType>::NonSphereDragForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    ParticleForce<CloudType>(owner, mesh, dict, typeName, true),
    phi_(readScalar(this->coeffs().lookup("phi"))),
    a_(exp(2.3288 - 6.4581*phi_ + 2.4486*sqr(phi_))),
    b_(0.0964 + 0.5565*phi_),
    c_(exp(4.9050 - 13.8944*phi_ + 18.4222*sqr(phi_) - 10.2599*pow3(phi_))),
    d_(exp(1.4681 + 12.2584*phi_ - 20.7322*sqr(phi_) + 15.8855*pow3(phi_)))
{
    if (phi_ <= 0 || phi_ > 1)
    {
        FatalErrorInFunction
            << "Ratio of surface of sphere having same volume as particle to "
            << "actual surface area of particle (phi) must be greater than 0 "
            << "and less than or equal to 1" << exit(FatalError);
    }
}


template<class CloudType>
CML::NonSphereDragForce<CloudType>::NonSphereDragForce
(
    const NonSphereDragForce<CloudType>& df
)
:
    ParticleForce<CloudType>(df),
    phi_(df.phi_),
    a_(df.a_),
    b_(df.b_),
    c_(df.c_),
    d_(df.d_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::NonSphereDragForce<CloudType>::~NonSphereDragForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::forceSuSp CML::NonSphereDragForce<CloudType>::calcCoupled
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    const scalar CdRe =
        24*(1 + a_*pow(Re, b_)) + Re*c_/(1 + d_/(Re + ROOTVSMALL));

    return forceSuSp(Zero, mass*0.75*muc*CdRe/(p.rho()*sqr(p.d())));
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
