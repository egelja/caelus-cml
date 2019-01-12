/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::thermophysicalPropertiesSelector

Description
    Wrapper class providing run-time selection of thermophysicalProperties
    for the templated thermodynamics packages.

SourceFiles
    thermophysicalPropertiesSelector.cpp

\*---------------------------------------------------------------------------*/

#ifndef thermophysicalPropertiesSelector_HPP
#define thermophysicalPropertiesSelector_HPP

#include "thermophysicalProperties.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
              Class thermophysicalPropertiesSelector Declaration
\*---------------------------------------------------------------------------*/

template<class ThermophysicalProperties>
class thermophysicalPropertiesSelector
{

    autoPtr<ThermophysicalProperties> propertiesPtr_;


public:

    //- Construct from name
    thermophysicalPropertiesSelector(const word& name);

    //- Construct from dictionary
    thermophysicalPropertiesSelector(const dictionary& dict);


    // Static data

    //- Is the equation of state is incompressible i.e. rho != f(p)
    static const bool incompressible =
        ThermophysicalProperties::incompressible;

    //- Is the equation of state is isochoric i.e. rho = const
    static const bool isochoric =
        ThermophysicalProperties::isochoric;


    // Member Functions

    //- Return the instantiated type name
    static word typeName()
    {
        return
            "thermophysicalPropertiesSelector<"
          + ThermophysicalProperties::typeName
          + '>';
    }

    //- Return reference to the selected physical properties class
    inline const ThermophysicalProperties& properties() const
    {
        return propertiesPtr_();
    }


    // Physical constants which define the specie

    //- Molecular weight [kg/kmol]
    inline scalar W() const
    {
        return propertiesPtr_->W();
    }

    //- Limit the temperature to be in the range Tlow_ to Thigh_
    inline scalar limit(const scalar T) const
    {
        return propertiesPtr_->limit(T);
    }


    // Fundamental equation of state properties

    //- Liquid density [kg/m^3]
    inline scalar rho(scalar p, scalar T) const
    {
        return propertiesPtr_->rho(p, T);
    }

    //- Liquid compressibility rho/p [s^2/m^2]
    //  Note: currently it is assumed the liquid is incompressible
    inline scalar psi(scalar p, scalar T) const
    {
        return propertiesPtr_->psi(p, T);
    }

    //- Return (Cp - Cv) [J/(kg K]
    //  Note: currently it is assumed the liquid is incompressible
    //  so CpMCv 0
    inline scalar CpMCv(scalar p, scalar T) const
    {
        return propertiesPtr_->CpMCv(p, T);
    }


    // Fundamental thermodynamic properties

    //- Heat capacity at constant pressure [J/(kg K)]
    inline scalar Cp(const scalar p, const scalar T) const
    {
        return propertiesPtr_->Cp(p, T);
    }

    //- Sensible enthalpy [J/kg]
    inline scalar Hs(const scalar p, const scalar T) const
    {
        return propertiesPtr_->Hs(p, T);
    }

    //- Chemical enthalpy [J/kg]
    inline scalar Hc() const
    {
        return propertiesPtr_->Hc();
    }

    //- Absolute Enthalpy [J/kg]
    inline scalar Ha(const scalar p, const scalar T) const
    {
        return propertiesPtr_->Ha(p, T);
    }

    //- Heat capacity at constant volume [J/(kg K)]
    inline scalar Cv(const scalar p, const scalar T) const
    {
        return propertiesPtr_->Cp(p, T) - CpMCv(p, T);
    }

    //- Sensible internal energy [J/kg]
    inline scalar Es(const scalar p, const scalar T) const
    {
        return propertiesPtr_->Hs(p, T) - p/propertiesPtr_->rho(p, T);
    }

    //- Absolute internal energy [J/kg]
    inline scalar Ea(const scalar p, const scalar T) const
    {
        return propertiesPtr_->Ha(p, T) - p/propertiesPtr_->rho(p, T);
    }

    // Entropy [J/(kg K)]
    inline scalar S(const scalar p, const scalar T) const
    {
        return propertiesPtr_->S(p, T);
    }


    // I-O

    //- Write to Ostream
    void write(Ostream& os) const;


    // Physical properties

    //- Liquid viscosity [Pa s]
    inline scalar mu(scalar p, scalar T) const
    {
        return propertiesPtr_->mu(p, T);
    }

    //- Liquid thermal conductivity  [W/(m K)]
    inline scalar kappa(scalar p, scalar T) const
    {
        return propertiesPtr_->kappa(p, T);
    }

    //- Liquid thermal diffusivity of enthalpy [kg/ms]
    inline scalar alphah(const scalar p, const scalar T) const
    {
        return propertiesPtr_->alphah(p, T);
    }
};


} // End namespace CML


template<class ThermophysicalProperties>
CML::thermophysicalPropertiesSelector<ThermophysicalProperties>::
thermophysicalPropertiesSelector
(
    const word& name
)
:
    propertiesPtr_(ThermophysicalProperties::New(name))
{}


template<class ThermophysicalProperties>
CML::thermophysicalPropertiesSelector<ThermophysicalProperties>::
thermophysicalPropertiesSelector
(
    const dictionary& dict
)
{
    const word name(dict.first()->keyword());

    if (dict.isDict(name))
    {
        propertiesPtr_ = ThermophysicalProperties::New(dict.subDict(name));
    }
    else
    {
        propertiesPtr_ = ThermophysicalProperties::New(name);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermophysicalProperties>
void CML::thermophysicalPropertiesSelector<ThermophysicalProperties>::write
(
    Ostream& os
) const
{
    propertiesPtr_->write(os);
}


#endif
