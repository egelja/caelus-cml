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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::isobaricPerfectGas::isobaricPerfectGas
(
    const specie& sp,  const scalar pRef
)
:
    specie(sp),
    pRef_(pRef)
{}


inline CML::isobaricPerfectGas::isobaricPerfectGas
(
    const isobaricPerfectGas& ipg
)
:
    specie(ipg),
    pRef_(ipg.pRef_)
{}


inline CML::isobaricPerfectGas::isobaricPerfectGas
(
    const word& name,
    const isobaricPerfectGas& ipg
)
:
    specie(name, ipg),
    pRef_(ipg.pRef_)
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
    return 1.0/(this->R()*T);
}


inline CML::scalar CML::isobaricPerfectGas::Z(scalar, scalar) const
{
    return 1.0;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline CML::isobaricPerfectGas&
CML::isobaricPerfectGas::operator=(const isobaricPerfectGas& ipg)
{
    specie::operator=(ipg);

    pRef_ = ipg.pRef_;

    return *this;
}


inline void CML::isobaricPerfectGas::operator+=(const isobaricPerfectGas& ipg)
 {
    scalar molr1 = this->nMoles();

    specie::operator+=(ipg);

    molr1 /= this->nMoles();
    scalar molr2 = ipg.nMoles()/this->nMoles();

    pRef_ = molr1*pRef_ + molr2*ipg.pRef_;
 }
 
 
inline void CML::isobaricPerfectGas::operator-=(const isobaricPerfectGas& ipg)
 {
    specie::operator-=(ipg);

    pRef_ = ipg.pRef_;
 }


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline CML::isobaricPerfectGas CML::operator+
(
    const isobaricPerfectGas& ipg1,
    const isobaricPerfectGas& ipg2
)
{
    scalar nMoles = ipg1.nMoles() + ipg2.nMoles();
    scalar molr1 = ipg1.nMoles()/nMoles;
    scalar molr2 = ipg2.nMoles()/nMoles;

    return isobaricPerfectGas
    (
        static_cast<const specie&>(ipg1)
      + static_cast<const specie&>(ipg2),
        molr1*ipg1.pRef_ + molr2*ipg2.pRef_
    );
}


inline CML::isobaricPerfectGas CML::operator-
(
    const isobaricPerfectGas& ipg1,
    const isobaricPerfectGas& ipg2
)
{
    return isobaricPerfectGas
    (
        static_cast<const specie&>(ipg1)
      - static_cast<const specie&>(ipg2),
        ipg1.pRef_
    );
}


inline CML::isobaricPerfectGas CML::operator*
(
    const scalar s,
    const isobaricPerfectGas& ipg
)
{
    return isobaricPerfectGas(s*static_cast<const specie&>(ipg), ipg.pRef_);
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
