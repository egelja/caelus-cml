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
    CML::exponentialSolidTransport

Description
    Exponential properties for solid heat transport
    Templated into a given thermodynamics package.

SourceFiles
    exponentialSolidTransportI.hpp
    exponentialSolidTransport.cpp

\*---------------------------------------------------------------------------*/

#ifndef exponentialSolidTransport_H
#define exponentialSolidTransport_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class thermo> class exponentialSolidTransport;

template<class thermo>
Ostream& operator<<
(
    Ostream&,
    const exponentialSolidTransport<thermo>&
);


/*---------------------------------------------------------------------------*\
                        Class exponentialSolidTransport Declaration
\*---------------------------------------------------------------------------*/

template<class thermo>
class exponentialSolidTransport
:
    public thermo
{
    // Private data

        //- Constant thermal coefficient.
        scalar K0_;

        //- Exponent coefficient
        scalar n0_;

        //- Reference temperature
        scalar Tref_;


    // Private Member Functions

        //- Construct from components
        inline exponentialSolidTransport
        (
            const thermo& t,
            const scalar K0,
            const scalar n0,
            const scalar Tref
        );


public:

    // Constructors

        //- Construct as named copy
        inline exponentialSolidTransport
        (
            const word&,
            const exponentialSolidTransport&
        );


        //- Construct from dictionary
        exponentialSolidTransport(const dictionary&);


    // Member functions

        //- Thermal conductivity [W/mK]
        inline scalar K(const scalar T) const;

        //- Thermal diffusivity [kg/ms]
        inline scalar alpha(const scalar T) const;


    // Ostream Operator

        friend Ostream& operator<< <thermo>
        (
            Ostream&,
            const exponentialSolidTransport&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class thermo>
inline CML::exponentialSolidTransport<thermo>::exponentialSolidTransport
(
    const thermo& t,
    const scalar K0,
    const scalar n0,
    const scalar Tref
)
:
    thermo(t),
    K0_(K0),
    n0_(n0),
    Tref_(Tref)
{}


template<class thermo>
inline CML::exponentialSolidTransport<thermo>::exponentialSolidTransport
(
    const word& name,
    const exponentialSolidTransport& ct
)
:
    thermo(name, ct),
    K0_(ct.K0_),
    n0_(ct.n0_),
    Tref_(ct.Tref_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class thermo>
inline CML::scalar CML::exponentialSolidTransport<thermo>::K
(
    const scalar T
) const
{
    return (K0_*pow(T/Tref_, n0_));
}


template<class thermo>
inline CML::scalar CML::exponentialSolidTransport<thermo>::alpha
(
    const scalar T
) const
{
    scalar Cp = this->Cp(T);

    scalar rho = this->rho(T);

    return (K(T)/(rho*Cp));
}


#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class thermo>
CML::exponentialSolidTransport<thermo>::exponentialSolidTransport
(
    const dictionary& dict
)
:
    thermo(dict),
    K0_(0.0),
    n0_(0.0),
    Tref_(0.0)
{
    const dictionary& subDict = dict.subDict("transport");
    K0_ = readScalar(subDict.lookup("K0"));
    n0_ = readScalar(subDict.lookup("n0"));
    Tref_ = readScalar(subDict.lookup("Tref"));
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class thermo>
CML::Ostream& CML::operator<<
(
    Ostream& os, const exponentialSolidTransport<thermo>& et
)
{
    operator<<(os, static_cast<const thermo&>(et));
    os << tab << et.K0_  << tab << et.n0_ << tab << et.Tref_;

    os.check
    (
        "Ostream& operator<<(Ostream& os, const exponentialSolidTransport& et)"
    );

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
