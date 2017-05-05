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

#include "instantList.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const char* const CML::instant::typeName = "instant";

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::instant::instant()
{}

CML::instant::instant(const scalar val, const word& tname)
:
    value_(val),
    name_(tname)
{}

CML::instant::instant(const scalar val)
:
    value_(val),
    name_(Time::timeName(val))
{}

CML::instant::instant(const word& tname)
:
    value_(atof(tname.c_str())),
    name_(tname)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::instant::equal(const scalar b) const
{
    return (value_ < b + SMALL  && value_ > b - SMALL);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

bool CML::operator==(const instant& a, const instant& b)
{
    return a.equal(b.value_);
}


bool CML::operator!=(const instant& a, const instant& b)
{
    return !operator==(a, b);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, instant& I)
{
    is >> I.value_ >> I.name_;

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const instant& I)
{
   os << I.value_ << tab << I.name_;

   return os;
}


// ************************************************************************* //
