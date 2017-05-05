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
    CML::polynomialTransport

Description
    Transport package using polynomial functions for mu and kappa

SourceFiles
    polynomialTransportI.hpp
    polynomialTransport.cpp

\*---------------------------------------------------------------------------*/

#ifndef polynomialTransport_H
#define polynomialTransport_H

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
inline polynomialTransport<Thermo, PolySize> operator-
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
inline polynomialTransport<Thermo, PolySize> operator==
(
    const polynomialTransport<Thermo, PolySize>&,
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

template<class Thermo, int PolySize>
class polynomialTransport
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
        inline polynomialTransport
        (
            const Thermo& t,
            const Polynomial<PolySize>& muPoly,
            const Polynomial<PolySize>& kappaPoly
        );


public:

    // Constructors

        //- Construct copy
        inline polynomialTransport(const polynomialTransport&);

        //- Construct as named copy
        inline polynomialTransport(const word&, const polynomialTransport&);

        //- Construct from Istream
        polynomialTransport(Istream& is);

        //- Construct from dictionary
        polynomialTransport(const dictionary& dict);

        //- Construct and return a clone
        inline autoPtr<polynomialTransport> clone() const;

        // Selector from Istream
        inline static autoPtr<polynomialTransport> New(Istream& is);

        // Selector from dictionary
        inline static autoPtr<polynomialTransport> New(const dictionary& dict);


    // Member functions

        //- Dynamic viscosity [kg/ms]
        inline scalar mu(const scalar T) const;

        //- Thermal conductivity [W/mK]
        inline scalar kappa(const scalar T) const;

        //- Thermal diffusivity for enthalpy [kg/ms]
        inline scalar alpha(const scalar T) const;

        // Species diffusivity
        //inline scalar D(const scalar T) const;

        //- Write to Ostream
        void write(Ostream& os) const;


    // Member operators

        inline polynomialTransport& operator=(const polynomialTransport&);
        inline void operator+=(const polynomialTransport&);
        inline void operator-=(const polynomialTransport&);
        inline void operator*=(const scalar);


    // Friend operators

        friend polynomialTransport operator+ <Thermo, PolySize>
        (
            const polynomialTransport&,
            const polynomialTransport&
        );

        friend polynomialTransport operator- <Thermo, PolySize>
        (
            const polynomialTransport&,
            const polynomialTransport&
        );

        friend polynomialTransport operator* <Thermo, PolySize>
        (
            const scalar,
            const polynomialTransport&
        );

        friend polynomialTransport operator== <Thermo, PolySize>
        (
            const polynomialTransport&,
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
    const polynomialTransport& pt
)
:
    Thermo(pt),
    muCoeffs_(pt.muCoeffs_),
    kappaCoeffs_(pt.kappaCoeffs_)
{}


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
CML::polynomialTransport<Thermo, PolySize>::New(Istream& is)
{
    return autoPtr<polynomialTransport<Thermo, PolySize> >
    (
        new polynomialTransport<Thermo, PolySize>(is)
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
    const scalar T
) const
{
    return muCoeffs_.value(T)/this->W();
}


template<class Thermo, int PolySize>
inline CML::scalar CML::polynomialTransport<Thermo, PolySize>::kappa
(
    const scalar T
) const
{
    return kappaCoeffs_.value(T)/this->W();
}


template<class Thermo, int PolySize>
inline CML::scalar CML::polynomialTransport<Thermo, PolySize>::alpha
(
    const scalar T
) const
{
    scalar deltaT = T - specie::Tstd;
    scalar CpBar =
        (deltaT*(this->H(T) - this->H(specie::Tstd)) + this->Cp(T))
       /(sqr(deltaT) + 1);

    return kappa(T)/CpBar;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Thermo, int PolySize>
inline CML::polynomialTransport<Thermo, PolySize>&
CML::polynomialTransport<Thermo, PolySize>::operator=
(
    const polynomialTransport<Thermo, PolySize>& pt
)
{
    Thermo::operator=(pt);

    muCoeffs_ = pt.muCoeffs_;
    kappaCoeffs_ = pt.kappaCoeffs_;

    return *this;
}


template<class Thermo, int PolySize>
inline void CML::polynomialTransport<Thermo, PolySize>::operator+=
(
    const polynomialTransport<Thermo, PolySize>& pt
)
{
    scalar molr1 = this->nMoles();

    Thermo::operator+=(pt);

    molr1 /= this->nMoles();
    scalar molr2 = pt.nMoles()/this->nMoles();

    muCoeffs_ = molr1*muCoeffs_ + molr2*pt.muCoeffs_;
    kappaCoeffs_ = molr1*kappaCoeffs_ + molr2*pt.kappaCoeffs_;
}


template<class Thermo, int PolySize>
inline void CML::polynomialTransport<Thermo, PolySize>::operator-=
(
    const polynomialTransport<Thermo, PolySize>& pt
)
{
    scalar molr1 = this->nMoles();

    Thermo::operator-=(pt);

    molr1 /= this->nMoles();
    scalar molr2 = pt.nMoles()/this->nMoles();

    muCoeffs_ = molr1*muCoeffs_ - molr2*pt.muCoeffs_;
    kappaCoeffs_ = molr1*kappaCoeffs_ - molr2*pt.kappaCoeffs_;
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

    scalar molr1 = pt1.nMoles()/t.nMoles();
    scalar molr2 = pt2.nMoles()/t.nMoles();

    return polynomialTransport<Thermo, PolySize>
    (
        t,
        molr1*pt1.muCoeffs_ + molr2*pt2.muCoeffs_,
        molr1*pt1.kappaCoeffs_ + molr2*pt2.kappaCoeffs_
    );
}


template<class Thermo, int PolySize>
inline CML::polynomialTransport<Thermo, PolySize> CML::operator-
(
    const polynomialTransport<Thermo, PolySize>& pt1,
    const polynomialTransport<Thermo, PolySize>& pt2
)
{
    Thermo t
    (
        static_cast<const Thermo&>(pt1) - static_cast<const Thermo&>(pt2)
    );

    scalar molr1 = pt1.nMoles()/t.nMoles();
    scalar molr2 = pt2.nMoles()/t.nMoles();

    return polynomialTransport<Thermo, PolySize>
    (
        t,
        molr1*pt1.muCoeffs_ - molr2*pt2.muCoeffs_,
        molr1*pt1.kappaCoeffs_ - molr2*pt2.kappaCoeffs_
    );
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


template<class Thermo, int PolySize>
inline CML::polynomialTransport<Thermo, PolySize> CML::operator==
(
    const polynomialTransport<Thermo, PolySize>& pt1,
    const polynomialTransport<Thermo, PolySize>& pt2
)
{
    return pt2 - pt1;
}


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo, int PolySize>
CML::polynomialTransport<Thermo, PolySize>::polynomialTransport(Istream& is)
:
    Thermo(is),
    muCoeffs_("muCoeffs<" + CML::name(PolySize) + '>', is),
    kappaCoeffs_("kappaCoeffs<" + CML::name(PolySize) + '>', is)
{
    muCoeffs_ *= this->W();
    kappaCoeffs_ *= this->W();
}


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
{
    muCoeffs_ *= this->W();
    kappaCoeffs_ *= this->W();
}


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
        muCoeffs_/this->W()
    );
    dict.add
    (
        word("kappaCoeffs<" + CML::name(PolySize) + '>'),
        kappaCoeffs_/this->W()
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
    os  << static_cast<const Thermo&>(pt) << tab
        << "muCoeffs<" << CML::name(PolySize) << '>' << tab
        << pt.muCoeffs_/pt.W() << tab
        << "kappaCoeffs<" << CML::name(PolySize) << '>' << tab
        << pt.kappaCoeffs_/pt.W();

    os.check
    (
        "Ostream& operator<<"
        "("
            "Ostream&, "
            "const polynomialTransport<Thermo, PolySize>&"
        ")"
    );

    return os;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
