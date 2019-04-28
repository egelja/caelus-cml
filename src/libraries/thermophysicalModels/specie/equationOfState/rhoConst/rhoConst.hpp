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
    CML::rhoConst

Description
    RhoConst (rho = const) of state.

SourceFiles
    rhoConstI.H
    rhoConst.C

\*---------------------------------------------------------------------------*/

#ifndef rhoConst_HPP
#define rhoConst_HPP

#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Specie> class rhoConst;

template<class Specie>
inline rhoConst<Specie> operator+
(
    const rhoConst<Specie>&,
    const rhoConst<Specie>&
);

template<class Specie>
inline rhoConst<Specie> operator*
(
    const scalar,
    const rhoConst<Specie>&
);

template<class Specie>
inline rhoConst<Specie> operator==
(
    const rhoConst<Specie>&,
    const rhoConst<Specie>&
);

template<class Specie>
Ostream& operator<<
(
    Ostream&,
    const rhoConst<Specie>&
);


/*---------------------------------------------------------------------------*\
                        Class rhoConst Declaration
\*---------------------------------------------------------------------------*/

template<class Specie>
class rhoConst
:
    public Specie
{

    //- Density
    scalar rho_;


public:


    //- Construct from components
    inline rhoConst(const Specie& sp, const scalar rho)
    :
        Specie(sp),
        rho_(rho)
    {}

    //- Construct from dictionary
    rhoConst(const dictionary& dict);

    //- Construct as named copy
    inline rhoConst(const word& name, const rhoConst& ico)
    :
        Specie(name, ico),
        rho_(ico.rho_)
    {}

    //- Construct and return a clone
    inline autoPtr<rhoConst> clone() const
    {
        return autoPtr<rhoConst>(new rhoConst(*this));
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "rhoConst<" + word(Specie::typeName_()) + '>';
    }


    // Fundamental properties

    //- Is the equation of state is incompressible i.e. rho != f(p)
    static const bool incompressible = true;

    //- Is the equation of state is isochoric i.e. rho = const
    static const bool isochoric = true;

    //- Return density [kg/m^3]
    inline scalar rho(scalar p, scalar T) const
    {
        return rho_;
    }

    //- Return enthalpy departure [J/kg]
    inline scalar H(const scalar p, const scalar T) const
    {
        return p/rho_;
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

    inline void operator+=(const rhoConst& ico);

    inline void operator*=(const scalar s);


    // Friend operators

    friend rhoConst operator+ <Specie>
    (
        const rhoConst& ico1,
        const rhoConst& ico2
    );

    friend rhoConst operator* <Specie>
    (
        const scalar s,
        const rhoConst& ico
    );

    friend rhoConst operator== <Specie>
    (
        const rhoConst& ico1,
        const rhoConst& ico2
    );

    // Ostream Operator

    friend Ostream& operator<< <Specie>
    (
        Ostream&,
        const rhoConst&
    );
};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
CML::rhoConst<Specie>::rhoConst(const dictionary& dict)
:
    Specie(dict),
    rho_(readScalar(dict.subDict("equationOfState").lookup("rho")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
void CML::rhoConst<Specie>::write(Ostream& os) const
{
    Specie::write(os);

    dictionary dict("equationOfState");
    dict.add("rho", rho_);

    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Specie>
CML::Ostream& CML::operator<<(Ostream& os, const rhoConst<Specie>& ico)
{
    ico.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Specie>
inline void CML::rhoConst<Specie>::operator+=(const rhoConst<Specie>& ico)
{
    scalar Y1 = this->Y();
    Specie::operator+=(ico);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        const scalar Y2 = ico.Y()/this->Y();

        rho_ = Y1*rho_ + Y2*ico.rho_;
    }
}


template<class Specie>
inline void CML::rhoConst<Specie>::operator*=(const scalar s)
{
    Specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Specie>
inline CML::rhoConst<Specie> CML::operator+
(
    const rhoConst<Specie>& ico1,
    const rhoConst<Specie>& ico2
)
{
    Specie sp
    (
        static_cast<const Specie&>(ico1)
      + static_cast<const Specie&>(ico2)
    );

    if (mag(sp.Y()) < SMALL)
    {
        return rhoConst<Specie>
        (
            sp,
            ico1.rho_
        );
    }
    else
    {
        const scalar Y1 = ico1.Y()/sp.Y();
        const scalar Y2 = ico2.Y()/sp.Y();

        return rhoConst<Specie>
        (
            sp,
            Y1*ico1.rho_ + Y2*ico2.rho_
        );
    }
}


template<class Specie>
inline CML::rhoConst<Specie> CML::operator*
(
    const scalar s,
    const rhoConst<Specie>& ico
)
{
    return rhoConst<Specie>(s*static_cast<const Specie&>(ico), ico.rho_);
}


template<class Specie>
inline CML::rhoConst<Specie> CML::operator==
(
    const rhoConst<Specie>& ico1,
    const rhoConst<Specie>& ico2
)
{
    Specie sp
    (
        static_cast<const Specie&>(ico1)
     == static_cast<const Specie&>(ico2)
    );

    const scalar Y1 = ico1.Y()/sp.Y();
    const scalar Y2 = ico2.Y()/sp.Y();

    return rhoConst<Specie>
    (
        sp,
        Y2*ico2.rho_ - Y1*ico1.rho_
    );
}


#endif
