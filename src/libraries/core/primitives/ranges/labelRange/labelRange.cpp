/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2017 OpenFOAM Foundation
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

#include "labelRange.hpp"
#include "token.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const CML::labelRange CML::labelRange::endLabelRange_;

const CML::labelRange::const_iterator CML::labelRange::endIter_;

int CML::labelRange::debug(::CML::debug::debugSwitch("labelRange", 0));


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::labelRange::labelRange(Istream& is)
:
    start_(0),
    size_(0)
{
    is  >> *this;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::labelRange::intersects
(
    const labelRange& range,
    const bool touches
) const
{
    label final = touches ? 1 : 0;

    return
    (
        this->size()
     && range.size()
     &&
        (
            (
                range.first() >= this->first()
             && range.first() <= this->last() + final
            )
         ||
            (
                this->first() >= range.first()
             && this->first() <= range.last() + final
            )
        )
    );
}


CML::labelRange CML::labelRange::join(const labelRange& range) const
{
    // trivial cases first
    if (!size_)
    {
        return *this;
    }
    else if (!range.size_)
    {
        return range;
    }

    const label lower = CML::min(this->first(), range.first());
    const label upper = CML::max(this->last(),  range.last());
    const label sz = upper - lower + 1;

    return labelRange(lower, sz);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

CML::labelRange& CML::labelRange::operator+=(const labelRange& rhs)
{
    if (!size_)
    {
        // trivial case
        operator=(rhs);
    }
    else if (rhs.size_)
    {
        const label lower = CML::min(this->first(), rhs.first());
        const label upper = CML::max(this->last(),  rhs.last());

        start_ = lower;
        size_  = upper - lower + 1;
    }

    return *this;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, labelRange& range)
{
    is.readBegin("labelRange");
    is  >> range.start_ >> range.size_;
    is.readEnd("labelRange");

    is.check("operator>>(Istream&, labelRange&)");

    // disallow invalid sizes
    if (range.size_ <= 0)
    {
        range.clear();
    }

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const labelRange& range)
{
    // write ASCII only for now
    os  << token::BEGIN_LIST
        << range.start_ << token::SPACE << range.size_
        << token::END_LIST;

//    os  << token::BEGIN_BLOCK
//        << range.start_ << "-" << range.last()
//        << token::END_BLOCK;

    os.check("operator<<(Ostream&, const labelRange&)");
    return os;
}


// ************************************************************************* //
