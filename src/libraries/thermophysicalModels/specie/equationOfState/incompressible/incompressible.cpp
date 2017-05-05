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
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::incompressible::incompressible(Istream& is)
:
    specie(is),
    rho_(readScalar(is))
{
    is.check("incompressible::incompressible(Istream& is)");
}


CML::incompressible::incompressible(const dictionary& dict)
:
    specie(dict),
    rho_(readScalar(dict.subDict("equationOfState").lookup("rho")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::incompressible::write(Ostream& os) const
{
    specie::write(os);

    dictionary dict("equationOfState");
    dict.add("rho", rho_);

    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const incompressible& ico)
{
    os  << static_cast<const specie&>(ico)
        << token::SPACE << ico.rho_;

    os.check("Ostream& operator<<(Ostream& os, const incompressible& ico)");
    return os;
}


// ************************************************************************* //
