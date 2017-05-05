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
    CML::liquidMixtureProperties

Description
    A mixture of liquids.

    The theory in this class is to a very large extent based on the book:
    'The Properties of Gases & Liquids' 4th ed.
    by Reid, Prausnitz and Poling

    For now it does not do much, since the perfect gas equation is used.

    The dictionary constructor searches for the entry \c liquidComponents,
    which is a wordList. The liquid properties of each component can either
    be contained within a \c liquidProperties sub-dictionary or (for legacy
    purposes) can be found directly in the dictionary.
    The \c liquidProperties sub-dictionary entry should be used when possible
    to avoid conflicts with identically named gas-phase entries.

    A simple example of a single-component liquidMixtureProperties:
    \verbatim
        liquidComponents
        (
            H2O
        );

        // the gas-phase species
        species
        (
            AIR H2O
        );

        // thermo values from BurcatCpData
        AIR
            AIR 1 28.96518      // specie: name/nMoles/MolWt
            200 6000 1000       // low/high/common temperature
            3.0879272 0.0012459718 -4.2371895e-07 6.7477479e-11
                -3.9707697e-15 -995.26275 5.9596093   // 7 upper Temp. coeffs
            3.5683962 -0.00067872943 1.5537148e-06 -3.2993706e-12
                -4.6639539e-13 -1062.3466 3.7158296 // 7 lower Temp. coeffs
            1.4792e-06 116      // sutherlandTransport for AIR (STAR-CD)
            ;
        H2O
            H2O 1 18.01528      // specie: name/nMoles/MolWt
            200 6000 1000       // low/high/common temperature
            2.6770389 0.0029731816 -7.7376889e-07 9.4433514e-11
                -4.2689991e-15 -29885.894 6.88255    // 7 upper Temp. coeffs
            4.1986352 -0.0020364017 6.5203416e-06 -5.4879269e-09
                1.771968e-12 -30293.726 -0.84900901 // 7 lower Temp. coeffs
            1.4792e-06 116      // sutherlandTransport for AIR (STAR-CD)
            ;

        liquidProperties
        {
            H2O H2O defaultCoeffs;
        }
    \endverbatim

\*---------------------------------------------------------------------------*/

#ifndef liquidMixtureProperties_H
#define liquidMixtureProperties_H

#include "word.hpp"
#include "scalarField.hpp"
#include "PtrList.hpp"
#include "liquidProperties.hpp"
#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                Class liquidMixtureProperties Declaration
\*---------------------------------------------------------------------------*/

class liquidMixtureProperties
{
    // Private data

        //- Maximum reduced temperature
        static const scalar TrMax;

        //- The names of the liquids
        List<word> components_;

        //- The liquid properties
        PtrList<liquidProperties> properties_;


public:


    // Constructors

        //- Construct from dictionary
        liquidMixtureProperties(const dictionary& dict);

        //- Construct copy
        liquidMixtureProperties(const liquidMixtureProperties& lm);

        //- Construct and return a clone
        virtual autoPtr<liquidMixtureProperties> clone() const
        {
            return autoPtr<liquidMixtureProperties>
            (
                new liquidMixtureProperties(*this)
            );
        }


    //- Destructor
    virtual ~liquidMixtureProperties()
    {}


    // Selectors

        //- Select construct from dictionary
        static autoPtr<liquidMixtureProperties> New(const dictionary&);


    // Member Functions

        //- Return the liquid names
        inline const List<word>& components() const
        {
            return components_;
        }

        //- Return the liquid properties
        inline const PtrList<liquidProperties>& properties() const
        {
            return properties_;
        }

        //- Return the number of liquids in the mixture
        inline label size() const
        {
            return components_.size();
        }

        //- Calculate the critical temperature of mixture
        scalar Tc(const scalarField& x) const;

        //- Invert the vapour pressure relationship to retrieve the boiling
        //  temperature of the mixture as a function of pressure
        scalar pvInvert(const scalar p, const scalarField& x) const;

        //- Return pseudocritical temperature according to Kay's rule
        scalar Tpc(const scalarField& x) const;

        //- Return pseudocritical pressure (modified Prausnitz and Gunn)
        scalar Ppc(const scalarField& x) const;

        //- Return pseudo triple point temperature (mole averaged formulation)
        scalar Tpt(const scalarField& x) const;

        //- Return mixture accentric factor
        scalar omega(const scalarField& x) const;

        //- Return the surface molar fractions
        scalarField Xs
        (
            const scalar p,
            const scalar Tg,
            const scalar Tl,
            const scalarField& xg,
            const scalarField& xl
        ) const;


        //- Calculate the mean molecular weight [kg/kmol]
        // from mole fractions
        scalar W(const scalarField& x) const;

        //- Returns the mass fractions, given mole fractions
        scalarField Y(const scalarField& X) const;

        //- Returns the mole fractions, given mass fractions
        scalarField X(const scalarField& Y) const;

        //- Calculate the mixture density [kg/m^3]
        scalar rho
        (
            const scalar p,
            const scalar T,
            const scalarField& x
        ) const;

        //- Calculate the mixture vapour pressure [Pa]
        scalar pv
        (
            const scalar p,
            const scalar T,
            const scalarField& x
        ) const;

        //- Calculate the mixture latent heat [J/kg]
        scalar hl
        (
            const scalar p,
            const scalar T,
            const scalarField& x
        ) const;

        //- Calculate the mixture heat capacity [J/(kg K)]
        scalar Cp
        (
            const scalar p,
            const scalar T,
            const scalarField& x
        ) const;

        //- Estimate mixture surface tension [N/m]
        scalar sigma
        (
            const scalar p,
            const scalar T,
            const scalarField& x
        ) const;

        //- Calculate the mixture viscosity [Pa s]
        scalar mu
        (
            const scalar p,
            const scalar T,
            const scalarField& x
        ) const;

        //- Estimate thermal conductivity  [W/(m K)]
        // Li's method, Eq. 10-12.27 - 10.12-19
        scalar K
        (
            const scalar p,
            const scalar T,
            const scalarField& x
        ) const;

        scalar D
        (
            const scalar p,
            const scalar T,
            const scalarField& x
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
