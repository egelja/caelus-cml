/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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

#include "isobaricPerfectGas.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline CML::isobaricPerfectGas::isobaricPerfectGas(const specie& sp)
:
    specie(sp)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::isobaricPerfectGas::isobaricPerfectGas
(
    const word& name,
    const isobaricPerfectGas& pg
)
:
    specie(name, pg)
{}


inline CML::autoPtr<CML::isobaricPerfectGas> CML::isobaricPerfectGas::
clone() const
{
    return autoPtr<isobaricPerfectGas>(new isobaricPerfectGas(*this));
}


inline CML::autoPtr<CML::isobaricPerfectGas> CML::isobaricPerfectGas::New
(
    Istream& is
)
{
    return autoPtr<isobaricPerfectGas>(new isobaricPerfectGas(is));
}


inline CML::autoPtr<CML::isobaricPerfectGas> CML::isobaricPerfectGas::New
(
    const dictionary& dict
)
{
    return autoPtr<isobaricPerfectGas>(new isobaricPerfectGas(dict));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::scalar CML::isobaricPerfectGas::rho(scalar p, scalar T) const
{
    return pRef_/(R()*T);
}


inline CML::scalar CML::isobaricPerfectGas::psi(scalar, scalar T) const
{
    return 0.0;
}


inline CML::scalar CML::isobaricPerfectGas::Z(scalar, scalar) const
{
    return 0.0;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void CML::isobaricPerfectGas::operator+=(const isobaricPerfectGas& pg)
{
    specie::operator+=(pg);
}


inline void CML::isobaricPerfectGas::operator-=(const isobaricPerfectGas& pg)
{
    specie::operator-=(pg);
}


inline void CML::isobaricPerfectGas::operator*=(const scalar s)
{
    specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline CML::isobaricPerfectGas CML::operator+
(
    const isobaricPerfectGas& pg1,
    const isobaricPerfectGas& pg2
)
{
    return isobaricPerfectGas
    (
        static_cast<const specie&>(pg1)
      + static_cast<const specie&>(pg2)
    );
}


inline CML::isobaricPerfectGas CML::operator-
(
    const isobaricPerfectGas& pg1,
    const isobaricPerfectGas& pg2
)
{
    return isobaricPerfectGas
    (
        static_cast<const specie&>(pg1)
      - static_cast<const specie&>(pg2)
    );
}


inline CML::isobaricPerfectGas CML::operator*
(
    const scalar s,
    const isobaricPerfectGas& pg
)
{
    return isobaricPerfectGas(s*static_cast<const specie&>(pg));
}


inline CML::isobaricPerfectGas CML::operator==
(
    const isobaricPerfectGas& pg1,
    const isobaricPerfectGas& pg2
)
{
    return pg2 - pg1;
}


// ************************************************************************* //
