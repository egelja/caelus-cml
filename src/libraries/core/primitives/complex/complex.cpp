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

#include "complex.hpp"
#include "IOstreams.hpp"

#include <sstream>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const char* const CML::complex::typeName = "complex";
const CML::complex CML::complex::zero(0, 0);
const CML::complex CML::complex::one(1, 1);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::complex::complex(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::word CML::name(const complex& c)
{
    std::ostringstream buf;
    buf << '(' << c.Re() << ',' << c.Im() << ')';
    return buf.str();
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, complex& c)
{
    // Read beginning of complex
    is.readBegin("complex");

    is  >> c.re >> c.im;

    // Read end of complex
    is.readEnd("complex");

    // Check state of Istream
    is.check("operator>>(Istream&, complex&)");

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const complex& c)
{
    os  << token::BEGIN_LIST
        << c.re << token::SPACE << c.im
        << token::END_LIST;

    return os;
}


// ************************************************************************* //
