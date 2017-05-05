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
    CML::constTransport

Description
    Constant properties Transport package.
    Templated into a given thermodynamics package (needed for thermal
    conductivity).

SourceFiles
    constTransportI.hpp
    constTransport.cpp

\*---------------------------------------------------------------------------*/

#ifndef constTransport_H
#define constTransport_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Thermo> class constTransport;

template<class Thermo>
inline constTransport<Thermo> operator+
(
    const constTransport<Thermo>&,
    const constTransport<Thermo>&
);

template<class Thermo>
inline constTransport<Thermo> operator-
(
    const constTransport<Thermo>&,
    const constTransport<Thermo>&
);

template<class Thermo>
inline constTransport<Thermo> operator*
(
    const scalar,
    const constTransport<Thermo>&
);

template<class Thermo>
inline constTransport<Thermo> operator==
(
    const constTransport<Thermo>&,
    const constTransport<Thermo>&
);

template<class Thermo>
Ostream& operator<<
(
    Ostream&,
    const constTransport<Thermo>&
);


/*---------------------------------------------------------------------------*\
                           Class constTransport Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo>
class constTransport
:
    public Thermo
{
    // Private data

        //- Constant dynamic viscosity [Pa.s]
        scalar mu_;

        //- Reciprocal Prandtl Number []
        scalar rPr_;


    // Private Member Functions

        //- Construct from components
        inline constTransport
        (
            const Thermo& t,
            const scalar mu,
            const scalar Pr
        );


public:

    // Constructors

        //- Construct as named copy
        inline constTransport(const word&, const constTransport&);

        //- Construct from Istream
        constTransport(Istream&);

        //- Construct from dictionary
        constTransport(const dictionary& dict);

        //- Construct and return a clone
        inline autoPtr<constTransport> clone() const;

        // Selector from Istream
        inline static autoPtr<constTransport> New(Istream& is);

        // Selector from dictionary
        inline static autoPtr<constTransport> New(const dictionary& dict);


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

        inline constTransport& operator=(const constTransport&);

        inline void operator+=(const constTransport&);

        inline void operator-=(const constTransport&);

        inline void operator*=(const scalar);


    // Friend operators

        friend constTransport operator+ <Thermo>
        (
            const constTransport&,
            const constTransport&
        );

        friend constTransport operator- <Thermo>
        (
            const constTransport&,
            const constTransport&
        );

        friend constTransport operator* <Thermo>
        (
            const scalar,
            const constTransport&
        );

        friend constTransport operator== <Thermo>
        (
            const constTransport&,
            const constTransport&
        );


    // Ostream Operator

        friend Ostream& operator<< <Thermo>
        (
            Ostream&,
            const constTransport&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
inline CML::constTransport<Thermo>::constTransport
(
    const Thermo& t,
    const scalar mu,
    const scalar Pr
)
:
    Thermo(t),
    mu_(mu),
    rPr_(1.0/Pr)
{}


template<class Thermo>
inline CML::constTransport<Thermo>::constTransport
(
    const word& name,
    const constTransport& ct
)
:
    Thermo(name, ct),
    mu_(ct.mu_),
    rPr_(ct.rPr_)
{}


template<class Thermo>
inline CML::autoPtr<CML::constTransport<Thermo> >
CML::constTransport<Thermo>::clone() const
{
    return autoPtr<constTransport<Thermo> >
    (
        new constTransport<Thermo>(*this)
    );
}


template<class Thermo>
inline CML::autoPtr<CML::constTransport<Thermo> >
CML::constTransport<Thermo>::New
(
    Istream& is
)
{
    return autoPtr<constTransport<Thermo> >
    (
        new constTransport<Thermo>(is)
    );
}


template<class Thermo>
inline CML::autoPtr<CML::constTransport<Thermo> >
CML::constTransport<Thermo>::New
(
    const dictionary& dict
)
{
    return autoPtr<constTransport<Thermo> >
    (
        new constTransport<Thermo>(dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
inline CML::scalar CML::constTransport<Thermo>::mu(const scalar) const
{
    return mu_;
}


template<class Thermo>
inline CML::scalar CML::constTransport<Thermo>::kappa(const scalar T) const
{
    return this->Cp(T)*mu(T)*rPr_;
}


template<class Thermo>
inline CML::scalar CML::constTransport<Thermo>::alpha(const scalar T) const
{
    scalar Cp_ = this->Cp(T);

    scalar deltaT = T - specie::Tstd;
    scalar CpBar =
        (deltaT*(this->H(T) - this->H(specie::Tstd)) + Cp_)/(sqr(deltaT) + 1);

    return Cp_*mu(T)*rPr_/CpBar;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Thermo>
inline CML::constTransport<Thermo>& CML::constTransport<Thermo>::operator=
(
    const constTransport<Thermo>& ct
)
{
    Thermo::operator=(ct);

    mu_ = ct.mu_;
    rPr_ = ct.rPr_;

    return *this;
}


template<class Thermo>
inline void CML::constTransport<Thermo>::operator+=
(
    const constTransport<Thermo>& st
)
{
    scalar molr1 = this->nMoles();

    Thermo::operator+=(st);

    molr1 /= this->nMoles();
    scalar molr2 = st.nMoles()/this->nMoles();

    mu_ = molr1*mu_ + molr2*st.mu_;
    rPr_ = 1.0/(molr1/rPr_ + molr2/st.rPr_);
}


template<class Thermo>
inline void CML::constTransport<Thermo>::operator-=
(
    const constTransport<Thermo>& st
)
{
    scalar molr1 = this->nMoles();

    Thermo::operator-=(st);

    molr1 /= this->nMoles();
    scalar molr2 = st.nMoles()/this->nMoles();

    mu_ = molr1*mu_ - molr2*st.mu_;
    rPr_ = 1.0/(molr1/rPr_ - molr2/st.rPr_);
}


template<class Thermo>
inline void CML::constTransport<Thermo>::operator*=
(
    const scalar s
)
{
    Thermo::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Thermo>
inline CML::constTransport<Thermo> CML::operator+
(
    const constTransport<Thermo>& ct1,
    const constTransport<Thermo>& ct2
)
{
    Thermo t
    (
        static_cast<const Thermo&>(ct1) + static_cast<const Thermo&>(ct2)
    );

    scalar molr1 = ct1.nMoles()/t.nMoles();
    scalar molr2 = ct2.nMoles()/t.nMoles();

    return constTransport<Thermo>
    (
        t,
        molr1*ct1.mu_ + molr2*ct2.mu_,
        1.0/(molr1/ct1.rPr_ + molr2/ct2.rPr_)
    );
}


template<class Thermo>
inline CML::constTransport<Thermo> CML::operator-
(
    const constTransport<Thermo>& ct1,
    const constTransport<Thermo>& ct2
)
{
    Thermo t
    (
        static_cast<const Thermo&>(ct1) - static_cast<const Thermo&>(ct2)
    );

    scalar molr1 = ct1.nMoles()/t.nMoles();
    scalar molr2 = ct2.nMoles()/t.nMoles();

    return constTransport<Thermo>
    (
        t,
        molr1*ct1.mu_ - molr2*ct2.mu_,
        1.0/(molr1/ct1.rPr_ - molr2/ct2.rPr_)
    );
}


template<class Thermo>
inline CML::constTransport<Thermo> CML::operator*
(
    const scalar s,
    const constTransport<Thermo>& ct
)
{
    return constTransport<Thermo>
    (
        s*static_cast<const Thermo&>(ct),
        ct.mu_,
        1.0/ct.rPr_
    );
}


template<class Thermo>
inline CML::constTransport<Thermo> CML::operator==
(
    const constTransport<Thermo>& ct1,
    const constTransport<Thermo>& ct2
)
{
    return ct2 - ct1;
}


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
CML::constTransport<Thermo>::constTransport(Istream& is)
:
    Thermo(is),
    mu_(readScalar(is)),
    rPr_(1.0/readScalar(is))
{
    is.check("constTransport::constTransport(Istream& is)");
}


template<class Thermo>
CML::constTransport<Thermo>::constTransport(const dictionary& dict)
:
    Thermo(dict),
    mu_(readScalar(dict.subDict("transport").lookup("mu"))),
    rPr_(1.0/readScalar(dict.subDict("transport").lookup("Pr")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
// CAELUSFIX -removed ::constTransport
template<class Thermo>
void CML::constTransport<Thermo>::write(Ostream& os) const
{
    os  << this->name() << endl;
    os  << token::BEGIN_BLOCK  << incrIndent << nl;

    Thermo::write(os);

    dictionary dict("transport");
    dict.add("mu", mu_);
    dict.add("Pr", 1.0/rPr_);
    os  << indent << dict.dictName() << dict;

    os  << decrIndent << token::END_BLOCK << nl;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Thermo>
CML::Ostream& CML::operator<<(Ostream& os, const constTransport<Thermo>& ct)
{
    operator<<(os, static_cast<const Thermo&>(ct));
    os << tab << ct.mu_ << tab << 1.0/ct.rPr_;

    os.check("Ostream& operator<<(Ostream&, const constTransport&)");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
