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
    CML::dieselMixture

Description
    CML::dieselMixture

SourceFiles
    dieselMixture.cpp

\*---------------------------------------------------------------------------*/

#ifndef dieselMixture_H
#define dieselMixture_H

#include "basicMultiComponentMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class dieselMixture Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class dieselMixture
:
    public basicMultiComponentMixture
{
    // Private data

        static const int nSpecies_ = 2;
        static const char* specieNames_[2];

        dimensionedScalar stoicRatio_;

        ThermoType fuel_;
        ThermoType oxidant_;
        ThermoType products_;

        mutable ThermoType mixture_;

        volScalarField& ft_;
        volScalarField& fu_;

        //- Construct as copy (not implemented)
        dieselMixture(const dieselMixture<ThermoType>&);


public:

    //- The type of thermodynamics this mixture is instantiated for
    typedef ThermoType thermoType;


    // Constructors

        //- Construct from dictionary and mesh
        dieselMixture(const dictionary&, const fvMesh&);


    //- Destructor
    virtual ~dieselMixture()
    {}


    // Member functions

        const dimensionedScalar& stoicRatio() const
        {
            return stoicRatio_;
        }

        const ThermoType& mixture(const scalar, const scalar) const;

        const ThermoType& cellMixture(const label celli) const
        {
            return mixture(ft_[celli], fu_[celli]);
        }

        const ThermoType& patchFaceMixture
        (
            const label patchi,
            const label facei
        ) const
        {
            return mixture
            (
                ft_.boundaryField()[patchi][facei],
                fu_.boundaryField()[patchi][facei]
            );
        }

        const ThermoType& cellReactants(const label celli) const
        {
            return mixture(ft_[celli], ft_[celli]);
        }

        const ThermoType& patchFaceReactants
        (
            const label patchi,
            const label facei
        ) const
        {
            return mixture
            (
                ft_.boundaryField()[patchi][facei],
                ft_.boundaryField()[patchi][facei]
            );
        }

        const ThermoType& cellProducts(const label celli) const
        {
            scalar ft = ft_[celli];
            return mixture(ft, fres(ft, stoicRatio().value()));
        }

        const ThermoType& patchFaceProducts
        (
            const label patchi,
            const label facei
        ) const
        {
            scalar ft = ft_.boundaryField()[patchi][facei];
            return mixture(ft, fres(ft, stoicRatio().value()));
        }

        //- Read dictionary
        void read(const dictionary&);

        //- Return thermo based on index
        const ThermoType& getLocalThermo(const label specieI) const;


        // Per specie properties

            //- Number of moles []
            virtual scalar nMoles(const label specieI) const;

            //- Molecular weight [kg/kmol]
            virtual scalar W(const label specieI) const;


        // Per specie thermo properties

            //- Heat capacity at constant pressure [J/(kg K)]
            virtual scalar Cp(const label specieI, const scalar T) const;

            //- Heat capacity at constant volume [J/(kg K)]
            virtual scalar Cv(const label specieI, const scalar T) const;

            //- Enthalpy [J/kg]
            virtual scalar H(const label specieI, const scalar T) const;

            //- Sensible enthalpy [J/kg]
            virtual scalar Hs(const label specieI, const scalar T) const;

            //- Chemical enthalpy [J/kg]
            virtual scalar Hc(const label specieI) const;

            //- Entropy [J/(kg K)]
            virtual scalar S(const label specieI, const scalar T) const;

            //- Internal energy [J/kg]
            virtual scalar E(const label specieI, const scalar T) const;

            //- Gibbs free energy [J/kg]
            virtual scalar G(const label specieI, const scalar T) const;

            //- Helmholtz free energy [J/kg]
            virtual scalar A(const label specieI, const scalar T) const;


        // Per specie transport properties

            //- Dynamic viscosity [kg/m/s]
            virtual scalar mu(const label specieI, const scalar T) const;

            //- Thermal conductivity [W/m/K]
            virtual scalar kappa(const label specieI, const scalar T) const;

            //- Thermal diffusivity [kg/m/s]
            virtual scalar alpha(const label specieI, const scalar T) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "fvMesh.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class ThermoType>
const char* CML::dieselMixture<ThermoType>::specieNames_[2] = {"ft", "fu"};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
CML::dieselMixture<ThermoType>::dieselMixture
(
    const dictionary& thermoDict,
    const fvMesh& mesh
)
:
    basicMultiComponentMixture
    (
        thermoDict,
        speciesTable(nSpecies_, specieNames_),
        mesh
    ),

    stoicRatio_(thermoDict.lookup("stoichiometricAirFuelMassRatio")),

    fuel_(thermoDict.subDict("fuel")),
    oxidant_(thermoDict.subDict("oxidant")),
    products_(thermoDict.subDict("burntProducts")),

    mixture_("mixture", fuel_),

    ft_(Y("ft")),
    fu_(Y("fu"))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
const ThermoType& CML::dieselMixture<ThermoType>::mixture
(
    const scalar ft,
    const scalar fu
) const
{
    if (ft < 0.0001)
    {
        return oxidant_;
    }
    else
    {
        scalar ox = 1 - ft - (ft - fu)*stoicRatio().value();
        scalar pr = 1 - fu - ox;

        mixture_ = fu/fuel_.W()*fuel_;
        mixture_ += ox/oxidant_.W()*oxidant_;
        mixture_ += pr/products_.W()*products_;

        return mixture_;
    }
}


template<class ThermoType>
void CML::dieselMixture<ThermoType>::read(const dictionary& thermoDict)
{
    fuel_ = ThermoType(thermoDict.subDict("fuel"));
    oxidant_ = ThermoType(thermoDict.subDict("oxidant"));
    products_ = ThermoType(thermoDict.subDict("burntProducts"));
}


template<class ThermoType>
const ThermoType& CML::dieselMixture<ThermoType>::getLocalThermo
(
    const label specieI
) const
{
    if (specieI == 0)
    {
        return fuel_;
    }
    else if (specieI == 1)
    {
        return oxidant_;
    }
    else if (specieI == 2)
    {
        return products_;
    }
    else
    {
        FatalErrorIn
        (
            "const ThermoType& CML::dieselMixture<ThermoType>::getLocalThermo"
            "("
                "const label "
            ") const"
        )   << "Unknown specie index " << specieI << ". Valid indices are 0..2"
            << abort(FatalError);

        return fuel_;
    }
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::nMoles
(
    const label specieI
) const
{
    return getLocalThermo(specieI).nMoles();
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::W
(
    const label specieI
) const
{
    return getLocalThermo(specieI).W();
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::Cp
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).Cp(T);
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::Cv
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).Cv(T);
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::H
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).H(T);
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::Hs
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).Hs(T);
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::Hc
(
    const label specieI
) const
{
    return getLocalThermo(specieI).Hc();
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::S
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).S(T);
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::E
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).E(T);
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::G
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).G(T);
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::A
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).A(T);
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::mu
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).mu(T);
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::kappa
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).kappa(T);
}


template<class ThermoType>
CML::scalar CML::dieselMixture<ThermoType>::alpha
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).alpha(T);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
