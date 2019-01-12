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

Description
    Base singly-linked list.

\*---------------------------------------------------------------------------*/

#include "error.hpp"

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

inline CML::SLListBase::link::link()
:
    next_(0)
{}


inline CML::SLListBase::link::link(link* p)
:
    next_(p)
{}


inline CML::SLListBase::SLListBase()
:
    last_(0),
    nElmts_(0)
{}


inline CML::SLListBase::SLListBase(link* a)
:
    last_(a->next_ = a),
    nElmts_(1)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

inline CML::SLListBase::~SLListBase()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::SLListBase::size() const
{
    return nElmts_;
}


inline bool CML::SLListBase::empty() const
{
    return !nElmts_;
}


inline CML::SLListBase::link*
CML::SLListBase::first()
{
    if (!nElmts_)
    {
        FatalErrorInFunction
            << "list is empty"
            << abort(FatalError);
    }
    return last_->next_;
}


inline const CML::SLListBase::link*
CML::SLListBase::first() const
{
    if (!nElmts_)
    {
        FatalErrorInFunction
            << "list is empty"
            << abort(FatalError);
    }
    return last_->next_;
}


inline CML::SLListBase::link*
CML::SLListBase::last()
{
    if (!nElmts_)
    {
        FatalErrorInFunction
            << "list is empty"
            << abort(FatalError);
    }
    return last_;
}


inline const CML::SLListBase::link*
CML::SLListBase::last() const
{
    if (!nElmts_)
    {
        FatalErrorInFunction
            << "list is empty"
            << abort(FatalError);
    }
    return last_;
}


inline void CML::SLListBase::clear()
{
    last_ = 0;
    nElmts_ = 0;
}


inline void CML::SLListBase::transfer(SLListBase& lst)
{
    last_   = lst.last_;
    nElmts_ = lst.nElmts_;

    lst.clear();
}


inline CML::SLListBase::link* CML::SLListBase::remove
(
    SLListBase::iterator& it
)
{
    return remove(it.curElmt_);
}


// * * * * * * * * * * * * * * * STL iterator  * * * * * * * * * * * * * * * //

inline CML::SLListBase::iterator::iterator(SLListBase& s, link* elmt)
:
    curList_(s),
    curElmt_(elmt),
    curLink_(*curElmt_)
{}


inline CML::SLListBase::iterator::iterator(SLListBase& s)
:
    curList_(s),
    curElmt_(nullptr),
    curLink_()
{}


inline void CML::SLListBase::iterator::operator=(const iterator& iter)
{
    curElmt_ = iter.curElmt_;
    curLink_ = iter.curLink_;
}


inline bool CML::SLListBase::iterator::operator==(const iterator& iter) const
{
    return curElmt_ == iter.curElmt_;
}


inline bool CML::SLListBase::iterator::operator!=(const iterator& iter) const
{
    return curElmt_ != iter.curElmt_;
}


inline CML::SLListBase::link& CML::SLListBase::iterator::operator*()
{
    return *curElmt_;
}


inline CML::SLListBase::iterator& CML::SLListBase::iterator::operator++()
{
    if (curElmt_ == curList_.last_ || curList_.last_ == 0)
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


inline CML::SLListBase::iterator
CML::SLListBase::iterator::operator++(int)
{
    iterator tmp = *this;
    ++*this;
    return tmp;
}


inline CML::SLListBase::iterator
CML::SLListBase::begin()
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


inline const CML::SLListBase::iterator&
CML::SLListBase::end()
{
    return endIter_;
}


// * * * * * * * * * * * * * * STL const_iterator  * * * * * * * * * * * * * //

inline CML::SLListBase::const_iterator::const_iterator
(
    const SLListBase& s,
    const link* elmt
)
:
    curList_(s),
    curElmt_(elmt)
{}


inline CML::SLListBase::const_iterator::const_iterator(const iterator& iter)
:
    curList_(iter.curList_),
    curElmt_(iter.curElmt_)
{}


inline void CML::SLListBase::const_iterator::operator=
(
    const const_iterator& iter
)
{
    curElmt_ = iter.curElmt_;
}


inline bool CML::SLListBase::const_iterator::operator==
(
    const const_iterator& iter
) const
{
    return curElmt_ == iter.curElmt_;
}


inline bool CML::SLListBase::const_iterator::operator!=
(
    const const_iterator& iter
) const
{
    return curElmt_ != iter.curElmt_;
}


inline const CML::SLListBase::link&
CML::SLListBase::const_iterator::operator*()
{
    return *curElmt_;
}


inline CML::SLListBase::const_iterator&
CML::SLListBase::const_iterator::operator++()
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


inline CML::SLListBase::const_iterator
CML::SLListBase::const_iterator::operator++(int)
{
    const_iterator tmp = *this;
    ++*this;
    return tmp;
}


inline CML::SLListBase::const_iterator
CML::SLListBase::cbegin() const
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


inline const CML::SLListBase::const_iterator&
CML::SLListBase::cend() const
{
    return endConstIter_;
}


inline CML::SLListBase::const_iterator
CML::SLListBase::begin() const
{
    return this->cbegin();
}


inline const CML::SLListBase::const_iterator&
CML::SLListBase::end() const
{
    return endConstIter_;
}


// ************************************************************************* //
