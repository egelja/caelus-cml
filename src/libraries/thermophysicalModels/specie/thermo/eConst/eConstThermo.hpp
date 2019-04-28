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
    CML::eConstThermo

Description
    Constant properties thermodynamics package templated on an equation of
    state


\*---------------------------------------------------------------------------*/

#ifndef eConstThermo_HPP
#define eConstThermo_HPP

#include "specieThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class EquationOfState> class eConstThermo;

template<class EquationOfState>
inline eConstThermo<EquationOfState> operator+
(
    const eConstThermo<EquationOfState>&,
    const eConstThermo<EquationOfState>&
);

template<class EquationOfState>
inline eConstThermo<EquationOfState> operator*
(
    const scalar,
    const eConstThermo<EquationOfState>&
);

template<class EquationOfState>
inline eConstThermo<EquationOfState> operator==
(
    const eConstThermo<EquationOfState>&,
    const eConstThermo<EquationOfState>&
);

template<class EquationOfState>
Ostream& operator<<
(
    Ostream&,
    const eConstThermo<EquationOfState>&
);


/*---------------------------------------------------------------------------*\
                        Class eConstThermo Declaration
\*---------------------------------------------------------------------------*/

template<class EquationOfState>
class eConstThermo
:
    public EquationOfState
{

    //- Heat capacity at constant volume
    //  Note: input in [J/(kg K)], but internally uses [J/(kmol K)]
    scalar Cv_;

    //- Heat of formation
    //  Note: input in [J/kg], but internally uses [J/kmol]
    scalar Hf_;


    //- Construct from components
    inline eConstThermo
    (
        const EquationOfState& st,
        const scalar cv,
       const scalar hf
    )
    :
        EquationOfState(st),
        Cv_(cv),
        Hf_(hf)
    {}


public:


    //- Construct from dictionary
    eConstThermo(const dictionary& dict);

    //- Construct as named copy
    inline eConstThermo(const word& name, const eConstThermo& ct)
    :
        EquationOfState(name, ct),
        Cv_(ct.Cv_),
        Hf_(ct.Hf_)
    {}

    //- Construct and return a clone
    inline autoPtr<eConstThermo> clone() const
    {
        return autoPtr<eConstThermo<EquationOfState> >
        (
            new eConstThermo<EquationOfState>(*this)
        );
    }

    // Selector from dictionary
    inline static autoPtr<eConstThermo> New(const dictionary& dict)
    {
        return autoPtr<eConstThermo<EquationOfState> >
        (
            new eConstThermo<EquationOfState>(dict)
        );
    }


    // Member Functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "eConst<" + EquationOfState::typeName() + '>';
    }

    //- Limit the temperature to be in the range Tlow_ to Thigh_
    inline scalar limit(const scalar T) const
    {
        return T;
    }

    // Fundamental properties

    //- Heat capacity at constant volume [J/(kg K)]
    inline scalar Cv(const scalar p, const scalar T) const
    {
        return Cv_ + EquationOfState::Cv(p, T);
    }

    //- Sensible internal energy [J/kg]
    inline scalar Es(const scalar p, const scalar T) const
    {
        return Cv_*T + EquationOfState::E(p, T);
    }

    //- Chemical enthalpy [J/kg]
    inline scalar Hc() const
    {
        return Hf_;
    }

    //- Absolute internal energy [J/kg]
    inline scalar Ea(const scalar p, const scalar T) const
    {
        return Es(p, T) + Hc();
    }

    //- Entropy [J/(kg K)]
    inline scalar S(const scalar p, const scalar T) const
    {
        return Cp(p, T)*log(T/Tstd) + EquationOfState::S(p, T);
    }

    #include "EtoHthermo.hpp"

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
    inline void operator+=(const eConstThermo& ct);


    // Friend operators
    friend eConstThermo operator+ <EquationOfState>
    (
        const eConstThermo& ct1,
        const eConstThermo& ct2
    );

    friend eConstThermo operator* <EquationOfState>
    (
        const scalar s,
        const eConstThermo& ct
    );

    friend eConstThermo operator== <EquationOfState>
    (
        const eConstThermo& ct1,
        const eConstThermo& ct2
    );

    // IOstream Operators
    friend Ostream& operator<< <EquationOfState>
    (
        Ostream&,
        const eConstThermo&
    );
};


} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


template<class EquationOfState>
CML::eConstThermo<EquationOfState>::eConstThermo(const dictionary& dict)
:
    EquationOfState(dict),
    Cv_(readScalar(dict.subDict("thermodynamics").lookup("Cv"))),
    Hf_(readScalar(dict.subDict("thermodynamics").lookup("Hf")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState>
void CML::eConstThermo<EquationOfState>::write(Ostream& os) const
{
    EquationOfState::write(os);

    dictionary dict("thermodynamics");
    dict.add("Cv", Cv_);
    dict.add("Hf", Hf_);
    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class EquationOfState>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const eConstThermo<EquationOfState>& ct
)
{
    ct.write(os);
    return os;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline void CML::eConstThermo<EquationOfState>::operator+=
(
    const eConstThermo<EquationOfState>& ct
)
{
    scalar Y1 = this->Y();

    EquationOfState::operator+=(ct);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        const scalar Y2 = ct.Y()/this->Y();

        Cv_ = Y1*Cv_ + Y2*ct.Cv_;
        Hf_ = Y1*Hf_ + Y2*ct.Hf_;
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline CML::eConstThermo<EquationOfState> CML::operator+
(
    const eConstThermo<EquationOfState>& ct1,
    const eConstThermo<EquationOfState>& ct2
)
{
    EquationOfState eofs
    (
        static_cast<const EquationOfState&>(ct1)
      + static_cast<const EquationOfState&>(ct2)
    );

    if (mag(eofs.Y()) < SMALL)
    {
        return eConstThermo<EquationOfState>
        (
            eofs,
            ct1.Cv_,
            ct1.Hf_
        );
    }
    else
    {
        return eConstThermo<EquationOfState>
        (
            eofs,
            ct1.Y()/eofs.Y()*ct1.Cv_
          + ct2.Y()/eofs.Y()*ct2.Cv_,
            ct1.Y()/eofs.Y()*ct1.Hf_
          + ct2.Y()/eofs.Y()*ct2.Hf_
        );
    }
}


template<class EquationOfState>
inline CML::eConstThermo<EquationOfState> CML::operator*
(
    const scalar s,
    const eConstThermo<EquationOfState>& ct
)
{
    return eConstThermo<EquationOfState>
    (
        s*static_cast<const EquationOfState&>(ct),
        ct.Cv_,
        ct.Hf_
    );
}


template<class EquationOfState>
inline CML::eConstThermo<EquationOfState> CML::operator==
(
    const eConstThermo<EquationOfState>& ct1,
    const eConstThermo<EquationOfState>& ct2
)
{
    EquationOfState eofs
    (
        static_cast<const EquationOfState&>(ct1)
     == static_cast<const EquationOfState&>(ct2)
    );

    return eConstThermo<EquationOfState>
    (
        eofs,
        ct2.Y()/eofs.Y()*ct2.Cv_
      - ct1.Y()/eofs.Y()*ct1.Cv_,
        ct2.Y()/eofs.Y()*ct2.Hf_
      - ct1.Y()/eofs.Y()*ct1.Hf_
    );
}


#endif
