/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "error.hpp"

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

inline CML::DLListBase::link::link()
:
    prev_(0),
    next_(0)
{}


inline CML::DLListBase::DLListBase()
:
    first_(0),
    last_(0),
    nElmts_(0)
{}


inline CML::DLListBase::DLListBase(link* a)
:
    first_(a),
    last_(a),
    nElmts_(1)
{
    a->prev_ = a;
    a->next_ = a;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

inline CML::DLListBase::~DLListBase()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool CML::DLListBase::link::registered() const
{
    return prev_ != 0 && next_ != 0;
}


inline void CML::DLListBase::link::deregister()
{
    prev_ = 0;
    next_ = 0;
}


inline CML::label CML::DLListBase::size() const
{
    return nElmts_;
}


inline bool CML::DLListBase::empty() const
{
    return !nElmts_;
}


inline CML::DLListBase::link*
CML::DLListBase::first()
{
    if (!nElmts_)
    {
        FatalErrorInFunction
            << "list is empty"
            << abort(FatalError);
    }
    return first_;
}


inline const CML::DLListBase::link*
CML::DLListBase::first() const
{
    if (!nElmts_)
    {
        FatalErrorInFunction
            << "list is empty"
            << abort(FatalError);
    }
    return first_;
}


inline CML::DLListBase::link*
CML::DLListBase::last()
{
    if (!nElmts_)
    {
        FatalErrorInFunction
            << "list is empty"
            << abort(FatalError);
    }
    return last_;
}


inline const CML::DLListBase::link*
CML::DLListBase::last() const
{
    if (!nElmts_)
    {
        FatalErrorInFunction
            << "list is empty"
            << abort(FatalError);
    }
    return last_;
}


inline void CML::DLListBase::clear()
{
    first_ = 0;
    last_  = 0;
    nElmts_ = 0;
}


inline void CML::DLListBase::transfer(DLListBase& lst)
{
    first_  = lst.first_;
    last_   = lst.last_;
    nElmts_ = lst.nElmts_;

    lst.clear();
}


inline CML::DLListBase::link*
CML::DLListBase::remove
(
    DLListBase::iterator& it
)
{
    return remove(it.curElmt_);
}


inline CML::DLListBase::link*
CML::DLListBase::replace
(
    DLListBase::iterator& oldIter,
    DLListBase::link* newLink
)
{
    return replace(oldIter.curElmt_, newLink);
}


// * * * * * * * * * * * * * * * STL iterator  * * * * * * * * * * * * * * * //

inline CML::DLListBase::iterator::iterator(DLListBase& s, link* elmt)
:
    curList_(s),
    curElmt_(elmt),
    curLink_(*curElmt_)
{}


inline CML::DLListBase::iterator::iterator(DLListBase& s)
:
    curList_(s),
    curElmt_(nullptr),
    curLink_()
{}


inline void CML::DLListBase::iterator::operator=(const iterator& iter)
{
    curElmt_ = iter.curElmt_;
    curLink_ = iter.curLink_;
}


inline bool CML::DLListBase::iterator::operator==(const iterator& iter) const
{
    return curElmt_ == iter.curElmt_;
}


inline bool CML::DLListBase::iterator::operator!=(const iterator& iter) const
{
    return curElmt_ != iter.curElmt_;
}


inline CML::DLListBase::link&
CML::DLListBase::iterator::operator*()
{
    return *curElmt_;
}


inline CML::DLListBase::iterator&
CML::DLListBase::iterator::operator++()
{
    // Check if the curElmt_ is the last element (if it points to itself)
    // or if the list is empty because the last element may have been removed
    if (curLink_.next_ == curElmt_ || curList_.last_ == 0)
    {
        curElmt_ = 0;
    }
    else
    {
        curElmt_ = curLink_.next_;
        curLink_ = *curElmt_;
    }

    return *this;
}


inline CML::DLListBase::iterator
CML::DLListBase::iterator::operator++(int)
{
    iterator tmp = *this;
    ++*this;
    return tmp;
}


inline CML::DLListBase::iterator
CML::DLListBase::begin()
{
    if (size())
    {
        return iterator(*this, first());
    }
    else
    {
        return endIter_;
    }
}


inline const CML::DLListBase::iterator& CML::DLListBase::end()
{
    return endIter_;
}


// * * * * * * * * * * * * * * STL const_iterator  * * * * * * * * * * * * * //

inline CML::DLListBase::const_iterator::const_iterator
(
    const DLListBase& s,
    const link* elmt
)
:
    curList_(s),
    curElmt_(elmt)
{}


inline CML::DLListBase::const_iterator::const_iterator(const iterator& iter)
:
    curList_(iter.curList_),
    curElmt_(iter.curElmt_)
{}


inline void CML::DLListBase::const_iterator::operator=
(
    const const_iterator& iter
)
{
    curElmt_ = iter.curElmt_;
}


inline bool CML::DLListBase::const_iterator::operator==
(
    const const_iterator& iter
) const
{
    return curElmt_ == iter.curElmt_;
}


inline bool CML::DLListBase::const_iterator::operator!=
(
    const const_iterator& iter
) const
{
    return curElmt_ != iter.curElmt_;
}


inline const CML::DLListBase::link&
CML::DLListBase::const_iterator::operator*()
{
    return *curElmt_;
}


inline CML::DLListBase::const_iterator&
CML::DLListBase::const_iterator::operator++()
{
    if (curElmt_ == curList_.last_)
    {
        curElmt_ = 0;
    }
    else
    {
        curElmt_ = curElmt_->next_;
    }

    return *this;
}


inline CML::DLListBase::const_iterator
CML::DLListBase::const_iterator::operator++(int)
{
    const_iterator tmp = *this;
    ++*this;
    return tmp;
}


inline CML::DLListBase::const_iterator
CML::DLListBase::cbegin() const
{
    if (size())
    {
        return const_iterator(*this, first());
    }
    else
    {
        return endConstIter_;
    }
}


inline const CML::DLListBase::const_iterator&
CML::DLListBase::cend() const
{
    return endConstIter_;
}


inline CML::DLListBase::const_iterator
CML::DLListBase::begin() const
{
    return this->cbegin();
}


inline const CML::DLListBase::const_iterator&
CML::DLListBase::end() const
{
    return endConstIter_;
}


// ************************************************************************* //
