/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2019 OpenFOAM Foundation
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
    CML::perfectFluid

Description
    Perfect gas equation of state.


\*---------------------------------------------------------------------------*/

#ifndef perfectFluid_HPP
#define perfectFluid_HPP

#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Specie> class perfectFluid;

template<class Specie>
inline perfectFluid<Specie> operator+
(
    const perfectFluid<Specie>&,
    const perfectFluid<Specie>&
);

template<class Specie>
inline perfectFluid<Specie> operator*
(
    const scalar,
    const perfectFluid<Specie>&
);

template<class Specie>
inline perfectFluid<Specie> operator==
(
    const perfectFluid<Specie>&,
    const perfectFluid<Specie>&
);

template<class Specie>
Ostream& operator<<
(
    Ostream&,
    const perfectFluid<Specie>&
);


/*---------------------------------------------------------------------------*\
                           Class perfectFluid Declaration
\*---------------------------------------------------------------------------*/

template<class Specie>
class perfectFluid
:
    public Specie
{

    //- Fluid constant
    scalar R_;

    //- The reference density
    scalar rho0_;


public:


    //- Construct from components
    inline perfectFluid
    (
        const Specie& sp,
        const scalar R,
        const scalar rho0
    )
    :
        Specie(sp),
        R_(R),
        rho0_(rho0)
    {}

    //- Construct from dictionary
    perfectFluid(const dictionary& dict);

    //- Construct as named copy
    inline perfectFluid(const word& name, const perfectFluid& pf)
    :
        Specie(name, pf),
        R_(pf.R_),
        rho0_(pf.rho0_)
    {}

    //- Construct and return a clone
    inline autoPtr<perfectFluid> clone() const
    {
        return autoPtr<perfectFluid>(new perfectFluid(*this));
    }

    // Selector from dictionary
    inline static autoPtr<perfectFluid> New(const dictionary& dict)
    {
        return autoPtr<perfectFluid>(new perfectFluid(dict));
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "perfectFluid<" + word(Specie::typeName_()) + '>';
    }


    // Fundamental properties

    //- Is the equation of state is incompressible i.e. rho != f(p)
    static const bool incompressible = false;

    //- Is the equation of state is isochoric i.e. rho = const
    static const bool isochoric = false;

    //- Return fluid constant [J/(kg K)]
    inline scalar R() const
    {
        return R_;
    }

    //- Return density [kg/m^3]
    inline scalar rho(scalar p, scalar T) const
    {
        return rho0_ + p/(this->R()*T);
    }

    //- Return enthalpy departure [J/kg]
    inline scalar H(const scalar p, const scalar T) const
    {
        return p/rho(p, T) - Pstd/rho(Pstd, T);
    }

    //- Return Cp departure [J/(kg K]
    inline scalar Cp(scalar p, scalar T) const
    {
        return
            sqr(p/(rho(p, T)*T))/this->R()
          - sqr(Pstd/(rho(Pstd, T)*T))/this->R();
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
        return -this->R()*log(p/Pstd);
    }

    //- Return compressibility rho/p [s^2/m^2]
    inline scalar psi(scalar p, scalar T) const
    {
        return 1.0/(this->R()*T);
    }

    //- Return compression factor []
    inline scalar Z(scalar p, scalar T) const
    {
        return 1;
    }

    //- Return (Cp - Cv) [J/(kg K]
    inline scalar CpMCv(scalar p, scalar T) const
    {
        const scalar R = this->R();
        const scalar rho = this->rho(p, T);

        return R*sqr(p/(rho*R*T));
    }


    // IO

    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators

    inline void operator+=(const perfectFluid& pf);

    inline void operator*=(const scalar s);


    // Friend operators

    friend perfectFluid operator+ <Specie>
    (
        const perfectFluid& pf1,
        const perfectFluid& pf2
    );

    friend perfectFluid operator* <Specie>
    (
        const scalar s,
        const perfectFluid& pf
    );

    friend perfectFluid operator== <Specie>
    (
        const perfectFluid& pf1,
        const perfectFluid& pf2
    );

    // Ostream Operator

    friend Ostream& operator<< <Specie>
    (
        Ostream&,
        const perfectFluid&
    );
};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
CML::perfectFluid<Specie>::perfectFluid(const dictionary& dict)
:
    Specie(dict),
    R_(readScalar(dict.subDict("equationOfState").lookup("R"))),
    rho0_(readScalar(dict.subDict("equationOfState").lookup("rho0")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
void CML::perfectFluid<Specie>::write(Ostream& os) const
{
    Specie::write(os);

    dictionary dict("equationOfState");
    dict.add("R", R_);
    dict.add("rho0", rho0_);

    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Specie>
CML::Ostream& CML::operator<<(Ostream& os, const perfectFluid<Specie>& pf)
{
    pf.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Specie>
inline void CML::perfectFluid<Specie>::operator+=
(
    const perfectFluid<Specie>& pf
)
{
    scalar Y1 = this->Y();
    Specie::operator+=(pf);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        const scalar Y2 = pf.Y()/this->Y();

        R_ = 1.0/(Y1/R_ + Y2/pf.R_);
        rho0_ = Y1*rho0_ + Y2*pf.rho0_;
    }
}


template<class Specie>
inline void CML::perfectFluid<Specie>::operator*=(const scalar s)
{
    Specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Specie>
inline CML::perfectFluid<Specie> CML::operator+
(
    const perfectFluid<Specie>& pf1,
    const perfectFluid<Specie>& pf2
)
{
    Specie sp
    (
        static_cast<const Specie&>(pf1)
      + static_cast<const Specie&>(pf2)
    );

    if (mag(sp.Y()) < SMALL)
    {
        return perfectFluid<Specie>
        (
            sp,
            pf1.R_,
            pf1.rho0_
        );
    }
    else
    {
        const scalar Y1 = pf1.Y()/sp.Y();
        const scalar Y2 = pf2.Y()/sp.Y();

        return perfectFluid<Specie>
        (
            sp,
            1.0/(Y1/pf1.R_ + Y2/pf2.R_),
            Y1*pf1.rho0_ + Y2*pf2.rho0_
        );
    }
}


template<class Specie>
inline CML::perfectFluid<Specie> CML::operator*
(
    const scalar s,
    const perfectFluid<Specie>& pf
)
{
    return perfectFluid<Specie>
    (
        s*static_cast<const Specie&>(pf),
        pf.R_,
        pf.rho0_
    );
}


template<class Specie>
inline CML::perfectFluid<Specie> CML::operator==
(
    const perfectFluid<Specie>& pf1,
    const perfectFluid<Specie>& pf2
)
{
    Specie sp
    (
        static_cast<const Specie&>(pf1)
     == static_cast<const Specie&>(pf2)
    );

    if (mag(sp.Y()) < SMALL)
    {
        return perfectFluid<Specie>
        (
            sp,
            pf1.R_,
            pf1.rho0_
        );
    }
    else
    {
        const scalar Y1 = pf1.Y()/sp.Y();
        const scalar Y2 = pf2.Y()/sp.Y();
        const scalar oneByR = Y2/pf2.R_ - Y1/pf1.R_;

        return perfectFluid<Specie>
        (
            sp,
            mag(oneByR) < SMALL ? GREAT : 1/oneByR,
            Y2*pf2.rho0_ - Y1*pf1.rho0_
        );
    }
}


#endif
