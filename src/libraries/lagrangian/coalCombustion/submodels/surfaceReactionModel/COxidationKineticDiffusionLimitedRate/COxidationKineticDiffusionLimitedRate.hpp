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
    CML::COxidationKineticDiffusionLimitedRate

Description
    Kinetic/diffusion limited rate surface reaction model for coal parcels.
    Limited to:

        C(s) + Sb*O2 -> CO2

    where Sb is the stoichiometry of the reaction

\*---------------------------------------------------------------------------*/

#ifndef COxidationKineticDiffusionLimitedRate_H
#define COxidationKineticDiffusionLimitedRate_H

#include "SurfaceReactionModel.hpp"
#include "mathematicalConstants.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward class declarations
template<class CloudType>
class COxidationKineticDiffusionLimitedRate;

/*---------------------------------------------------------------------------*\
            Class COxidationKineticDiffusionLimitedRate Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class COxidationKineticDiffusionLimitedRate
:
    public SurfaceReactionModel<CloudType>
{
    // Private data

        // Model constants

            //- Stoichiometry of reaction
            const scalar Sb_;

            //- Mass diffusion limited rate constant, C1
            const scalar C1_;

            //- Kinetics limited rate pre-exponential constant, C2
            const scalar C2_;

            //- Kinetics limited rate activation energy
            const scalar E_;


        // Addressing

            //- Cs positions in global/local lists
            label CsLocalId_;

            //- O2 position in global list
            label O2GlobalId_;

            //- CO2 positions in global list
            label CO2GlobalId_;


        // Local copies of thermo properties

            //- Molecular weight of C [kg/kmol]
            scalar WC_;

            //- Molecular weight of O2 [kg/kmol]
            scalar WO2_;

            //- Formation enthalpy for CO2 [J/kg]
            scalar HcCO2_;


public:

    //- Runtime type information
    TypeName("COxidationKineticDiffusionLimitedRate");


    // Constructors

        //- Construct from dictionary
        COxidationKineticDiffusionLimitedRate
        (
            const dictionary& dict,
            CloudType& owner
        );

        //- Construct copy
        COxidationKineticDiffusionLimitedRate
        (
            const COxidationKineticDiffusionLimitedRate<CloudType>& srm
        );

        //- Construct and return a clone
        virtual autoPtr<SurfaceReactionModel<CloudType> > clone() const
        {
            return autoPtr<SurfaceReactionModel<CloudType> >
            (
                new COxidationKineticDiffusionLimitedRate<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~COxidationKineticDiffusionLimitedRate();


    // Member Functions

        //- Update surface reactions
        virtual scalar calculate
        (
            const scalar dt,
            const label celli,
            const scalar d,
            const scalar T,
            const scalar Tc,
            const scalar pc,
            const scalar rhoc,
            const scalar mass,
            const scalarField& YGas,
            const scalarField& YLiquid,
            const scalarField& YSolid,
            const scalarField& YMixture,
            const scalar N,
            scalarField& dMassGas,
            scalarField& dMassLiquid,
            scalarField& dMassSolid,
            scalarField& dMassSRCarrier
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::COxidationKineticDiffusionLimitedRate<CloudType>::
COxidationKineticDiffusionLimitedRate
(
    const dictionary& dict,
    CloudType& owner
)
:
    SurfaceReactionModel<CloudType>(dict, owner, typeName),
    Sb_(readScalar(this->coeffDict().lookup("Sb"))),
    C1_(readScalar(this->coeffDict().lookup("C1"))),
    C2_(readScalar(this->coeffDict().lookup("C2"))),
    E_(readScalar(this->coeffDict().lookup("E"))),
    CsLocalId_(-1),
    O2GlobalId_(owner.composition().carrierId("O2")),
    CO2GlobalId_(owner.composition().carrierId("CO2")),
    WC_(0.0),
    WO2_(0.0),
    HcCO2_(0.0)
{
    // Determine Cs ids
    label idSolid = owner.composition().idSolid();
    CsLocalId_ = owner.composition().localId(idSolid, "C");

    // Set local copies of thermo properties
    WO2_ = owner.thermo().carrier().Wi(O2GlobalId_);
    const scalar WCO2 = owner.thermo().carrier().Wi(CO2GlobalId_);
    WC_ = WCO2 - WO2_;

    HcCO2_ = owner.thermo().carrier().Hc(CO2GlobalId_);

    const scalar YCloc = owner.composition().Y0(idSolid)[CsLocalId_];
    const scalar YSolidTot = owner.composition().YMixture0()[idSolid];
    Info<< "    C(s): particle mass fraction = " << YCloc*YSolidTot << endl;
}


template<class CloudType>
CML::COxidationKineticDiffusionLimitedRate<CloudType>::
COxidationKineticDiffusionLimitedRate
(
    const COxidationKineticDiffusionLimitedRate<CloudType>& srm
)
:
    SurfaceReactionModel<CloudType>(srm),
    Sb_(srm.Sb_),
    C1_(srm.C1_),
    C2_(srm.C2_),
    E_(srm.E_),
    CsLocalId_(srm.CsLocalId_),
    O2GlobalId_(srm.O2GlobalId_),
    CO2GlobalId_(srm.CO2GlobalId_),
    WC_(srm.WC_),
    WO2_(srm.WO2_),
    HcCO2_(srm.HcCO2_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::COxidationKineticDiffusionLimitedRate<CloudType>::
~COxidationKineticDiffusionLimitedRate()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
CML::scalar CML::COxidationKineticDiffusionLimitedRate<CloudType>::calculate
(
    const scalar dt,
    const label celli,
    const scalar d,
    const scalar T,
    const scalar Tc,
    const scalar pc,
    const scalar rhoc,
    const scalar mass,
    const scalarField& YGas,
    const scalarField& YLiquid,
    const scalarField& YSolid,
    const scalarField& YMixture,
    const scalar N,
    scalarField& dMassGas,
    scalarField& dMassLiquid,
    scalarField& dMassSolid,
    scalarField& dMassSRCarrier
) const
{
    // Fraction of remaining combustible material
    const label idSolid = CloudType::parcelType::SLD;
    const scalar fComb = YMixture[idSolid]*YSolid[CsLocalId_];

    // Surface combustion active combustible fraction is consumed
    if (fComb < SMALL)
    {
        return 0.0;
    }

    const SLGThermo& thermo = this->owner().thermo();

    // Local mass fraction of O2 in the carrier phase
    const scalar YO2 = thermo.carrier().Y(O2GlobalId_)[celli];

    // Diffusion rate coefficient
    const scalar D0 = C1_/d*pow(0.5*(T + Tc), 0.75);

    // Kinetic rate
    const scalar Rk = C2_*exp(-E_/(RR*Tc));

    // Particle surface area
    const scalar Ap = constant::mathematical::pi*sqr(d);

    // Change in C mass [kg]
    scalar dmC = Ap*rhoc*RR*Tc*YO2/WO2_*D0*Rk/(D0 + Rk)*dt;

    // Limit mass transfer by availability of C
    dmC = min(mass*fComb, dmC);

    // Molar consumption
    const scalar dOmega = dmC/WC_;

    // Change in O2 mass [kg]
    const scalar dmO2 = dOmega*Sb_*WO2_;

    // Mass of newly created CO2 [kg]
    const scalar dmCO2 = dOmega*(WC_ + Sb_*WO2_);

    // Update local particle C mass
    dMassSolid[CsLocalId_] += dOmega*WC_;

    // Update carrier O2 and CO2 mass
    dMassSRCarrier[O2GlobalId_] -= dmO2;
    dMassSRCarrier[CO2GlobalId_] += dmCO2;

    const scalar HsC = thermo.solids().properties()[CsLocalId_].Hs(T);

    // carrier sensible enthalpy exchange handled via change in mass

    // Heat of reaction [J]
    return dmC*HsC - dmCO2*HcCO2_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
