/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
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
    CML::incompressiblePerfectGas

Description
    Incompressible gas equation of state using a constant reference pressure in
    the perfect gas equation of state rather than the local pressure so that the
    density only varies with temperature and composition.


\*---------------------------------------------------------------------------*/

#ifndef incompressiblePerfectGas_HPP
#define incompressiblePerfectGas_HPP

#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Specie> class incompressiblePerfectGas;

template<class Specie>
inline incompressiblePerfectGas<Specie> operator+
(
    const incompressiblePerfectGas<Specie>&,
    const incompressiblePerfectGas<Specie>&
);

template<class Specie>
inline incompressiblePerfectGas<Specie> operator*
(
    const scalar,
    const incompressiblePerfectGas<Specie>&
);

template<class Specie>
inline incompressiblePerfectGas<Specie> operator==
(
    const incompressiblePerfectGas<Specie>&,
    const incompressiblePerfectGas<Specie>&
);

template<class Specie>
Ostream& operator<<
(
    Ostream&,
    const incompressiblePerfectGas<Specie>&
);


/*---------------------------------------------------------------------------*\
                    Class incompressiblePerfectGas Declaration
\*---------------------------------------------------------------------------*/

template<class Specie>
class incompressiblePerfectGas
:
    public Specie
{

    //- Reference pressure
    scalar pRef_;


public:


    //- Construct from components
    inline incompressiblePerfectGas(const Specie& sp, const scalar pRef)
    :
        Specie(sp),
        pRef_(pRef)
    {}

    //- Construct from dictionary
    incompressiblePerfectGas(const dictionary& dict);

    //- Construct as named copy
    inline incompressiblePerfectGas
    (
        const word& name,
        const incompressiblePerfectGas& ipg
    )
    :
        Specie(name, ipg),
        pRef_(ipg.pRef_)
    {}

    //- Construct and return a clone
    inline autoPtr<incompressiblePerfectGas> clone() const
    {
        return autoPtr<incompressiblePerfectGas>
        (
            new incompressiblePerfectGas(*this)
        );
    }

    // Selector from dictionary
    inline static autoPtr<incompressiblePerfectGas> New
    (
        const dictionary& dict
    )
    {
        return autoPtr<incompressiblePerfectGas>
        (
            new incompressiblePerfectGas(dict)
        );
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return
            "incompressiblePerfectGas<"
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
        return pRef_/(this->R()*T);
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

    inline void operator=(const incompressiblePerfectGas& ipg);

    inline void operator+=(const incompressiblePerfectGas& ipg);

    inline void operator*=(const scalar s);


    // Friend operators

    friend incompressiblePerfectGas operator+ <Specie>
    (
        const incompressiblePerfectGas& ipg1,
        const incompressiblePerfectGas& ipg2
    );

    friend incompressiblePerfectGas operator* <Specie>
    (
        const scalar s,
        const incompressiblePerfectGas& ipg
    );

    friend incompressiblePerfectGas operator== <Specie>
    (
        const incompressiblePerfectGas& ipg1,
        const incompressiblePerfectGas& ipg2
    );


    // Ostream Operator

    friend Ostream& operator<< <Specie>
    (
        Ostream&,
        const incompressiblePerfectGas&
    );
};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
CML::incompressiblePerfectGas<Specie>::incompressiblePerfectGas
(
    const dictionary& dict
)
:
    Specie(dict),
    pRef_(readScalar(dict.subDict("equationOfState").lookup("pRef")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
void CML::incompressiblePerfectGas<Specie>::write(Ostream& os) const
{
    Specie::write(os);
    dictionary dict("equationOfState");
    dict.add("pRef", pRef_);

    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Specie>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const incompressiblePerfectGas<Specie>& pg
)
{
    pg.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Specie>
inline void CML::incompressiblePerfectGas<Specie>::operator=
(
    const incompressiblePerfectGas<Specie>& ipg
)
{
    Specie::operator=(ipg);
    pRef_ = ipg.pRef_;
}


template<class Specie>
inline void CML::incompressiblePerfectGas<Specie>::operator+=
(
    const incompressiblePerfectGas<Specie>& ipg
)
{
    scalar Y1 = this->Y();
    Specie::operator+=(ipg);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        const scalar Y2 = ipg.Y()/this->Y();

        pRef_ = Y1*pRef_ + Y2*ipg.pRef_;
    }
}


template<class Specie>
inline void CML::incompressiblePerfectGas<Specie>::operator*=(const scalar s)
{
    Specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Specie>
inline CML::incompressiblePerfectGas<Specie> CML::operator+
(
    const incompressiblePerfectGas<Specie>& ipg1,
    const incompressiblePerfectGas<Specie>& ipg2
)
{
    Specie sp
    (
        static_cast<const Specie&>(ipg1)
      + static_cast<const Specie&>(ipg2)
    );

    if (mag(sp.Y()) < SMALL)
    {
        return incompressiblePerfectGas<Specie>
        (
            sp,
            ipg1.pRef_
        );
    }
    else
    {
        const scalar Y1 = ipg1.Y()/sp.Y();
        const scalar Y2 = ipg2.Y()/sp.Y();

        return incompressiblePerfectGas<Specie>
        (
            sp,
            Y1*ipg1.pRef_ + Y2*ipg2.pRef_
        );
    }
}


template<class Specie>
inline CML::incompressiblePerfectGas<Specie> CML::operator*
(
    const scalar s,
    const incompressiblePerfectGas<Specie>& ipg
)
{
    return incompressiblePerfectGas<Specie>
    (
        s*static_cast<const Specie&>(ipg),
        ipg.pRef_
    );
}


template<class Specie>
inline CML::incompressiblePerfectGas<Specie> CML::operator==
(
    const incompressiblePerfectGas<Specie>& ipg1,
    const incompressiblePerfectGas<Specie>& ipg2
)
{
    Specie sp
    (
        static_cast<const Specie&>(ipg1)
     == static_cast<const Specie&>(ipg2)
    );

    const scalar Y1 = ipg1.Y()/sp.Y();
    const scalar Y2 = ipg2.Y()/sp.Y();

    return incompressiblePerfectGas<Specie>
    (
        sp,
        Y2*ipg2.pRef_ - Y1*ipg1.pRef_
    );
}


#endif
