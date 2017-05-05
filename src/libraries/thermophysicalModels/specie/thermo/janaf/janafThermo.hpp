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
    CML::janafThermo

Description
    JANAF tables based thermodynamics package templated
    into the equation of state.

SourceFiles
    janafThermoI.hpp
    janafThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef janafThermo_H
#define janafThermo_H

#include "scalar.hpp"
#include "FixedList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class EquationOfState> class janafThermo;

template<class EquationOfState>
inline janafThermo<EquationOfState> operator+
(
    const janafThermo<EquationOfState>&,
    const janafThermo<EquationOfState>&
);

template<class EquationOfState>
inline janafThermo<EquationOfState> operator-
(
    const janafThermo<EquationOfState>&,
    const janafThermo<EquationOfState>&
);

template<class EquationOfState>
inline janafThermo<EquationOfState> operator*
(
    const scalar,
    const janafThermo<EquationOfState>&
);

template<class EquationOfState>
inline janafThermo<EquationOfState> operator==
(
    const janafThermo<EquationOfState>&,
    const janafThermo<EquationOfState>&
);

template<class EquationOfState>
Ostream& operator<<
(
    Ostream&,
    const janafThermo<EquationOfState>&
);


/*---------------------------------------------------------------------------*\
                           Class janafThermo Declaration
\*---------------------------------------------------------------------------*/

template<class EquationOfState>
class janafThermo
:
    public EquationOfState
{

public:

    // Public data

        static const int nCoeffs_ = 7;
        typedef FixedList<scalar, nCoeffs_> coeffArray;


private:

    // Private data

        // Temperature limits of applicability of functions
        scalar Tlow_, Thigh_, Tcommon_;

        coeffArray highCpCoeffs_;
        coeffArray lowCpCoeffs_;


    // Private Member Functions

        //- Check that input data is valid
        void checkInputData() const;

        //- Return the coefficients corresponding to the given temperature
        inline const coeffArray& coeffs(const scalar T) const;


public:

    // Constructors

        //- Construct from components
        inline janafThermo
        (
            const EquationOfState& st,
            const scalar Tlow,
            const scalar Thigh,
            const scalar Tcommon,
            const coeffArray& highCpCoeffs,
            const coeffArray& lowCpCoeffs
        );

        //- Construct from Istream
        janafThermo(Istream&);

        //- Construct from dictionary
        janafThermo(const dictionary& dict);

        //- Construct as a named copy
        inline janafThermo(const word&, const janafThermo&);


    // Member Functions

        //- Limit the temperature to be in the range Tlow_ to Thigh_
        inline scalar limit(const scalar T) const;


        // Access

            //- Return const access to the low temperature limit
            inline scalar Tlow() const;

            //- Return const access to the high temperature limit
            inline scalar Thigh() const;

            //- Return const access to the common temperature
            inline scalar Tcommon() const;

            //- Return const access to the high temperature poly coefficients
            inline const coeffArray& highCpCoeffs() const;

            //- Return const access to the low temperature poly coefficients
            inline const coeffArray& lowCpCoeffs() const;


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

        inline void operator+=(const janafThermo&);
        inline void operator-=(const janafThermo&);


    // Friend operators

        friend janafThermo operator+ <EquationOfState>
        (
            const janafThermo&,
            const janafThermo&
        );

        friend janafThermo operator- <EquationOfState>
        (
            const janafThermo&,
            const janafThermo&
        );

        friend janafThermo operator* <EquationOfState>
        (
            const scalar,
            const janafThermo&
        );

        friend janafThermo operator== <EquationOfState>
        (
            const janafThermo&,
            const janafThermo&
        );


    // Ostream Operator

        friend Ostream& operator<< <EquationOfState>
        (
            Ostream&,
            const janafThermo&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "specie.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class EquationOfState>
inline CML::janafThermo<EquationOfState>::janafThermo
(
    const EquationOfState& st,
    const scalar Tlow,
    const scalar Thigh,
    const scalar Tcommon,
    const typename janafThermo<EquationOfState>::coeffArray& highCpCoeffs,
    const typename janafThermo<EquationOfState>::coeffArray& lowCpCoeffs
)
:
    EquationOfState(st),
    Tlow_(Tlow),
    Thigh_(Thigh),
    Tcommon_(Tcommon)
{
    for (register label coefLabel=0; coefLabel<nCoeffs_; coefLabel++)
    {
        highCpCoeffs_[coefLabel] = highCpCoeffs[coefLabel];
        lowCpCoeffs_[coefLabel] = lowCpCoeffs[coefLabel];
    }
}


template<class EquationOfState>
inline const typename CML::janafThermo<EquationOfState>::coeffArray&
CML::janafThermo<EquationOfState>::coeffs
(
    const scalar T
) const
{
    if (T < Tcommon_)
    {
        return lowCpCoeffs_;
    }
    else
    {
        return highCpCoeffs_;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class EquationOfState>
inline CML::janafThermo<EquationOfState>::janafThermo
(
    const word& name,
    const janafThermo& jt
)
:
    EquationOfState(name, jt),
    Tlow_(jt.Tlow_),
    Thigh_(jt.Thigh_),
    Tcommon_(jt.Tcommon_)
{
    for (register label coefLabel=0; coefLabel<nCoeffs_; coefLabel++)
    {
        highCpCoeffs_[coefLabel] = jt.highCpCoeffs_[coefLabel];
        lowCpCoeffs_[coefLabel] = jt.lowCpCoeffs_[coefLabel];
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState>
inline CML::scalar CML::janafThermo<EquationOfState>::limit
(
    const scalar T
) const
{
    if (T < Tlow_ || T > Thigh_)
    {
        WarningIn
        (
            "janafThermo<EquationOfState>::limit(const scalar T) const"
        )   << "attempt to use janafThermo<EquationOfState>"
               " out of temperature range "
            << Tlow_ << " -> " << Thigh_ << ";  T = " << T
            << endl;

        return min(max(T, Tlow_), Thigh_);
    }
    else
    {
        return T;
    }
}


template<class EquationOfState>
inline CML::scalar CML::janafThermo<EquationOfState>::Tlow() const
{
    return Tlow_;
}


template<class EquationOfState>
inline CML::scalar CML::janafThermo<EquationOfState>::Thigh() const
{
    return Thigh_;
}


template<class EquationOfState>
inline CML::scalar CML::janafThermo<EquationOfState>::Tcommon() const
{
    return Tcommon_;
}


template<class EquationOfState>
inline const typename CML::janafThermo<EquationOfState>::coeffArray&
CML::janafThermo<EquationOfState>::highCpCoeffs() const
{
    return highCpCoeffs_;
}


template<class EquationOfState>
inline const typename CML::janafThermo<EquationOfState>::coeffArray&
CML::janafThermo<EquationOfState>::lowCpCoeffs() const
{
    return lowCpCoeffs_;
}


template<class EquationOfState>
inline CML::scalar CML::janafThermo<EquationOfState>::cp
(
    const scalar T
) const
{
    const coeffArray& a = coeffs(T);
    return this->RR*((((a[4]*T + a[3])*T + a[2])*T + a[1])*T + a[0]);
}


template<class EquationOfState>
inline CML::scalar CML::janafThermo<EquationOfState>::h
(
    const scalar T
) const
{
    const coeffArray& a = coeffs(T);
    return this->RR*
    (
        ((((a[4]/5.0*T + a[3]/4.0)*T + a[2]/3.0)*T + a[1]/2.0)*T + a[0])*T
      + a[5]
    );
}


template<class EquationOfState>
inline CML::scalar CML::janafThermo<EquationOfState>::hs
(
    const scalar T
) const
{
    return h(T) - hc();
}


template<class EquationOfState>
inline CML::scalar CML::janafThermo<EquationOfState>::hc() const
{
    const coeffArray& a = lowCpCoeffs_;
    const scalar Tstd = specie::Tstd;
    return this->RR*
    (
        (
            (((a[4]/5.0*Tstd + a[3]/4.0)*Tstd + a[2]/3.0)*Tstd + a[1]/2.0)*Tstd
          + a[0]
        )*Tstd + a[5]
    );
}


template<class EquationOfState>
inline CML::scalar CML::janafThermo<EquationOfState>::s
(
    const scalar T
) const
{
    const coeffArray& a = coeffs(T);
    return
    this->RR*
    (
        (((a[4]/4.0*T + a[3]/3.0)*T + a[2]/2.0)*T + a[1])*T + a[0]*::log(T)
      + a[6]
    );
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline void CML::janafThermo<EquationOfState>::operator+=
(
    const janafThermo<EquationOfState>& jt
)
{
    scalar molr1 = this->nMoles();

    EquationOfState::operator+=(jt);

    molr1 /= this->nMoles();
    scalar molr2 = jt.nMoles()/this->nMoles();

    Tlow_ = max(Tlow_, jt.Tlow_);
    Thigh_ = min(Thigh_, jt.Thigh_);

    if (janafThermo<EquationOfState>::debug && notEqual(Tcommon_, jt.Tcommon_))
    {
        FatalErrorIn
        (
            "janafThermo<EquationOfState>::operator+="
            "(const janafThermo<EquationOfState>& jt) const"
        )   << "Tcommon " << Tcommon_ << " for "
            << (this->name().size() ? this->name() : "others")
            << " != " << jt.Tcommon_ << " for "
            << (jt.name().size() ? jt.name() : "others")
            << exit(FatalError);
    }

    for
    (
        register label coefLabel=0;
        coefLabel<janafThermo<EquationOfState>::nCoeffs_;
        coefLabel++
    )
    {
        highCpCoeffs_[coefLabel] =
            molr1*highCpCoeffs_[coefLabel]
          + molr2*jt.highCpCoeffs_[coefLabel];

        lowCpCoeffs_[coefLabel] =
            molr1*lowCpCoeffs_[coefLabel]
          + molr2*jt.lowCpCoeffs_[coefLabel];
    }
}


template<class EquationOfState>
inline void CML::janafThermo<EquationOfState>::operator-=
(
    const janafThermo<EquationOfState>& jt
)
{
    scalar molr1 = this->nMoles();

    EquationOfState::operator-=(jt);

    molr1 /= this->nMoles();
    scalar molr2 = jt.nMoles()/this->nMoles();

    Tlow_ = max(Tlow_, jt.Tlow_);
    Thigh_ = min(Thigh_, jt.Thigh_);

    if (janafThermo<EquationOfState>::debug && notEqual(Tcommon_, jt.Tcommon_))
    {
        FatalErrorIn
        (
            "janafThermo<EquationOfState>::operator-="
            "(const janafThermo<EquationOfState>& jt) const"
        )   << "Tcommon " << Tcommon_ << " for "
            << (this->name().size() ? this->name() : "others")
            << " != " << jt.Tcommon_ << " for "
            << (jt.name().size() ? jt.name() : "others")
            << exit(FatalError);
    }

    for
    (
        register label coefLabel=0;
        coefLabel<janafThermo<EquationOfState>::nCoeffs_;
        coefLabel++
    )
    {
        highCpCoeffs_[coefLabel] =
            molr1*highCpCoeffs_[coefLabel]
          - molr2*jt.highCpCoeffs_[coefLabel];

        lowCpCoeffs_[coefLabel] =
            molr1*lowCpCoeffs_[coefLabel]
          - molr2*jt.lowCpCoeffs_[coefLabel];
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline CML::janafThermo<EquationOfState> CML::operator+
(
    const janafThermo<EquationOfState>& jt1,
    const janafThermo<EquationOfState>& jt2
)
{
    EquationOfState eofs = jt1;
    eofs += jt2;

    scalar molr1 = jt1.nMoles()/eofs.nMoles();
    scalar molr2 = jt2.nMoles()/eofs.nMoles();

    typename janafThermo<EquationOfState>::coeffArray highCpCoeffs;
    typename janafThermo<EquationOfState>::coeffArray lowCpCoeffs;

    for
    (
        register label coefLabel=0;
        coefLabel<janafThermo<EquationOfState>::nCoeffs_;
        coefLabel++
    )
    {
        highCpCoeffs[coefLabel] =
            molr1*jt1.highCpCoeffs_[coefLabel]
          + molr2*jt2.highCpCoeffs_[coefLabel];

        lowCpCoeffs[coefLabel] =
            molr1*jt1.lowCpCoeffs_[coefLabel]
          + molr2*jt2.lowCpCoeffs_[coefLabel];
    }

    if
    (
        janafThermo<EquationOfState>::debug
     && notEqual(jt1.Tcommon_, jt2.Tcommon_)
    )
    {
        FatalErrorIn
        (
            "operator+"
            "(const janafThermo<EquationOfState>& jt1,"
            " const janafThermo<EquationOfState>& jt2)"
        )   << "Tcommon " << jt1.Tcommon_ << " for "
            << (jt1.name().size() ? jt1.name() : "others")
            << " != " << jt2.Tcommon_ << " for "
            << (jt2.name().size() ? jt2.name() : "others")
            << exit(FatalError);
    }

    return janafThermo<EquationOfState>
    (
        eofs,
        max(jt1.Tlow_, jt2.Tlow_),
        min(jt1.Thigh_, jt2.Thigh_),
        jt1.Tcommon_,
        highCpCoeffs,
        lowCpCoeffs
    );
}


template<class EquationOfState>
inline CML::janafThermo<EquationOfState> CML::operator-
(
    const janafThermo<EquationOfState>& jt1,
    const janafThermo<EquationOfState>& jt2
)
{
    EquationOfState eofs = jt1;
    eofs -= jt2;

    scalar molr1 = jt1.nMoles()/eofs.nMoles();
    scalar molr2 = jt2.nMoles()/eofs.nMoles();

    typename janafThermo<EquationOfState>::coeffArray highCpCoeffs;
    typename janafThermo<EquationOfState>::coeffArray lowCpCoeffs;

    for
    (
        register label coefLabel=0;
        coefLabel<janafThermo<EquationOfState>::nCoeffs_;
        coefLabel++
    )
    {
        highCpCoeffs[coefLabel] =
            molr1*jt1.highCpCoeffs_[coefLabel]
          - molr2*jt2.highCpCoeffs_[coefLabel];

        lowCpCoeffs[coefLabel] =
            molr1*jt1.lowCpCoeffs_[coefLabel]
          - molr2*jt2.lowCpCoeffs_[coefLabel];
    }

    if
    (
        janafThermo<EquationOfState>::debug
     && notEqual(jt1.Tcommon_, jt2.Tcommon_)
    )
    {
        FatalErrorIn
        (
            "operator-"
            "(const janafThermo<EquationOfState>& jt1,"
            " const janafThermo<EquationOfState>& jt2)"
        )   << "Tcommon " << jt1.Tcommon_ << " for "
            << (jt1.name().size() ? jt1.name() : "others")
            << " != " << jt2.Tcommon_ << " for "
            << (jt2.name().size() ? jt2.name() : "others")
            << exit(FatalError);
    }

    return janafThermo<EquationOfState>
    (
        eofs,
        max(jt1.Tlow_, jt2.Tlow_),
        min(jt1.Thigh_, jt2.Thigh_),
        jt1.Tcommon_,
        highCpCoeffs,
        lowCpCoeffs
    );
}


template<class EquationOfState>
inline CML::janafThermo<EquationOfState> CML::operator*
(
    const scalar s,
    const janafThermo<EquationOfState>& jt
)
{
    return janafThermo<EquationOfState>
    (
        s*static_cast<const EquationOfState&>(jt),
        jt.Tlow_,
        jt.Thigh_,
        jt.Tcommon_,
        jt.highCpCoeffs_,
        jt.lowCpCoeffs_
    );
}


template<class EquationOfState>
inline CML::janafThermo<EquationOfState> CML::operator==
(
    const janafThermo<EquationOfState>& jt1,
    const janafThermo<EquationOfState>& jt2
)
{
    return jt2 - jt1;
}


#include "IOstreams.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class EquationOfState>
void CML::janafThermo<EquationOfState>::checkInputData() const
{
    if (Tlow_ >= Thigh_)
    {
        FatalErrorIn("janafThermo<EquationOfState>::check()")
            << "Tlow(" << Tlow_ << ") >= Thigh(" << Thigh_ << ')'
            << exit(FatalError);
    }

    if (Tcommon_ <= Tlow_)
    {
        FatalErrorIn("janafThermo<EquationOfState>::check()")
            << "Tcommon(" << Tcommon_ << ") <= Tlow(" << Tlow_ << ')'
            << exit(FatalError);
    }

    if (Tcommon_ > Thigh_)
    {
        FatalErrorIn("janafThermo<EquationOfState>::check()")
            << "Tcommon(" << Tcommon_ << ") > Thigh(" << Thigh_ << ')'
            << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class EquationOfState>
CML::janafThermo<EquationOfState>::janafThermo(Istream& is)
:
    EquationOfState(is),
    Tlow_(readScalar(is)),
    Thigh_(readScalar(is)),
    Tcommon_(readScalar(is))
{
    checkInputData();

    forAll(highCpCoeffs_, i)
    {
        is >> highCpCoeffs_[i];
    }

    forAll(lowCpCoeffs_, i)
    {
        is >> lowCpCoeffs_[i];
    }

    // Check state of Istream
    is.check("janafThermo::janafThermo(Istream& is)");
}


template<class EquationOfState>
CML::janafThermo<EquationOfState>::janafThermo(const dictionary& dict)
:
    EquationOfState(dict),
    Tlow_(readScalar(dict.subDict("thermodynamics").lookup("Tlow"))),
    Thigh_(readScalar(dict.subDict("thermodynamics").lookup("Thigh"))),
    Tcommon_(readScalar(dict.subDict("thermodynamics").lookup("Tcommon"))),
    highCpCoeffs_(dict.subDict("thermodynamics").lookup("highCpCoeffs")),
    lowCpCoeffs_(dict.subDict("thermodynamics").lookup("lowCpCoeffs"))
{
    checkInputData();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState>
void CML::janafThermo<EquationOfState>::write(Ostream& os) const
{
    EquationOfState::write(os);

    dictionary dict("thermodynamics");
    dict.add("Tlow", Tlow_);
    dict.add("Thigh", Thigh_);
    dict.add("Tcommon", Tcommon_);
    dict.add("highCpCoeffs", highCpCoeffs_);
    dict.add("lowCpCoeffs", lowCpCoeffs_);
    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class EquationOfState>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const janafThermo<EquationOfState>& jt
)
{
    os  << static_cast<const EquationOfState&>(jt) << nl
        << "    " << jt.Tlow_
        << tab << jt.Thigh_
        << tab << jt.Tcommon_;

    os << nl << "    ";

    forAll(jt.highCpCoeffs_, i)
    {
        os << jt.highCpCoeffs_[i] << ' ';
    }

    os << nl << "    ";

    forAll(jt.lowCpCoeffs_, i)
    {
        os << jt.lowCpCoeffs_[i] << ' ';
    }

    os << endl;

    os.check
    (
        "operator<<(Ostream& os, const janafThermo<EquationOfState>& jt)"
    );

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
