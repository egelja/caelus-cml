/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

SourceFiles
    eConstThermoI.hpp
    eConstThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef eConstThermo_H
#define eConstThermo_H

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
inline eConstThermo<EquationOfState> operator-
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
    // Private data

        //- Heat capacity at constant volume
        //  Note: input in [J/(kg K)], but internally uses [J/(kmol K)]
        scalar Cv_;

        //- Heat of formation
        //  Note: input in [J/kg], but internally uses [J/kmol]
        scalar Hf_;


    // Private Member Functions

        //- Construct from components
        inline eConstThermo
        (
            const EquationOfState& st,
            const scalar cv,
            const scalar hf
        );


public:

    // Constructors

        //- Construct from Istream
        eConstThermo(Istream&);

        //- Construct from dictionary
        eConstThermo(const dictionary& dict);

        //- Construct as named copy
        inline eConstThermo(const word&, const eConstThermo&);

        //- Construct and return a clone
        inline autoPtr<eConstThermo> clone() const;

        // Selector from Istream
        inline static autoPtr<eConstThermo> New(Istream& is);

        // Selector from dictionary
        inline static autoPtr<eConstThermo> New(const dictionary& dict);


    // Member Functions

        //- Limit the temperature to be in the range Tlow_ to Thigh_
        inline scalar limit(const scalar T) const;

        // Fundamental properties

            //- Heat capacity at constant pressure [J/(kmol K)]
            inline scalar cp(const scalar T) const;

            //- Enthalpy [J/kmol]
            inline scalar h(const scalar T) const;

            //- Sensible Enthalpy [J/kmol]
            inline scalar hs(const scalar T) const;

            //- Chemical enthalpy [J/kmol]
            inline scalar hc() const;

            //- Entropy [J/(kmol K)]
            inline scalar s(const scalar T) const;


        // I-O

            //- Write to Ostream
            void write(Ostream& os) const;


    // Member operators

        inline void operator+=(const eConstThermo&);
        inline void operator-=(const eConstThermo&);


    // Friend operators

        friend eConstThermo operator+ <EquationOfState>
        (
            const eConstThermo&,
            const eConstThermo&
        );

        friend eConstThermo operator- <EquationOfState>
        (
            const eConstThermo&,
            const eConstThermo&
        );

        friend eConstThermo operator* <EquationOfState>
        (
            const scalar,
            const eConstThermo&
        );

        friend eConstThermo operator== <EquationOfState>
        (
            const eConstThermo&,
            const eConstThermo&
        );


    // IOstream Operators

        friend Ostream& operator<< <EquationOfState>
        (
            Ostream&,
            const eConstThermo&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class EquationOfState>
inline CML::eConstThermo<EquationOfState>::eConstThermo
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


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class EquationOfState>
inline CML::eConstThermo<EquationOfState>::eConstThermo
(
    const word& name,
    const eConstThermo<EquationOfState>& ct
)
:
    EquationOfState(name, ct),
    Cv_(ct.Cv_),
    Hf_(ct.Hf_)
{}


template<class EquationOfState>
inline CML::autoPtr<CML::eConstThermo<EquationOfState> >
CML::eConstThermo<EquationOfState>::clone() const
{
    return autoPtr<eConstThermo<EquationOfState> >
    (
        new eConstThermo<EquationOfState>(*this)
    );
}


template<class EquationOfState>
inline CML::autoPtr<CML::eConstThermo<EquationOfState> >
CML::eConstThermo<EquationOfState>::New(Istream& is)
{
    return autoPtr<eConstThermo<EquationOfState> >
    (
        new eConstThermo<EquationOfState>(is)
    );
}


template<class EquationOfState>
inline CML::autoPtr<CML::eConstThermo<EquationOfState> >
CML::eConstThermo<EquationOfState>::New(const dictionary& dict)
{
    return autoPtr<eConstThermo<EquationOfState> >
    (
        new eConstThermo<EquationOfState>(dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState>
inline CML::scalar CML::eConstThermo<EquationOfState>::limit
(
    const scalar T
) const
{
    return T;
}


template<class EquationOfState>
inline CML::scalar CML::eConstThermo<EquationOfState>::cp
(
    const scalar
) const
{
    return Cv_ + specie::RR;
}


template<class EquationOfState>
inline CML::scalar CML::eConstThermo<EquationOfState>::h
(
    const scalar T
) const
{
    return cp(T)*T + Hf_;
}


template<class EquationOfState>
inline CML::scalar CML::eConstThermo<EquationOfState>::hs
(
    const scalar T
) const
{
    return cp(T)*T;
}


template<class EquationOfState>
inline CML::scalar CML::eConstThermo<EquationOfState>::hc() const
{
    return Hf_;
}


template<class EquationOfState>
inline CML::scalar CML::eConstThermo<EquationOfState>::s
(
    const scalar T
) const
{
    notImplemented
    (
        "scalar eConstThermo<EquationOfState>::s(const scalar) const"
    );
    return T;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EquationOfState>
inline void CML::eConstThermo<EquationOfState>::operator+=
(
    const eConstThermo<EquationOfState>& ct
)
{
    scalar molr1 = this->nMoles();

    EquationOfState::operator+=(ct);

    molr1 /= this->nMoles();
    scalar molr2 = ct.nMoles()/this->nMoles();

    Cv_ = molr1*Cv_ + molr2*ct.Cv_;
    Hf_ = molr1*Hf_ + molr2*ct.Hf_;
}


template<class EquationOfState>
inline void CML::eConstThermo<EquationOfState>::operator-=
(
    const eConstThermo<EquationOfState>& ct
)
{
    scalar molr1 = this->nMoles();

    EquationOfState::operator-=(ct);

    molr1 /= this->nMoles();
    scalar molr2 = ct.nMoles()/this->nMoles();

    Cv_ = molr1*Cv_ - molr2*ct.Cv_;
    Hf_ = molr1*Hf_ - molr2*ct.Hf_;
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

    return eConstThermo<EquationOfState>
    (
        eofs,
        ct1.nMoles()/eofs.nMoles()*ct1.Cv_
      + ct2.nMoles()/eofs.nMoles()*ct2.Cv_,
        ct1.nMoles()/eofs.nMoles()*ct1.Hf_
      + ct2.nMoles()/eofs.nMoles()*ct2.Hf_
    );
}


template<class EquationOfState>
inline CML::eConstThermo<EquationOfState> CML::operator-
(
    const eConstThermo<EquationOfState>& ct1,
    const eConstThermo<EquationOfState>& ct2
)
{
    EquationOfState eofs
    (
        static_cast<const EquationOfState&>(ct1)
      - static_cast<const EquationOfState&>(ct2)
    );

    return eConstThermo<EquationOfState>
    (
        eofs,
        ct1.nMoles()/eofs.nMoles()*ct1.Cv_
      - ct2.nMoles()/eofs.nMoles()*ct2.Cv_,
        ct1.nMoles()/eofs.nMoles()*ct1.Hf_
      - ct2.nMoles()/eofs.nMoles()*ct2.Hf_
    );
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
    return ct2 - ct1;
}


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class EquationOfState>
CML::eConstThermo<EquationOfState>::eConstThermo(Istream& is)
:
    EquationOfState(is),
    Cv_(readScalar(is)),
    Hf_(readScalar(is))
{
    is.check("eConstThermo<EquationOfState>::eConstThermo(Istream&)");

    Cv_ *= this->W();
    Hf_ *= this->W();
}


template<class EquationOfState>
CML::eConstThermo<EquationOfState>::eConstThermo(const dictionary& dict)
:
    EquationOfState(dict),
    Cv_(readScalar(dict.subDict("thermodynamics").lookup("Cv"))),
    Hf_(readScalar(dict.subDict("thermodynamics").lookup("Hf")))
{
    Cv_ *= this->W();
    Hf_ *= this->W();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState>
void CML::eConstThermo<EquationOfState>::write(Ostream& os) const
{
    EquationOfState::write(os);

    dictionary dict("thermodynamics");
    dict.add("Cv", Cv_/this->W());
    dict.add("Hf", Hf_/this->W());
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
    os  << static_cast<const EquationOfState&>(ct) << tab
        << ct.Cv_/ct.W() << tab << ct.Hf_/ct.W();

    os.check("Ostream& operator<<(Ostream&, const eConstThermo&)");
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
