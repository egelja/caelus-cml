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
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::isobaricPerfectGas::isobaricPerfectGas(Istream& is)
:
    specie(is),
    pRef_(readScalar(is))
{
    is.check("isobaricPerfectGas::isobaricPerfectGas(Istream& is)");
}


CML::isobaricPerfectGas::isobaricPerfectGas(const dictionary& dict)
:
    specie(dict),
    pRef_(readScalar(dict.subDict("equationOfState").lookup("pRef")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::isobaricPerfectGas::write(Ostream& os) const
{
    specie::write(os);
    dictionary dict("equationOfState");
    dict.add("pRef", pRef_);

    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const isobaricPerfectGas& pg)
{
    os  << static_cast<const specie&>(pg)
        << token::SPACE << pg.pRef_;

    os.check("Ostream& operator<<(Ostream& os, const isobaricPerfectGas& st)");
    return os;
}


// ************************************************************************* //
