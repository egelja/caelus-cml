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
    CML::janafThermo

Description
    JANAF tables based thermodynamics package templated
    into the equation of state.


\*---------------------------------------------------------------------------*/

#ifndef janafThermo_HPP
#define janafThermo_HPP

#include "scalar.hpp"
#include "FixedList.hpp"
#include "specie.hpp"

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

    static const int nCoeffs_ = 7;
    typedef FixedList<scalar, nCoeffs_> coeffArray;


private:

    // Temperature limits of applicability of functions
    scalar Tlow_, Thigh_, Tcommon_;

    coeffArray highCpCoeffs_;
    coeffArray lowCpCoeffs_;


    //- Check that input data is valid
    void checkInputData() const;

    //- Return the coefficients corresponding to the given temperature
    inline const coeffArray& coeffs(const scalar T) const
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


public:


    //- Construct from components
    inline janafThermo
    (
        const EquationOfState& st,
        const scalar Tlow,
        const scalar Thigh,
        const scalar Tcommon,
        const coeffArray& highCpCoeffs,
        const coeffArray& lowCpCoeffs,
        const bool convertCoeffs = false
    )
    :
        EquationOfState(st),
        Tlow_(Tlow),
        Thigh_(Thigh),
        Tcommon_(Tcommon)
    {
        if (convertCoeffs)
        {
            for (label coefLabel=0; coefLabel<nCoeffs_; coefLabel++)
            {
                highCpCoeffs_[coefLabel] = highCpCoeffs[coefLabel]*this->R();
                lowCpCoeffs_[coefLabel] = lowCpCoeffs[coefLabel]*this->R();
            }
        }
        else
        {
            for (label coefLabel=0; coefLabel<nCoeffs_; coefLabel++)
            {
                highCpCoeffs_[coefLabel] = highCpCoeffs[coefLabel];
                lowCpCoeffs_[coefLabel] = lowCpCoeffs[coefLabel];
            }
        }
    }

    //- Construct from dictionary
    janafThermo(const dictionary& dict);

    //- Construct as a named copy
    inline janafThermo(const word& name, const janafThermo& jt)
    :
        EquationOfState(name, jt),
        Tlow_(jt.Tlow_),
        Thigh_(jt.Thigh_),
        Tcommon_(jt.Tcommon_)
    {
        for (label coefLabel=0; coefLabel<nCoeffs_; coefLabel++)
        {
            highCpCoeffs_[coefLabel] = jt.highCpCoeffs_[coefLabel];
            lowCpCoeffs_[coefLabel] = jt.lowCpCoeffs_[coefLabel];
        }
    }


    // Member Functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "janaf<" + EquationOfState::typeName() + '>';
    }

    //- Limit the temperature to be in the range Tlow_ to Thigh_
    inline scalar limit(const scalar T) const
    {
        if (T < Tlow_ || T > Thigh_)
        {
            WarningInFunction
                << "attempt to use janafThermo<EquationOfState>"
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

    // Access

    //- Return const access to the low temperature limit
    inline scalar Tlow() const
    {
        return Tlow_;
    }

    //- Return const access to the high temperature limit
    inline scalar Thigh() const
    {
        return Thigh_;
    }

    //- Return const access to the common temperature
    inline scalar Tcommon() const
    {
        return Tcommon_;
    }

    //- Return const access to the high temperature poly coefficients
    inline const coeffArray& highCpCoeffs() const
    {
        return highCpCoeffs_;
    }

    //- Return const access to the low temperature poly coefficients
    inline const coeffArray& lowCpCoeffs() const
    {
        return lowCpCoeffs_;
    }


    // Fundamental properties

    //- Heat capacity at constant pressure [J/(kg K)]
    inline scalar Cp(const scalar p, const scalar T) const
    {
        const coeffArray& a = coeffs(T);
        return
            ((((a[4]*T + a[3])*T + a[2])*T + a[1])*T + a[0])
          + EquationOfState::Cp(p, T);
    }

    //- Absolute Enthalpy [J/kg]
    inline scalar Ha(const scalar p, const scalar T) const
    {
        const coeffArray& a = coeffs(T);
        return
        (
            ((((a[4]/5.0*T + a[3]/4.0)*T + a[2]/3.0)*T + a[1]/2.0)*T + a[0])*T
          + a[5]
        ) + EquationOfState::H(p, T);
    }

    //- Sensible enthalpy [J/kg]
    inline scalar Hs(const scalar p, const scalar T) const
    {
        return Ha(p, T) - Hc();
    }

    //- Chemical enthalpy [J/kg]
    inline scalar Hc() const
    {
        const coeffArray& a = lowCpCoeffs_;
        return
        (
            (
                (((a[4]/5.0*Tstd + a[3]/4.0)*Tstd + a[2]/3.0)*Tstd + a[1]/2.0)*Tstd
              + a[0]
            )*Tstd + a[5]
        );
    }

    //- Entropy [J/(kg K)]
    inline scalar S(const scalar p, const scalar T) const
    {
        const coeffArray& a = coeffs(T);
        return
        (
            (((a[4]/4.0*T + a[3]/3.0)*T + a[2]/2.0)*T + a[1])*T + a[0]*log(T)
          + a[6]
        ) + EquationOfState::S(p, T);
    }

    #include "HtoEthermo.hpp"

    // Derivative term used for Jacobian

    //- Derivative of Gibbs free energy w.r.t. temperature
    inline scalar dGdT(const scalar p, const scalar T) const
    {
        const coeffArray& a = coeffs(T);
        return -((a[0] + a[5]/T)/T + a[1]/2 + T*(a[2]/3 + T*(a[3]/4 + T*a[4]/5)));
    }

    //- Temperature derivative of heat capacity at constant pressure
    inline scalar dCpdT(const scalar p, const scalar T) const
    {
        const coeffArray& a = coeffs(T);
        return
            (((4*a[4]*T + 3*a[3])*T + 2*a[2])*T + a[1]);
    }


    // I-O

    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators
    inline void operator+=(const janafThermo& jt);

    // Friend operators
    friend janafThermo operator+ <EquationOfState>
    (
        const janafThermo& jt1,
        const janafThermo& jt2
    );

    friend janafThermo operator* <EquationOfState>
    (
        const scalar s,
        const janafThermo& jt
    );

    friend janafThermo operator== <EquationOfState>
    (
        const janafThermo& jt1,
        const janafThermo& jt2
    );

    // Ostream Operator

    friend Ostream& operator<< <EquationOfState>
    (
        Ostream&,
        const janafThermo&
    );
};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class EquationOfState>
void CML::janafThermo<EquationOfState>::checkInputData() const
{
    if (Tlow_ >= Thigh_)
    {
        FatalErrorInFunction
            << "Tlow(" << Tlow_ << ") >= Thigh(" << Thigh_ << ')'
            << exit(FatalError);
    }

    if (Tcommon_ <= Tlow_)
    {
        FatalErrorInFunction
            << "Tcommon(" << Tcommon_ << ") <= Tlow(" << Tlow_ << ')'
            << exit(FatalError);
    }

    if (Tcommon_ > Thigh_)
    {
        FatalErrorInFunction
            << "Tcommon(" << Tcommon_ << ") > Thigh(" << Thigh_ << ')'
            << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

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
    // Convert coefficients to mass-basis
    for (label coefLabel=0; coefLabel<nCoeffs_; coefLabel++)
    {
        highCpCoeffs_[coefLabel] *= this->R();
        lowCpCoeffs_[coefLabel] *= this->R();
    }

    checkInputData();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState>
void CML::janafThermo<EquationOfState>::write(Ostream& os) const
{
    EquationOfState::write(os);

    // Convert coefficients back to dimensionless form
    coeffArray highCpCoeffs;
    coeffArray lowCpCoeffs;
    for (label coefLabel=0; coefLabel<nCoeffs_; coefLabel++)
    {
        highCpCoeffs[coefLabel] = highCpCoeffs_[coefLabel]/this->R();
        lowCpCoeffs[coefLabel] = lowCpCoeffs_[coefLabel]/this->R();
    }

    dictionary dict("thermodynamics");
    dict.add("Tlow", Tlow_);
    dict.add("Thigh", Thigh_);
    dict.add("Tcommon", Tcommon_);
    dict.add("highCpCoeffs", highCpCoeffs);
    dict.add("lowCpCoeffs", lowCpCoeffs);
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
    jt.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline void CML::janafThermo<EquationOfState>::operator+=
(
    const janafThermo<EquationOfState>& jt
)
{
    scalar Y1 = this->Y();

    EquationOfState::operator+=(jt);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        const scalar Y2 = jt.Y()/this->Y();

        Tlow_ = max(Tlow_, jt.Tlow_);
        Thigh_ = min(Thigh_, jt.Thigh_);

        if
        (
            janafThermo<EquationOfState>::debug
         && notEqual(Tcommon_, jt.Tcommon_)
        )
        {
            FatalErrorInFunction
                << "Tcommon " << Tcommon_ << " for "
                << (this->name().size() ? this->name() : "others")
                << " != " << jt.Tcommon_ << " for "
                << (jt.name().size() ? jt.name() : "others")
                << exit(FatalError);
        }

        for
        (
            label coefLabel=0;
            coefLabel<janafThermo<EquationOfState>::nCoeffs_;
            coefLabel++
        )
        {
            highCpCoeffs_[coefLabel] =
                Y1*highCpCoeffs_[coefLabel]
              + Y2*jt.highCpCoeffs_[coefLabel];

            lowCpCoeffs_[coefLabel] =
                Y1*lowCpCoeffs_[coefLabel]
              + Y2*jt.lowCpCoeffs_[coefLabel];
        }
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

    if (mag(eofs.Y()) < SMALL)
    {
        return janafThermo<EquationOfState>
        (
            eofs,
            jt1.Tlow_,
            jt1.Thigh_,
            jt1.Tcommon_,
            jt1.highCpCoeffs_,
            jt1.lowCpCoeffs_
        );
    }
    else
    {
        const scalar Y1 = jt1.Y()/eofs.Y();
        const scalar Y2 = jt2.Y()/eofs.Y();

        typename janafThermo<EquationOfState>::coeffArray highCpCoeffs;
        typename janafThermo<EquationOfState>::coeffArray lowCpCoeffs;

        for
        (
            label coefLabel=0;
            coefLabel<janafThermo<EquationOfState>::nCoeffs_;
            coefLabel++
        )
        {
            highCpCoeffs[coefLabel] =
                Y1*jt1.highCpCoeffs_[coefLabel]
              + Y2*jt2.highCpCoeffs_[coefLabel];

            lowCpCoeffs[coefLabel] =
                Y1*jt1.lowCpCoeffs_[coefLabel]
              + Y2*jt2.lowCpCoeffs_[coefLabel];
        }

        if
        (
            janafThermo<EquationOfState>::debug
         && notEqual(jt1.Tcommon_, jt2.Tcommon_)
        )
        {
            FatalErrorInFunction
                << "Tcommon " << jt1.Tcommon_ << " for "
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
    EquationOfState eofs
    (
        static_cast<const EquationOfState&>(jt1)
     == static_cast<const EquationOfState&>(jt2)
    );

    const scalar Y1 = jt2.Y()/eofs.Y();
    const scalar Y2 = jt1.Y()/eofs.Y();

    typename janafThermo<EquationOfState>::coeffArray highCpCoeffs;
    typename janafThermo<EquationOfState>::coeffArray lowCpCoeffs;

    for
    (
        label coefLabel=0;
        coefLabel<janafThermo<EquationOfState>::nCoeffs_;
        coefLabel++
    )
    {
        highCpCoeffs[coefLabel] =
            Y1*jt2.highCpCoeffs_[coefLabel]
          - Y2*jt1.highCpCoeffs_[coefLabel];

        lowCpCoeffs[coefLabel] =
            Y1*jt2.lowCpCoeffs_[coefLabel]
          - Y2*jt1.lowCpCoeffs_[coefLabel];
    }

    if
    (
        janafThermo<EquationOfState>::debug
     && notEqual(jt2.Tcommon_, jt1.Tcommon_)
    )
    {
        FatalErrorInFunction
            << "Tcommon " << jt2.Tcommon_ << " for "
            << (jt2.name().size() ? jt2.name() : "others")
            << " != " << jt1.Tcommon_ << " for "
            << (jt1.name().size() ? jt1.name() : "others")
            << exit(FatalError);
    }

    return janafThermo<EquationOfState>
    (
        eofs,
        max(jt2.Tlow_, jt1.Tlow_),
        min(jt2.Thigh_, jt1.Thigh_),
        jt2.Tcommon_,
        highCpCoeffs,
        lowCpCoeffs
    );
}


#endif
