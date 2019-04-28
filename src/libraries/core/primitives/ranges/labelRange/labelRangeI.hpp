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


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::labelRange::labelRange()
:
    start_(0),
    size_(0)
{}


inline CML::labelRange::labelRange(const label start, const label size)
:
    start_(start),
    size_(size)
{
    // disallow invalid sizes
    if (size_ <= 0)
    {
        this->clear();
    }
}


// * * * * * * * * * * * * * * * * Iterators * * * * * * * * * * * * * * * * //

inline CML::labelRange::const_iterator::const_iterator()
:
   range_(endLabelRange_),
   index_(-1)
{}


inline CML::labelRange::const_iterator::const_iterator(const labelRange& range)
:
   range_(range),
   index_(range_.empty() ? -1 : 0)
{}


inline bool CML::labelRange::const_iterator::operator==
(
    const const_iterator& iter
) const
{
    return (this->index_ == iter.index_);
}


inline bool CML::labelRange::const_iterator::operator!=
(
    const const_iterator& iter
) const
{
    return !(this->operator==(iter));
}


inline CML::label CML::labelRange::const_iterator::operator*()
{
    return range_[index_];
}


inline CML::label CML::labelRange::const_iterator::operator()()
{
    return range_[index_];
}


inline CML::labelRange::const_iterator&
CML::labelRange::const_iterator::operator++()
{
    if (++index_ >= range_.size())
    {
        // equivalent to end iterator
        index_ = -1;
    }

    return *this;
}


inline CML::labelRange::const_iterator
CML::labelRange::const_iterator::operator++(int)
{
    const_iterator old = *this;
    this->operator++();
    return old;
}


inline CML::labelRange::const_iterator CML::labelRange::cbegin() const
{
    return const_iterator(*this);
}


inline const CML::labelRange::const_iterator& CML::labelRange::cend() const
{
    return endIter_;
}


inline CML::labelRange::const_iterator CML::labelRange::begin() const
{
    return const_iterator(*this);
}


inline const CML::labelRange::const_iterator& CML::labelRange::end() const
{
    return endIter_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline void CML::labelRange::clear()
{
    start_ = size_ = 0;
}


inline bool CML::labelRange::empty() const
{
    return !size_;
}


inline CML::label CML::labelRange::size() const
{
    return size_;
}


inline CML::label CML::labelRange::first() const
{
    return start_;
}


inline CML::label CML::labelRange::last() const
{
    return start_ + size_ - 1;
}


inline bool CML::labelRange::contains(const label value) const
{
    return value >= this->first() && value <= this->last();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline CML::label CML::labelRange::operator[](const label i) const
{
    return start_ + i;
}


inline bool CML::labelRange::operator<(const labelRange& rhs) const
{
    return this->first() < rhs.first();
}


inline bool CML::labelRange::operator==(const labelRange& rhs) const
{
    return start_ == rhs.start_ && size_ == rhs.size_;
}


inline bool CML::labelRange::operator!=(const labelRange& rhs) const
{
    return !(operator==(rhs));
}


// ************************************************************************* //
