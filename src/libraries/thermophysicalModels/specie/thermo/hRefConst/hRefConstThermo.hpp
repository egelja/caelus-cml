/*---------------------------------------------------------------------------*\
Copyright (C) 2015-2018 OpenFOAM Foundation
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
    CML::hRefConstThermo

Description
    Constant properties thermodynamics package
    templated into the EquationOfState.


\*---------------------------------------------------------------------------*/

#ifndef hRefConstThermo_HPP
#define hRefConstThermo_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class EquationOfState> class hRefConstThermo;

template<class EquationOfState>
inline hRefConstThermo<EquationOfState> operator+
(
    const hRefConstThermo<EquationOfState>&,
    const hRefConstThermo<EquationOfState>&
);

template<class EquationOfState>
inline hRefConstThermo<EquationOfState> operator*
(
    const scalar,
    const hRefConstThermo<EquationOfState>&
);

template<class EquationOfState>
inline hRefConstThermo<EquationOfState> operator==
(
    const hRefConstThermo<EquationOfState>&,
    const hRefConstThermo<EquationOfState>&
);

template<class EquationOfState>
Ostream& operator<<
(
    Ostream&,
    const hRefConstThermo<EquationOfState>&
);


/*---------------------------------------------------------------------------*\
                           Class hRefConstThermo Declaration
\*---------------------------------------------------------------------------*/

template<class EquationOfState>
class hRefConstThermo
:
    public EquationOfState
{

    scalar Cp_;
    scalar Hf_;
    scalar Tref_;
    scalar Href_;


    //- Construct from components
    inline hRefConstThermo
    (
        const EquationOfState& st,
        const scalar cp,
        const scalar hf,
        const scalar tref,
        const scalar href
    )
    :
        EquationOfState(st),
        Cp_(cp),
        Hf_(hf),
        Tref_(tref),
        Href_(href)
    {}


public:


    //- Construct from dictionary
    hRefConstThermo(const dictionary& dict);

    //- Construct as named copy
    inline hRefConstThermo(const word& name, const hRefConstThermo& ct)
    :
        EquationOfState(name, ct),
        Cp_(ct.Cp_),
        Hf_(ct.Hf_),
        Tref_(ct.Tref_),
        Href_(ct.Href_)
    {}

    //- Construct and return a clone
    inline autoPtr<hRefConstThermo> clone() const
    {
        return autoPtr<hRefConstThermo<EquationOfState> >
        (
            new hRefConstThermo<EquationOfState>(*this)
        );
    }

    //- Selector from dictionary
    inline static autoPtr<hRefConstThermo> New(const dictionary& dict)
    {
        return autoPtr<hRefConstThermo<EquationOfState> >
        (
            new hRefConstThermo<EquationOfState>(dict)
        );
    }


    // Member Functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "hRefConst<" + EquationOfState::typeName() + '>';
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

    //- Absolute Enthalpy [J/kg]
    inline scalar Ha(const scalar p, const scalar T) const
    {
        return Cp_*(T-Tref_) + Href_ + Hf_ + EquationOfState::H(p, T);
    }

    //- Sensible enthalpy [J/kg]
    inline scalar Hs(const scalar p, const scalar T) const
    {
        return Cp_*(T-Tref_) + Href_ + EquationOfState::H(p, T);
    }

    //- Chemical enthalpy [J/kg]
    inline scalar Hc() const
    {
        return Hf_;
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
    inline void operator+=(const hRefConstThermo&);

    // Friend operators
    friend hRefConstThermo operator+ <EquationOfState>
    (
        const hRefConstThermo& ct1,
        const hRefConstThermo& ct2
    );

    friend hRefConstThermo operator* <EquationOfState>
    (
        const scalar s,
        const hRefConstThermo& ct
    );

    friend hRefConstThermo operator== <EquationOfState>
    (
        const hRefConstThermo& ct1,
        const hRefConstThermo& ct2
    );

    // IOstream Operators
    friend Ostream& operator<< <EquationOfState>
    (
        Ostream&,
        const hRefConstThermo&
    );
};


} // End namespace CML

#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class EquationOfState>
CML::hRefConstThermo<EquationOfState>::hRefConstThermo(const dictionary& dict)
:
    EquationOfState(dict),
    Cp_(readScalar(dict.subDict("thermodynamics").lookup("Cp"))),
    Hf_(readScalar(dict.subDict("thermodynamics").lookup("Hf"))),
    Tref_(readScalar(dict.subDict("thermodynamics").lookup("Tref"))),
    Href_(readScalar(dict.subDict("thermodynamics").lookup("Href")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState>
void CML::hRefConstThermo<EquationOfState>::write(Ostream& os) const
{
    EquationOfState::write(os);

    dictionary dict("thermodynamics");
    dict.add("Cp", Cp_);
    dict.add("Hf", Hf_);
    dict.add("Tref", Tref_);
    dict.add("Href", Href_);
    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class EquationOfState>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const hRefConstThermo<EquationOfState>& ct
)
{
    ct.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline void CML::hRefConstThermo<EquationOfState>::operator+=
(
    const hRefConstThermo<EquationOfState>& ct
)
{
    scalar Y1 = this->Y();

    EquationOfState::operator+=(ct);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        const scalar Y2 = ct.Y()/this->Y();

        Cp_ = Y1*Cp_ + Y2*ct.Cp_;
        Hf_ = Y1*Hf_ + Y2*ct.Hf_;
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline CML::hRefConstThermo<EquationOfState> CML::operator+
(
    const hRefConstThermo<EquationOfState>& ct1,
    const hRefConstThermo<EquationOfState>& ct2
)
{
    EquationOfState eofs
    (
        static_cast<const EquationOfState&>(ct1)
      + static_cast<const EquationOfState&>(ct2)
    );

    if (mag(eofs.Y()) < SMALL)
    {
        return hRefConstThermo<EquationOfState>
        (
            eofs,
            ct1.Cp_,
            ct1.Hf_,
            ct1.Tref_,
            ct1.Href_
        );
    }
    else
    {
        return hRefConstThermo<EquationOfState>
        (
            eofs,
            ct1.Y()/eofs.Y()*ct1.Cp_
          + ct2.Y()/eofs.Y()*ct2.Cp_,
            ct1.Y()/eofs.Y()*ct1.Hf_
          + ct2.Y()/eofs.Y()*ct2.Hf_,
            ct1.Y()/eofs.Y()*ct1.Tref_
          + ct2.Y()/eofs.Y()*ct2.Tref_,
            ct1.Y()/eofs.Y()*ct1.Href_
          + ct2.Y()/eofs.Y()*ct2.Href_
        );
    }
}


template<class EquationOfState>
inline CML::hRefConstThermo<EquationOfState> CML::operator*
(
    const scalar s,
    const hRefConstThermo<EquationOfState>& ct
)
{
    return hRefConstThermo<EquationOfState>
    (
        s*static_cast<const EquationOfState&>(ct),
        ct.Cp_,
        ct.Hf_,
        ct.Tref_,
        ct.Href_
    );
}


template<class EquationOfState>
inline CML::hRefConstThermo<EquationOfState> CML::operator==
(
    const hRefConstThermo<EquationOfState>& ct1,
    const hRefConstThermo<EquationOfState>& ct2
)
{
    EquationOfState eofs
    (
        static_cast<const EquationOfState&>(ct1)
     == static_cast<const EquationOfState&>(ct2)
    );

    return hRefConstThermo<EquationOfState>
    (
        eofs,
        ct2.Y()/eofs.Y()*ct2.Cp_
      - ct1.Y()/eofs.Y()*ct1.Cp_,
        ct2.Y()/eofs.Y()*ct2.Hf_
      - ct1.Y()/eofs.Y()*ct1.Hf_
    );
}


#endif
