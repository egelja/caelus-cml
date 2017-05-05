/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

SourceFiles
    icoPolynomialI.hpp
    icoPolynomial.cpp

\*---------------------------------------------------------------------------*/

#ifndef icoPolynomial_H
#define icoPolynomial_H

#include "specie.hpp"
#include "autoPtr.hpp"
#include "Polynomial_.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<int PolySize>
class icoPolynomial;

template<int PolySize>
icoPolynomial<PolySize> operator+
(
    const icoPolynomial<PolySize>&,
    const icoPolynomial<PolySize>&
);

template<int PolySize>
icoPolynomial<PolySize> operator-
(
    const icoPolynomial<PolySize>&,
    const icoPolynomial<PolySize>&
);

template<int PolySize>
icoPolynomial<PolySize> operator*
(
    const scalar,
    const icoPolynomial<PolySize>&
);

template<int PolySize>
icoPolynomial<PolySize> operator==
(
    const icoPolynomial<PolySize>&,
    const icoPolynomial<PolySize>&
);

template<int PolySize>
Ostream& operator<<
(
    Ostream&,
    const icoPolynomial<PolySize>&
);


/*---------------------------------------------------------------------------*\
                        Class icoPolynomial Declaration
\*---------------------------------------------------------------------------*/

template<int PolySize>
class icoPolynomial
:
    public specie
{
    // Private data

        //- Density polynomial coefficients
        //  Note: input in [kg/m3], but internally uses [kg/m3/kmol]
        Polynomial<PolySize> rhoCoeffs_;


public:

    // Constructors

        //- Construct from components
        inline icoPolynomial
        (
            const specie& sp,
            const Polynomial<PolySize>& rhoPoly
        );

        //- Construct from Istream
        icoPolynomial(Istream&);

        //- Construct from dictionary
        icoPolynomial(const dictionary& dict);

        //- Construct as copy
        inline icoPolynomial(const icoPolynomial&);

        //- Construct as named copy
        inline icoPolynomial(const word& name, const icoPolynomial&);

        //- Construct and return a clone
        inline autoPtr<icoPolynomial> clone() const;

        // Selector from Istream
        inline static autoPtr<icoPolynomial> New(Istream& is);

        // Selector from dictionary
        inline static autoPtr<icoPolynomial> New(const dictionary& dict);


    // Member functions

        // Fundamental properties

            //- Return density [kg/m^3]
            inline scalar rho(scalar p, scalar T) const;

            //- Return compressibility rho/p [s^2/m^2]
            inline scalar psi(scalar p, scalar T) const;

            //- Return compression factor []
            inline scalar Z(scalar p, scalar T) const;


        // I-O

            //- Write to Ostream
            void write(Ostream& os) const;


    // Member operators

        inline icoPolynomial& operator=(const icoPolynomial&);
        inline void operator+=(const icoPolynomial&);
        inline void operator-=(const icoPolynomial&);

        inline void operator*=(const scalar);


    // Friend operators

        friend icoPolynomial operator+ <PolySize>
        (
            const icoPolynomial&,
            const icoPolynomial&
        );

        friend icoPolynomial operator- <PolySize>
        (
            const icoPolynomial&,
            const icoPolynomial&
        );

        friend icoPolynomial operator* <PolySize>
        (
            const scalar s,
            const icoPolynomial&
        );

        friend icoPolynomial operator== <PolySize>
        (
            const icoPolynomial&,
            const icoPolynomial&
        );


    // Ostream Operator

        friend Ostream& operator<< <PolySize>(Ostream&, const icoPolynomial&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<int PolySize>
inline CML::icoPolynomial<PolySize>::icoPolynomial
(
    const specie& sp,
    const Polynomial<PolySize>& rhoCoeffs
)
:
    specie(sp),
    rhoCoeffs_(rhoCoeffs)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<int PolySize>
inline CML::icoPolynomial<PolySize>::icoPolynomial
(
    const icoPolynomial<PolySize>& ip
)
:
    specie(ip),
    rhoCoeffs_(ip.rhoCoeffs_)
{}


template<int PolySize>
inline CML::icoPolynomial<PolySize>::icoPolynomial
(
    const word& name,
    const icoPolynomial<PolySize>& ip
)
:
    specie(name, ip),
    rhoCoeffs_(ip.rhoCoeffs_)
{}


template<int PolySize>
inline CML::autoPtr<CML::icoPolynomial<PolySize> >
CML::icoPolynomial<PolySize>::clone() const
{
    return autoPtr<icoPolynomial<PolySize> >
    (
        new icoPolynomial<PolySize>(*this)
    );
}


template<int PolySize>
inline CML::autoPtr<CML::icoPolynomial<PolySize> >
CML::icoPolynomial<PolySize>::New(Istream& is)
{
    return autoPtr<icoPolynomial<PolySize> >(new icoPolynomial<PolySize>(is));
}


template<int PolySize>
inline CML::autoPtr<CML::icoPolynomial<PolySize> >
CML::icoPolynomial<PolySize>::New(const dictionary& dict)
{
    return autoPtr<icoPolynomial<PolySize> >
    (
        new icoPolynomial<PolySize>(dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<int PolySize>
inline CML::scalar CML::icoPolynomial<PolySize>::rho(scalar, scalar T) const
{
    return rhoCoeffs_.value(T)/this->W();
}


template<int PolySize>
inline CML::scalar CML::icoPolynomial<PolySize>::psi(scalar, scalar) const
{
    return 0.0;
}


template<int PolySize>
inline CML::scalar CML::icoPolynomial<PolySize>::Z(scalar, scalar) const
{
    return 0.0;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<int PolySize>
inline CML::icoPolynomial<PolySize>& CML::icoPolynomial<PolySize>::operator=
(
    const icoPolynomial<PolySize>& ip
)
{
    specie::operator=(ip);

    rhoCoeffs_ = ip.rhoCoeffs_;

    return *this;
}


template<int PolySize>
inline void CML::icoPolynomial<PolySize>::operator+=
(
    const icoPolynomial<PolySize>& ip
)
{
    scalar molr1 = this->nMoles();

    specie::operator+=(ip);

    molr1 /= this->nMoles();
    scalar molr2 = ip.nMoles()/this->nMoles();

    rhoCoeffs_ = molr1*rhoCoeffs_ + molr2*ip.rhoCoeffs_;
}


template<int PolySize>
inline void CML::icoPolynomial<PolySize>::operator-=
(
    const icoPolynomial<PolySize>& ip
)
{
    scalar molr1 = this->nMoles();

    specie::operator-=(ip);

    molr1 /= this->nMoles();
    scalar molr2 = ip.nMoles()/this->nMoles();

    rhoCoeffs_ = molr1*rhoCoeffs_ - molr2*ip.rhoCoeffs_;
}


template<int PolySize>
inline void CML::icoPolynomial<PolySize>::operator*=(const scalar s)
{
    specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<int PolySize>
CML::icoPolynomial<PolySize> CML::operator+
(
    const icoPolynomial<PolySize>& ip1,
    const icoPolynomial<PolySize>& ip2
)
{
    scalar nMoles = ip1.nMoles() + ip2.nMoles();
    scalar molr1 = ip1.nMoles()/nMoles;
    scalar molr2 = ip2.nMoles()/nMoles;

    return icoPolynomial<PolySize>
    (
        static_cast<const specie&>(ip1)
      + static_cast<const specie&>(ip2),
        molr1*ip1.rhoCoeffs_ + molr2*ip2.rhoCoeffs_
    );
}


template<int PolySize>
CML::icoPolynomial<PolySize> CML::operator-
(
    const icoPolynomial<PolySize>& ip1,
    const icoPolynomial<PolySize>& ip2
)
{
    scalar nMoles = ip1.nMoles() + ip2.nMoles();
    scalar molr1 = ip1.nMoles()/nMoles;
    scalar molr2 = ip2.nMoles()/nMoles;

    return icoPolynomial<PolySize>
    (
        static_cast<const specie&>(ip1)
      - static_cast<const specie&>(ip2),
        molr1*ip1.rhoCoeffs_ - molr2*ip2.rhoCoeffs_
    );
}


template<int PolySize>
CML::icoPolynomial<PolySize> CML::operator*
(
    const scalar s,
    const icoPolynomial<PolySize>& ip
)
{
    return icoPolynomial<PolySize>
    (
        s*static_cast<const specie&>(ip),
        ip.rhoCoeffs_
    );
}


template<int PolySize>
CML::icoPolynomial<PolySize> CML::operator==
(
    const icoPolynomial<PolySize>& ip1,
    const icoPolynomial<PolySize>& ip2
)
{
    return ip2 - ip1;
}


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<int PolySize>
icoPolynomial<PolySize>::icoPolynomial(Istream& is)
:
    specie(is),
    rhoCoeffs_("rhoCoeffs<" + CML::name(PolySize) + '>', is)
{
    rhoCoeffs_ *= this->W();
}


template<int PolySize>
icoPolynomial<PolySize>::icoPolynomial(const dictionary& dict)
:
    specie(dict),
    rhoCoeffs_
(
    dict.subDict("equationOfState").lookup
    (
        "rhoCoeffs<" + CML::name(PolySize) + '>'
    )
)
{
    rhoCoeffs_ *= this->W();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<int PolySize>
void icoPolynomial<PolySize>::write(Ostream& os) const
{
    specie::write(os);

    dictionary dict("equationOfState");
    dict.add
    (
        word("rhoCoeffs<" + CML::name(PolySize) + '>'),
        rhoCoeffs_/this->W()
    );

    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<int PolySize>
Ostream& operator<<(Ostream& os, const icoPolynomial<PolySize>& ip)
{
    os  << static_cast<const specie&>(ip) << tab
        << "rhoCoeffs<" << CML::name(PolySize) << '>' << tab
        << ip.rhoCoeffs_/ip.W();

    os.check
    (
        "Ostream& operator<<(Ostream& os, const icoPolynomial<PolySize>& ip)"
    );

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
