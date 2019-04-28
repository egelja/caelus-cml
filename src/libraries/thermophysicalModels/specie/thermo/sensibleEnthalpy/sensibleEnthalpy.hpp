/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
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
    CML::sensibleEnthalpy

Description
    Thermodynamics mapping class to expose the sensible enthalpy functions.

\*---------------------------------------------------------------------------*/

#ifndef sensibleEnthalpy_HPP
#define sensibleEnthalpy_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class sensibleEnthalpy Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo>
class sensibleEnthalpy
{

public:


    //- Construct
    sensibleEnthalpy()
    {}


    // Member Functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "sensibleEnthalpy";
    }

    // Fundamental properties

    static word energyName()
    {
        return "h";
    }

    // Heat capacity at constant pressure [J/(kg K)]
    scalar Cpv
    (
        const Thermo& thermo,
        const scalar p,
        const scalar T
    ) const
    {
        return thermo.Cp(p, T);
    }

    //- Cp/Cp []
    scalar CpByCpv
    (
        const Thermo& thermo,
        const scalar p,
        const scalar T
    ) const
    {
        return 1;
    }

    // Sensible enthalpy [J/kg]
    scalar HE
    (
        const Thermo& thermo,
        const scalar p,
        const scalar T
    ) const
    {
        return thermo.Hs(p, T);
    }

    //- Temperature from sensible enthalpy
    //  given an initial temperature T0
    scalar THE
    (
        const Thermo& thermo,
        const scalar h,
        const scalar p,
        const scalar T0
    ) const
    {
       return thermo.THs(h, p, T0);
    }
};


} // End namespace CML

#endif
