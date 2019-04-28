/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
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
    CML::logPolynomialTransport

Description
    Transport package using polynomial functions of \c ln(T) for \c mu and
    \c kappa:

        \f[
            ln(mu)    = \sum_{i=1}^N \left( a[i] * ln(T)^{i-1} \right)
        \f]

        \f[
            ln(kappa) = \sum_{i=1}^N \left( b[i] * ln(T)^{i-1} \right)
        \f]

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
            \mu    = 1000 - 0.05 ln(T) + 0.003 ln(T)^2
        \f]

        \f[
            \kappa = 2000 - 0.15 ln(T) + 0.023 ln(T)^2
        \f]

Note
    - Dynamic viscosity polynomial coefficients evaluate to an expression in
      [Pa.s], but internally uses [Pa.s/kmol].
    - Thermal conductivity polynomial coefficients evaluate to an expression in
      [W/m/K], but internally uses [W/m/K/kmol].


See also
    CML::Polynomial

\*---------------------------------------------------------------------------*/

#ifndef logPolynomialTransport_HPP
#define logPolynomialTransport_HPP

#include "Polynomial.hpp"
#include "specie.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Thermo, int PolySize> class logPolynomialTransport;

template<class Thermo, int PolySize>
inline logPolynomialTransport<Thermo, PolySize> operator+
(
    const logPolynomialTransport<Thermo, PolySize>&,
    const logPolynomialTransport<Thermo, PolySize>&
);

template<class Thermo, int PolySize>
inline logPolynomialTransport<Thermo, PolySize> operator*
(
    const scalar,
    const logPolynomialTransport<Thermo, PolySize>&
);

template<class Thermo, int PolySize>
Ostream& operator<<
(
    Ostream&,
    const logPolynomialTransport<Thermo, PolySize>&
);


/*---------------------------------------------------------------------------*\
                     Class logPolynomialTransport Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo, int PolySize=8>
class logPolynomialTransport
:
    public Thermo
{
    // Private data

    //- Dynamic viscosity polynomial coefficients
    //  Note: input in [Pa.s], but internally uses [Pa.s/kmol]
    Polynomial<PolySize> muCoeffs_;

    //- Thermal conductivity polynomial coefficients
    //  Note: input in [W/m/K], but internally uses [W/m/K/kmol]
    Polynomial<PolySize> kappaCoeffs_;


    // Private Member Functions

    //- Construct from components
    inline logPolynomialTransport
    (
        const Thermo& t,
        const Polynomial<PolySize>& muPoly,
        const Polynomial<PolySize>& kappaPoly
    )
    :
        Thermo(t),
        muCoeffs_(muCoeffs),
        kappaCoeffs_(kappaCoeffs)
    {}


public:


    //- Construct as named copy
    inline logPolynomialTransport
    (
        const word& name,
        const logPolynomialTransport& pt
    )
    :
        Thermo(name, pt),
        muCoeffs_(pt.muCoeffs_),
        kappaCoeffs_(pt.kappaCoeffs_)
    {}

    //- Construct from dictionary
    logPolynomialTransport(const dictionary& dict);

    //- Construct and return a clone
    inline autoPtr<logPolynomialTransport> clone() const
    {
        return autoPtr<logPolynomialTransport<Thermo, PolySize> >
        (
            new logPolynomialTransport<Thermo, PolySize>(*this)
        );
    }

    // Selector from dictionary
    inline static autoPtr<logPolynomialTransport> New
    (
        const dictionary& dict
    )
    {
        return autoPtr<logPolynomialTransport<Thermo, PolySize> >
        (
            new logPolynomialTransport<Thermo, PolySize>(dict)
        );
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "logPolynomial<" + Thermo::typeName() + '>';
    }

    //- Dynamic viscosity [kg/ms]
    inline scalar mu(const scalar p, const scalar T) const
    {
        return exp(muCoeffs_.value(log(T)));
    }

    //- Thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const
    {
        return exp(kappaCoeffs_.value(log(T)));
    }

    //- Thermal diffusivity of enthalpy [kg/ms]
    inline scalar alphah(const scalar p, const scalar T) const
    {
        return kappa(p, T)/this->Cp(p, T);
    }

    // Species diffusivity
    // inline scalar D(const scalar p, const scalar T) const;

    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators

    inline void operator=(const logPolynomialTransport& pt)
    {
        Thermo::operator=(pt);

        muCoeffs_ = pt.muCoeffs_;
        kappaCoeffs_ = pt.kappaCoeffs_;
    }

    inline void operator+=(const logPolynomialTransport& pt)
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

    inline void operator*=(const scalar s)
    {
        Thermo::operator*=(s);
    }


    // Friend operators
    friend logPolynomialTransport operator+ <Thermo, PolySize>
    (
        const logPolynomialTransport& pt1,
        const logPolynomialTransport& pt2
    )
    {
        Thermo t
        (
            static_cast<const Thermo&>(pt1) + static_cast<const Thermo&>(pt2)
        );

        if (mag(t.Y()) < SMALL)
        {
            return logPolynomialTransport<Thermo>
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

            return logPolynomialTransport<Thermo, PolySize>
            (
                t,
                Y1*pt1.muCoeffs_ + Y2*pt2.muCoeffs_,
                Y1*pt1.kappaCoeffs_ + Y2*pt2.kappaCoeffs_
            );
        }
    }

    friend logPolynomialTransport operator* <Thermo, PolySize>
    (
        const scalar s,
        const logPolynomialTransport& pt
    )
    {
        return logPolynomialTransport<Thermo, PolySize>
        (
            s*static_cast<const Thermo&>(pt),
            pt.muCoeffs_,
            pt.kappaCoeffs_
        );
    }


    // Ostream Operator
    friend Ostream& operator<< <Thermo, PolySize>
    (
        Ostream&,
        const logPolynomialTransport&
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo, int PolySize>
CML::logPolynomialTransport<Thermo, PolySize>::logPolynomialTransport
(
    const dictionary& dict
)
:
    Thermo(dict),
    muCoeffs_
    (
        dict.subDict("transport").lookup
        (
            "muLogCoeffs<" + CML::name(PolySize) + '>'
        )
    ),
    kappaCoeffs_
    (
        dict.subDict("transport").lookup
        (
            "kappaLogCoeffs<" + CML::name(PolySize) + '>'
        )
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo, int PolySize>
void CML::logPolynomialTransport<Thermo, PolySize>::write(Ostream& os) const
{
    os  << this->name() << endl;
    os  << token::BEGIN_BLOCK << incrIndent << nl;

    Thermo::write(os);

    dictionary dict("transport");
    dict.add
    (
        word("muLogCoeffs<" + CML::name(PolySize) + '>'),
        muCoeffs_
    );
    dict.add
    (
        word("kappaLogCoeffs<" + CML::name(PolySize) + '>'),
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
    const logPolynomialTransport<Thermo, PolySize>& pt
)
{
    pt.write(os);
    return os;
}


#endif
