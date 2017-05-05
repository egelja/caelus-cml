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

#include "incompressible.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline CML::incompressible::incompressible
(
    const specie& sp,
    const scalar rho
)
:
    specie(sp),
    rho_(rho)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::incompressible::incompressible
(
    const word& name,
    const incompressible& ico
)
:
    specie(name, ico),
    rho_(ico.rho_)
{}

inline CML::autoPtr<CML::incompressible>
CML::incompressible::clone() const
{
    return autoPtr<incompressible>(new incompressible(*this));
}

inline CML::autoPtr<CML::incompressible>
CML::incompressible::New(Istream& is)
{
    return autoPtr<incompressible>(new incompressible(is));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::scalar CML::incompressible::rho(scalar p, scalar T) const
{
    return rho_;
}

inline CML::scalar CML::incompressible::psi(scalar, scalar T) const
{
    return 0.0;
}

inline CML::scalar CML::incompressible::Z(scalar, scalar) const
{
    return 0.0;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void CML::incompressible::operator+=(const incompressible& ico)
{
    scalar molr1 = this->nMoles();

    specie::operator+=(ico);

    molr1 /= this->nMoles();
    scalar molr2 = ico.nMoles()/this->nMoles();

    rho_ = molr1*rho_ + molr2*ico.rho_;
}

inline void CML::incompressible::operator-=(const incompressible& ico)
{
    scalar molr1 = this->nMoles();

    specie::operator-=(ico);

    molr1 /= this->nMoles();
    scalar molr2 = ico.nMoles()/this->nMoles();

    rho_ = molr1*rho_ - molr2*ico.rho_;
}

inline void CML::incompressible::operator*=(const scalar s)
{
    specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline CML::incompressible CML::operator+
(
    const incompressible& ico1,
    const incompressible& ico2
)
{
    scalar nMoles = ico1.nMoles() + ico2.nMoles();
    scalar molr1 = ico1.nMoles()/nMoles;
    scalar molr2 = ico2.nMoles()/nMoles;

    return incompressible
    (
        static_cast<const specie&>(ico1)
      + static_cast<const specie&>(ico2),
        molr1*ico1.rho_ + molr2*ico2.rho_
    );
}

inline CML::incompressible CML::operator-
(
    const incompressible& ico1,
    const incompressible& ico2
)
{
    scalar nMoles = ico1.nMoles() + ico2.nMoles();
    scalar molr1 = ico1.nMoles()/nMoles;
    scalar molr2 = ico2.nMoles()/nMoles;

    return incompressible
    (
        static_cast<const specie&>(ico1)
      - static_cast<const specie&>(ico2),
        molr1*ico1.rho_ - molr2*ico2.rho_
    );
}

inline CML::incompressible CML::operator*
(
    const scalar s,
    const incompressible& ico
)
{
    return incompressible(s*static_cast<const specie&>(ico), ico.rho_);
}

inline CML::incompressible CML::operator==
(
    const incompressible& ico1,
    const incompressible& ico2
)
{
    return ico2 - ico1;
}


// ************************************************************************* //
