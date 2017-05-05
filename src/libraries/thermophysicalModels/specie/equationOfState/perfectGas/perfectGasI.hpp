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

\*---------------------------------------------------------------------------*/

#include "perfectGas.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline CML::perfectGas::perfectGas(const specie& sp)
:
    specie(sp)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::perfectGas::perfectGas(const word& name, const perfectGas& pg)
:
    specie(name, pg)
{}


inline CML::autoPtr<CML::perfectGas> CML::perfectGas::clone() const
{
    return autoPtr<perfectGas>(new perfectGas(*this));
}


inline CML::autoPtr<CML::perfectGas> CML::perfectGas::New(Istream& is)
{
    return autoPtr<perfectGas>(new perfectGas(is));
}


inline CML::autoPtr<CML::perfectGas> CML::perfectGas::New
(
    const dictionary& dict
)
{
    return autoPtr<perfectGas>(new perfectGas(dict));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::scalar CML::perfectGas::rho(scalar p, scalar T) const
{
    return p/(R()*T);
}


inline CML::scalar CML::perfectGas::psi(scalar, scalar T) const
{
    return 1.0/(R()*T);
}


inline CML::scalar CML::perfectGas::Z(scalar, scalar) const
{
    return 1.0;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void CML::perfectGas::operator+=(const perfectGas& pg)
{
    specie::operator+=(pg);
}


inline void CML::perfectGas::operator-=(const perfectGas& pg)
{
    specie::operator-=(pg);
}


inline void CML::perfectGas::operator*=(const scalar s)
{
    specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline CML::perfectGas CML::operator+
(
    const perfectGas& pg1,
    const perfectGas& pg2
)
{
    return perfectGas
    (
        static_cast<const specie&>(pg1)
      + static_cast<const specie&>(pg2)
    );
}


inline CML::perfectGas CML::operator-
(
    const perfectGas& pg1,
    const perfectGas& pg2
)
{
    return perfectGas
    (
        static_cast<const specie&>(pg1)
      - static_cast<const specie&>(pg2)
    );
}


inline CML::perfectGas CML::operator*
(
    const scalar s,
    const perfectGas& pg
)
{
    return perfectGas(s*static_cast<const specie&>(pg));
}


inline CML::perfectGas CML::operator==
(
    const perfectGas& pg1,
    const perfectGas& pg2
)
{
    return pg2 - pg1;
}


// ************************************************************************* //
