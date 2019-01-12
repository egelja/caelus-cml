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
    CML::polynomialSolidTransport

Description
    Transport package using polynomial functions for solid \c kappa.

Usage

    \table
        Property        | Description
        kappaCoeffs<8>  | Thermal conductivity polynomial coefficients
    \endtable

    Example of the specification of the transport properties:
    \verbatim
    transport
    {
        kappaCoeffs<8>     ( 1000 -0.05 0.003 0 0 0 0 0 );
    }
    \endverbatim

    The polynomial expression is evaluated as so:

        \f[
            \kappa = 1000 - 0.05 T + 0.003 T^2
        \f]

Note
    Thermal conductivity polynomial coefficients evaluate to an expression in
    [W/m/K].

See also
    CML::Polynomial

\*---------------------------------------------------------------------------*/

#ifndef polynomialSolidTransport_HPP
#define polynomialSolidTransport_HPP

#include "Polynomial_.hpp"
#include "specie.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Thermo, int PolySize> class polynomialSolidTransport;

template<class Thermo, int PolySize>
inline polynomialSolidTransport<Thermo, PolySize> operator+
(
    const polynomialSolidTransport<Thermo, PolySize>&,
    const polynomialSolidTransport<Thermo, PolySize>&
);

template<class Thermo, int PolySize>
inline polynomialSolidTransport<Thermo, PolySize> operator*
(
    const scalar,
    const polynomialSolidTransport<Thermo, PolySize>&
);

template<class Thermo, int PolySize>
Ostream& operator<<
(
    Ostream&,
    const polynomialSolidTransport<Thermo, PolySize>&
);


/*---------------------------------------------------------------------------*\
                     Class polynomialSolidTransport Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo, int PolySize=8>
class polynomialSolidTransport
:
    public Thermo
{

    //- Thermal conductivity polynomial coefficients
    //  Note: input in [W/m/K]
    Polynomial<PolySize> kappaCoeffs_;


    //- Construct from components
    inline polynomialSolidTransport
    (
        const Thermo& t,
        const Polynomial<PolySize>& kappaPoly
    )
    :
        Thermo(t),
        kappaCoeffs_(kappaPoly)
    {}


public:


    //- Construct as named copy
    inline polynomialSolidTransport
    (
        const word& name,
        const polynomialSolidTransport& pt
    )
    :
        Thermo(name, pt),
        kappaCoeffs_(pt.kappaCoeffs_)
    {}

    //- Construct from dictionary
    polynomialSolidTransport(const dictionary& dict);

    //- Construct and return a clone
    inline autoPtr<polynomialSolidTransport> clone() const
    {
        return autoPtr<polynomialSolidTransport<Thermo, PolySize> >
        (
            new polynomialSolidTransport<Thermo, PolySize>(*this)
        );
    }

    // Selector from dictionary
    inline static autoPtr<polynomialSolidTransport> New
    (
        const dictionary&dict
    )
    {
        return autoPtr<polynomialSolidTransport<Thermo, PolySize> >
        (
            new polynomialSolidTransport<Thermo, PolySize>(dict)
        );
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "polynomial<" + Thermo::typeName() + '>';
    }

    //- Is the thermal conductivity isotropic
    static const bool isotropic = true;

    //- Dynamic viscosity [kg/ms]
    inline scalar mu(const scalar p, const scalar T) const
    {
        NotImplemented;
        return scalar(0);
    }

    //- Thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const
    {
        return kappaCoeffs_.value(T);
    }

    //- Thermal conductivity [W/mK]
    inline vector Kappa(const scalar p, const scalar T) const
    {
        const scalar kappa(kappaCoeffs_.value(T));
        return vector(kappa, kappa, kappa);
    }

    //- Thermal diffusivity of enthalpy [kg/ms]
    inline scalar alphah(const scalar p, const scalar T) const
    {
        return kappa(p, T)/this->Cp(p, T);
    }


    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators

    inline void operator=(const polynomialSolidTransport& pt)
    {
        Thermo::operator=(pt);
    
        kappaCoeffs_ = pt.kappaCoeffs_;
    }

    inline void operator+=(const polynomialSolidTransport& pt)
    {
        scalar Y1 = this->Y();

        Thermo::operator+=(pt);

        Y1 /= this->Y();
        scalar Y2 = pt.Y()/this->Y();

        kappaCoeffs_ = Y1*kappaCoeffs_ + Y2*pt.kappaCoeffs_;
    }

    inline void operator*=(const scalar s)
    {
        Thermo::operator*=(s);
    }


    // Friend operators

    friend polynomialSolidTransport operator+
    (
        const polynomialSolidTransport& pt1,
        const polynomialSolidTransport& pt2
    )
    {
        Thermo t
        (
            static_cast<const Thermo&>(pt1) + static_cast<const Thermo&>(pt2)
        );

        scalar Y1 = pt1.Y()/t.Y();
        scalar Y2 = pt2.Y()/t.Y();

        return polynomialSolidTransport<Thermo, PolySize>
        (
            t,
            Y1*pt1.kappaCoeffs_ + Y2*pt2.kappaCoeffs_
        );
    }

    friend polynomialSolidTransport operator*
    (
        const scalar s,
        const polynomialSolidTransport& pt
    )
    {
        return polynomialSolidTransport<Thermo, PolySize>
        (
            s*static_cast<const Thermo&>(pt),
            pt.kappaCoeffs_
        );
    }


    // Ostream Operator

    friend Ostream& operator<< <Thermo, PolySize>
    (
        Ostream&,
        const polynomialSolidTransport&
    );

};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo, int PolySize>
CML::polynomialSolidTransport<Thermo, PolySize>::polynomialSolidTransport
(
    const dictionary& dict
)
:
    Thermo(dict),
    kappaCoeffs_
    (
        dict.subDict("transport").lookup
        (
            "kappaCoeffs<" + CML::name(PolySize) + '>'
        )
    )
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo, int PolySize>
void CML::polynomialSolidTransport<Thermo, PolySize>::write(Ostream& os) const
{
    Thermo::write(os);

    dictionary dict("transport");

    dict.add
    (
        word("kappaCoeffs<" + CML::name(PolySize) + '>'),
        kappaCoeffs_
    );
    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Thermo, int PolySize>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const polynomialSolidTransport<Thermo, PolySize>& pt
)
{
    pt.write(os);
    return os;
}


#endif
