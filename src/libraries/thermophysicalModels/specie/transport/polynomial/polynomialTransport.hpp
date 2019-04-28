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
    CML::polynomialTransport

Description
    Transport package using polynomial functions for \c mu and \c kappa.

Usage

    \table
        Property        | Description
        muCoeffs<8>     | Dynamic viscosity polynomial coefficients
        kappaCoeffs<8>  | Thermal conductivity polynomial coefficients
    \endtable

    Example of the specification of the transport properties:
    \verbatim
    transport
    {
        muCoeffs<8>     ( 1000 -0.05 0.003 0 0 0 0 0 );
        kappaCoeffs<8>  ( 2000 -0.15 0.023 0 0 0 0 0 );
    }
    \endverbatim

    The polynomial expressions are evaluated as so:

        \f[
            \mu    = 1000 - 0.05 T + 0.003 T^2
        \f]

        \f[
            \kappa = 2000 - 0.15 T + 0.023 T^2
        \f]

Note
    - Dynamic viscosity polynomial coefficients evaluate to an expression in
      [Pa.s], but internally uses [Pa.s/kmol].
    - Thermal conductivity polynomial coefficients evaluate to an expression in
      [W/m/K], but internally uses [W/m/K/kmol].

SourceFiles
    polynomialTransportI.hpp
    polynomialTransport.cpp

See also
    CML::Polynomial

\*---------------------------------------------------------------------------*/

#ifndef polynomialTransport_HPP
#define polynomialTransport_HPP

#include "Polynomial_.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Thermo, int PolySize> class polynomialTransport;

template<class Thermo, int PolySize>
inline polynomialTransport<Thermo, PolySize> operator+
(
    const polynomialTransport<Thermo, PolySize>&,
    const polynomialTransport<Thermo, PolySize>&
);

template<class Thermo, int PolySize>
inline polynomialTransport<Thermo, PolySize> operator*
(
    const scalar,
    const polynomialTransport<Thermo, PolySize>&
);

template<class Thermo, int PolySize>
Ostream& operator<<
(
    Ostream&,
    const polynomialTransport<Thermo, PolySize>&
);


/*---------------------------------------------------------------------------*\
                     Class polynomialTransport Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo, int PolySize=8>
class polynomialTransport
:
    public Thermo
{
    // Private data

    //- Dynamic viscosity polynomial coefficients
    Polynomial<PolySize> muCoeffs_;

    //- Thermal conductivity polynomial coefficients
    Polynomial<PolySize> kappaCoeffs_;


    // Private Member Functions

    //- Construct from components
    inline polynomialTransport
    (
        const Thermo& t,
        const Polynomial<PolySize>& muPoly,
        const Polynomial<PolySize>& kappaPoly
    );


public:


    //- Construct as named copy
    inline polynomialTransport(const word&, const polynomialTransport&);

    //- Construct from dictionary
    polynomialTransport(const dictionary& dict);

    //- Construct and return a clone
    inline autoPtr<polynomialTransport> clone() const;

    // Selector from dictionary
    inline static autoPtr<polynomialTransport> New(const dictionary& dict);


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "polynomial<" + Thermo::typeName() + '>';
    }

    //- Dynamic viscosity [kg/ms]
    inline scalar mu(const scalar p, const scalar T) const;

    //- Thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const;

    //- Thermal diffusivity of enthalpy [kg/ms]
    inline scalar alphah(const scalar p, const scalar T) const;

    // Species diffusivity
    // inline scalar D(const scalar p, const scalar T) const;

    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators

    inline void operator=(const polynomialTransport&);

    inline void operator+=(const polynomialTransport&);

    inline void operator*=(const scalar);


    // Friend operators
    friend polynomialTransport operator+ <Thermo, PolySize>
    (
        const polynomialTransport&,
        const polynomialTransport&
    );

    friend polynomialTransport operator* <Thermo, PolySize>
    (
        const scalar,
        const polynomialTransport&
    );


    // Ostream Operator
    friend Ostream& operator<< <Thermo, PolySize>
    (
        Ostream&,
        const polynomialTransport&
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "specie.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo, int PolySize>
inline CML::polynomialTransport<Thermo, PolySize>::polynomialTransport
(
    const Thermo& t,
    const Polynomial<PolySize>& muCoeffs,
    const Polynomial<PolySize>& kappaCoeffs
)
:
    Thermo(t),
    muCoeffs_(muCoeffs),
    kappaCoeffs_(kappaCoeffs)
{}


template<class Thermo, int PolySize>
inline CML::polynomialTransport<Thermo, PolySize>::polynomialTransport
(
    const word& name,
    const polynomialTransport& pt
)
:
    Thermo(name, pt),
    muCoeffs_(pt.muCoeffs_),
    kappaCoeffs_(pt.kappaCoeffs_)
{}


template<class Thermo, int PolySize>
inline CML::autoPtr<CML::polynomialTransport<Thermo, PolySize> >
CML::polynomialTransport<Thermo, PolySize>::clone() const
{
    return autoPtr<polynomialTransport<Thermo, PolySize> >
    (
        new polynomialTransport<Thermo, PolySize>(*this)
    );
}


template<class Thermo, int PolySize>
inline CML::autoPtr<CML::polynomialTransport<Thermo, PolySize> >
CML::polynomialTransport<Thermo, PolySize>::New(const dictionary& dict)
{
    return autoPtr<polynomialTransport<Thermo, PolySize> >
    (
        new polynomialTransport<Thermo, PolySize>(dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo, int PolySize>
inline CML::scalar CML::polynomialTransport<Thermo, PolySize>::mu
(
    const scalar p,
    const scalar T
) const
{
    return muCoeffs_.value(T);
}


template<class Thermo, int PolySize>
inline CML::scalar CML::polynomialTransport<Thermo, PolySize>::kappa
(
    const scalar p,
    const scalar T
) const
{
    return kappaCoeffs_.value(T);
}


template<class Thermo, int PolySize>
inline CML::scalar CML::polynomialTransport<Thermo, PolySize>::alphah
(
    const scalar p, const scalar T
) const
{
    return kappa(p, T)/this->Cp(p, T);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Thermo, int PolySize>
inline void CML::polynomialTransport<Thermo, PolySize>::operator=
(
    const polynomialTransport<Thermo, PolySize>& pt
)
{
    Thermo::operator=(pt);

    muCoeffs_ = pt.muCoeffs_;
    kappaCoeffs_ = pt.kappaCoeffs_;
}


template<class Thermo, int PolySize>
inline void CML::polynomialTransport<Thermo, PolySize>::operator+=
(
    const polynomialTransport<Thermo, PolySize>& pt
)
{
    scalar Y1 = this->Y();

    Thermo::operator+=(pt);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        scalar Y2 = pt.Y()/this->Y();

        muCoeffs_ = Y1*muCoeffs_ + Y2*pt.muCoeffs_;
        kappaCoeffs_ = Y1*kappaCoeffs_ + Y2*pt.kappaCoeffs_;
    }
}


template<class Thermo, int PolySize>
inline void CML::polynomialTransport<Thermo, PolySize>::operator*=
(
    const scalar s
)
{
    Thermo::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Thermo, int PolySize>
inline CML::polynomialTransport<Thermo, PolySize> CML::operator+
(
    const polynomialTransport<Thermo, PolySize>& pt1,
    const polynomialTransport<Thermo, PolySize>& pt2
)
{
    Thermo t
    (
        static_cast<const Thermo&>(pt1) + static_cast<const Thermo&>(pt2)
    );

    if (mag(t.Y()) < SMALL)
    {
        return polynomialTransport<Thermo>
        (
            t,
            0,
            pt1.muCoeffs_,
            pt1.kappaCoeffs_
        );
    }
    else
    {
        scalar Y1 = pt1.Y()/t.Y();
        scalar Y2 = pt2.Y()/t.Y();

        return polynomialTransport<Thermo, PolySize>
        (
            t,
            Y1*pt1.muCoeffs_ + Y2*pt2.muCoeffs_,
            Y1*pt1.kappaCoeffs_ + Y2*pt2.kappaCoeffs_
        );
    }
}


template<class Thermo, int PolySize>
inline CML::polynomialTransport<Thermo, PolySize> CML::operator*
(
    const scalar s,
    const polynomialTransport<Thermo, PolySize>& pt
)
{
    return polynomialTransport<Thermo, PolySize>
    (
        s*static_cast<const Thermo&>(pt),
        pt.muCoeffs_,
        pt.kappaCoeffs_
    );
}


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


template<class Thermo, int PolySize>
CML::polynomialTransport<Thermo, PolySize>::polynomialTransport
(
    const dictionary& dict
)
:
    Thermo(dict),
    muCoeffs_
    (
        dict.subDict("transport").lookup
        (
            "muCoeffs<" + CML::name(PolySize) + '>'
        )
    ),
    kappaCoeffs_
    (
        dict.subDict("transport").lookup
        (
            "kappaCoeffs<" + CML::name(PolySize) + '>'
        )
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo, int PolySize>
void CML::polynomialTransport<Thermo, PolySize>::write(Ostream& os) const
{
    os  << this->name() << endl;
    os  << token::BEGIN_BLOCK << incrIndent << nl;

    Thermo::write(os);

    dictionary dict("transport");
    dict.add
    (
        word("muCoeffs<" + CML::name(PolySize) + '>'),
        muCoeffs_
    );
    dict.add
    (
        word("kappaCoeffs<" + CML::name(PolySize) + '>'),
        kappaCoeffs_
    );
    os  << indent << dict.dictName() << dict;

    os  << decrIndent << token::END_BLOCK << nl;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Thermo, int PolySize>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const polynomialTransport<Thermo, PolySize>& pt
)
{
    pt.write(os);
    return os;
}


#endif

// ************************************************************************* //
