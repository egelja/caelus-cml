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
    CML::icoPolynomial

Description
    Incompressible, polynomial form of equation of state, using a polynomial
    function for density.

Usage
    \table
        Property     | Description
        rhoCoeffs<8> | Density polynomial coefficients
    \endtable

    Example of the specification of the equation of state:
    \verbatim
    equationOfState
    {
        rhoCoeffs<8>    ( 1000 -0.05 0.003 0 0 0 0 0 );
    }
    \endverbatim

    The polynomial expression is evaluated as so:

        \f[
            \rho = 1000 - 0.05 T + 0.003 T^2
        \f]

Note
    Input in [kg/m3], but internally uses [kg/m3/kmol].

See also
    CML::Polynomial

\*---------------------------------------------------------------------------*/

#ifndef icoPolynomial_HPP
#define icoPolynomial_HPP

#include "autoPtr.hpp"
#include "Polynomial_.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Specie, int PolySize>
class icoPolynomial;

template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize> operator+
(
    const icoPolynomial<Specie, PolySize>&,
    const icoPolynomial<Specie, PolySize>&
);

template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize> operator*
(
    const scalar,
    const icoPolynomial<Specie, PolySize>&
);

template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize> operator==
(
    const icoPolynomial<Specie, PolySize>&,
    const icoPolynomial<Specie, PolySize>&
);

template<class Specie, int PolySize>
Ostream& operator<<
(
    Ostream&,
    const icoPolynomial<Specie, PolySize>&
);


/*---------------------------------------------------------------------------*\
                        Class icoPolynomial Declaration
\*---------------------------------------------------------------------------*/

template<class Specie, int PolySize=8>
class icoPolynomial
:
    public Specie
{

    //- Density polynomial coefficients
    Polynomial<PolySize> rhoCoeffs_;


public:


    //- Construct from components
    inline icoPolynomial
    (
        const Specie& sp,
        const Polynomial<PolySize>& rhoCoeffs
    )
    :
        Specie(sp),
        rhoCoeffs_(rhoCoeffs)
    {}

    //- Construct from dictionary
    icoPolynomial(const dictionary& dict);

    //- Construct as named copy
    inline icoPolynomial(const word& name, const icoPolynomial& ip)
    :
        Specie(name, ip),
        rhoCoeffs_(ip.rhoCoeffs_)
    {}

    //- Construct and return a clone
    inline autoPtr<icoPolynomial> clone() const
    {
        return autoPtr<icoPolynomial>
        (
            new icoPolynomial(*this)
        );
    }

    // Selector from dictionary
    inline static autoPtr<icoPolynomial> New(const dictionary& dict)
    {
        return autoPtr<icoPolynomial>
        (
            new icoPolynomial(dict)
        );
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "icoPolynomial<" + word(Specie::typeName_()) + '>';
    }


    // Fundamental properties

    //- Is the equation of state is incompressible i.e. rho != f(p)
    static const bool incompressible = true;

    //- Is the equation of state is isochoric i.e. rho = const
    static const bool isochoric = false;

    //- Return density [kg/m^3]
    inline scalar rho(scalar p, scalar T) const
    {
        return rhoCoeffs_.value(T);
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

    inline void operator=(const icoPolynomial& ip);

    inline void operator+=(const icoPolynomial& ip);

    inline void operator*=(const scalar s);


    // Friend operators

    friend icoPolynomial operator+ <Specie, PolySize>
    (
        const icoPolynomial& ip1,
        const icoPolynomial& ip2
    );

    friend icoPolynomial operator* <Specie, PolySize>
    (
        const scalar s,
        const icoPolynomial& ip
    );

    friend icoPolynomial operator== <Specie, PolySize>
    (
        const icoPolynomial& ip1,
        const icoPolynomial& ip2
    );


    // Ostream Operator

    friend Ostream& operator<< <Specie, PolySize>
    (
        Ostream&,
        const icoPolynomial&
    );
};


} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeIcoPolynomial(PolySize)                                          \
                                                                             \
defineTemplateTypeNameAndDebugWithName                                       \
(                                                                            \
    icoPolynomial<PolySize>,                                                 \
    "icoPolynomial<"#PolySize">",                                            \
    0                                                                        \
);


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie, int PolySize>
icoPolynomial<Specie, PolySize>::icoPolynomial(const dictionary& dict)
:
    Specie(dict),
    rhoCoeffs_
(
    dict.subDict("equationOfState").lookup
    (
        "rhoCoeffs<" + CML::name(PolySize) + '>'
    )
)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie, int PolySize>
void icoPolynomial<Specie, PolySize>::write(Ostream& os) const
{
    Specie::write(os);

    dictionary dict("equationOfState");
    dict.add
    (
        word("rhoCoeffs<" + CML::name(PolySize) + '>'),
        rhoCoeffs_
    );

    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Specie, int PolySize>
Ostream& operator<<(Ostream& os, const icoPolynomial<Specie, PolySize>& ip)
{
    ip.write(os);
    return os;
}


} // End namespace CML


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Specie, int PolySize>
inline void CML::icoPolynomial<Specie, PolySize>::operator=
(
    const icoPolynomial<Specie, PolySize>& ip
)
{
    Specie::operator=(ip);

    rhoCoeffs_ = ip.rhoCoeffs_;
}


template<class Specie, int PolySize>
inline void CML::icoPolynomial<Specie, PolySize>::operator+=
(
    const icoPolynomial<Specie, PolySize>& ip
)
{
    scalar Y1 = this->Y();
    Specie::operator+=(ip);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        const scalar Y2 = ip.Y()/this->Y();

        rhoCoeffs_ = Y1*rhoCoeffs_ + Y2*ip.rhoCoeffs_;
    }
}


template<class Specie, int PolySize>
inline void CML::icoPolynomial<Specie, PolySize>::operator*=(const scalar s)
{
    Specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Specie, int PolySize>
CML::icoPolynomial<Specie, PolySize> CML::operator+
(
    const icoPolynomial<Specie, PolySize>& ip1,
    const icoPolynomial<Specie, PolySize>& ip2
)
{
    Specie sp
    (
        static_cast<const Specie&>(ip1)
      + static_cast<const Specie&>(ip2)
    );

    if (mag(sp.Y()) < SMALL)
    {
        return icoPolynomial<Specie, PolySize>
        (
            sp,
            ip1.rhoCoeffs_
        );
    }
    else
    {
        const scalar Y1 = ip1.Y()/sp.Y();
        const scalar Y2 = ip2.Y()/sp.Y();

        return icoPolynomial<Specie, PolySize>
        (
            sp,
            Y1*ip1.rhoCoeffs_ + Y2*ip2.rhoCoeffs_
        );
    }
}


template<class Specie, int PolySize>
CML::icoPolynomial<Specie, PolySize> CML::operator*
(
    const scalar s,
    const icoPolynomial<Specie, PolySize>& ip
)
{
    return icoPolynomial<Specie, PolySize>
    (
        s*static_cast<const Specie&>(ip),
        ip.rhoCoeffs_
    );
}


template<class Specie, int PolySize>
CML::icoPolynomial<Specie, PolySize> CML::operator==
(
    const icoPolynomial<Specie, PolySize>& ip1,
    const icoPolynomial<Specie, PolySize>& ip2
)
{
    Specie sp
    (
        static_cast<const Specie&>(ip1)
     == static_cast<const Specie&>(ip2)
    );

    const scalar Y1 = ip1.Y()/sp.Y();
    const scalar Y2 = ip2.Y()/sp.Y();

    return icoPolynomial<Specie, PolySize>
    (
        sp,
        Y2*ip2.rhoCoeffs_ - Y1*ip1.rhoCoeffs_
    );
}



#endif
