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
    CML::hPolynomialThermo

Description
    Thermodynamics package templated on the equation of state, using polynomial
    functions for cp, h and s

    Polynomials for h and s derived from cp

SourceFiles
    hPolynomialThermoI.hpp
    hPolynomialThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef hPolynomialThermo_H
#define hPolynomialThermo_H

#include "scalar.hpp"
#include "Polynomial_.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class EquationOfState, int PolySize>
class hPolynomialThermo;

template<class EquationOfState, int PolySize>
inline hPolynomialThermo<EquationOfState, PolySize> operator+
(
    const hPolynomialThermo<EquationOfState, PolySize>&,
    const hPolynomialThermo<EquationOfState, PolySize>&
);

template<class EquationOfState, int PolySize>
inline hPolynomialThermo<EquationOfState, PolySize> operator-
(
    const hPolynomialThermo<EquationOfState, PolySize>&,
    const hPolynomialThermo<EquationOfState, PolySize>&
);

template<class EquationOfState, int PolySize>
inline hPolynomialThermo<EquationOfState, PolySize> operator*
(
    const scalar,
    const hPolynomialThermo<EquationOfState, PolySize>&
);

template<class EquationOfState, int PolySize>
inline hPolynomialThermo<EquationOfState, PolySize> operator==
(
    const hPolynomialThermo<EquationOfState, PolySize>&,
    const hPolynomialThermo<EquationOfState, PolySize>&
);

template<class EquationOfState, int PolySize>
Ostream& operator<<
(
    Ostream&,
    const hPolynomialThermo<EquationOfState, PolySize>&
);


/*---------------------------------------------------------------------------*\
                      Class hPolynomialThermo Declaration
\*---------------------------------------------------------------------------*/

template<class EquationOfState, int PolySize>
class hPolynomialThermo
:
    public EquationOfState
{
    // Private data

        //- Heat of formation
        //  Note: input in [J/kg], but internally uses [J/kmol]
        scalar Hf_;

        //- Standard entropy
        //  Note: input in [J/kg/K], but internally uses [J/kmol/K]
        scalar Sf_;

        //- Specific heat at constant pressure polynomial coeffs [J/(kg.K)]
        Polynomial<PolySize> CpCoeffs_;

        //- Enthalpy polynomial coeffs - derived from cp [J/kg]
        //  NOTE: relative to Tstd
        typename Polynomial<PolySize>::intPolyType hCoeffs_;

        //- Entropy - derived from Cp [J/(kg.K)] - relative to Tstd
        Polynomial<PolySize> sCoeffs_;


    // Private Member Functions

        //- Construct from components
        inline hPolynomialThermo
        (
            const EquationOfState& pt,
            const scalar Hf,
            const scalar Sf,
            const Polynomial<PolySize>& CpCoeffs,
            const typename Polynomial<PolySize>::intPolyType& hCoeffs,
            const Polynomial<PolySize>& sCoeffs
        );


public:

    // Constructors

        //- Construct from Istream
        hPolynomialThermo(Istream& is);

        //- Construct from dictionary
        hPolynomialThermo(const dictionary& dict);

        //- Construct as copy
        inline hPolynomialThermo(const hPolynomialThermo&);

        //- Construct as a named copy
        inline hPolynomialThermo(const word&, const hPolynomialThermo&);


    // Member Functions

        //- Limit the temperature to be in the range Tlow_ to Thigh_
        inline scalar limit(const scalar T) const;

        // Fundamental properties

            //- Heat capacity at constant pressure [J/(kmol K)]
            inline scalar cp(const scalar T) const;

            //- Enthalpy [J/kmol]
            inline scalar h(const scalar T) const;

            //- Sensible enthalpy [J/kmol]
            inline scalar hs(const scalar T) const;

            //- Chemical enthalpy [J/kmol]
            inline scalar hc() const;

            //- Entropy [J/(kmol K)]
            inline scalar s(const scalar T) const;


        // I-O

            //- Write to Ostream
            void write(Ostream& os) const;


    // Member operators

        inline hPolynomialThermo& operator=(const hPolynomialThermo&);
        inline void operator+=(const hPolynomialThermo&);
        inline void operator-=(const hPolynomialThermo&);
        inline void operator*=(const scalar);


    // Friend operators

        friend hPolynomialThermo operator+ <EquationOfState, PolySize>
        (
            const hPolynomialThermo&,
            const hPolynomialThermo&
        );

        friend hPolynomialThermo operator- <EquationOfState, PolySize>
        (
            const hPolynomialThermo&,
            const hPolynomialThermo&
        );

        friend hPolynomialThermo operator* <EquationOfState, PolySize>
        (
            const scalar,
            const hPolynomialThermo&
        );

        friend hPolynomialThermo operator== <EquationOfState, PolySize>
        (
            const hPolynomialThermo&,
            const hPolynomialThermo&
        );


    // Ostream Operator

        friend Ostream& operator<< <EquationOfState, PolySize>
        (
            Ostream&,
            const hPolynomialThermo&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
inline CML::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
    const EquationOfState& pt,
    const scalar Hf,
    const scalar Sf,
    const Polynomial<PolySize>& CpCoeffs,
    const typename Polynomial<PolySize>::intPolyType& hCoeffs,
    const Polynomial<PolySize>& sCoeffs
)
:
    EquationOfState(pt),
    Hf_(Hf),
    Sf_(Sf),
    CpCoeffs_(CpCoeffs),
    hCoeffs_(hCoeffs),
    sCoeffs_(sCoeffs)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
inline CML::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
    const hPolynomialThermo& pt
)
:
    EquationOfState(pt),
    Hf_(pt.Hf_),
    Sf_(pt.Sf_),
    CpCoeffs_(pt.CpCoeffs_),
    hCoeffs_(pt.hCoeffs_),
    sCoeffs_(pt.sCoeffs_)
{}


template<class EquationOfState, int PolySize>
inline CML::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
    const word& name,
    const hPolynomialThermo& pt
)
:
    EquationOfState(name, pt),
    Hf_(pt.Hf_),
    Sf_(pt.Sf_),
    CpCoeffs_(pt.CpCoeffs_),
    hCoeffs_(pt.hCoeffs_),
    sCoeffs_(pt.sCoeffs_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
inline CML::scalar CML::hPolynomialThermo<EquationOfState, PolySize>::limit
(
    const scalar T
) const
{
    return T;
}


template<class EquationOfState, int PolySize>
inline CML::scalar CML::hPolynomialThermo<EquationOfState, PolySize>::cp
(
    const scalar T
) const
{
    return CpCoeffs_.value(T);
}


template<class EquationOfState, int PolySize>
inline CML::scalar CML::hPolynomialThermo<EquationOfState, PolySize>::h
(
    const scalar T
) const
{
    return hCoeffs_.value(T);
}


template<class EquationOfState, int PolySize>
inline CML::scalar CML::hPolynomialThermo<EquationOfState, PolySize>::hs
(
    const scalar T
) const
{
    return h(T) - hc();
}


template<class EquationOfState, int PolySize>
inline CML::scalar CML::hPolynomialThermo<EquationOfState, PolySize>::hc()
const
{
    return Hf_;
}


template<class EquationOfState, int PolySize>
inline CML::scalar CML::hPolynomialThermo<EquationOfState, PolySize>::s
(
    const scalar T
) const
{
    return sCoeffs_.value(T);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
inline CML::hPolynomialThermo<EquationOfState, PolySize>&
CML::hPolynomialThermo<EquationOfState, PolySize>::operator=
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
    EquationOfState::operator=(pt);

    Hf_ = pt.Hf_;
    Sf_ = pt.Sf_;
    CpCoeffs_ = pt.CpCoeffs_;
    hCoeffs_ = pt.hCoeffs_;
    sCoeffs_ = pt.sCoeffs_;

    return *this;
}


template<class EquationOfState, int PolySize>
inline void CML::hPolynomialThermo<EquationOfState, PolySize>::operator+=
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
    scalar molr1 = this->nMoles();

    EquationOfState::operator+=(pt);

    molr1 /= this->nMoles();
    scalar molr2 = pt.nMoles()/this->nMoles();

    Hf_ = molr1*Hf_ + molr2*pt.Hf_;
    Sf_ = molr1*Sf_ + molr2*pt.Sf_;
    CpCoeffs_ = molr1*CpCoeffs_ + molr2*pt.CpCoeffs_;
    hCoeffs_ = molr1*hCoeffs_ + molr2*pt.hCoeffs_;
    sCoeffs_ = molr1*sCoeffs_ + molr2*pt.sCoeffs_;
}


template<class EquationOfState, int PolySize>
inline void CML::hPolynomialThermo<EquationOfState, PolySize>::operator-=
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
    scalar molr1 = this->nMoles();

    EquationOfState::operator-=(pt);

    molr1 /= this->nMoles();
    scalar molr2 = pt.nMoles()/this->nMoles();

    Hf_ = molr1*Hf_ - molr2*pt.Hf_;
    Sf_ = molr1*Sf_ - molr2*pt.Sf_;
    CpCoeffs_ = molr1*CpCoeffs_ - molr2*pt.CpCoeffs_;
    hCoeffs_ = molr1*hCoeffs_ - molr2*pt.hCoeffs_;
    sCoeffs_ = molr1*sCoeffs_ - molr2*pt.sCoeffs_;
}


template<class EquationOfState, int PolySize>
inline void CML::hPolynomialThermo<EquationOfState, PolySize>::operator*=
(
    const scalar s
)
{
    EquationOfState::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
inline CML::hPolynomialThermo<EquationOfState, PolySize> CML::operator+
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt1,
    const hPolynomialThermo<EquationOfState, PolySize>& pt2
)
{
    EquationOfState eofs = pt1;
    eofs += pt2;

    scalar molr1 = pt1.nMoles()/eofs.nMoles();
    scalar molr2 = pt2.nMoles()/eofs.nMoles();

    return hPolynomialThermo<EquationOfState, PolySize>
    (
        eofs,
        molr1*pt1.Hf_ + molr2*pt2.Hf_,
        molr1*pt1.Sf_ + molr2*pt2.Sf_,
        molr1*pt1.CpCoeffs_ + molr2*pt2.CpCoeffs_,
        molr1*pt1.hCoeffs_ + molr2*pt2.hCoeffs_,
        molr1*pt1.sCoeffs_ + molr2*pt2.sCoeffs_
    );
}


template<class EquationOfState, int PolySize>
inline CML::hPolynomialThermo<EquationOfState, PolySize> CML::operator-
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt1,
    const hPolynomialThermo<EquationOfState, PolySize>& pt2
)
{
    EquationOfState eofs = pt1;
    eofs -= pt2;

    scalar molr1 = pt1.nMoles()/eofs.nMoles();
    scalar molr2 = pt2.nMoles()/eofs.nMoles();

    return hPolynomialThermo<EquationOfState, PolySize>
    (
        eofs,
        molr1*pt1.Hf_ - molr2*pt2.Hf_,
        molr1*pt1.Sf_ - molr2*pt2.Sf_,
        molr1*pt1.CpCoeffs_ - molr2*pt2.CpCoeffs_,
        molr1*pt1.hCoeffs_ - molr2*pt2.hCoeffs_,
        molr1*pt1.sCoeffs_ - molr2*pt2.sCoeffs_
    );
}


template<class EquationOfState, int PolySize>
inline CML::hPolynomialThermo<EquationOfState, PolySize> CML::operator*
(
    const scalar s,
    const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
    return hPolynomialThermo<EquationOfState, PolySize>
    (
        s*static_cast<const EquationOfState&>(pt),
        pt.Hf_,
        pt.Sf_,
        pt.CpCoeffs_,
        pt.hCoeffs_,
        pt.sCoeffs_
    );
}


template<class EquationOfState, int PolySize>
inline CML::hPolynomialThermo<EquationOfState, PolySize> CML::operator==
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt1,
    const hPolynomialThermo<EquationOfState, PolySize>& pt2
)
{
    return pt2 - pt1;
}


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
CML::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
    Istream& is
)
:
    EquationOfState(is),
    Hf_(readScalar(is)),
    Sf_(readScalar(is)),
    CpCoeffs_("CpCoeffs<" + CML::name(PolySize) + '>', is),
    hCoeffs_(),
    sCoeffs_()
{
    Hf_ *= this->W();
    Sf_ *= this->W();
    CpCoeffs_ *= this->W();

    hCoeffs_ = CpCoeffs_.integral();
    sCoeffs_ = CpCoeffs_.integralMinus1();

    // Offset h poly so that it is relative to the enthalpy at Tstd
    hCoeffs_[0] += Hf_ - hCoeffs_.value(specie::Tstd);

    // Offset s poly so that it is relative to the entropy at Tstd
    sCoeffs_[0] += Sf_ - sCoeffs_.value(specie::Tstd);
}


template<class EquationOfState, int PolySize>
CML::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
    const dictionary& dict
)
:
    EquationOfState(dict),
    Hf_(readScalar(dict.subDict("thermodynamics").lookup("Hf"))),
    Sf_(readScalar(dict.subDict("thermodynamics").lookup("Sf"))),
    CpCoeffs_
    (
        dict.subDict("thermodynamics").lookup
        (
            "CpCoeffs<" + CML::name(PolySize) + '>'
        )
    ),
    hCoeffs_(),
    sCoeffs_()
{
    Hf_ *= this->W();
    Sf_ *= this->W();
    CpCoeffs_ *= this->W();

    hCoeffs_ = CpCoeffs_.integral();
    sCoeffs_ = CpCoeffs_.integralMinus1();

    // Offset h poly so that it is relative to the enthalpy at Tstd
    hCoeffs_[0] += Hf_ - hCoeffs_.value(specie::Tstd);

    // Offset s poly so that it is relative to the entropy at Tstd
    sCoeffs_[0] += Sf_ - sCoeffs_.value(specie::Tstd);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
void CML::hPolynomialThermo<EquationOfState, PolySize>::write
(
    Ostream& os
) const
{
    EquationOfState::write(os);

    dictionary dict("thermodynamics");
    dict.add("Hf", Hf_/this->W());
    dict.add("Sf", Sf_/this->W());
    dict.add
    (
        word("CpCoeffs<" + CML::name(PolySize) + '>'),
        CpCoeffs_/this->W()
    );
    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
    os  << static_cast<const EquationOfState&>(pt) << tab
        << pt.Hf_/pt.W() << tab
        << pt.Sf_/pt.W() << tab
        << "CpCoeffs<" << CML::name(PolySize) << '>' << tab
        << pt.CpCoeffs_/pt.W();

    os.check
    (
        "operator<<"
        "("
            "Ostream&, "
            "const hPolynomialThermo<EquationOfState, PolySize>&"
        ")"
    );

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
