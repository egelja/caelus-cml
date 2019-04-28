/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
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
    CML::hPowerThermo

Description
    Power-function based thermodynamics package templated on EquationOfState.

    In this thermodynamics package the heat capacity is a simple power of
    temperature:

        Cp(T) = c0*(T/Tref)^n0;

    which is particularly suitable for solids.


\*---------------------------------------------------------------------------*/

#ifndef hPowerThermo_HPP
#define hPowerThermo_HPP

#include "scalar.hpp"
#include "specie.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class EquationOfState> class hPowerThermo;

template<class EquationOfState>
inline hPowerThermo<EquationOfState> operator+
(
    const hPowerThermo<EquationOfState>&,
    const hPowerThermo<EquationOfState>&
);

template<class EquationOfState>
inline hPowerThermo<EquationOfState> operator*
(
    const scalar,
    const hPowerThermo<EquationOfState>&
);


template<class EquationOfState>
inline hPowerThermo<EquationOfState> operator==
(
    const hPowerThermo<EquationOfState>&,
    const hPowerThermo<EquationOfState>&
);


template<class EquationOfState>
Ostream& operator<<
(
    Ostream&,
    const hPowerThermo<EquationOfState>&
);


/*---------------------------------------------------------------------------*\
                         Class hPowerThermo Declaration
\*---------------------------------------------------------------------------*/

template<class EquationOfState>
class hPowerThermo
:
    public EquationOfState
{

    scalar c0_;
    scalar n0_;
    scalar Tref_;
    scalar Hf_;


    //- Check given temperature is within the range of the fitted coeffs
    inline void checkT(const scalar T) const
    {
        if (T < 0)
        {
            FatalErrorInFunction
                << "attempt to evaluate hPowerThermo<EquationOfState>"
                   " for negative temperature " << T
                << abort(FatalError);
        }
    }

    //- Construct from components
    inline hPowerThermo
    (
        const EquationOfState& st,
        const scalar c0,
        const scalar n0,
        const scalar Tref,
        const scalar Hf
    )
    :
        EquationOfState(st),
        c0_(c0),
        n0_(n0),
        Tref_(Tref),
        Hf_(Hf)
    {}



public:


    //- Construct from dictionary
    hPowerThermo(const dictionary&);

    //- Construct as a named copy
    inline hPowerThermo
    (
        const word& name,
        const hPowerThermo& jt
    )
    :
        EquationOfState(name, jt),
        c0_(jt.c0_),
        n0_(jt.n0_),
        Tref_(jt.Tref_),
        Hf_(jt.Hf_)
    {}


    //- Construct and return a clone
    inline autoPtr<hPowerThermo> clone() const
    {
        return autoPtr<hPowerThermo<EquationOfState> >
        (
            new hPowerThermo<EquationOfState>(*this)
        );
    }

    //- Selector from dictionary
    inline static autoPtr<hPowerThermo> New(const dictionary& dict)
    {
        return autoPtr<hPowerThermo<EquationOfState> >
        (
            new hPowerThermo<EquationOfState>(dict)
        );
    }


    // Member Functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "hPower<" + EquationOfState::typeName() + '>';
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
        return c0_*pow(T/Tref_, n0_) + EquationOfState::Cp(p, T);
    }

    //- Absolute Enthalpy [J/kg]
    inline scalar Ha(const scalar p, const scalar T) const
    {
        return Hs(p, T) + Hc();
    }

    //- Sensible enthalpy [J/kg]
    inline scalar Hs(const scalar p, const scalar T) const
    {
        return
            c0_*(pow(T, n0_ + 1) - pow(Tstd, n0_ + 1))/(pow(Tref_, n0_)*(n0_ + 1))
          + EquationOfState::H(p, T);
    }

    //- Chemical enthalpy [J/kg]
    inline scalar Hc() const
    {
        return Hf_;
    }

    //- Entropy [J/(kg K)]
    inline scalar S(const scalar p, const scalar T) const
    {
        return
            c0_*(pow(T, n0_) - pow(Tstd, n0_))/(pow(Tref_, n0_)*n0_)
          + EquationOfState::S(p, T);
    }

    #include "HtoEthermo.hpp"

    // Derivative term used for Jacobian

    //- Derivative of Gibbs free energy w.r.t. temperature
    inline scalar dGdT(const scalar p, const scalar T) const
    {
        // To be implemented
        NotImplemented;
        return 0;
    }

    //- Temperature derivative of heat capacity at constant pressure
    inline scalar dCpdT(const scalar p, const scalar T) const
    {
        // To be implemented
        NotImplemented;
        return 0;
    }


    // Member operators
    inline void operator+=(const hPowerThermo& ct);


    // Friend operators
    friend hPowerThermo operator+ <EquationOfState>
    (
        const hPowerThermo& ct1,
        const hPowerThermo& ct2
    );

    friend hPowerThermo operator* <EquationOfState>
    (
        const scalar s,
        const hPowerThermo& ct
    );

    friend hPowerThermo operator== <EquationOfState>
    (
        const hPowerThermo& ct1,
        const hPowerThermo& ct2
    );

    // Ostream Operator
    friend Ostream& operator<< <EquationOfState>
    (
        Ostream&,
        const hPowerThermo&
    );
};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class EquationOfState>
CML::hPowerThermo<EquationOfState>::hPowerThermo
(
    const dictionary& dict
)
:
    EquationOfState(dict),
    c0_(readScalar(dict.subDict("thermodynamics").lookup("C0"))),
    n0_(readScalar(dict.subDict("thermodynamics").lookup("n0"))),
    Tref_(readScalar(dict.subDict("thermodynamics").lookup("Tref"))),
    Hf_(readScalar(dict.subDict("thermodynamics").lookup("Hf")))
{}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class EquationOfState>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const hPowerThermo<EquationOfState>& et
)
{
    et.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline void CML::hPowerThermo<EquationOfState>::operator+=
(
    const hPowerThermo<EquationOfState>& ct
)
{
    scalar Y1 = this->Y();

    EquationOfState::operator+=(ct);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        const scalar Y2 = ct.Y()/this->Y();

        Hf_ = Y1*Hf_ + Y2*ct.Hf_;
        c0_ = Y1*c0_ + Y2*ct.c0_;
        n0_ = Y1*n0_ + Y2*ct.n0_;
        Tref_ = Y1*Tref_ + Y2*ct.Tref_;
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline CML::hPowerThermo<EquationOfState> CML::operator+
(
    const hPowerThermo<EquationOfState>& ct1,
    const hPowerThermo<EquationOfState>& ct2
)
{
    EquationOfState eofs
    (
        static_cast<const EquationOfState&>(ct1)
      + static_cast<const EquationOfState&>(ct2)
    );

    if (mag(eofs.Y()) < SMALL)
    {
        return hPowerThermo<EquationOfState>
        (
            eofs,
            ct1.c0_,
            ct1.n0_,
            ct1.Tref_,
            ct1.Hf_
        );
    }
    else
    {
        return hPowerThermo<EquationOfState>
        (
            eofs,
            ct1.Y()/eofs.Y()*ct1.c0_
          + ct2.Y()/eofs.Y()*ct2.c0_,
            ct1.Y()/eofs.Y()*ct1.n0_
          + ct2.Y()/eofs.Y()*ct2.n0_,
            ct1.Y()/eofs.Y()*ct1.Tref_
          + ct2.Y()/eofs.Y()*ct2.Tref_,
            ct1.Y()/eofs.Y()*ct1.Hf_
          + ct2.Y()/eofs.Y()*ct2.Hf_
        );
    }
}


template<class EquationOfState>
inline CML::hPowerThermo<EquationOfState> CML::operator*
(
    const scalar s,
    const hPowerThermo<EquationOfState>& ct
)
{
    return hPowerThermo<EquationOfState>
    (
        s*static_cast<const EquationOfState&>(ct),
        ct.c0_,
        ct.n0_,
        ct.Tref_,
        ct.Hf_
    );
}


template<class EquationOfState>
inline CML::hPowerThermo<EquationOfState> CML::operator==
(
    const hPowerThermo<EquationOfState>& ct1,
    const hPowerThermo<EquationOfState>& ct2
)
{
    EquationOfState eofs
    (
        static_cast<const EquationOfState&>(ct1)
     == static_cast<const EquationOfState&>(ct2)
    );

    return hPowerThermo<EquationOfState>
    (
        eofs,
        ct2.Y()/eofs.Y()*ct2.c0_
      - ct1.Y()/eofs.Y()*ct1.c0_,
        ct2.Y()/eofs.Y()*ct2.n0_
      - ct1.Y()/eofs.Y()*ct1.n0_,
        ct2.Y()/eofs.Y()*ct2.Tref_
      - ct1.Y()/eofs.Y()*ct1.Tref_,
        ct2.Y()/eofs.Y()*ct2.Hf_
      - ct1.Y()/eofs.Y()*ct1.Hf_
    );
}


#endif
