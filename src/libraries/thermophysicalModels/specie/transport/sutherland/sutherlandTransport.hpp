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
    CML::sutherlandTransport

Description
    Transport package using Sutherland's formula.

    Templated into a given thermodynamics package (needed for thermal
    conductivity).

    Dynamic viscosity [kg/m.s]
    \f[
        \mu = A_s \frac{\sqrt{T}}{1 + T_s / T}
    \f]


\*---------------------------------------------------------------------------*/

#ifndef sutherlandTransport_H
#define sutherlandTransport_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Thermo> class sutherlandTransport;

template<class Thermo>
inline sutherlandTransport<Thermo> operator+
(
    const sutherlandTransport<Thermo>&,
    const sutherlandTransport<Thermo>&
);

template<class Thermo>
inline sutherlandTransport<Thermo> operator*
(
    const scalar,
    const sutherlandTransport<Thermo>&
);

template<class Thermo>
Ostream& operator<<
(
    Ostream&,
    const sutherlandTransport<Thermo>&
);


/*---------------------------------------------------------------------------*\
                    Class sutherlandTransport Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo>
class sutherlandTransport
:
    public Thermo
{
    // Private data

        // Sutherland's coefficients
        scalar As_, Ts_;


    // Private Member Functions

        //- Calculate the Sutherland coefficients
        //  given two viscosities and temperatures
        inline void calcCoeffs
        (
            const scalar mu1, const scalar T1,
            const scalar mu2, const scalar T2
        );

        //- Read coefficient from dictionary
        scalar readCoeff(const word& coeffName, const dictionary& dict);


public:

    // Constructors

        //- Construct from components
        inline sutherlandTransport
        (
            const Thermo& t,
            const scalar As,
            const scalar Ts
        );

        //- Construct from two viscosities
        inline sutherlandTransport
        (
            const Thermo& t,
            const scalar mu1, const scalar T1,
            const scalar mu2, const scalar T2
        );

        //- Construct as named copy
        inline sutherlandTransport(const word&, const sutherlandTransport&);

        //- Construct from dictionary
        sutherlandTransport(const dictionary& dict);

        //- Construct from base thermo and dictionary
        sutherlandTransport(const Thermo& t,const dictionary& dict);

        //- Construct and return a clone
        inline autoPtr<sutherlandTransport> clone() const;

        // Selector from dictionary
        inline static autoPtr<sutherlandTransport> New(const dictionary& dict);


    // Member functions

        //- Return the instantiated type name
        static word typeName()
        {
            return "sutherland<" + Thermo::typeName() + '>';
        }

        //- Dynamic viscosity [kg/ms]
        inline scalar mu(const scalar p, const scalar T) const;

        //- Thermal conductivity [W/mK]
        inline scalar kappa(const scalar p, const scalar T) const;

        //- Thermal diffusivity of enthalpy [kg/ms]
        inline scalar alphah(const scalar p, const scalar T) const;

        // Species diffusivity
        // inline scalar D(const scalar p, const scalar T) const;

        //- Write to Ostream
        void write(Ostream& os) const;


    // Member operators

        inline void operator=(const sutherlandTransport&);

        inline void operator+=(const sutherlandTransport&);

        inline void operator*=(const scalar);


    // Friend operators

        friend sutherlandTransport operator+ <Thermo>
        (
            const sutherlandTransport&,
            const sutherlandTransport&
        );

        friend sutherlandTransport operator* <Thermo>
        (
            const scalar,
            const sutherlandTransport&
        );


    // Ostream Operator

        friend Ostream& operator<< <Thermo>
        (
            Ostream&,
            const sutherlandTransport&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "specie.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Thermo>
inline void CML::sutherlandTransport<Thermo>::calcCoeffs
(
    const scalar mu1, const scalar T1,
    const scalar mu2, const scalar T2
)
{
    scalar rootT1 = sqrt(T1);
    scalar mu1rootT2 = mu1*sqrt(T2);
    scalar mu2rootT1 = mu2*rootT1;

    Ts_ = (mu2rootT1 - mu1rootT2)/(mu1rootT2/T1 - mu2rootT1/T2);

    As_ = mu1*(1.0 + Ts_/T1)/rootT1;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
inline CML::sutherlandTransport<Thermo>::sutherlandTransport
(
    const Thermo& t,
    const scalar As,
    const scalar Ts
)
:
    Thermo(t),
    As_(As),
    Ts_(Ts)
{}


template<class Thermo>
inline CML::sutherlandTransport<Thermo>::sutherlandTransport
(
    const Thermo& t,
    const scalar mu1, const scalar T1,
    const scalar mu2, const scalar T2
)
:
    Thermo(t)
{
    calcCoeffs(mu1, T1, mu2, T2);
}


template<class Thermo>
inline CML::sutherlandTransport<Thermo>::sutherlandTransport
(
    const word& name,
    const sutherlandTransport& st
)
:
    Thermo(name, st),
    As_(st.As_),
    Ts_(st.Ts_)
{}


template<class Thermo>
inline CML::autoPtr<CML::sutherlandTransport<Thermo> >
CML::sutherlandTransport<Thermo>::clone() const
{
    return autoPtr<sutherlandTransport<Thermo> >
    (
        new sutherlandTransport<Thermo>(*this)
    );
}


template<class Thermo>
inline CML::autoPtr<CML::sutherlandTransport<Thermo> >
CML::sutherlandTransport<Thermo>::New
(
    const dictionary& dict
)
{
    return autoPtr<sutherlandTransport<Thermo> >
    (
        new sutherlandTransport<Thermo>(dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
inline CML::scalar CML::sutherlandTransport<Thermo>::mu
(
    const scalar p,
    const scalar T
) const
{
    return As_*::sqrt(T)/(1.0 + Ts_/T);
}


template<class Thermo>
inline CML::scalar CML::sutherlandTransport<Thermo>::kappa
(
    const scalar p, const scalar T
) const
{
    scalar Cv_ = this->Cv(p, T);
    return mu(p, T)*Cv_*(1.32 + 1.77*this->R()/Cv_);
}


template<class Thermo>
inline CML::scalar CML::sutherlandTransport<Thermo>::alphah
(
    const scalar p,
    const scalar T
) const
{

    return kappa(p, T)/this->Cp(p, T);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Thermo>
inline void CML::sutherlandTransport<Thermo>::operator=
(
    const sutherlandTransport<Thermo>& st
)
{
    Thermo::operator=(st);

    As_ = st.As_;
    Ts_ = st.Ts_;
}


template<class Thermo>
inline void CML::sutherlandTransport<Thermo>::operator+=
(
    const sutherlandTransport<Thermo>& st
)
{
    scalar Y1 = this->Y();

    Thermo::operator+=(st);

    if (mag(this->Y()) > SMALL)
    {
        Y1 /= this->Y();
        scalar Y2 = st.Y()/this->Y();

        As_ = Y1*As_ + Y2*st.As_;
        Ts_ = Y1*Ts_ + Y2*st.Ts_;
    }
}


template<class Thermo>
inline void CML::sutherlandTransport<Thermo>::operator*=
(
    const scalar s
)
{
    Thermo::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Thermo>
inline CML::sutherlandTransport<Thermo> CML::operator+
(
    const sutherlandTransport<Thermo>& st1,
    const sutherlandTransport<Thermo>& st2
)
{
    Thermo t
    (
        static_cast<const Thermo&>(st1) + static_cast<const Thermo&>(st2)
    );

    if (mag(t.Y()) < SMALL)
    {
        return sutherlandTransport<Thermo>
        (
            t,
            0,
            st1.As_,
            st1.Ts_
        );
    }
    else
    {
        scalar Y1 = st1.Y()/t.Y();
        scalar Y2 = st2.Y()/t.Y();

        return sutherlandTransport<Thermo>
        (
            t,
            Y1*st1.As_ + Y2*st2.As_,
            Y1*st1.Ts_ + Y2*st2.Ts_
        );
    }
}


template<class Thermo>
inline CML::sutherlandTransport<Thermo> CML::operator*
(
    const scalar s,
    const sutherlandTransport<Thermo>& st
)
{
    return sutherlandTransport<Thermo>
    (
        s*static_cast<const Thermo&>(st),
        st.As_,
        st.Ts_
    );
}

#include "IOstreams.hpp"


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Thermo>
CML::scalar CML::sutherlandTransport<Thermo>::readCoeff
(
    const word& coeffName,
    const dictionary& dict
)
{
    return readScalar(dict.subDict("transport").lookup(coeffName));
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
CML::sutherlandTransport<Thermo>::sutherlandTransport(const dictionary& dict)
:
    Thermo(dict),
    As_(readCoeff("As", dict)),
    Ts_(readCoeff("Ts", dict))
{}


template<class Thermo>
CML::sutherlandTransport<Thermo>::sutherlandTransport
(
    const Thermo& t,
    const dictionary& dict
)
:
    Thermo(t),
    As_(readCoeff("As", dict)),
    Ts_(readCoeff("Ts", dict))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
void CML::sutherlandTransport<Thermo>::write(Ostream& os) const
{
    os  << this->specie::name() << endl
        << token::BEGIN_BLOCK  << incrIndent << nl;

    Thermo::write(os);

    dictionary dict("transport");
    dict.add("As", As_);
    dict.add("Ts", Ts_);

    os  << indent << dict.dictName() << dict
        << decrIndent << token::END_BLOCK << nl;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Thermo>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const sutherlandTransport<Thermo>& st
)
{
    st.write(os);
    return os;
}


#endif

// ************************************************************************* //
