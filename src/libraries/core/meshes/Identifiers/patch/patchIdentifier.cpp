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

#include "patchIdentifier.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::patchIdentifier::patchIdentifier
(
    const word& name,
    const label index,
    const word& physicalType
)
:
    name_(name),
    index_(index),
    physicalType_(physicalType)
{}


CML::patchIdentifier::patchIdentifier
(
    const word& name,
    const dictionary& dict,
    const label index
)
:
    name_(name),
    index_(index)
{
    dict.readIfPresent("physicalType", physicalType_);
}


CML::patchIdentifier::patchIdentifier
(
    const patchIdentifier& p,
    const label index
)
:
    name_(p.name_),
    index_(index),
    physicalType_(p.physicalType_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::patchIdentifier::~patchIdentifier()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::patchIdentifier::write(Ostream& os) const
{
    if (physicalType_.size())
    {
        os.writeKeyword("physicalType") << physicalType_
            << token::END_STATEMENT << nl;
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const patchIdentifier& pi)
{
    pi.write(os);
    os.check("Ostream& operator<<(Ostream&, const patchIdentifier&)");
    return os;
}


// ************************************************************************* //
