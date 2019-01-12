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
    CML::LiquidEvaporation

Description
    Liquid evaporation model
    - uses ideal gas assumption

\*---------------------------------------------------------------------------*/

#ifndef LiquidEvaporation_H
#define LiquidEvaporation_H

#include "PhaseChangeModel.hpp"
#include "liquidMixtureProperties.hpp"
#include "specie.hpp"
#include "mathematicalConstants.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                     Class LiquidEvaporation Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class LiquidEvaporation
:
    public PhaseChangeModel<CloudType>
{
protected:

    // Protected data

        //- Global liquid properties data
        const liquidMixtureProperties& liquids_;

        //- List of active liquid names
        List<word> activeLiquids_;

        //- Mapping between liquid and carrier species
        List<label> liqToCarrierMap_;

        //- Mapping between local and global liquid species
        List<label> liqToLiqMap_;


    // Protected Member Functions

        //- Sherwood number as a function of Reynolds and Schmidt numbers
        scalar Sh(const scalar Re, const scalar Sc) const;

        //- Calculate the carrier phase component volume fractions at celli
        tmp<scalarField> calcXc(const label celli) const;


public:

    //- Runtime type information
    TypeName("liquidEvaporation");


    // Constructors

        //- Construct from dictionary
        LiquidEvaporation(const dictionary& dict, CloudType& cloud);

        //- Construct copy
        LiquidEvaporation(const LiquidEvaporation<CloudType>& pcm);

        //- Construct and return a clone
        virtual autoPtr<PhaseChangeModel<CloudType> > clone() const
        {
            return autoPtr<PhaseChangeModel<CloudType> >
            (
                new LiquidEvaporation<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~LiquidEvaporation();


    // Member Functions

        //- Update model
        virtual void calculate
        (
            const scalar dt,
            const label celli,
            const scalar Re,
            const scalar Pr,
            const scalar d,
            const scalar nu,
            const scalar T,
            const scalar Ts,
            const scalar pc,
            const scalar Tc,
            const scalarField& X,
            scalarField& dMassPC
        ) const;

        //- Return the enthalpy per unit mass
        virtual scalar dh
        (
            const label idc,
            const label idl,
            const scalar p,
            const scalar T
        ) const;

        //- Return vapourisation temperature
        virtual scalar Tvap(const scalarField& X) const;

        //- Return maximum/limiting temperature
        virtual scalar TMax(const scalar p, const scalarField& X) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


using namespace CML::constant::mathematical;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class CloudType>
CML::tmp<CML::scalarField> CML::LiquidEvaporation<CloudType>::calcXc
(
    const label celli
) const
{
    scalarField Xc(this->owner().thermo().carrier().Y().size());

    forAll(Xc, i)
    {
        Xc[i] =
            this->owner().thermo().carrier().Y()[i][celli]
           /this->owner().thermo().carrier().Wi(i);
    }

    return Xc/sum(Xc);
}


template<class CloudType>
CML::scalar CML::LiquidEvaporation<CloudType>::Sh
(
    const scalar Re,
    const scalar Sc
) const
{
    return 2.0 + 0.6*CML::sqrt(Re)*cbrt(Sc);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::LiquidEvaporation<CloudType>::LiquidEvaporation
(
    const dictionary& dict,
    CloudType& owner
)
:
    PhaseChangeModel<CloudType>(dict, owner, typeName),
    liquids_(owner.thermo().liquids()),
    activeLiquids_(this->coeffDict().lookup("activeLiquids")),
    liqToCarrierMap_(activeLiquids_.size(), -1),
    liqToLiqMap_(activeLiquids_.size(), -1)
{
    if (activeLiquids_.size() == 0)
    {
        WarningInFunction
            << "Evaporation model selected, but no active liquids defined"
            << nl << endl;
    }
    else
    {
        Info<< "Participating liquid species:" << endl;

        // Determine mapping between liquid and carrier phase species
        forAll(activeLiquids_, i)
        {
            Info<< "    " << activeLiquids_[i] << endl;
            liqToCarrierMap_[i] =
                owner.composition().carrierId(activeLiquids_[i]);
        }

        // Determine mapping between model active liquids and global liquids
        const label idLiquid = owner.composition().idLiquid();
        forAll(activeLiquids_, i)
        {
            liqToLiqMap_[i] =
                owner.composition().localId(idLiquid, activeLiquids_[i]);
        }
    }
}


template<class CloudType>
CML::LiquidEvaporation<CloudType>::LiquidEvaporation
(
    const LiquidEvaporation<CloudType>& pcm
)
:
    PhaseChangeModel<CloudType>(pcm),
    liquids_(pcm.owner().thermo().liquids()),
    activeLiquids_(pcm.activeLiquids_),
    liqToCarrierMap_(pcm.liqToCarrierMap_),
    liqToLiqMap_(pcm.liqToLiqMap_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::LiquidEvaporation<CloudType>::~LiquidEvaporation()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::LiquidEvaporation<CloudType>::calculate
(
    const scalar dt,
    const label celli,
    const scalar Re,
    const scalar Pr,
    const scalar d,
    const scalar nu,
    const scalar T,
    const scalar Ts,
    const scalar pc,
    const scalar Tc,
    const scalarField& X,
    scalarField& dMassPC
) const
{
    // immediately evaporate mass that has reached critical condition
    if ((liquids_.Tc(X) - T) < SMALL)
    {
        if (debug)
        {
            WarningInFunction
                << "Parcel reached critical conditions: "
                << "evaporating all available mass" << endl;
        }

        forAll(activeLiquids_, i)
        {
            const label lid = liqToLiqMap_[i];
            dMassPC[lid] = GREAT;
        }

        return;
    }

    // construct carrier phase species volume fractions for cell, celli
    const scalarField Xc(calcXc(celli));

    // calculate mass transfer of each specie in liquid
    forAll(activeLiquids_, i)
    {
        const label gid = liqToCarrierMap_[i];
        const label lid = liqToLiqMap_[i];

        // vapour diffusivity [m2/s]
        const scalar Dab = liquids_.properties()[lid].D(pc, Ts);

        // saturation pressure for species i [pa]
        // - carrier phase pressure assumed equal to the liquid vapour pressure
        //   close to the surface
        // NOTE: if pSat > pc then particle is superheated
        // calculated evaporation rate will be greater than that of a particle
        // at boiling point, but this is not a boiling model
        const scalar pSat = liquids_.properties()[lid].pv(pc, T);

        // Schmidt number
        const scalar Sc = nu/(Dab + ROOTVSMALL);

        // Sherwood number
        const scalar Sh = this->Sh(Re, Sc);

        // mass transfer coefficient [m/s]
        const scalar kc = Sh*Dab/(d + ROOTVSMALL);

        // vapour concentration at surface [kmol/m3] at film temperature
        const scalar Cs = pSat/(RR*Ts);

        // vapour concentration in bulk gas [kmol/m3] at film temperature
        const scalar Cinf = Xc[gid]*pc/(RR*Ts);

        // molar flux of vapour [kmol/m2/s]
        const scalar Ni = max(kc*(Cs - Cinf), 0.0);

        // mass transfer [kg]
        dMassPC[lid] += Ni*pi*sqr(d)*liquids_.properties()[lid].W()*dt;
    }
}


template<class CloudType>
CML::scalar CML::LiquidEvaporation<CloudType>::dh
(
    const label idc,
    const label idl,
    const scalar p,
    const scalar T
) const
{
    scalar dh = 0;

    typedef PhaseChangeModel<CloudType> parent;
    switch (parent::enthalpyTransfer_)
    {
        case (parent::etLatentHeat):
        {
            dh = liquids_.properties()[idl].hl(p, T);
            break;
        }
        case (parent::etEnthalpyDifference):
        {
            scalar hc = this->owner().composition().carrier().Ha(idc, p, T);
            scalar hp = liquids_.properties()[idl].h(p, T);

            dh = hc - hp;
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unknown enthalpyTransfer type" << abort(FatalError);
        }
    }

    return dh;
}


template<class CloudType>
CML::scalar CML::LiquidEvaporation<CloudType>::Tvap
(
    const scalarField& X
) const
{
    return liquids_.Tpt(X);
}


template<class CloudType>
CML::scalar CML::LiquidEvaporation<CloudType>::TMax
(
    const scalar p,
    const scalarField& X
) const
{
    return liquids_.pvInvert(p, X);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
