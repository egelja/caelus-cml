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
    CML::constSolidThermo

Description
    Constant properties thermodynamics package for solids


\*---------------------------------------------------------------------------*/

#ifndef constSolidThermo_H
#define constSolidThermo_H

#include "specie.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class rhoType> class constSolidThermo;

template<class rhoType>
Ostream& operator<<
(
    Ostream&,
    const constSolidThermo<rhoType>&
);


/*---------------------------------------------------------------------------*\
                      Class constSolidThermo Declaration
\*---------------------------------------------------------------------------*/

template<class rhoType>
class constSolidThermo
:
    public rhoType
{
    // Private data

        scalar Cp_;
        scalar Hf_;


    // Private Member Functions

        //- Construct from components
        inline constSolidThermo
        (
            const rhoType& st,
            const scalar Cp,
            const scalar hf
        );


public:

    // Constructors

        //- Construct from dictionary
        constSolidThermo(const dictionary&);

        //- Construct as named copy
        inline constSolidThermo(const word&, const constSolidThermo&);

        //- Construct and return a clone
        inline autoPtr<constSolidThermo> clone() const;

        //- Selector from Istream
        inline static autoPtr<constSolidThermo> New(dictionary& is);


    // Member Functions

        // Fundamental properties

            //- Heat capacity at constant pressure [J/(kg K)]
            inline scalar Cp(const scalar T) const;

            //- Enthalpy [J/kg]
            inline scalar h(const scalar T) const;

            //- Sensible enthalpy [J/kg]
            inline scalar hs(const scalar T) const;

            //- Enthalpy of formation [J/kg]
            inline scalar hf() const;


    // IOstream Operators

        friend Ostream& operator<< <rhoType>
        (
            Ostream&,
            const constSolidThermo&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class rhoType>
inline CML::constSolidThermo<rhoType>::constSolidThermo
(
    const word& name,
    const constSolidThermo& ct
)
:
    rhoType(name, ct),
    Cp_(ct.Cp_),
    Hf_(ct.Hf_)
{}


template<class rhoType>
inline CML::constSolidThermo<rhoType>::constSolidThermo
(
    const rhoType& st,
    const scalar Cp,
    const scalar hf
)
:
    rhoType(st),
    Cp_(Cp),
    Hf_(hf)
{}


template<class rhoType>
inline CML::autoPtr<CML::constSolidThermo<rhoType> >
CML::constSolidThermo<rhoType>::clone() const
{
    return autoPtr<constSolidThermo<rhoType> >
    (
        new constSolidThermo<rhoType>(*this)
    );
}


template<class rhoType>
inline CML::autoPtr<CML::constSolidThermo<rhoType> >
CML::constSolidThermo<rhoType>::New(dictionary& is)
{
    return autoPtr<constSolidThermo<rhoType> >
    (
        new constSolidThermo<rhoType>(is)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class rhoType>
inline CML::scalar CML::constSolidThermo<rhoType>::Cp
(
    const scalar
) const
{
    return Cp_;
}


template<class rhoType>
inline CML::scalar CML::constSolidThermo<rhoType>::h
(
    const scalar T
) const
{
    scalar hOffset = Cp_*(specie::Tstd);

    return (Cp_*T + Hf_ - hOffset);
}


template<class rhoType>
inline CML::scalar CML::constSolidThermo<rhoType>::hs
(
    const scalar T
) const
{
    return h(T) - Hf_;
}


template<class rhoType>
inline CML::scalar CML::constSolidThermo<rhoType>::hf() const
{
    return Hf_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class rhoType>
CML::constSolidThermo<rhoType>::constSolidThermo
(
    const dictionary& dict
)
:
    rhoType(dict),
    Cp_(readScalar(dict.subDict("thermodynamics").lookup("Cp"))),
    Hf_(readScalar(dict.subDict("thermodynamics").lookup("Hf")))
{}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //


template<class rhoType>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const constSolidThermo<rhoType>& ct
)
{
    os  << static_cast<const rhoType&>(ct) << tab
        << ct.Cp_ << tab << ct.Hf_;

    os.check("Ostream& operator<<(Ostream& os, const constSolidThermo& ct)");
    return os;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
