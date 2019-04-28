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
    CML::constTransport

Description
    Constant properties Transport package.
    Templated into a given thermodynamics package (needed for thermal
    conductivity).

\*---------------------------------------------------------------------------*/

#ifndef constTransport_H
#define constTransport_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Thermo> class constTransport;

template<class Thermo>
inline constTransport<Thermo> operator+
(
    const constTransport<Thermo>&,
    const constTransport<Thermo>&
);

template<class Thermo>
inline constTransport<Thermo> operator*
(
    const scalar,
    const constTransport<Thermo>&
);

template<class Thermo>
Ostream& operator<<
(
    Ostream&,
    const constTransport<Thermo>&
);


/*---------------------------------------------------------------------------*\
                           Class constTransport Declaration
\*---------------------------------------------------------------------------*/

template<class Thermo>
class constTransport
:
    public Thermo
{
    // Private data

    //- Constant dynamic viscosity [Pa.s]
    scalar mu_;

    //- Reciprocal Prandtl Number []
    scalar rPr_;


    // Private Member Functions

    //- Construct from components
    inline constTransport
    (
        const Thermo& t,
        const scalar mu,
        const scalar Pr
    )
    :
        Thermo(t),
        mu_(mu),
        rPr_(1.0/Pr)
    {}


public:


    //- Construct as named copy
    inline constTransport(const word& name, const constTransport& ct)
    :
        Thermo(name, ct),
        mu_(ct.mu_),
        rPr_(ct.rPr_)
    {}

    //- Construct from dictionary
    constTransport(const dictionary& dict);

    //- Construct and return a clone
    inline autoPtr<constTransport> clone() const
    {
        return autoPtr<constTransport<Thermo> >
        (
            new constTransport<Thermo>(*this)
        );
    }


    // Selector from dictionary
    inline static autoPtr<constTransport> New(const dictionary& dict)
    {
        return autoPtr<constTransport<Thermo> >
        (
            new constTransport<Thermo>(dict)
        );
    }


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "const<" + Thermo::typeName() + '>';
    }

    //- Dynamic viscosity [kg/ms]
    inline scalar mu(const scalar p, const scalar T) const
    {
        return mu_;
    }

    //- Thermal conductivity [W/mK]
    inline scalar kappa(const scalar p, const scalar T) const
    {
        return this->Cp(p, T)*mu(p, T)*rPr_;
    }

    //- Thermal diffusivity of enthalpy [kg/ms]
    inline scalar alphah(const scalar p, const scalar T) const
    {
        return mu(p, T)*rPr_;
    }

    // Species diffusivity
    // inline scalar D(const scalar p, const scalar T) const;

    //- Write to Ostream
    void write(Ostream& os) const;


    // Member operators
    inline void operator=(const constTransport& ct)
    {
        Thermo::operator=(ct);

        mu_ = ct.mu_;
        rPr_ = ct.rPr_;
    }

    inline void operator+=(const constTransport& st)
    {
        scalar Y1 = this->Y();

        Thermo::operator+=(st);

        if (mag(this->Y()) > SMALL)
        {
            Y1 /= this->Y();
            scalar Y2 = st.Y()/this->Y();

            mu_ = Y1*mu_ + Y2*st.mu_;
            rPr_ = 1.0/(Y1/rPr_ + Y2/st.rPr_);
        }
    }

    inline void operator*=(const scalar s)
    {
        Thermo::operator*=(s);
    }

    // Friend operators
    friend constTransport operator+
    (
        const constTransport& ct1,
        const constTransport& ct2
    )
    {
        Thermo t
        (
            static_cast<const Thermo&>(ct1) + static_cast<const Thermo&>(ct2)
        );

        if (mag(t.Y()) < SMALL)
        {
            return constTransport<Thermo>
            (
                t,
                0,
                ct1.rPr_
            );
        }
        else
        {
            scalar Y1 = ct1.Y()/t.Y();
            scalar Y2 = ct2.Y()/t.Y();

            return constTransport<Thermo>
            (
                t,
                Y1*ct1.mu_ + Y2*ct2.mu_,
                1.0/(Y1/ct1.rPr_ + Y2/ct2.rPr_)
            );
        }
    }

    friend constTransport operator*
    (
        const scalar s,
        const constTransport& ct
    )
    {
        return constTransport<Thermo>
        (
            s*static_cast<const Thermo&>(ct),
            ct.mu_,
            1.0/ct.rPr_
        );
    }

    // Ostream Operator
    friend Ostream& operator<< <Thermo>
    (
        Ostream&,
        const constTransport&
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
CML::constTransport<Thermo>::constTransport(const dictionary& dict)
:
    Thermo(dict),
    mu_(readScalar(dict.subDict("transport").lookup("mu"))),
    rPr_(1.0/readScalar(dict.subDict("transport").lookup("Pr")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
// CAELUSFIX -removed ::constTransport
template<class Thermo>
void CML::constTransport<Thermo>::write(Ostream& os) const
{
    os  << this->name() << endl;
    os  << token::BEGIN_BLOCK  << incrIndent << nl;

    Thermo::write(os);

    dictionary dict("transport");
    dict.add("mu", mu_);
    dict.add("Pr", 1.0/rPr_);
    os  << indent << dict.dictName() << dict;

    os  << decrIndent << token::END_BLOCK << nl;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Thermo>
CML::Ostream& CML::operator<<(Ostream& os, const constTransport<Thermo>& ct)
{
    ct.write(os);
    return os;
}


#endif

