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

    function for nu.  All other properties are derived from
    this primitive function.

\*---------------------------------------------------------------------------*/

#include "speciesTransport.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::speciesTransport::speciesTransport(Istream& is)
:
    janafThermo(is)
{
    is.check("speciesTransport::speciesTransport(Istream& is)");
}


CML::speciesTransport::speciesTransport(const dictionary& dict)
:
    janafThermo(dict)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::speciesTransport::write(Ostream& os) const
{
    os  << this->name() << endl;
    os  << token::BEGIN_BLOCK << incrIndent << nl;
    janafThermo::write(os);
    os  << decrIndent << token::END_BLOCK << nl;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const speciesTransport& sTranport)
{
    os << (const janafThermo&)sTranport;

    os.check("Ostream& operator<<(Ostream&, const speciesTransport)");

    return os;
}


// ************************************************************************* //
