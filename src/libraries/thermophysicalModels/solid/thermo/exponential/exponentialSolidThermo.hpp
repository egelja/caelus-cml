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
    CML::exponentialSolidThermo

Description
    Exponential thermodynamics package for solids templated into the rhoType.


\*---------------------------------------------------------------------------*/

#ifndef exponentialSolidThermo_H
#define exponentialSolidThermo_H

#include "scalar.hpp"
#include "dictionary.hpp"
#include "specie.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class rhoType> class exponentialSolidThermo;

template<class rhoType>
Ostream& operator<<
(
    Ostream&,
    const exponentialSolidThermo<rhoType>&
);


/*---------------------------------------------------------------------------*\
                         Class exponentialSolidThermo Declaration
\*---------------------------------------------------------------------------*/

template<class rhoType>
class exponentialSolidThermo
:
    public rhoType
{

    // Private data

        scalar c0_;
        scalar n0_;
        scalar Tref_;
        scalar Hf_;


    // Private Member Functions

        //- Check given temperature is within the range of the fitted coeffs
        inline void checkT(const scalar T) const;

        //- Integrate Cp expression
        inline scalar integrateCp(const scalar T) const;


public:

    // Constructors

        //- Construct from components
        inline exponentialSolidThermo
        (
            const rhoType& st,
            const scalar c0,
            const scalar n0,
            const scalar Tref,
            const scalar Hf
        );

        //- Construct from dictionary
        exponentialSolidThermo(const dictionary&);

        //- Construct as a named copy
        inline exponentialSolidThermo
        (
            const word&,
            const exponentialSolidThermo&
        );


    // Member Functions

        //- Heat capacity at constant pressure [J/(kg K)]
        inline scalar Cp(const scalar T) const;

        //- Enthalpy [J/kmol]
        inline scalar h(const scalar T) const;

        //- Sensible enthalpy [J/kg]
        inline scalar hs(const scalar T) const;

        //- Formation enthalpy [J/kg]
        inline scalar hf() const;


    // Ostream Operator

        friend Ostream& operator<< <rhoType>
        (
            Ostream&,
            const exponentialSolidThermo&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "specie.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


template<class rhoType>
inline void CML::exponentialSolidThermo<rhoType>::checkT(const scalar T) const
{
    if (T < 0.0)
    {
        FatalErrorIn
        (
            "exponentialSolidThermo<rhoType>::checkT(const scalar T) const"
        )   << "attempt to use exponentialSolidThermo<rhoType>"
               " out of temperature range "
            << T
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class rhoType>
inline CML::exponentialSolidThermo<rhoType>::exponentialSolidThermo
(
    const word& name,
    const exponentialSolidThermo& jt
)
:
    rhoType(name, jt),
    c0_(jt.c0_),
    n0_(jt.n0_),
    Tref_(jt.Tref_),
    Hf_(jt.Hf_)
{}


template<class rhoType>
inline CML::exponentialSolidThermo<rhoType>::exponentialSolidThermo
(
    const rhoType& st,
    const scalar c0,
    const scalar n0,
    const scalar Tref,
    const scalar Hf
)
:
    rhoType(st),
    c0_(c0),
    n0_(n0),
    Tref_(Tref),
    Hf_(Hf)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class rhoType>
inline CML::scalar CML::exponentialSolidThermo<rhoType>::integrateCp
(
    const scalar T
) const
{
    return
    (
        c0_*pow(T, n0_ + 1.0)
       /(pow(Tref_, n0_)*(n0_ + 1.0))
    );
}


template<class rhoType>
inline CML::scalar CML::exponentialSolidThermo<rhoType>::Cp
(
    const scalar T
) const
{
    return c0_*pow(T/Tref_, n0_);
}


template<class rhoType>
inline CML::scalar CML::exponentialSolidThermo<rhoType>::h
(
    const scalar T
) const
{
    scalar hOffset = integrateCp(specie::Tstd);

    return
    (
        integrateCp(T) + Hf_ - hOffset
    );
}


template<class rhoType>
inline CML::scalar CML::exponentialSolidThermo<rhoType>::hs
(
    const scalar T
) const
{
    return h(T) - Hf_;
}


template<class rhoType>
inline CML::scalar CML::exponentialSolidThermo<rhoType>::hf() const
{
    return Hf_;
}

#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class rhoType>
CML::exponentialSolidThermo<rhoType>::exponentialSolidThermo
(
    const dictionary& dict
)
:
    rhoType(dict),
    c0_(readScalar(dict.subDict("thermodynamics").lookup("C0"))),
    n0_(readScalar(dict.subDict("thermodynamics").lookup("n0"))),
    Tref_(readScalar(dict.subDict("thermodynamics").lookup("Tref"))),
    Hf_(readScalar(dict.subDict("thermodynamics").lookup("Hf")))
{}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class rhoType>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const exponentialSolidThermo<rhoType>& et
)
{
    os  << static_cast<const rhoType&>(et) << nl
        << "    " << et.c0_
        << tab << et.n0_
        << tab << et.Tref_
        << tab << et.Hf_;

    os << nl << "    ";

    os << endl;

    os.check
    (
        "operator<<(Ostream& os, const exponentialSolidThermo<rhoType>& et)"
    );

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
