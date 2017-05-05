/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
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

#include "gradingDescriptor.hpp"
#include "IOstream.hpp"
#include "token.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::gradingDescriptor::gradingDescriptor()
:
    blockFraction_(1),
    nDivFraction_(1),
    expansionRatio_(1)
{}


CML::gradingDescriptor::gradingDescriptor
(
    const scalar blockFraction,
    const scalar nDivFraction,
    const scalar expansionRatio
)
:
    blockFraction_(blockFraction),
    nDivFraction_(nDivFraction),
    expansionRatio_(expansionRatio)
{}


CML::gradingDescriptor::gradingDescriptor
(
    const scalar expansionRatio
)
:
    blockFraction_(1.0),
    nDivFraction_(1.0),
    expansionRatio_(expansionRatio)
{}


CML::gradingDescriptor::gradingDescriptor(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::gradingDescriptor::~gradingDescriptor()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::gradingDescriptor CML::gradingDescriptor::inv() const
{
    return gradingDescriptor
    (
        blockFraction_,
        nDivFraction_,
        1.0/expansionRatio_
    );
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

bool CML::gradingDescriptor::operator==(const gradingDescriptor& gd) const
{
    return
        equal(blockFraction_, gd.blockFraction_)
     && equal(nDivFraction_, gd.nDivFraction_)
     && equal(expansionRatio_, gd.expansionRatio_);
}


bool CML::gradingDescriptor::operator!=(const gradingDescriptor& gd) const
{
    return !operator==(gd);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, gradingDescriptor& gd)
{
    // Examine next token
    token t(is);

    if (t.isNumber())
    {
        gd.blockFraction_ = 1.0;
        gd.nDivFraction_ = 1.0;
        gd.expansionRatio_ = t.number();
    }
    else if (t.isPunctuation() && t.pToken() == token::BEGIN_LIST)
    {
        is >> gd.blockFraction_ >> gd.nDivFraction_ >> gd.expansionRatio_;
        is.readEnd("gradingDescriptor");
    }

    // Check state of Istream
    is.check("operator>>(Istream&, gradingDescriptor&)");

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const gradingDescriptor& gd)
{
    if (equal(gd.blockFraction_, 1))
    {
        os  << gd.expansionRatio_;
    }
    else
    {
        os  << token::BEGIN_LIST
            << gd.blockFraction_ << token::SPACE
            << gd.nDivFraction_ << token::SPACE
            << gd.expansionRatio_
            << token::END_LIST;
    }

    return os;
}


// ************************************************************************* //
