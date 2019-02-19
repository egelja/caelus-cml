/*---------------------------------------------------------------------------*\
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
    CML::specieThermo

Description
    Basic thermodynamics type based on the use of fitting functions for
    cp, h, s obtained from the template argument type thermo.  All other
    properties are derived from these primitive functions.


\*---------------------------------------------------------------------------*/

#ifndef specieThermo_HPP
#define specieThermo_HPP

#include "IOmanip.hpp"
#include "thermodynamicConstants.hpp"
using namespace CML::constant::thermodynamic;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace species
{

// Forward declaration of friend functions and operators

template<class Thermo, template<class> class Type> class thermo;

template<class Thermo, template<class> class Type>
inline thermo<Thermo, Type> operator+
(
    const thermo<Thermo, Type>&,
    const thermo<Thermo, Type>&
);

template<class Thermo, template<class> class Type>
inline thermo<Thermo, Type> operator*
(
    const scalar,
    const thermo<Thermo, Type>&
);

template<class Thermo, template<class> class Type>
inline thermo<Thermo, Type> operator==
(
    const thermo<Thermo, Type>&,
    const thermo<Thermo, Type>&
);

template<class Thermo, template<class> class Type>
Ostream& operator<<
(
    Ostream&,
    const thermo<Thermo, Type>&
);


/*---------------------------------------------------------------------------*\
                           Class thermo Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo, template<class> class Type>
class thermo
:
    public Thermo,
    public Type<thermo<Thermo, Type> >
{

    //- Convergence tolerance of energy -> temperature inversion functions
    static const scalar tol_;

    //- Max number of iterations in energy->temperature inversion functions
    static const int maxIter_;


    //- Return the temperature corresponding to the value of the
    //  thermodynamic property f, given the function f = F(p, T)
    //  and dF(p, T)/dT
    inline scalar T
    (
        const scalar f,
        const scalar p,
        const scalar T0,
        scalar (thermo::*F)(const scalar, const scalar) const,
        scalar (thermo::*dFdT)(const scalar, const scalar) const,
        scalar (thermo::*limit)(const scalar) const,
        const bool diagnostics = false
    ) const
    {
        if (T0 < 0)
        {
            FatalErrorInFunction
                << "Negative initial temperature T0: " << T0
                << abort(FatalError);
        }

        scalar Test = T0;
        scalar Tnew = T0;
        scalar Ttol = T0*tol_;
        int    iter = 0;

        if (diagnostics)
        {
            const unsigned int width = IOstream::defaultPrecision() + 8;

            InfoInFunction
                << "Energy -> temperature conversion failed to converge:" << endl;
            Pout<< setw(width) << "iter"
                << setw(width) << "Test"
                << setw(width) << "e/h"
                << setw(width) << "Cv/p"
                << setw(width) << "Tnew"
                << endl;
        }

        do
        {
            Test = Tnew;
            Tnew =
                (this->*limit)
                (Test - ((this->*F)(p, Test) - f)/(this->*dFdT)(p, Test));

            if (diagnostics)
            {
                const unsigned int width = IOstream::defaultPrecision() + 8;

                Pout<< setw(width) << iter
                    << setw(width) << Test
                    << setw(width) << ((this->*F)(p, Test))
                    << setw(width) << ((this->*dFdT)(p, Test))
                    << setw(width) << Tnew
                    << endl;
            }

            if (iter++ > maxIter_)
            {
                if (!diagnostics)
                {
                    T(f, p, T0, F, dFdT, limit, true);
                }

                FatalErrorInFunction
                    << "Maximum number of iterations exceeded: " << maxIter_
                    << abort(FatalError);
            }

        } while (mag(Tnew - Test) > Ttol);

        return Tnew;
    }


public:

    //- The thermodynamics of the individual species'
    typedef thermo<Thermo, Type> thermoType;


    //- Construct from components
    inline thermo(const Thermo& sp)
    :
        Thermo(sp)
    {}

    //- Construct from dictionary
    thermo(const dictionary& dict);

    //- Construct as named copy
    inline thermo(const word& name, const thermo& st)
    :
        Thermo(name, st)
    {}


    // Member Functions

    //- Return the instantiated type name
    static word typeName()
    {
        return
              Thermo::typeName() + ','
            + Type<thermo<Thermo, Type> >::typeName();
    }

    //- Name of Enthalpy/Internal energy
    static inline word heName()
    {
        return Type<thermo<Thermo, Type> >::energyName();
    }


    // Fundamental properties
    // (These functions must be provided in derived types)

    // Heat capacity at constant pressure [J/(kg K)]
    // inline scalar Cp(const scalar p, const scalar T) const;

    // Sensible enthalpy [J/kg]
    // inline scalar Hs(const scalar p, const scalar T) const;

    // Chemical enthalpy [J/kg]
    // inline scalar Hc() const;

    // Absolute Enthalpy [J/kg]
    // inline scalar Ha(const scalar p, const scalar T) const;

    // Heat capacity at constant volume [J/(kg K)]
    // inline scalar Cv(const scalar p, const scalar T) const;

    // Sensible internal energy [J/kg]
    // inline scalar Es(const scalar p, const scalar T) const;

    // Absolute internal energy [J/kg]
    // inline scalar Ea(const scalar p, const scalar T) const;

    // Entropy [J/(kg K)]
    // inline scalar S(const scalar p, const scalar T) const;


    // Mass specific derived properties

    //- Heat capacity at constant pressure/volume [J/(kg K)]
    inline scalar Cpv(const scalar p, const scalar T) const
    {
        return Type<thermo<Thermo, Type> >::Cpv(*this, p, T);
    }

    //- Gamma = Cp/Cv []
    inline scalar gamma(const scalar p, const scalar T) const
    {
        const scalar Cp = this->Cp(p, T);
        return Cp/(Cp - this->CpMCv(p, T));
    }

    //- Ratio of heat capacity at constant pressure to that at
    //  constant pressure/volume []
    inline scalar CpByCpv(const scalar p, const scalar T) const
    {
        return Type<thermo<Thermo, Type> >::CpByCpv(*this, p, T);
    }

    //- Enthalpy/Internal energy [J/kg]
    inline scalar HE(const scalar p, const scalar T) const
    {
        return Type<thermo<Thermo, Type> >::HE(*this, p, T);
    }

    //- Gibbs free energy [J/kg]
    inline scalar G(const scalar p, const scalar T) const
    {
        return this->Ha(p, T) - T*this->S(p, T);
    }

    //- Helmholtz free energy [J/kg]
    inline scalar A(const scalar p, const scalar T) const
    {
        return this->Ea(p, T) - T*this->S(p, T);
    }


    // Mole specific derived properties

    //- Heat capacity at constant pressure [J/(kmol K)]
    inline scalar cp(const scalar p, const scalar T) const
    {
        return this->Cp(p, T)*this->W();
    }

    //- Absolute Enthalpy [J/kmol]
    inline scalar ha(const scalar p, const scalar T) const
    {
        return this->Ha(p, T)*this->W();
    }

    //- Sensible enthalpy [J/kmol]
    inline scalar hs(const scalar p, const scalar T) const
    {
        return this->Hs(p, T)*this->W();
    }

    //- Chemical enthalpy [J/kmol]
    inline scalar hc() const
    {
        return this->Hc()*this->W();
    }

    //- Entropy [J/(kmol K)]
    inline scalar s(const scalar p, const scalar T) const
    {
        return this->S(p, T)*this->W();
    }

    //- Enthalpy/Internal energy [J/kmol]
    inline scalar he(const scalar p, const scalar T) const
    {
        return this->HE(p, T)*this->W();
    }

    //- Heat capacity at constant volume [J/(kmol K)]
    inline scalar cv(const scalar p, const scalar T) const
    {
        return this->Cv(p, T)*this->W();
    }

    //- Sensible internal energy [J/kmol]
    inline scalar es(const scalar p, const scalar T) const
    {
        return this->Es(p, T)*this->W();
    }

    //- Absolute internal energy [J/kmol]
    inline scalar ea(const scalar p, const scalar T) const
    {
        return this->Ea(p, T)*this->W();
    }

    //- Gibbs free energy [J/kmol]
    inline scalar g(const scalar p, const scalar T) const
    {
        return this->G(p, T)*this->W();
    }

    //- Helmholtz free energy [J/kmol]
    inline scalar a(const scalar p, const scalar T) const
    {
        return this->A(p, T)*this->W();
    }


    // Equilibrium reaction thermodynamics

    //- Equilibrium constant [] i.t.o fugacities
    //  = PIi(fi/Pstd)^nui
    inline scalar K(const scalar p, const scalar T) const
    {
        scalar arg = -this->Y()*this->G(Pstd, T)/(RR*T);

        if (arg < 600)
        {
            return exp(arg);
        }
        else
        {
            return VGREAT;
        }
    }

    //- Equilibrium constant [] i.t.o. partial pressures
    //  = PIi(pi/Pstd)^nui
    //  For low pressures (where the gas mixture is near perfect) Kp = K
    inline scalar Kp(const scalar p, const scalar T) const
    {
        return K(p, T);
    }

    //- Equilibrium constant i.t.o. molar concentration
    //  = PIi(ci/cstd)^nui
    //  For low pressures (where the gas mixture is near perfect)
    //  Kc = Kp(pstd/(RR*T))^nu
    inline scalar Kc(const scalar p, const scalar T) const
    {
        const scalar nm = this->Y()/this->W();

        if (equal(nm, SMALL))
        {
            return Kp(p, T);
        }
        else
        {
            return Kp(p, T)*pow(Pstd/(RR*T), nm);
        }
    }

    //- Equilibrium constant [] i.t.o. mole-fractions
    //  For low pressures (where the gas mixture is near perfect)
    //  Kx = Kp(pstd/p)^nui
    inline scalar Kx
    (
        const scalar p,
        const scalar T
    ) const
    {
        const scalar nm = this->Y()/this->W();

        if (equal(nm, SMALL))
        {
            return Kp(p, T);
        }
        else
        {
            return Kp(p, T)*pow(Pstd/p, nm);
        }
    }

    //- Equilibrium constant [] i.t.o. number of moles
    //  For low pressures (where the gas mixture is near perfect)
    //  Kn = Kp(n*pstd/p)^nui where n = number of moles in mixture
    inline scalar Kn
    (
        const scalar p,
        const scalar T,
        const scalar n
    ) const
    {
        const scalar nm = this->Y()/this->W();

        if (equal(nm, SMALL))
        {
            return Kp(p, T);
        }
        else
        {
            return Kp(p, T)*pow(n*Pstd/p, nm);
        }
    }


    // Energy->temperature  inversion functions

    //- Temperature from enthalpy or internal energy
    //  given an initial temperature T0
    inline scalar THE
    (
        const scalar he,
        const scalar p,
        const scalar T0
    ) const
    {
        return Type<thermo<Thermo, Type> >::THE(*this, he, p, T0);
    }

    //- Temperature from sensible enthalpy given an initial T0
    inline scalar THs
    (
        const scalar hs,
        const scalar p,
        const scalar T0
    ) const
    {
        return T
        (
            hs,
            p,
            T0,
            &thermo<Thermo, Type>::Hs,
            &thermo<Thermo, Type>::Cp,
            &thermo<Thermo, Type>::limit
        );
    }

    //- Temperature from absolute enthalpy
    //  given an initial temperature T0
    inline scalar THa
    (
        const scalar ha,
        const scalar p,
        const scalar T0
    ) const
    {
        return T
        (
            ha,
            p,
            T0,
            &thermo<Thermo, Type>::Ha,
            &thermo<Thermo, Type>::Cp,
            &thermo<Thermo, Type>::limit
        );
    }

    //- Temperature from sensible internal energy
    //  given an initial temperature T0
    inline scalar TEs
    (
        const scalar es,
        const scalar p,
        const scalar T0
    ) const
    {
        return T
        (
            es,
            p,
            T0,
            &thermo<Thermo, Type>::Es,
            &thermo<Thermo, Type>::Cv,
            &thermo<Thermo, Type>::limit
        );
    }

    //- Temperature from absolute internal energy
    //  given an initial temperature T0
    inline scalar TEa
    (
        const scalar ea,
        const scalar p,
        const scalar T0
    ) const
    {
        return T
        (
            ea,
            p,
            T0,
            &thermo<Thermo, Type>::Ea,
            &thermo<Thermo, Type>::Cv,
            &thermo<Thermo, Type>::limit
        );
    }


    // Derivative term used for Jacobian

    //- Derivative of B (acooding to Niemeyer et al.) w.r.t. temperature
    inline scalar dKcdTbyKc(const scalar p, const scalar T) const
    {
        const scalar nm = this->Y()/this->W();

        if (equal(nm, SMALL))
        {
            return -this->dGdT(Pstd, T)*this->Y()/RR;
        }
        else
        {
            return -(nm/T + this->dGdT(Pstd, T)*this->Y()/RR);
        }
    }

    //- Derivative of cp w.r.t. temperature
    inline scalar dcpdT(const scalar p, const scalar T) const
    {
        return this->dCpdT(p, T)*this->W();;
    }


    // I-O

    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators

    inline void operator+=(const thermo& st);

    inline void operator*=(const scalar s);


    // Friend operators

    friend thermo operator+ <Thermo, Type>
    (
        const thermo& st1,
        const thermo& st2
    );

    friend thermo operator* <Thermo, Type>
    (
        const scalar s,
        const thermo& st
    );

    friend thermo operator== <Thermo, Type>
    (
        const thermo& st1,
        const thermo& st2
    );

    // Ostream Operator
    friend Ostream& operator<< <Thermo, Type>
    (
        Ostream&,
        const thermo&
    );

};


} // End namespace species
} // End namespace CML


#include "IOstreams.hpp"

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

template<class Thermo, template<class> class Type>
const CML::scalar CML::species::thermo<Thermo, Type>::tol_ = 1.0e-4;

template<class Thermo, template<class> class Type>
const int CML::species::thermo<Thermo, Type>::maxIter_ = 100;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


template<class Thermo, template<class> class Type>
CML::species::thermo<Thermo, Type>::thermo(const dictionary& dict)
:
    Thermo(dict)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo, template<class> class Type>
void CML::species::thermo<Thermo, Type>::write(Ostream& os) const
{
    Thermo::write(os);
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Thermo, template<class> class Type>
CML::Ostream& CML::species::operator<<
(
    Ostream& os, const thermo<Thermo, Type>& st
)
{
    st.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Thermo, template<class> class Type>
inline void CML::species::thermo<Thermo, Type>::operator+=
(
    const thermo<Thermo, Type>& st
)
{
    Thermo::operator+=(st);
}


template<class Thermo, template<class> class Type>
inline void CML::species::thermo<Thermo, Type>::operator*=(const scalar s)
{
    Thermo::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Thermo, template<class> class Type>
inline CML::species::thermo<Thermo, Type> CML::species::operator+
(
    const thermo<Thermo, Type>& st1,
    const thermo<Thermo, Type>& st2
)
{
    return thermo<Thermo, Type>
    (
        static_cast<const Thermo&>(st1) + static_cast<const Thermo&>(st2)
    );
}


template<class Thermo, template<class> class Type>
inline CML::species::thermo<Thermo, Type> CML::species::operator*
(
    const scalar s,
    const thermo<Thermo, Type>& st
)
{
    return thermo<Thermo, Type>
    (
        s*static_cast<const Thermo&>(st)
    );
}


template<class Thermo, template<class> class Type>
inline CML::species::thermo<Thermo, Type> CML::species::operator==
(
    const thermo<Thermo, Type>& st1,
    const thermo<Thermo, Type>& st2
)
{
    return thermo<Thermo, Type>
    (
        static_cast<const Thermo&>(st1) == static_cast<const Thermo&>(st2)
    );
}


#endif
