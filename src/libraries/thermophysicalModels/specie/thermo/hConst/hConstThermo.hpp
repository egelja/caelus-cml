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
    CML::hConstThermo

Description
    Constant properties thermodynamics package
    templated into the EquationOfState.


\*---------------------------------------------------------------------------*/

#ifndef hConstThermo_HPP
#define hConstThermo_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class EquationOfState> class hConstThermo;

template<class EquationOfState>
inline hConstThermo<EquationOfState> operator+
(
    const hConstThermo<EquationOfState>&,
    const hConstThermo<EquationOfState>&
);

template<class EquationOfState>
inline hConstThermo<EquationOfState> operator*
(
    const scalar,
    const hConstThermo<EquationOfState>&
);

template<class EquationOfState>
inline hConstThermo<EquationOfState> operator==
(
    const hConstThermo<EquationOfState>&,
    const hConstThermo<EquationOfState>&
);

template<class EquationOfState>
Ostream& operator<<
(
    Ostream&,
    const hConstThermo<EquationOfState>&
);


/*---------------------------------------------------------------------------*\
                           Class hConstThermo Declaration
\*---------------------------------------------------------------------------*/

template<class EquationOfState>
class hConstThermo
:
    public EquationOfState
{

    scalar Cp_;
    scalar Hf_;


    //- Construct from components
    inline hConstThermo
    (
        const EquationOfState& st,
        const scalar cp,
        const scalar hf
    )
    :
        EquationOfState(st),
        Cp_(cp),
        Hf_(hf)
    {}


public:


    //- Construct from dictionary
    hConstThermo(const dictionary& dict);

    //- Construct as named copy
    inline hConstThermo(const word& name, const hConstThermo& ct)
    :
        EquationOfState(name, ct),
        Cp_(ct.Cp_),
        Hf_(ct.Hf_)
    {}


    //- Construct and return a clone
    inline autoPtr<hConstThermo> clone() const
    {
        return autoPtr<hConstThermo<EquationOfState> >
        (
            new hConstThermo<EquationOfState>(*this)
        );
    }

    //- Selector from dictionary
    inline static autoPtr<hConstThermo> New(const dictionary& dict)
    {
        return autoPtr<hConstThermo<EquationOfState> >
        (
            new hConstThermo<EquationOfState>(dict)
        );
    }


    // Member Functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "hConst<" + EquationOfState::typeName() + '>';
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
        return Cp_ + EquationOfState::Cp(p, T);
    }

    //- Sensible enthalpy [J/kg]
    inline scalar Hs(const scalar p, const scalar T) const
    {
        return Cp_*T + EquationOfState::H(p, T);
    }

    //- Chemical enthalpy [J/kg]
    inline scalar Hc() const
    {
        return Hf_;
    }

    //- Absolute Enthalpy [J/kg]
    inline scalar Ha(const scalar p, const scalar T) const
    {
        return Hs(p, T) + Hc();
    }

    //- Entropy [J/(kg K)]
    inline scalar S(const scalar p, const scalar T) const
    {
        return Cp_*log(T/Tstd) + EquationOfState::S(p, T);
    }

    #include "HtoEthermo.hpp"

    // Derivative term used for Jacobian

    //- Derivative of Gibbs free energy w.r.t. temperature
    inline scalar dGdT(const scalar p, const scalar T) const
    {
        return 0;
    }

    //- Temperature derivative of heat capacity at constant pressure
    inline scalar dCpdT(const scalar p, const scalar T) const
    {
        return 0;
    }


    // I-O

    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators

    inline void operator+=(const hConstThermo& ct);


    // Friend operators
    friend hConstThermo operator+ <EquationOfState>
    (
        const hConstThermo& ct1,
        const hConstThermo& ct2
    );

    friend hConstThermo operator* <EquationOfState>
    (
        const scalar s,
        const hConstThermo& ct
    );

    friend hConstThermo operator== <EquationOfState>
    (
        const hConstThermo& ct1,
        const hConstThermo& ct2
    );

    // IOstream Operators
    friend Ostream& operator<< <EquationOfState>
    (
        Ostream&,
        const hConstThermo&
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


template<class EquationOfState>
CML::hConstThermo<EquationOfState>::hConstThermo(const dictionary& dict)
:
    EquationOfState(dict),
    Cp_(readScalar(dict.subDict("thermodynamics").lookup("Cp"))),
    Hf_(readScalar(dict.subDict("thermodynamics").lookup("Hf")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState>
void CML::hConstThermo<EquationOfState>::write(Ostream& os) const
{
    EquationOfState::write(os);

    dictionary dict("thermodynamics");
    dict.add("Cp", Cp_);
    dict.add("Hf", Hf_);
    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class EquationOfState>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const hConstThermo<EquationOfState>& ct
)
{
    ct.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline void CML::hConstThermo<EquationOfState>::operator+=
(
    const hConstThermo<EquationOfState>& ct
)
{
    scalar Y1 = this->Y();

    EquationOfState::operator+=(ct);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        scalar Y2 = ct.Y()/this->Y();

        Cp_ = Y1*Cp_ + Y2*ct.Cp_;
        Hf_ = Y1*Hf_ + Y2*ct.Hf_;
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline CML::hConstThermo<EquationOfState> CML::operator+
(
    const hConstThermo<EquationOfState>& ct1,
    const hConstThermo<EquationOfState>& ct2
)
{
    EquationOfState eofs
    (
        static_cast<const EquationOfState&>(ct1)
      + static_cast<const EquationOfState&>(ct2)
    );

    if (mag(eofs.Y()) < SMALL)
    {
        return hConstThermo<EquationOfState>
        (
            eofs,
            ct1.Cp_,
            ct1.Hf_
        );
    }
    else
    {
        return hConstThermo<EquationOfState>
        (
            eofs,
            ct1.Y()/eofs.Y()*ct1.Cp_
          + ct2.Y()/eofs.Y()*ct2.Cp_,
            ct1.Y()/eofs.Y()*ct1.Hf_
          + ct2.Y()/eofs.Y()*ct2.Hf_
        );
    }
}


template<class EquationOfState>
inline CML::hConstThermo<EquationOfState> CML::operator*
(
    const scalar s,
    const hConstThermo<EquationOfState>& ct
)
{
    return hConstThermo<EquationOfState>
    (
        s*static_cast<const EquationOfState&>(ct),
        ct.Cp_,
        ct.Hf_
    );
}


template<class EquationOfState>
inline CML::hConstThermo<EquationOfState> CML::operator==
(
    const hConstThermo<EquationOfState>& ct1,
    const hConstThermo<EquationOfState>& ct2
)
{
    EquationOfState eofs
    (
        static_cast<const EquationOfState&>(ct1)
     == static_cast<const EquationOfState&>(ct2)
    );

    return hConstThermo<EquationOfState>
    (
        eofs,
        ct2.Y()/eofs.Y()*ct2.Cp_
      - ct1.Y()/eofs.Y()*ct1.Cp_,
        ct2.Y()/eofs.Y()*ct2.Hf_
      - ct1.Y()/eofs.Y()*ct1.Hf_
    );
}


#endif
