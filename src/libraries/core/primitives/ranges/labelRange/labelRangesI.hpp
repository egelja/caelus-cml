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

inline CML::labelRanges::labelRanges()
:
    ParentType()
{}


inline CML::labelRanges::labelRanges(const label nElem)
:
    ParentType(nElem)
{}


// * * * * * * * * * * * * * * * * Iterators * * * * * * * * * * * * * * * * //

inline CML::labelRanges::const_iterator::const_iterator()
:
   list_(endLabelRanges_),
   index_(-1),
   subIndex_(-1)
{}


inline CML::labelRanges::const_iterator::const_iterator(const labelRanges& lst)
:
   list_(lst),
   index_(0),
   subIndex_(0)
{
    if (list_.empty())
    {
        // equivalent to end iterator
        index_ = subIndex_ = -1;
    }
}


inline bool CML::labelRanges::const_iterator::operator==
(
    const const_iterator& iter
) const
{
    return
    (
        this->index_ == iter.index_
     && this->subIndex_ == iter.subIndex_
    );
}


inline bool CML::labelRanges::const_iterator::operator!=
(
    const const_iterator& iter
) const
{
    return !(this->operator==(iter));
}


inline CML::label CML::labelRanges::const_iterator::operator*()
{
    return list_[index_][subIndex_];
}


inline CML::label CML::labelRanges::const_iterator::operator()()
{
    return list_[index_][subIndex_];
}


inline CML::labelRanges::const_iterator&
CML::labelRanges::const_iterator::operator++()
{
    if (++subIndex_ >= list_[index_].size())
    {
        // go to next list entry
        subIndex_ = 0;
        if (++index_ >= list_.size())
        {
            // equivalent to end iterator
            index_ = subIndex_ = -1;
        }
    }

    return *this;
}


inline CML::labelRanges::const_iterator
CML::labelRanges::const_iterator::operator++(int)
{
    const_iterator old = *this;
    this->operator++();
    return old;
}


inline CML::labelRanges::const_iterator CML::labelRanges::cbegin() const
{
    return const_iterator(*this);
}


inline const CML::labelRanges::const_iterator& CML::labelRanges::cend() const
{
    return endIter_;
}


inline CML::labelRanges::const_iterator CML::labelRanges::begin() const
{
    return const_iterator(*this);
}


inline const CML::labelRanges::const_iterator& CML::labelRanges::end() const
{
    return endIter_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool CML::labelRanges::contains(const label value) const
{
    forAll(*this, i)
    {
        if (this->ParentType::operator[](i).contains(value))
        {
            return true;
        }
    }

    return false;
}


// ************************************************************************* //
