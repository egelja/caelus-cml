/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::IDEA

Description
    The IDEA fuel is constructed by adding 30% alphaMethylNaphthalene
    with 70% n-decane.

    The new properties have been calculated by adding the values in these
    proportions and making a least square fit, using the same NSRDS-eq.
    so that Y = 0.3*Y_naphthalene + 0.7*Y_decane

    The valid Temperature range for n-decane is normally 243.51 - 617.70 K
    and for the naphthalene it is 242.67 - 772.04 K
    The least square fit was done in the interval 244 - 617 K

    The critical temperature was taken to be 618.074 K, since this
    is the 'c'-value in the rho-equation, which corresponds to Tcrit,
    This value was then used in the fit for the NSRDS6-eq, which uses Tcrit.
    (important for the latent heat and surface tension)

    The molecular weights are 142.20 and 142.285 and for the IDEA fuel
    it is thus 142.26 ( approximately 0.3*142.2 + 0.7*142.285 )

    Critical pressure was set to the lowest one (n-Decane)

    Critical volume... also the lowest one (naphthalene) 0.523 m^3/kmol

    Second Virial Coefficient is n-Decane

SourceFiles
    IDEA.cpp

\*---------------------------------------------------------------------------*/

#ifndef IDEA_HPP
#define IDEA_HPP

#include "liquidProperties.hpp"
#include "NSRDSfunc0.hpp"
#include "NSRDSfunc1.hpp"
#include "NSRDSfunc2.hpp"
#include "NSRDSfunc3.hpp"
#include "NSRDSfunc4.hpp"
#include "NSRDSfunc5.hpp"
#include "NSRDSfunc6.hpp"
#include "NSRDSfunc7.hpp"
#include "APIdiffCoefFunc.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class IDEA Declaration
\*---------------------------------------------------------------------------*/

class IDEA
:
    public liquidProperties
{

    NSRDSfunc5 rho_;
    NSRDSfunc1 pv_;
    NSRDSfunc6 hl_;
    NSRDSfunc0 Cp_;
    NSRDSfunc0 h_;
    NSRDSfunc7 Cpg_;
    NSRDSfunc4 B_;
    NSRDSfunc1 mu_;
    NSRDSfunc2 mug_;
    NSRDSfunc0 kappa_;
    NSRDSfunc2 kappag_;
    NSRDSfunc6 sigma_;
    APIdiffCoefFunc D_;


public:

    friend class liquidProperties;

    //- Runtime type information
    TypeName("IDEA");

    //- Construct null
    IDEA();

    // Construct from components
    IDEA
    (
        const liquidProperties& l,
        const NSRDSfunc5& density,
        const NSRDSfunc1& vapourPressure,
        const NSRDSfunc6& heatOfVapourisation,
        const NSRDSfunc0& heatCapacity,
        const NSRDSfunc0& enthalpy,
        const NSRDSfunc7& idealGasHeatCapacity,
        const NSRDSfunc4& secondVirialCoeff,
        const NSRDSfunc1& dynamicViscosity,
        const NSRDSfunc2& vapourDynamicViscosity,
        const NSRDSfunc0& thermalConductivity,
        const NSRDSfunc2& vapourThermalConductivity,
        const NSRDSfunc6& surfaceTension,
        const APIdiffCoefFunc& vapourDiffussivity
    );

    //- Construct from dictionary
    IDEA(const dictionary& dict);

    //- Construct and return clone
    virtual autoPtr<liquidProperties> clone() const
    {
        return autoPtr<liquidProperties>(new IDEA(*this));
    }


    // Member Functions

    //- Liquid density [kg/m^3]
    inline scalar rho(scalar p, scalar T) const
    {
        return rho_.f(p, T);
    }

    //- Vapour pressure [Pa]
    inline scalar pv(scalar p, scalar T) const
    {
        return pv_.f(p, T);
    }

    //- Heat of vapourisation [J/kg]
    inline scalar hl(scalar p, scalar T) const
    {
        return hl_.f(p, T);
    }

    //- Liquid heat capacity [J/(kg K)]
    inline scalar Cp(scalar p, scalar T) const
    {
        return Cp_.f(p, T);
    }

    //- Liquid Enthalpy [J/(kg)]
    inline scalar h(scalar p, scalar T) const
    {
        return h_.f(p, T);
    }

    //- Ideal gas heat capacity [J/(kg K)]
    inline scalar Cpg(scalar p, scalar T) const
    {
        return Cpg_.f(p, T);
    }

    //- Second Virial Coefficient [m^3/kg]
    inline scalar B(scalar p, scalar T) const
    {
        return B_.f(p, T);
    }

    //- Liquid viscosity [Pa s]
    inline scalar mu(scalar p, scalar T) const
    {
        return mu_.f(p, T);
    }

    //- Vapour viscosity [Pa s]
    inline scalar mug(scalar p, scalar T) const
    {
        return mug_.f(p, T);
    }

    //- Liquid thermal conductivity  [W/(m K)]
    inline scalar kappa(scalar p, scalar T) const
    {
        return kappa_.f(p, T);
    }

    //- Vapour thermal conductivity  [W/(m K)]
    inline scalar kappag(scalar p, scalar T) const
    {
        return kappag_.f(p, T);
    }

    //- Surface tension [N/m]
    inline scalar sigma(scalar p, scalar T) const
    {
        return sigma_.f(p, T);
    }

    //- Vapour diffussivity [m2/s]
    inline scalar D(scalar p, scalar T) const
    {
        return D_.f(p, T);
    }

    //- Vapour diffussivity [m2/s] with specified binary pair
    inline scalar D(scalar p, scalar T, scalar Wb) const
    {
        return D_.f(p, T, Wb);
    }


    // I-O

    //- Write the function coefficients
    void writeData(Ostream& os) const;

    //- Ostream Operator
    friend Ostream& operator<<(Ostream& os, const IDEA& l);
};


Ostream& operator<<(Ostream& os, const IDEA& l);


} // End namespace CML


#endif
