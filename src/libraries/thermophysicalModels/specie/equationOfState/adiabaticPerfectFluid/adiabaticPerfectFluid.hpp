/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
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
    CML::adiabaticPerfectFluid

Description
    Adiabatic perfect fluid equation of state.


\*---------------------------------------------------------------------------*/

#ifndef adiabaticPerfectFluid_HPP
#define adiabaticPerfectFluid_HPP

#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Specie> class adiabaticPerfectFluid;

template<class Specie>
inline adiabaticPerfectFluid<Specie> operator+
(
    const adiabaticPerfectFluid<Specie>&,
    const adiabaticPerfectFluid<Specie>&
);

template<class Specie>
inline adiabaticPerfectFluid<Specie> operator*
(
    const scalar,
    const adiabaticPerfectFluid<Specie>&
);

template<class Specie>
inline adiabaticPerfectFluid<Specie> operator==
(
    const adiabaticPerfectFluid<Specie>&,
    const adiabaticPerfectFluid<Specie>&
);

template<class Specie>
Ostream& operator<<
(
    Ostream&,
    const adiabaticPerfectFluid<Specie>&
);


/*---------------------------------------------------------------------------*\
                           Class adiabaticPerfectFluid Declaration
\*---------------------------------------------------------------------------*/

template<class Specie>
class adiabaticPerfectFluid
:
    public Specie
{

    //- Reference pressure
    scalar p0_;

    //- Reference density
    scalar rho0_;

    //- The isentropic exponent
    scalar gamma_;

    //- Pressure offset for a stiffened gas
    scalar B_;


public:


    //- Construct from components
    inline adiabaticPerfectFluid
    (
        const Specie& sp,
        const scalar p0,
        const scalar rho0,
        const scalar gamma,
        const scalar B
    )
    :
        Specie(sp),
        p0_(p0),
        rho0_(rho0),
        gamma_(gamma),
        B_(B)
    {}

    //- Construct from dictionary
    adiabaticPerfectFluid(const dictionary& dict);

    //- Construct as named copy
    inline adiabaticPerfectFluid
    (
        const word& name,
        const adiabaticPerfectFluid& pf
    )
    :
        Specie(name, pf),
        p0_(pf.p0_),
        rho0_(pf.rho0_),
        gamma_(pf.gamma_),
        B_(pf.B_)
    {}

    //- Construct and return a clone
    inline autoPtr<adiabaticPerfectFluid> clone() const
    {
        return autoPtr<adiabaticPerfectFluid>
        (
            new adiabaticPerfectFluid(*this)
        );
    }

    // Selector from dictionary
    inline static autoPtr<adiabaticPerfectFluid> New
    (
            const dictionary& dict
    )
    {
        return autoPtr<adiabaticPerfectFluid>
        (
            new adiabaticPerfectFluid(dict)
        );
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "adiabaticPerfectFluid<" + word(Specie::typeName_()) + '>';
    }


    // Fundamental properties

    //- Is the equation of state is incompressible i.e. rho != f(p)
    static const bool incompressible = false;

    //- Is the equation of state is isochoric i.e. rho = const
    static const bool isochoric = false;

    //- Return density [kg/m^3]
    inline scalar rho(scalar p, scalar T) const
    {
        return rho0_*pow((p + B_)/(p0_ + B_), 1.0/gamma_);
    }

    //- Return enthalpy departure [J/kg]
    inline scalar H(const scalar p, const scalar T) const
    {
        // ***HGW This needs to be added, it is not 0
        return 0;
    }

    //- Return Cp departure [J/(kg K]
    inline scalar Cp(scalar p, scalar T) const
    {
        return 0;
    }

    //- Return internal energy departure [J/kg]
    inline scalar E(const scalar p, const scalar T) const
    {
        // ***HGW This needs to be added, it is H - p/rho
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
        scalar n = 1 - 1.0/gamma_;
        return
           -pow(p0_ + B_, 1.0/gamma_)*(pow((p + B_), n) - pow((Pstd + B_), n))
           /(rho0_*T*n);
    }


    //- Return compressibility rho/p [s^2/m^2]
    inline scalar psi(scalar p, scalar T) const
    {
        return
            (rho0_/(gamma_*(p0_ + B_)))
           *pow((p + B_)/(p0_ + B_), 1.0/gamma_ - 1.0);
    }

    //- Return compression factor []
    inline scalar Z(scalar p, scalar T) const
    {
        return 1;
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

    inline void operator+=(const adiabaticPerfectFluid& pf);

    inline void operator*=(const scalar s);


    // Friend operators

    friend adiabaticPerfectFluid operator+ <Specie>
    (
        const adiabaticPerfectFluid<Specie>& pf1,
        const adiabaticPerfectFluid<Specie>& pf2
    );

    friend adiabaticPerfectFluid operator* <Specie>
    (
        const scalar s,
        const adiabaticPerfectFluid<Specie>& pf
    );

    friend adiabaticPerfectFluid operator== <Specie>
    (
        const adiabaticPerfectFluid<Specie>& pf1,
        const adiabaticPerfectFluid<Specie>& pf2
    );

    // Ostream Operator

    friend Ostream& operator<< <Specie>
    (
        Ostream&,
        const adiabaticPerfectFluid&
    );
};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
CML::adiabaticPerfectFluid<Specie>::adiabaticPerfectFluid
(
    const dictionary& dict
)
:
    Specie(dict),
    p0_(readScalar(dict.subDict("equationOfState").lookup("p0"))),
    rho0_(readScalar(dict.subDict("equationOfState").lookup("rho0"))),
    gamma_(readScalar(dict.subDict("equationOfState").lookup("gamma"))),
    B_(readScalar(dict.subDict("equationOfState").lookup("B")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
void CML::adiabaticPerfectFluid<Specie>::write(Ostream& os) const
{
    Specie::write(os);

    dictionary dict("equationOfState");
    dict.add("p0", p0_);
    dict.add("rho0", rho0_);
    dict.add("gamma", gamma_);
    dict.add("B", B_);

    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Specie>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const adiabaticPerfectFluid<Specie>& pf
)
{
    pf.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Specie>
inline void CML::adiabaticPerfectFluid<Specie>::operator+=
(
    const adiabaticPerfectFluid<Specie>& pf
)
{
    scalar Y1 = this->Y();
    Specie::operator+=(pf);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        const scalar Y2 = pf.Y()/this->Y();

        p0_ = Y1*p0_ + Y2*pf.p0_;
        rho0_ = Y1*rho0_ + Y2*pf.rho0_;
        gamma_ = Y1*gamma_ + Y2*pf.gamma_;
        B_ = Y1*B_ + Y2*pf.B_;
    }
}


template<class Specie>
inline void CML::adiabaticPerfectFluid<Specie>::operator*=(const scalar s)
{
    Specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Specie>
inline CML::adiabaticPerfectFluid<Specie> CML::operator+
(
    const adiabaticPerfectFluid<Specie>& pf1,
    const adiabaticPerfectFluid<Specie>& pf2
)
{
    Specie sp
    (
        static_cast<const Specie&>(pf1)
      + static_cast<const Specie&>(pf2)
    );

    if (mag(sp.Y()) < SMALL)
    {
        return adiabaticPerfectFluid<Specie>
        (
            sp,
            pf1.p0_,
            pf1.rho0_,
            pf1.gamma_,
            pf1.B_
        );
    }
    else
    {
        const scalar Y1 = pf1.Y()/sp.Y();
        const scalar Y2 = pf2.Y()/sp.Y();

        return adiabaticPerfectFluid<Specie>
        (
            sp,
            Y1*pf1.p0_ + Y2*pf2.p0_,
            Y1*pf1.rho0_ + Y2*pf2.rho0_,
            Y1*pf1.gamma_ + Y2*pf2.gamma_,
            Y1*pf1.B_ + Y2*pf2.B_
        );
    }
}


template<class Specie>
inline CML::adiabaticPerfectFluid<Specie> CML::operator*
(
    const scalar s,
    const adiabaticPerfectFluid<Specie>& pf
)
{
    return adiabaticPerfectFluid<Specie>
    (
        s*static_cast<const Specie&>(pf),
        pf.p0_,
        pf.rho0_,
        pf.gamma_,
        pf.B_
    );
}


template<class Specie>
inline CML::adiabaticPerfectFluid<Specie> CML::operator==
(
    const adiabaticPerfectFluid<Specie>& pf1,
    const adiabaticPerfectFluid<Specie>& pf2
)
{
    Specie sp
    (
        static_cast<const Specie&>(pf1)
     == static_cast<const Specie&>(pf2)
    );

    const scalar Y1 = pf1.Y()/sp.Y();
    const scalar Y2 = pf2.Y()/sp.Y();

    return adiabaticPerfectFluid<Specie>
    (
        sp,
        Y2*pf2.p0_    - Y1*pf1.p0_,
        Y2*pf2.rho0_  - Y1*pf1.rho0_,
        Y2*pf2.gamma_ - Y1*pf1.gamma_,
        Y2*pf2.B_     - Y1*pf1.B_
    );
}


#endif
