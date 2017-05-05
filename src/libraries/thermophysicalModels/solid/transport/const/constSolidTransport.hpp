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
    CML::constSolidTransport

Description
    Constant properties Transport package.
    Templated into a given thermodynamics package (needed for thermal
    conductivity).


\*---------------------------------------------------------------------------*/

#ifndef constSolidTransport_H
#define constSolidTransport_H

#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class thermo> class constSolidTransport;

template<class thermo>
Ostream& operator<<
(
    Ostream&,
    const constSolidTransport<thermo>&
);


/*---------------------------------------------------------------------------*\
                         Class constSolidTransport Declaration
\*---------------------------------------------------------------------------*/

template<class thermo>
class constSolidTransport
:
    public thermo
{
    // Private data

        //- Constant thermal conductivity.
        scalar K_;


    // Private Member Functions

        //- Construct from components
        inline constSolidTransport
        (
            const thermo& t,
            const scalar k
        );


public:

    // Constructors

        //- Construct as named copy
        inline constSolidTransport(const word&, const constSolidTransport&);

        //- Construct from Istream
        //constSolidTransport(Istream&);
        constSolidTransport(const dictionary&);


    // Member functions

        //- Thermal conductivity [W/mK]
        inline scalar K(const scalar T) const;

        //- Thermal diffusivity [kg/ms]
        inline scalar alpha(const scalar T) const;


    // Ostream Operator

        friend Ostream& operator<< <thermo>
        (
            Ostream&,
            const constSolidTransport&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class thermo>
inline CML::constSolidTransport<thermo>::constSolidTransport
(
    const thermo& t,
    const scalar k
)
:
    thermo(t),
    K_(k)
{}


template<class thermo>
inline CML::constSolidTransport<thermo>::constSolidTransport
(
    const word& name,
    const constSolidTransport& ct
)
:
    thermo(name, ct),
    K_(ct.K_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class thermo>
inline CML::scalar CML::constSolidTransport<thermo>::K(const scalar T) const
{
    return K_;
}


template<class thermo>
inline CML::scalar CML::constSolidTransport<thermo>::alpha
(
    const scalar T
) const
{
    scalar Cp = this->Cp(T);

    scalar rho = this->rho(T);

    return K_/(rho*Cp);
}



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class thermo>
CML::constSolidTransport<thermo>::constSolidTransport(const dictionary& dict)
:
    thermo(dict),
    K_(readScalar(dict.subDict("transport").lookup("K")))
{}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class thermo>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const constSolidTransport<thermo>& ct
)
{
    operator<<(os, static_cast<const thermo&>(ct));
    os << tab << ct.K_;

    os.check("Ostream& operator<<(Ostream& os,const constSolidTransport& ct)");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
