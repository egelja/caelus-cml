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

#include "surfZoneIdentifier.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::surfZoneIdentifier::surfZoneIdentifier()
:
    name_(word::null),
    index_(0),
    geometricType_(word::null)
{}


CML::surfZoneIdentifier::surfZoneIdentifier
(
    const word& name,
    const label index,
    const word& geometricType
)
:
    name_(name),
    index_(index),
    geometricType_(geometricType)
{}


CML::surfZoneIdentifier::surfZoneIdentifier
(
    const word& name,
    const dictionary& dict,
    const label index
)
:
    name_(name),
    index_(index)
{
    dict.readIfPresent("geometricType", geometricType_);
}


CML::surfZoneIdentifier::surfZoneIdentifier
(
    const surfZoneIdentifier& p,
    const label index
)
:
    name_(p.name()),
    index_(index),
    geometricType_(p.geometricType())
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::surfZoneIdentifier::~surfZoneIdentifier()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void CML::surfZoneIdentifier::write(Ostream& os) const
{
    if (geometricType_.size())
    {
        os.writeKeyword("geometricType") << geometricType_
            << token::END_STATEMENT << nl;
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

// bool CML::surfZoneIdentifier::operator!=
// (
//     const surfZoneIdentifier& p
// ) const
// {
//     return !(*this == p);
// }
//
//
// bool CML::surfZoneIdentifier::operator==
// (
//     const surfZoneIdentifier& p
// ) const
// {
//     return geometricType() == p.geometricType() && name() == p.name();
// }


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

// CML::Istream& CML::operator>>(Istream& is, surfZoneIdentifier& p)
// {
//     is >> p.name_ >> p.geometricType_;
//
//     return is;
// }


CML::Ostream& CML::operator<<(Ostream& os, const surfZoneIdentifier& p)
{
    p.write(os);
    os.check
    (
        "Ostream& operator<<(Ostream&, const surfZoneIdentifier&)"
    );
    return os;
}


// ************************************************************************* //
