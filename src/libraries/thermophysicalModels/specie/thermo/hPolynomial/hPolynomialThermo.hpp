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
    CML::hPolynomialThermo

Description
    Thermodynamics package templated on the equation of state, using polynomial
    functions for \c cp, \c h and \c s.

    Polynomials for \c h and \c s derived from \c cp.

Usage

    \table
        Property     | Description
        Hf           | Heat of formation
        Sf           | Standard entropy
        CpCoeffs<8>  | Specific heat at constant pressure polynomial coeffs
    \endtable

    Example of the specification of the thermodynamic properties:
    \verbatim
    thermodynamics
    {
        Hf              0;
        Sf              0;
        CpCoeffs<8>     ( 1000 -0.05 0.003 0 0 0 0 0 );
    }
    \endverbatim

    The polynomial expression is evaluated as so:

        \f[
            Cp = 1000 - 0.05 T + 0.003 T^2
        \f]

Note
    - Heat of formation is inputted in [J/kg], but internally uses [J/kmol]
    - Standard entropy is inputted in [J/kg/K], but internally uses [J/kmol/K]
    - Specific heat at constant pressure polynomial coefficients evaluate to an
      expression in [J/(kg.K)].

See also
    CML::Polynomial

\*---------------------------------------------------------------------------*/

#ifndef hPolynomialThermo_HPP
#define hPolynomialThermo_HPP

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

template<class EquationOfState, int PolySize=8>
class hPolynomialThermo
:
    public EquationOfState
{

    //- Heat of formation
    scalar Hf_;

    //- Standard entropy
    scalar Sf_;

    //- Specific heat at constant pressure polynomial coeffs
    Polynomial<PolySize> CpCoeffs_;

    //- Enthalpy polynomial coeffs - derived from cp [J/kg]
    //  NOTE: relative to Tstd
    typename Polynomial<PolySize>::intPolyType hCoeffs_;

    //- Entropy - derived from Cp [J/(kg.K)] - relative to Tstd
    Polynomial<PolySize> sCoeffs_;


    //- Construct from components
    inline hPolynomialThermo
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


public:


    //- Construct from dictionary
    hPolynomialThermo(const dictionary& dict);

    //- Construct as a named copy
    inline hPolynomialThermo(const word& name, const hPolynomialThermo& pt)
    :
        EquationOfState(name, pt),
        Hf_(pt.Hf_),
        Sf_(pt.Sf_),
        CpCoeffs_(pt.CpCoeffs_),
        hCoeffs_(pt.hCoeffs_),
        sCoeffs_(pt.sCoeffs_)
    {}


    // Member Functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "hPolynomial<" + EquationOfState::typeName() + '>';
    }

    //- Limit the temperature to be in the range Tlow_ to Thigh_
    inline scalar limit(const scalar T) const
    {
        return T;
    }

    // Fundamental properties

    //- Heat capacity at constant pressure [J/(kg K)]
    inline scalar Cp(const scalar p, const scalar T) const
    {
        return CpCoeffs_.value(T) + EquationOfState::Cp(p, T);
    }

    //- Absolute Enthalpy [J/kg]
    inline scalar Ha(const scalar p, const scalar T) const
    {
        return hCoeffs_.value(T) + EquationOfState::H(p, T);
    }

    //- Sensible enthalpy [J/kg]
    inline scalar Hs(const scalar p, const scalar T) const
    {
        return Ha(p, T) - Hc();
    }

    //- Chemical enthalpy [J/kg]
    inline scalar Hc() const
    {
        return Hf_;
    }

    //- Entropy [J/(kg K)]
    inline scalar S(const scalar p, const scalar T) const
    {
        return sCoeffs_.value(T) + EquationOfState::S(p, T);
    }

    #include "HtoEthermo.hpp"

    // Derivative term used for Jacobian

    //- Derivative of Gibbs free energy w.r.t. temperature
    inline scalar dGdT(const scalar p, const scalar T) const
    {
        return
        (
            hCoeffs_.derivative(T)
          - T*sCoeffs_.derivative(T)
          - sCoeffs_.value(T)
        );
    }

    //- Temperature derivative of heat capacity at constant pressure
    inline scalar dCpdT(const scalar p, const scalar T) const
    {
        return
        (
            CpCoeffs_.derivative(T)
        );
    }


    // I-O

    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators
    inline void operator=(const hPolynomialThermo& pt);

    inline void operator+=(const hPolynomialThermo& pt);

    inline void operator*=(const scalar s);


    // Friend operators
    friend hPolynomialThermo operator+ <EquationOfState, PolySize>
    (
        const hPolynomialThermo& pt1,
        const hPolynomialThermo& pt2
    );

    friend hPolynomialThermo operator* <EquationOfState, PolySize>
    (
        const scalar s,
        const hPolynomialThermo& pt
    );

    friend hPolynomialThermo operator== <EquationOfState, PolySize>
    (
        const hPolynomialThermo& pt1,
        const hPolynomialThermo& pt2
    );

    // Ostream Operator
    friend Ostream& operator<< <EquationOfState, PolySize>
    (
        Ostream&,
        const hPolynomialThermo&
    );
};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

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
    hCoeffs_ = CpCoeffs_.integral();
    sCoeffs_ = CpCoeffs_.integralMinus1();

    // Offset h poly so that it is relative to the enthalpy at Tstd
    hCoeffs_[0] += Hf_ - hCoeffs_.value(Tstd);

    // Offset s poly so that it is relative to the entropy at Tstd
    sCoeffs_[0] += Sf_ - sCoeffs_.value(Tstd);
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
    dict.add("Hf", Hf_);
    dict.add("Sf", Sf_);
    dict.add
    (
        word("CpCoeffs<" + CML::name(PolySize) + '>'),
        CpCoeffs_
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
    pt.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
inline void CML::hPolynomialThermo<EquationOfState, PolySize>::operator=
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
}


template<class EquationOfState, int PolySize>
inline void CML::hPolynomialThermo<EquationOfState, PolySize>::operator+=
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
    scalar Y1 = this->Y();

    EquationOfState::operator+=(pt);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        const scalar Y2 = pt.Y()/this->Y();

        Hf_ = Y1*Hf_ + Y2*pt.Hf_;
        Sf_ = Y1*Sf_ + Y2*pt.Sf_;
        CpCoeffs_ = Y1*CpCoeffs_ + Y2*pt.CpCoeffs_;
        hCoeffs_ = Y1*hCoeffs_ + Y2*pt.hCoeffs_;
        sCoeffs_ = Y1*sCoeffs_ + Y2*pt.sCoeffs_;
    }
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

    if (mag(eofs.Y()) < SMALL)
    {
        return hPolynomialThermo<EquationOfState, PolySize>
        (
            eofs,
            pt1.Hf_,
            pt1.Sf_,
            pt1.CpCoeffs_,
            pt1.hCoeffs_,
            pt1.sCoeffs_
        );
    }
    {
        const scalar Y1 = pt1.Y()/eofs.Y();
        const scalar Y2 = pt2.Y()/eofs.Y();

        return hPolynomialThermo<EquationOfState, PolySize>
        (
            eofs,
            Y1*pt1.Hf_ + Y2*pt2.Hf_,
            Y1*pt1.Sf_ + Y2*pt2.Sf_,
            Y1*pt1.CpCoeffs_ + Y2*pt2.CpCoeffs_,
            Y1*pt1.hCoeffs_ + Y2*pt2.hCoeffs_,
            Y1*pt1.sCoeffs_ + Y2*pt2.sCoeffs_
        );
    }
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
    EquationOfState eofs
    (
        static_cast<const EquationOfState&>(pt1)
     == static_cast<const EquationOfState&>(pt2)
    );

    const scalar Y1 = pt1.Y()/eofs.Y();
    const scalar Y2 = pt2.Y()/eofs.Y();

    return hPolynomialThermo<EquationOfState, PolySize>
    (
        eofs,
        Y2*pt2.Hf_       - Y1*pt1.Hf_,
        Y2*pt2.Sf_       - Y1*pt1.Sf_,
        Y2*pt2.CpCoeffs_ - Y1*pt1.CpCoeffs_,
        Y2*pt2.hCoeffs_  - Y1*pt1.hCoeffs_,
        Y2*pt2.sCoeffs_  - Y1*pt1.sCoeffs_
    );
}


#endif
