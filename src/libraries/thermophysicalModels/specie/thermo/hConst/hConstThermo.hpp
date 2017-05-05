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
    CML::hConstThermo

Description
    Constant properties thermodynamics package
    templated into the equationOfState.

SourceFiles
    hConstThermoI.hpp
    hConstThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef hConstThermo_H
#define hConstThermo_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class equationOfState> class hConstThermo;

template<class equationOfState>
inline hConstThermo<equationOfState> operator+
(
    const hConstThermo<equationOfState>&,
    const hConstThermo<equationOfState>&
);

template<class equationOfState>
inline hConstThermo<equationOfState> operator-
(
    const hConstThermo<equationOfState>&,
    const hConstThermo<equationOfState>&
);

template<class equationOfState>
inline hConstThermo<equationOfState> operator*
(
    const scalar,
    const hConstThermo<equationOfState>&
);

template<class equationOfState>
inline hConstThermo<equationOfState> operator==
(
    const hConstThermo<equationOfState>&,
    const hConstThermo<equationOfState>&
);

template<class equationOfState>
Ostream& operator<<
(
    Ostream&,
    const hConstThermo<equationOfState>&
);


/*---------------------------------------------------------------------------*\
                           Class hConstThermo Declaration
\*---------------------------------------------------------------------------*/

template<class equationOfState>
class hConstThermo
:
    public equationOfState
{
    // Private data

        //- Heat capacity at constant pressure
        //  Note: input in [J/(kg K)], but internally uses [J/(kmol K)]
        scalar Cp_;

        //- Heat of formation
        //  Note: input in [J/kg], but internally uses [J/kmol]
        scalar Hf_;


    // Private Member Functions

        //- Construct from components
        inline hConstThermo
        (
            const equationOfState& st,
            const scalar cp,
            const scalar hf
        );


public:

    // Constructors

        //- Construct from Istream
        hConstThermo(Istream&);

        //- Construct from dictionary
        hConstThermo(const dictionary& dict);

        //- Construct as named copy
        inline hConstThermo(const word&, const hConstThermo&);

        //- Construct and return a clone
        inline autoPtr<hConstThermo> clone() const;

        //- Selector from Istream
        inline static autoPtr<hConstThermo> New(Istream& is);

        //- Selector from dictionary
        inline static autoPtr<hConstThermo> New(const dictionary& dict);


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

        inline void operator+=(const hConstThermo&);
        inline void operator-=(const hConstThermo&);


    // Friend operators

        friend hConstThermo operator+ <equationOfState>
        (
            const hConstThermo&,
            const hConstThermo&
        );

        friend hConstThermo operator- <equationOfState>
        (
            const hConstThermo&,
            const hConstThermo&
        );

        friend hConstThermo operator* <equationOfState>
        (
            const scalar,
            const hConstThermo&
        );

        friend hConstThermo operator== <equationOfState>
        (
            const hConstThermo&,
            const hConstThermo&
        );


    // IOstream Operators

        friend Ostream& operator<< <equationOfState>
        (
            Ostream&,
            const hConstThermo&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class equationOfState>
inline CML::hConstThermo<equationOfState>::hConstThermo
(
    const equationOfState& st,
    const scalar cp,
    const scalar hf
)
:
    equationOfState(st),
    Cp_(cp),
    Hf_(hf)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class equationOfState>
inline CML::hConstThermo<equationOfState>::hConstThermo
(
    const word& name,
    const hConstThermo& ct
)
:
    equationOfState(name, ct),
    Cp_(ct.Cp_),
    Hf_(ct.Hf_)
{}


template<class equationOfState>
inline CML::autoPtr<CML::hConstThermo<equationOfState> >
CML::hConstThermo<equationOfState>::clone() const
{
    return autoPtr<hConstThermo<equationOfState> >
    (
        new hConstThermo<equationOfState>(*this)
    );
}


template<class equationOfState>
inline CML::autoPtr<CML::hConstThermo<equationOfState> >
CML::hConstThermo<equationOfState>::New(Istream& is)
{
    return autoPtr<hConstThermo<equationOfState> >
    (
        new hConstThermo<equationOfState>(is)
    );
}


template<class equationOfState>
inline CML::autoPtr<CML::hConstThermo<equationOfState> >
CML::hConstThermo<equationOfState>::New(const dictionary& dict)
{
    return autoPtr<hConstThermo<equationOfState> >
    (
        new hConstThermo<equationOfState>(dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState>
inline CML::scalar CML::hConstThermo<EquationOfState>::limit
(
    const scalar T
) const
{
    return T;
}


template<class equationOfState>
inline CML::scalar CML::hConstThermo<equationOfState>::cp
(
    const scalar
) const
{
    return Cp_;
}


template<class equationOfState>
inline CML::scalar CML::hConstThermo<equationOfState>::h
(
    const scalar T
) const
{
    return Cp_*T + Hf_;
}


template<class equationOfState>
inline CML::scalar CML::hConstThermo<equationOfState>::hs
(
    const scalar T
) const
{
    return Cp_*T;
}


template<class equationOfState>
inline CML::scalar CML::hConstThermo<equationOfState>::hc() const
{
    return Hf_;
}


template<class equationOfState>
inline CML::scalar CML::hConstThermo<equationOfState>::s
(
    const scalar T
) const
{
    notImplemented
    (
        "scalar hConstThermo<equationOfState>::s(const scalar T) const"
    );
    return T;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class equationOfState>
inline void CML::hConstThermo<equationOfState>::operator+=
(
    const hConstThermo<equationOfState>& ct
)
{
    scalar molr1 = this->nMoles();

    equationOfState::operator+=(ct);

    molr1 /= this->nMoles();
    scalar molr2 = ct.nMoles()/this->nMoles();

    Cp_ = molr1*Cp_ + molr2*ct.Cp_;
    Hf_ = molr1*Hf_ + molr2*ct.Hf_;
}


template<class equationOfState>
inline void CML::hConstThermo<equationOfState>::operator-=
(
    const hConstThermo<equationOfState>& ct
)
{
    scalar molr1 = this->nMoles();

    equationOfState::operator-=(ct);

    molr1 /= this->nMoles();
    scalar molr2 = ct.nMoles()/this->nMoles();

    Cp_ = molr1*Cp_ - molr2*ct.Cp_;
    Hf_ = molr1*Hf_ - molr2*ct.Hf_;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class equationOfState>
inline CML::hConstThermo<equationOfState> CML::operator+
(
    const hConstThermo<equationOfState>& ct1,
    const hConstThermo<equationOfState>& ct2
)
{
    equationOfState eofs
    (
        static_cast<const equationOfState&>(ct1)
      + static_cast<const equationOfState&>(ct2)
    );

    return hConstThermo<equationOfState>
    (
        eofs,
        ct1.nMoles()/eofs.nMoles()*ct1.Cp_
      + ct2.nMoles()/eofs.nMoles()*ct2.Cp_,
        ct1.nMoles()/eofs.nMoles()*ct1.Hf_
      + ct2.nMoles()/eofs.nMoles()*ct2.Hf_
    );
}


template<class equationOfState>
inline CML::hConstThermo<equationOfState> CML::operator-
(
    const hConstThermo<equationOfState>& ct1,
    const hConstThermo<equationOfState>& ct2
)
{
    equationOfState eofs
    (
        static_cast<const equationOfState&>(ct1)
      - static_cast<const equationOfState&>(ct2)
    );

    return hConstThermo<equationOfState>
    (
        eofs,
        ct1.nMoles()/eofs.nMoles()*ct1.Cp_
      - ct2.nMoles()/eofs.nMoles()*ct2.Cp_,
        ct1.nMoles()/eofs.nMoles()*ct1.Hf_
      - ct2.nMoles()/eofs.nMoles()*ct2.Hf_
    );
}


template<class equationOfState>
inline CML::hConstThermo<equationOfState> CML::operator*
(
    const scalar s,
    const hConstThermo<equationOfState>& ct
)
{
    return hConstThermo<equationOfState>
    (
        s*static_cast<const equationOfState&>(ct),
        ct.Cp_,
        ct.Hf_
    );
}


template<class equationOfState>
inline CML::hConstThermo<equationOfState> CML::operator==
(
    const hConstThermo<equationOfState>& ct1,
    const hConstThermo<equationOfState>& ct2
)
{
    return ct2 - ct1;
}


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class equationOfState>
CML::hConstThermo<equationOfState>::hConstThermo(Istream& is)
:
    equationOfState(is),
    Cp_(readScalar(is)),
    Hf_(readScalar(is))
{
    is.check("hConstThermo::hConstThermo(Istream& is)");

    Cp_ *= this->W();
    Hf_ *= this->W();
}


template<class equationOfState>
CML::hConstThermo<equationOfState>::hConstThermo(const dictionary& dict)
:
    equationOfState(dict),
    Cp_(readScalar(dict.subDict("thermodynamics").lookup("Cp"))),
    Hf_(readScalar(dict.subDict("thermodynamics").lookup("Hf")))
{
    Cp_ *= this->W();
    Hf_ *= this->W();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class equationOfState>
void CML::hConstThermo<equationOfState>::write(Ostream& os) const
{
    equationOfState::write(os);

    dictionary dict("thermodynamics");
    dict.add("Cp", Cp_/this->W());
    dict.add("Hf", Hf_/this->W());
    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class equationOfState>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const hConstThermo<equationOfState>& ct
)
{
    os  << static_cast<const equationOfState&>(ct) << tab
        << ct.Cp_/ct.W() << tab << ct.Hf_/ct.W();

    os.check("Ostream& operator<<(Ostream& os, const hConstThermo& ct)");
    return os;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
