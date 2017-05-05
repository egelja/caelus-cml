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
    CML::specieThermo

Description
    Basic thermodynamics type based on the use of fitting functions for
    cp, h, s obtained from the template argument type thermo.  All other
    properties are derived from these primitive functions.

SourceFiles
    specieThermoI.hpp
    specieThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef specieThermo_H
#define specieThermo_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Thermo> class specieThermo;

template<class Thermo>
inline specieThermo<Thermo> operator+
(
    const specieThermo<Thermo>&,
    const specieThermo<Thermo>&
);

template<class Thermo>
inline specieThermo<Thermo> operator-
(
    const specieThermo<Thermo>&,
    const specieThermo<Thermo>&
);

template<class Thermo>
inline specieThermo<Thermo> operator*
(
    const scalar,
    const specieThermo<Thermo>&
);

template<class Thermo>
inline specieThermo<Thermo> operator==
(
    const specieThermo<Thermo>&,
    const specieThermo<Thermo>&
);

template<class Thermo>
Ostream& operator<<
(
    Ostream&,
    const specieThermo<Thermo>&
);


/*---------------------------------------------------------------------------*\
                           Class specieThermo Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo>
class specieThermo
:
    public Thermo
{
    // Private data

        //- Convergence tolerance of energy -> temperature inversion functions
        static const scalar tol_;

        //- Max number of iterations in energy->temperature inversion functions
        static const int maxIter_;


    // Private Member Functions

        //- Return the temperature corresponding to the value of the
        //  thermodynamic property f, given the function f = F(T) and dF(T)/dT
        inline scalar T
        (
            scalar f,
            scalar T0,
            scalar (specieThermo::*F)(const scalar) const,
            scalar (specieThermo::*dFdT)(const scalar) const,
            scalar (specieThermo::*limit)(const scalar) const
        ) const;


public:

    // Constructors

        //- construct from components
        inline specieThermo(const Thermo& sp);

        //- Construct from Istream
        specieThermo(Istream&);

        //- Construct from dictionary
        specieThermo(const dictionary& dict);

        //- Construct as named copy
        inline specieThermo(const word& name, const specieThermo&);


    // Member Functions

        // Fundamental properties
        // (These functions must be provided in derived types)

            // Heat capacity at constant pressure [J/(kmol K)]
            // scalar cp(const scalar) const;

            // Enthalpy [J/kmol]
            // scalar h(const scalar) const;

            // Sensible enthalpy [J/kmol]
            // scalar hs(const scalar) const;

            // Chemical enthalpy [J/kmol]
            // scalar hc(const scalar) const;

            // Entropy [J/(kmol K)]
            // scalar s(const scalar) const;


        // Calculate and return derived properties
        // (These functions need not provided in derived types)

            // Mole specific properties

                //- Heat capacity at constant volume [J/(kmol K)]
                inline scalar cv(const scalar T) const;

                //- gamma = cp/cv []
                inline scalar gamma(const scalar T) const;

                //- Internal energy [J/kmol]
                inline scalar e(const scalar T) const;

                //- Sensible internal energy [J/kmol]
                inline scalar es(const scalar T) const;

                //- Gibbs free energy [J/kmol]
                inline scalar g(const scalar T) const;

                //- Helmholtz free energy [J/kmol]
                inline scalar a(const scalar T) const;


            // Mass specific properties

                //- Heat capacity at constant pressure [J/(kg K)]
                inline scalar Cp(const scalar T) const;

                //- Heat capacity at constant volume [J/(kg K)]
                inline scalar Cv(const scalar T) const;

                //- Enthalpy [J/kg]
                inline scalar H(const scalar T) const;

                //- Sensible enthalpy [J/kg]
                inline scalar Hs(const scalar T) const;

                //- Chemical enthalpy [J/kg]
                inline scalar Hc() const;

                //- Entropy [J/(kg K)]
                inline scalar S(const scalar T) const;

                //- Internal energy [J/kg]
                inline scalar E(const scalar T) const;

                //- Gibbs free energy [J/kg]
                inline scalar G(const scalar T) const;

                //- Helmholtz free energy [J/kg]
                inline scalar A(const scalar T) const;


        // Equilibrium reaction thermodynamics

            //- Equilibrium constant [] i.t.o fugacities
            //  = PIi(fi/Pstd)^nui
            inline scalar K(const scalar T) const;

            //- Equilibrium constant [] i.t.o. partial pressures
            //  = PIi(pi/Pstd)^nui
            //  For low pressures (where the gas mixture is near perfect) Kp = K
            inline scalar Kp(const scalar T) const;

            //- Equilibrium constant i.t.o. molar concentration
            //  = PIi(ci/cstd)^nui
            //  For low pressures (where the gas mixture is near perfect)
            //  Kc = Kp(pstd/(RR*T))^nu
            inline scalar Kc(const scalar T) const;

            //- Equilibrium constant [] i.t.o. mole-fractions
            //  For low pressures (where the gas mixture is near perfect)
            //  Kx = Kp(pstd/p)^nui
            inline scalar Kx(const scalar T, const scalar p) const;

            //- Equilibrium constant [] i.t.o. number of moles
            //  For low pressures (where the gas mixture is near perfect)
            //  Kn = Kp(n*pstd/p)^nui where n = number of moles in mixture
            inline scalar Kn
            (
                const scalar T,
                const scalar p,
                const scalar n
            ) const;


        // Energy->temperature  inversion functions

            //- Temperature from Enthalpy given an initial temperature T0
            inline scalar TH(const scalar H, const scalar T0) const;

            //- Temperature from sensible Enthalpy given an initial T0
            inline scalar THs(const scalar Hs, const scalar T0) const;

            //- Temperature from internal energy given an initial temperature T0
            inline scalar TE(const scalar E, const scalar T0) const;


        // I-O

            //- Write to Ostream
            void write(Ostream& os) const;


    // Member operators

        inline void operator+=(const specieThermo&);
        inline void operator-=(const specieThermo&);

        inline void operator*=(const scalar);


    // Friend operators

        friend specieThermo operator+ <Thermo>
        (
            const specieThermo&,
            const specieThermo&
        );

        friend specieThermo operator- <Thermo>
        (
            const specieThermo&,
            const specieThermo&
        );

        friend specieThermo operator* <Thermo>
        (
            const scalar s,
            const specieThermo&
        );

        friend specieThermo operator== <Thermo>
        (
            const specieThermo&,
            const specieThermo&
        );


    // Ostream Operator

        friend Ostream& operator<< <Thermo>
        (
            Ostream&,
            const specieThermo&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Thermo>
inline CML::specieThermo<Thermo>::specieThermo
(
    const Thermo& sp
)
:
    Thermo(sp)
{}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::T
(
    scalar f,
    scalar T0,
    scalar (specieThermo<Thermo>::*F)(const scalar) const,
    scalar (specieThermo<Thermo>::*dFdT)(const scalar) const,
    scalar (specieThermo<Thermo>::*limit)(const scalar) const
) const
{
    scalar Test = T0;
    scalar Tnew = T0;
    scalar Ttol = T0*tol_;
    int    iter = 0;

    do
    {
        Test = Tnew;
        Tnew =
            (this->*limit)(Test - ((this->*F)(Test) - f)/(this->*dFdT)(Test));

        if (iter++ > maxIter_)
        {
            FatalErrorIn
            (
                "specieThermo<Thermo>::T(scalar f, scalar T0, "
                "scalar (specieThermo<Thermo>::*F)(const scalar) const, "
                "scalar (specieThermo<Thermo>::*dFdT)(const scalar) const"
                ") const"
            )   << "Maximum number of iterations exceeded"
                << abort(FatalError);
        }

    } while (mag(Tnew - Test) > Ttol);

    return Tnew;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
inline CML::specieThermo<Thermo>::specieThermo
(
    const word& name,
    const specieThermo& st
)
:
    Thermo(name, st)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::cv(const scalar T) const
{
    return this->cp(T) - this->RR;
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::gamma(const scalar T) const
{
    scalar CP = this->cp(T);
    return CP/(CP - this->RR);
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::e(const scalar T) const
{
    return this->h(T) - this->RR*(T - this->Tstd);
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::es(const scalar T) const
{
    return this->hs(T) - this->RR*(T - this->Tstd);
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::g(const scalar T) const
{
    return this->h(T) - T*this->s(T);
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::a(const scalar T) const
{
    return this->e(T) - T*this->s(T);
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::Cp(const scalar T) const
{
    return this->cp(T)/this->W();
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::Cv(const scalar T) const
{
    return this->cv(T)/this->W();
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::H(const scalar T) const
{
    return this->h(T)/this->W();
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::Hs(const scalar T) const
{
    return this->hs(T)/this->W();
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::Hc() const
{
    return this->hc()/this->W();
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::S(const scalar T) const
{
    return this->s(T)/this->W();
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::E(const scalar T) const
{
    return this->e(T)/this->W();
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::G(const scalar T) const
{
    return this->g(T)/this->W();
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::A(const scalar T) const
{
    return this->a(T)/this->W();
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::K(const scalar T) const
{
    scalar arg = -this->nMoles()*this->g(T)/(this->RR*T);

    if (arg < 600.0)
    {
        return exp(arg);
    }
    else
    {
        return VGREAT;
    }
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::Kp(const scalar T) const
{
    return K(T);
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::Kc(const scalar T) const
{
    if (equal(this->nMoles(), SMALL))
    {
        return Kp(T);
    }
    else
    {
        return Kp(T)*pow(this->Pstd/(this->RR*T), this->nMoles());
    }
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::Kx
(
    const scalar T,
    const scalar p
) const
{
    if (equal(this->nMoles(), SMALL))
    {
        return Kp(T);
    }
    else
    {
        return Kp(T)*pow(this->Pstd/p, this->nMoles());
    }
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::Kn
(
    const scalar T,
    const scalar p,
    const scalar n
) const
{
    if (equal(this->nMoles(), SMALL))
    {
        return Kp(T);
    }
    else
    {
        return Kp(T)*pow(n*this->Pstd/p, this->nMoles());
    }
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::TH
(
    const scalar h,
    const scalar T0
) const
{
    return T
    (
        h,
        T0,
        &specieThermo<Thermo>::H,
        &specieThermo<Thermo>::Cp,
        &specieThermo<Thermo>::limit
    );
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::THs
(
    const scalar hs,
    const scalar T0
) const
{
    return T
    (
        hs,
        T0,
        &specieThermo<Thermo>::Hs,
        &specieThermo<Thermo>::Cp,
        &specieThermo<Thermo>::limit
    );
}


template<class Thermo>
inline CML::scalar CML::specieThermo<Thermo>::TE
(
    const scalar e,
    const scalar T0
) const
{
    return T
    (
        e,
        T0,
        &specieThermo<Thermo>::E,
        &specieThermo<Thermo>::Cv,
        &specieThermo<Thermo>::limit
    );
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Thermo>
inline void CML::specieThermo<Thermo>::operator+=
(
    const specieThermo<Thermo>& st
)
{
    Thermo::operator+=(st);
}


template<class Thermo>
inline void CML::specieThermo<Thermo>::operator-=
(
    const specieThermo<Thermo>& st
)
{
    Thermo::operator-=(st);
}


template<class Thermo>
inline void CML::specieThermo<Thermo>::operator*=(const scalar s)
{
    Thermo::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Thermo>
inline CML::specieThermo<Thermo> CML::operator+
(
    const specieThermo<Thermo>& st1,
    const specieThermo<Thermo>& st2
)
{
    return specieThermo<Thermo>
    (
        static_cast<const Thermo&>(st1) + static_cast<const Thermo&>(st2)
    );
}


template<class Thermo>
inline CML::specieThermo<Thermo> CML::operator-
(
    const specieThermo<Thermo>& st1,
    const specieThermo<Thermo>& st2
)
{
    return specieThermo<Thermo>
    (
        static_cast<const Thermo&>(st1) - static_cast<const Thermo&>(st2)
    );
}


template<class Thermo>
inline CML::specieThermo<Thermo> CML::operator*
(
    const scalar s,
    const specieThermo<Thermo>& st
)
{
    return specieThermo<Thermo>
    (
        s*static_cast<const Thermo&>(st)
    );
}


template<class Thermo>
inline CML::specieThermo<Thermo> CML::operator==
(
    const specieThermo<Thermo>& st1,
    const specieThermo<Thermo>& st2
)
{
    return st2 - st1;
}


#include "IOstreams.hpp"

/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */

template<class Thermo>
const CML::scalar CML::specieThermo<Thermo>::tol_ = 1.0e-4;

template<class Thermo>
const int CML::specieThermo<Thermo>::maxIter_ = 100;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
CML::specieThermo<Thermo>::specieThermo(Istream& is)
:
    Thermo(is)
{
    is.check("specieThermo<Thermo>::specieThermo(Istream&)");
}


template<class Thermo>
CML::specieThermo<Thermo>::specieThermo(const dictionary& dict)
:
    Thermo(dict)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
void CML::specieThermo<Thermo>::write(Ostream& os) const
{
    Thermo::write(os);
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Thermo>
CML::Ostream& CML::operator<<(Ostream& os, const specieThermo<Thermo>& st)
{
    os  << static_cast<const Thermo&>(st);

    os.check("Ostream& operator<<(Ostream&, const specieThermo&)");
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
