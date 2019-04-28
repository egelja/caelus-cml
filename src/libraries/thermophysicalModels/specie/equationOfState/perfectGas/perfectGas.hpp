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
    CML::perfectGas

Description
    Perfect gas equation of state.


\*---------------------------------------------------------------------------*/

#ifndef perfectGas_HPP
#define perfectGas_HPP

#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Specie> class perfectGas;

template<class Specie>
inline perfectGas<Specie> operator+
(
    const perfectGas<Specie>&,
    const perfectGas<Specie>&
);

template<class Specie>
inline perfectGas<Specie> operator*
(
    const scalar,
    const perfectGas<Specie>&
);

template<class Specie>
inline perfectGas<Specie> operator==
(
    const perfectGas<Specie>&,
    const perfectGas<Specie>&
);

template<class Specie>
Ostream& operator<<
(
    Ostream&,
    const perfectGas<Specie>&
);


/*---------------------------------------------------------------------------*\
                           Class perfectGas Declaration
\*---------------------------------------------------------------------------*/

template<class Specie>
class perfectGas
:
    public Specie
{

public:


    //- Construct from components
    inline perfectGas(const Specie& sp)
    :
        Specie(sp)
    {}

    //- Construct from dictionary
    perfectGas(const dictionary& dict);

    //- Construct as named copy
    inline perfectGas(const word& name, const perfectGas& pg)
    :
        Specie(name, pg)
    {}

    //- Construct and return a clone
    inline autoPtr<perfectGas> clone() const
    {
        return autoPtr<perfectGas>(new perfectGas(*this));
    }

    // Selector from dictionary
    inline static autoPtr<perfectGas> New(const dictionary& dict)
    {
        return autoPtr<perfectGas>(new perfectGas(dict));
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "perfectGas<" + word(Specie::typeName_()) + '>';
    }


    // Fundamental properties

    //- Is the equation of state is incompressible i.e. rho != f(p)
    static const bool incompressible = false;

    //- Is the equation of state is isochoric i.e. rho = const
    static const bool isochoric = false;

    //- Return density [kg/m^3]
    inline scalar rho(scalar p, scalar T) const
    {
        return p/(this->R()*T);
    }

    //- Return enthalpy departure [J/kg]
    inline scalar H(const scalar p, const scalar T) const
    {
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
        return 1.0;
    }

    //- Return (Cp - Cv) [J/(kg K]
    inline scalar CpMCv(scalar p, scalar T) const
    {
        return this->R();
    }


    // IO

    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators
    inline void operator+=(const perfectGas& pg);

    inline void operator*=(const scalar s);


    // Friend operators
    friend perfectGas operator+ <Specie>
    (
        const perfectGas& pg1,
        const perfectGas& pg2
    );

    friend perfectGas operator* <Specie>
    (
        const scalar s,
        const perfectGas& pg
    );

    friend perfectGas operator== <Specie>
    (
        const perfectGas& pg1,
        const perfectGas& pg2
    );

    // Ostream Operator

    friend Ostream& operator<< <Specie>
    (
        Ostream&,
        const perfectGas&
    );
};


} // End namespace CML


#include "IOstreams.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
CML::perfectGas<Specie>::perfectGas(const dictionary& dict)
:
    Specie(dict)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
void CML::perfectGas<Specie>::write(Ostream& os) const
{
    Specie::write(os);
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Specie>
CML::Ostream& CML::operator<<(Ostream& os, const perfectGas<Specie>& pg)
{
    pg.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Specie>
inline void CML::perfectGas<Specie>::operator+=(const perfectGas<Specie>& pg)
{
    Specie::operator+=(pg);
}


template<class Specie>
inline void CML::perfectGas<Specie>::operator*=(const scalar s)
{
    Specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Specie>
inline CML::perfectGas<Specie> CML::operator+
(
    const perfectGas<Specie>& pg1,
    const perfectGas<Specie>& pg2
)
{
    return perfectGas<Specie>
    (
        static_cast<const Specie&>(pg1) + static_cast<const Specie&>(pg2)
    );
}


template<class Specie>
inline CML::perfectGas<Specie> CML::operator*
(
    const scalar s,
    const perfectGas<Specie>& pg
)
{
    return perfectGas<Specie>(s*static_cast<const Specie&>(pg));
}


template<class Specie>
inline CML::perfectGas<Specie> CML::operator==
(
    const perfectGas<Specie>& pg1,
    const perfectGas<Specie>& pg2
)
{
    return perfectGas<Specie>
    (
        static_cast<const Specie&>(pg1) == static_cast<const Specie&>(pg2)
    );
}


#endif
