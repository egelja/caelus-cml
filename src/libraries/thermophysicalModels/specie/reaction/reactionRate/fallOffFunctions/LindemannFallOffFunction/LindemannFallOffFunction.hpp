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
    CML::LindemannFallOffFunction

Description
    Lindemann fall-off function


\*---------------------------------------------------------------------------*/

#ifndef LindemannFallOffFunction_HPP
#define LindemannFallOffFunction_HPP

#include "scalar.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class LindemannFallOffFunction;
Ostream& operator<<(Ostream&, const LindemannFallOffFunction&);


/*---------------------------------------------------------------------------*\
                 Class LindemannFallOffFunction Declaration
\*---------------------------------------------------------------------------*/

class LindemannFallOffFunction
{

public:


    //- Construct null
    inline LindemannFallOffFunction()
    {}

    //- Construct from dictionary
    inline LindemannFallOffFunction(const dictionary& dict)
    {}


    // Member Functions

    //- Return the type name
    static word type()
    {
        return "Lindemann";
    }

    inline scalar operator()
    (
        const scalar T,
        const scalar Pr
    ) const
    {
        return 1.0;
    }

    inline scalar ddT
    (
        const scalar Pr,
        const scalar F,
        const scalar dPrdT,
        const scalar T
    ) const
    {
        return 0;
    }

    inline scalar ddc
    (
        const scalar Pr,
        const scalar F,
        const scalar dPrdc,
        const scalar T
    ) const
    {
        return 0;
    }

    //- Write to stream
    inline void write(Ostream& os) const
    {}


    // Ostream Operator
    friend Ostream& operator<<
    (
        Ostream& os,
        const LindemannFallOffFunction&
    )
    {
        return os;
    }
};


} // End namespace CML


#endif
