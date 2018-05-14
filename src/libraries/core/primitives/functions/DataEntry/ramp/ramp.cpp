/*---------------------------------------------------------------------------*\
Copyright (C) 2017 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "ramp.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

void CML::DataEntryTypes::ramp::read(const dictionary& coeffs)
{
    start_ = coeffs.lookupOrDefault<scalar>("start", 0);
    duration_ = coeffs.lookupType<scalar>("duration");
}


CML::DataEntryTypes::ramp::ramp
(
    const word& entryName,
    const dictionary& dict
)
:
    DataEntry<scalar>(entryName)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::DataEntryTypes::ramp::~ramp()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::DataEntryTypes::ramp::writeData(Ostream& os) const
{
    DataEntry<scalar>::writeData(os);
    os  << token::END_STATEMENT << nl;
    os  << indent << word(this->name() + "Coeffs") << nl;
    os  << indent << token::BEGIN_BLOCK << incrIndent << nl;
    os.writeKeyword("start") << start_ << token::END_STATEMENT << nl;
    os.writeKeyword("duration") << duration_ << token::END_STATEMENT << nl;
    os  << decrIndent << indent << token::END_BLOCK << endl;
}


// ************************************************************************* //
