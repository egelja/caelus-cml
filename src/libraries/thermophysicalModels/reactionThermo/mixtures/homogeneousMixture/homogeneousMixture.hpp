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
    CML::homogeneousMixture

Description
    CML::homogeneousMixture

SourceFiles
    homogeneousMixture.cpp

\*---------------------------------------------------------------------------*/

#ifndef homogeneousMixture_H
#define homogeneousMixture_H

#include "basicMultiComponentMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class homogeneousMixture Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class homogeneousMixture
:
    public basicMultiComponentMixture
{
    // Private data

        static const int nSpecies_ = 1;
        static const char* specieNames_[1];

        ThermoType reactants_;
        ThermoType products_;

        mutable ThermoType mixture_;

        //- Construct as copy (not implemented)
        homogeneousMixture(const homogeneousMixture<ThermoType>&);

        //- Regress variable
        volScalarField& b_;


public:

    //- The type of thermodynamics this mixture is instantiated for
    typedef ThermoType thermoType;


    // Constructors

        //- Construct from dictionary and mesh
        homogeneousMixture(const dictionary&, const fvMesh&);


    //- Destructor
    virtual ~homogeneousMixture()
    {}


    // Member functions

        const ThermoType& mixture(const scalar) const;

        const ThermoType& cellMixture(const label celli) const
        {
            return mixture(b_[celli]);
        }

        const ThermoType& patchFaceMixture
        (
            const label patchi,
            const label facei
        ) const
        {
            return mixture(b_.boundaryField()[patchi][facei]);
        }

        const ThermoType& cellReactants(const label) const
        {
            return reactants_;
        }

        const ThermoType& patchFaceReactants(const label, const label) const
        {
            return reactants_;
        }

        const ThermoType& cellProducts(const label) const
        {
            return products_;
        }

        const ThermoType& patchFaceProducts(const label, const label) const
        {
            return products_;
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
const char* CML::homogeneousMixture<ThermoType>::specieNames_[1] = {"b"};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
CML::homogeneousMixture<ThermoType>::homogeneousMixture
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

    reactants_(thermoDict.subDict("reactants")),
    products_(thermoDict.subDict("products")),
    mixture_("mixture", reactants_),
    b_(Y("b"))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
const ThermoType& CML::homogeneousMixture<ThermoType>::mixture
(
    const scalar b
) const
{
    if (b > 0.999)
    {
        return reactants_;
    }
    else if (b < 0.001)
    {
        return products_;
    }
    else
    {
        mixture_ = b/reactants_.W()*reactants_;
        mixture_ += (1 - b)/products_.W()*products_;

        return mixture_;
    }
}


template<class ThermoType>
void CML::homogeneousMixture<ThermoType>::read(const dictionary& thermoDict)
{
    reactants_ = ThermoType(thermoDict.subDict("reactants"));
    products_ = ThermoType(thermoDict.subDict("products"));
}


template<class ThermoType>
const ThermoType& CML::homogeneousMixture<ThermoType>::getLocalThermo
(
    const label specieI
) const
{
    if (specieI == 0)
    {
        return reactants_;
    }
    else if (specieI == 1)
    {
        return products_;
    }
    else
    {
        FatalErrorIn
        (
            "const ThermoType& CML::homogeneousMixture<ThermoType>::"
            "getLocalThermo"
            "("
                "const label "
            ") const"
        )   << "Unknown specie index " << specieI << ". Valid indices are 0..1"
            << abort(FatalError);

        return reactants_;
    }
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::nMoles
(
    const label specieI
) const
{
    return getLocalThermo(specieI).nMoles();
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::W
(
    const label specieI
) const
{
    return getLocalThermo(specieI).W();
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::Cp
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).Cp(T);
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::Cv
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).Cv(T);
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::H
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).H(T);
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::Hs
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).Hs(T);
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::Hc
(
    const label specieI
) const
{
    return getLocalThermo(specieI).Hc();
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::S
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).S(T);
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::E
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).E(T);
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::G
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).G(T);
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::A
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).A(T);
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::mu
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).mu(T);
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::kappa
(
    const label specieI,
    const scalar T
) const
{
    return getLocalThermo(specieI).kappa(T);
}


template<class ThermoType>
CML::scalar CML::homogeneousMixture<ThermoType>::alpha
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
