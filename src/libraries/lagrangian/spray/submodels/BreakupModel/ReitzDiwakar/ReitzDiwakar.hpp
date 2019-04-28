/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::ReitzDiwakar

Description
    secondary breakup model

    @verbatim
    Reitz, R.D.
    "Modelling atomization processes in highpressure vaporizing sprays"
    Atomization and Spray Technology 3 (1987), 309-337
    @endverbatim

    @verbatim
    Reitz, R.D. and Diwakar, R.
    "Effect of drop breakup on fuel sprays"
    SAE Tech. paper series, 860469 (1986)
    @endverbatim

    @verbatim
    Reitz, R.D. and Diwakar, R.
    "Structure of high-pressure fuel sprays"
    SAE Tech. paper series, 870598 (1987)
    @endverbatim

\*---------------------------------------------------------------------------*/

#ifndef ReitzDiwakar_H
#define ReitzDiwakar_H

#include "BreakupModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                       Class ReitzDiwakar Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ReitzDiwakar
:
    public BreakupModel<CloudType>
{
private:

    // Private data

        scalar Cbag_;
        scalar Cb_;
        scalar Cstrip_;
        scalar Cs_;


public:

    //- Runtime type information
    TypeName("ReitzDiwakar");


    // Constructors

        //- Construct from dictionary
        ReitzDiwakar(const dictionary&, CloudType&);

        //- Construct copy
        ReitzDiwakar(const ReitzDiwakar<CloudType>& bum);

        //- Construct and return a clone
        virtual autoPtr<BreakupModel<CloudType> > clone() const
        {
            return autoPtr<BreakupModel<CloudType> >
            (
                new ReitzDiwakar<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ReitzDiwakar();


    // Member Functions

        //- Update the parcel properties
        virtual bool update
        (
            const scalar dt,
            const vector& g,
            scalar& d,
            scalar& tc,
            scalar& ms,
            scalar& nParticle,
            scalar& KHindex,
            scalar& y,
            scalar& yDot,
            const scalar d0,
            const scalar rho,
            const scalar mu,
            const scalar sigma,
            const vector& U,
            const scalar rhoc,
            const scalar muc,
            const vector& Urel,
            const scalar Urmag,
            const scalar tMom,
            scalar& dChild,
            scalar& massChild
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ReitzDiwakar<CloudType>::ReitzDiwakar
(
    const dictionary& dict,
    CloudType& owner
)
:
    BreakupModel<CloudType>(dict, owner, typeName),
    Cbag_(6.0),
    Cb_(0.785),
    Cstrip_(0.5),
    Cs_(10.0)
{
    if (!this->defaultCoeffs(true))
    {
        this->coeffDict().lookup("Cbag") >> Cbag_;
        this->coeffDict().lookup("Cb") >> Cb_;
        this->coeffDict().lookup("Cstrip") >> Cstrip_;
        this->coeffDict().lookup("Cs") >> Cs_;
    }
}


template<class CloudType>
CML::ReitzDiwakar<CloudType>::ReitzDiwakar(const ReitzDiwakar<CloudType>& bum)
:
    BreakupModel<CloudType>(bum),
    Cbag_(bum.Cbag_),
    Cb_(bum.Cb_),
    Cstrip_(bum.Cstrip_),
    Cs_(bum.Cs_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ReitzDiwakar<CloudType>::~ReitzDiwakar()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
bool CML::ReitzDiwakar<CloudType>::update
(
    const scalar dt,
    const vector& g,
    scalar& d,
    scalar& tc,
    scalar& ms,
    scalar& nParticle,
    scalar& KHindex,
    scalar& y,
    scalar& yDot,
    const scalar d0,
    const scalar rho,
    const scalar mu,
    const scalar sigma,
    const vector& U,
    const scalar rhoc,
    const scalar muc,
    const vector& Urel,
    const scalar Urmag,
    const scalar tMom,
    scalar& dChild,
    scalar& massChild
)
{
    scalar d1 = d;
    scalar nuc = muc/rhoc;
    scalar We = 0.5*rhoc*sqr(Urmag)*d/sigma;
    scalar Re = Urmag*d/nuc;

    if (We > Cbag_)
    {
        if (We > Cstrip_*sqrt(Re))
        {
            scalar dStrip = sqr(2.0*Cstrip_*sigma)/(rhoc*pow3(Urmag)*muc);
            scalar tauStrip = Cs_*d*sqrt(rho/rhoc)/Urmag;
            scalar fraction = dt/tauStrip;

            // new droplet diameter, implicit calculation
            d = (fraction*dStrip + d)/(1.0 + fraction);
        }
        else
        {
            scalar dBag = 2.0*Cbag_*sigma/(rhoc*sqr(Urmag));
            scalar tauBag = Cb_*d*sqrt(rho*d/sigma);
            scalar fraction = dt/tauBag;

            // new droplet diameter, implicit calculation
            d = (fraction*dBag + d)/(1.0 + fraction);
        }

        // preserve the total mass/volume by updating the number of
        // particles in parcels due to breakup
        nParticle *= pow3(d1/d);
    }

    return false;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
