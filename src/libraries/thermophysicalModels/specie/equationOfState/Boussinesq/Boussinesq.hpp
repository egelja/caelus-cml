/*---------------------------------------------------------------------------*\
 Copyright (C) 2015-2018 OpenFOAM Foundation
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
    CML::Boussinesq

Description
    Incompressible gas equation of state using the Boussinesq approximation for
    the density as a function of temperature only:

    \verbatim
        rho = rho0*(1 - beta*(T - T0))
    \endverbatim

SourceFiles
    BoussinesqI.H

\*---------------------------------------------------------------------------*/

#ifndef Boussinesq_HPP
#define Boussinesq_HPP

#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Specie> class Boussinesq;

template<class Specie>
inline Boussinesq<Specie> operator+
(
    const Boussinesq<Specie>&,
    const Boussinesq<Specie>&
);

template<class Specie>
inline Boussinesq<Specie> operator*
(
    const scalar,
    const Boussinesq<Specie>&
);

template<class Specie>
inline Boussinesq<Specie> operator==
(
    const Boussinesq<Specie>&,
    const Boussinesq<Specie>&
);

template<class Specie>
Ostream& operator<<
(
    Ostream&,
    const Boussinesq<Specie>&
);


/*---------------------------------------------------------------------------*\
                    Class Boussinesq Declaration
\*---------------------------------------------------------------------------*/

template<class Specie>
class Boussinesq
:
    public Specie
{

    //- Reference density
    scalar rho0_;

    //- Reference temperature
    scalar T0_;

    //- Thermal expansion coefficient
    scalar beta_;


public:


    //- Construct from components
    inline Boussinesq
    (
        const Specie& sp,
        const scalar rho0,
        const scalar T0,
        const scalar beta
    )
    :
        Specie(sp),
        rho0_(rho0),
        T0_(T0),
        beta_(beta)
    {}

    //- Construct from dictionary
    Boussinesq(const dictionary& dict);

    //- Construct as named copy
    inline Boussinesq
    (
        const word& name,
        const Boussinesq& b
    )
    :
        Specie(name, b),
        rho0_(b.rho0_),
        T0_(b.T0_),
        beta_(b.beta_)
    {}

    //- Construct and return a clone
    inline autoPtr<Boussinesq> clone() const
    {
        return autoPtr<Boussinesq>
        (
            new Boussinesq(*this)
        );
    }

    // Selector from dictionary
    inline static autoPtr<Boussinesq> New
    (
        const dictionary& dict
    )
    {
        return autoPtr<Boussinesq>
        (
            new Boussinesq(dict)
        );
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return
            "Boussinesq<"
          + word(Specie::typeName_()) + '>';
    }


    // Fundamental properties

    //- Is the equation of state is incompressible i.e. rho != f(p)
    static const bool incompressible = true;

    //- Is the equation of state is isochoric i.e. rho = const
    static const bool isochoric = false;

    //- Return density [kg/m^3]
    inline scalar rho(scalar p, scalar T) const
    {
        return rho0_*(1.0 - beta_*(T - T0_));
    }

    //- Return enthalpy departure [J/kg]
    inline scalar H(const scalar p, const scalar T) const
    {
        return p/this->rho(p, T);
    }

    //- Return Cp departure [J/(kg K]
    inline scalar Cp(scalar p, scalar T) const
    {
        return 0;
    }

    //- Return internal energy departure [J/kg]
    inline scalar E(const scalar p, const scalar T) const
    {
        return 0;
    }

    //- Return Cv departure [J/(kg K]
    inline scalar Cv(scalar p, scalar T) const
    {
        return 0;
    }

    //- Return entropy [J/(kg K)]
    inline scalar S(const scalar p, const scalar T) const
    {
        return 0;
    }

    //- Return compressibility rho/p [s^2/m^2]
    inline scalar psi(scalar p, scalar T) const
    {
        return 0;
    }

    //- Return compression factor []
    inline scalar Z(scalar p, scalar T) const
    {
        return 0;
    }

    //- Return (Cp - Cv) [J/(kg K]
    inline scalar CpMCv(scalar p, scalar T) const
    {
        return 0;
    }


    // IO

    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators

    inline void operator=(const Boussinesq& b);

    inline void operator+=(const Boussinesq& b);

    inline void operator*=(const scalar s);


    // Friend operators

    friend Boussinesq operator+ <Specie>
    (
        const Boussinesq<Specie>& b1,
        const Boussinesq<Specie>& b2
    );

    friend Boussinesq operator* <Specie>
    (
        const scalar s,
        const Boussinesq<Specie>& b
    );

    friend Boussinesq operator== <Specie>
    (
        const Boussinesq<Specie>& b1,
        const Boussinesq<Specie>& b2
    );


    // Ostream Operator

    friend Ostream& operator<< <Specie>
    (
        Ostream&,
        const Boussinesq&
    );
};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
CML::Boussinesq<Specie>::Boussinesq
(
    const dictionary& dict
)
:
    Specie(dict),
    rho0_(readScalar(dict.subDict("equationOfState").lookup("rho0"))),
    T0_(readScalar(dict.subDict("equationOfState").lookup("T0"))),
    beta_(readScalar(dict.subDict("equationOfState").lookup("beta")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
void CML::Boussinesq<Specie>::write(Ostream& os) const
{
    Specie::write(os);
    dictionary dict("equationOfState");
    dict.add("rho0", rho0_);
    dict.add("T0", T0_);
    dict.add("beta", beta_);

    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Specie>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const Boussinesq<Specie>& b
)
{
    b.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Specie>
inline void CML::Boussinesq<Specie>::operator=
(
    const Boussinesq<Specie>& b
)
{
    Specie::operator=(b);

    rho0_ = b.rho0_;
    T0_ = b.T0_;
    beta_ = b.beta_;
}


template<class Specie>
inline void CML::Boussinesq<Specie>::operator+=
(
    const Boussinesq<Specie>& b
)
{
    scalar Y1 = this->Y();
    Specie::operator+=(b);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        const scalar Y2 = b.Y()/this->Y();

        rho0_ = Y1*rho0_ + Y2*b.rho0_;
        T0_ = Y1*T0_ + Y2*b.T0_;
        beta_ = Y1*beta_ + Y2*b.beta_;
    }
}


template<class Specie>
inline void CML::Boussinesq<Specie>::operator*=(const scalar s)
{
    Specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Specie>
inline CML::Boussinesq<Specie> CML::operator+
(
    const Boussinesq<Specie>& b1,
    const Boussinesq<Specie>& b2
)
{
    Specie sp(static_cast<const Specie&>(b1) + static_cast<const Specie&>(b2));

    if (mag(sp.Y()) < SMALL)
    {
        return Boussinesq<Specie>
        (
            sp,
            b1.rho0_,
            b1.T0_,
            b1.beta_
        );
    }
    else
    {
        const scalar Y1 = b1.Y()/sp.Y();
        const scalar Y2 = b2.Y()/sp.Y();

        return Boussinesq<Specie>
        (
            sp,
            Y1*b1.rho0_ + Y2*b2.rho0_,
            Y1*b1.T0_ + Y2*b2.T0_,
            Y1*b1.beta_ + Y2*b2.beta_
        );
    }
}


template<class Specie>
inline CML::Boussinesq<Specie> CML::operator*
(
    const scalar s,
    const Boussinesq<Specie>& b
)
{
    return Boussinesq<Specie>
    (
        s*static_cast<const Specie&>(b),
        b.rho0_,
        b.T0_,
        b.beta_
    );
}


template<class Specie>
inline CML::Boussinesq<Specie> CML::operator==
(
    const Boussinesq<Specie>& b1,
    const Boussinesq<Specie>& b2
)
{
    Specie sp(static_cast<const Specie&>(b1) == static_cast<const Specie&>(b2));

    const scalar Y1 = b1.Y()/sp.Y();
    const scalar Y2 = b2.Y()/sp.Y();

    return Boussinesq<Specie>
    (
        sp,
        Y2*b2.rho0_ - Y1*b1.rho0_,
        Y2*b2.T0_   - Y1*b1.T0_,
        Y2*b2.beta_ - Y1*b1.beta_
    );
}


#endif
