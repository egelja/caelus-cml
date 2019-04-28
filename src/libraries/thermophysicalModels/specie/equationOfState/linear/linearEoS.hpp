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
    CML::linear

Description
    Linear equation of state with constant compressibility

    \verbatim
        rho = rho0 + psi*p
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef linear_HPP
#define linear_HPP

#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Specie> class linear;

template<class Specie>
inline linear<Specie> operator+
(
    const linear<Specie>&,
    const linear<Specie>&
);

template<class Specie>
inline linear<Specie> operator*
(
    const scalar,
    const linear<Specie>&
);

template<class Specie>
inline linear<Specie> operator==
(
    const linear<Specie>&,
    const linear<Specie>&
);

template<class Specie>
Ostream& operator<<
(
    Ostream&,
    const linear<Specie>&
);


/*---------------------------------------------------------------------------*\
                           Class linear Declaration
\*---------------------------------------------------------------------------*/

template<class Specie>
class linear
:
    public Specie
{

    //- Compressibility
    scalar psi_;

    //- The reference density
    scalar rho0_;


public:

    //- Construct from components
    inline linear
    (
        const Specie& sp,
        const scalar psi,
        const scalar rho0
    )
    :
        Specie(sp),
        psi_(psi),
        rho0_(rho0)
    {}

    //- Construct from dictionary
    linear(const dictionary& dict);

    //- Construct as named copy
    inline linear(const word& name, const linear& pf);
    :
        Specie(name, pf),
        psi_(pf.psi_),
        rho0_(pf.rho0_)
    {}

    //- Construct and return a clone
    inline autoPtr<linear> clone() const
    {
        return autoPtr<linear>(new linear(*this));
    }

    // Selector from dictionary
    inline static autoPtr<linear> New(const dictionary& dict)
    {
        return autoPtr<linear>(new linear(dict));
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "linear<" + word(Specie::typeName_()) + '>';
    }


    // Fundamental properties

    //- Is the equation of state is incompressible i.e. rho != f(p)
    static const bool incompressible = false;

    //- Is the equation of state is isochoric i.e. rho = const
    static const bool isochoric = false;

    //- Return density [kg/m^3]
    inline scalar rho(scalar p, scalar T) const
    {
        return rho0_ + psi_*p;
    }

    //- Return enthalpy departure [J/kg]
    inline scalar H(const scalar p, const scalar T) const
    {
        return log(rho(p, T))/psi_;
    }

    //- Return Cp departure [J/(kg K]
    inline scalar Cp(scalar p, scalar T) const
    {
        return 0;
    }

    //- Return internal energy departure [J/kg]
    inline scalar E(const scalar p, const scalar T) const
    {
        const scalar rho = this->rho(p, T);

        return log(rho)/psi_ - p/rho;
    }

    //- Return Cv departure [J/(kg K]
    inline scalar Cv(scalar p, scalar T) const
    {
        return 0;
    }

    //- Return entropy [J/(kg K)]
    inline scalar S(const scalar p, const scalar T) const
    {
        return -log((rho0_ + psi_*p)/(rho0_ + psi_*Pstd))/(T*psi_);
    }

    //- Return compressibility rho/p [s^2/m^2]
    inline scalar psi(scalar p, scalar T) const
    {
        return psi_;
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

    inline void operator+=(const linear& pf);

    inline void operator*=(const scalar s);


    // Friend operators

    friend linear operator+ <Specie>
    (
        const linear& pf1,
        const linear& pf2
    );

    friend linear operator* <Specie>
    (
        const scalar s,
        const linear& pf
    );

    friend linear operator== <Specie>
    (
        const linear& pf1,
        const linear& pf2
    );


    // Ostream Operator

    friend Ostream& operator<< <Specie>
    (
        Ostream&,
        const linear&
    );
};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
CML::linear<Specie>::linear(const dictionary& dict)
:
    Specie(dict),
    psi_(readScalar(dict.subDict("equationOfState").lookup("psi"))),
    rho0_(readScalar(dict.subDict("equationOfState").lookup("rho0")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
void CML::linear<Specie>::write(Ostream& os) const
{
    Specie::write(os);

    dictionary dict("equationOfState");
    dict.add("psi", psi_);
    dict.add("rho0", rho0_);

    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Specie>
CML::Ostream& CML::operator<<(Ostream& os, const linear<Specie>& pf)
{
    pf.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Specie>
inline void CML::linear<Specie>::operator+=
(
    const linear<Specie>& pf
)
{
    scalar Y1 = this->Y();
    Specie::operator+=(pf);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        const scalar Y2 = pf.Y()/this->Y();

        psi_ = Y1*psi_ + Y2*pf.psi_;
        rho0_ = Y1*rho0_ + Y2*pf.rho0_;
    }
}


template<class Specie>
inline void CML::linear<Specie>::operator*=(const scalar s)
{
    Specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Specie>
inline CML::linear<Specie> CML::operator+
(
    const linear<Specie>& pf1,
    const linear<Specie>& pf2
)
{
    Specie sp
    (
        static_cast<const Specie&>(pf1)
      + static_cast<const Specie&>(pf2)
    );

    if (mag(sp.Y()) < SMALL)
    {
        return linear<Specie>
        (
            sp,
            pf1.psi_,
            pf1.rho0_
        );
    }
    else
    {
        const scalar Y1 = pf1.Y()/sp.Y();
        const scalar Y2 = pf2.Y()/sp.Y();

        return linear<Specie>
        (
            sp,
            Y1*pf1.psi_ + Y2*pf2.psi_,
            Y1*pf1.rho0_ + Y2*pf2.rho0_
        );
    }
}


template<class Specie>
inline CML::linear<Specie> CML::operator*
(
    const scalar s,
    const linear<Specie>& pf
)
{
    return linear<Specie>
    (
        s*static_cast<const Specie&>(pf),
        pf.psi_,
        pf.rho0_
    );
}


template<class Specie>
inline CML::linear<Specie> CML::operator==
(
    const linear<Specie>& pf1,
    const linear<Specie>& pf2
)
{
    Specie sp
    (
        static_cast<const Specie&>(pf1)
     == static_cast<const Specie&>(pf2)
    );

    const scalar Y1 = pf1.Y()/sp.Y();
    const scalar Y2 = pf2.Y()/sp.Y();

    return linear<Specie>
    (
        sp,
        Y2*pf2.psi_  - Y1*pf1.psi_,
        Y2*pf2.rho0_ - Y1*pf1.rho0_
    );
}


#endif
