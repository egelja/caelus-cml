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

#include "hashedWordList.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::hashedWordList::rehash()
{
    indices_.clear();
    forAll(*this, i)
    {
        indices_.insert(List<word>::operator[](i), i);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::hashedWordList::hashedWordList()
:
    List<word>()
{}


CML::hashedWordList::hashedWordList(const UList<word>& names)
:
    List<word>(names)
{
    rehash();
}


CML::hashedWordList::hashedWordList(const hashedWordList& names)
:
    List<word>(static_cast<const UList<word>&>(names))
{
    rehash();
}


CML::hashedWordList::hashedWordList(const Xfer< List<word> >& names)
:
    List<word>(names)
{
    rehash();
}


CML::hashedWordList::hashedWordList
(
    const label nNames,
    const char** names
)
:
    List<word>(nNames)
{
    forAll(*this, i)
    {
        List<word>::operator[](i) = names[i];
    }

    rehash();
}


CML::hashedWordList::hashedWordList
(
    const char** names
)
{
    // count names
    label nNames = 0;
    for (unsigned i = 0; names[i] && *(names[i]); ++i)
    {
        ++nNames;
    }

    List<word>::setSize(nNames);
    forAll(*this, i)
    {
        List<word>::operator[](i) = names[i];
    }

    rehash();
}


CML::hashedWordList::hashedWordList(Istream& is)
{
    is  >> *this;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::hashedWordList::clear()
{
    List<word>::clear();
    indices_.clear();
}


void CML::hashedWordList::append(const word& name)
{
    const label idx = size();
    List<word>::append(name);
    indices_.insert(name, idx);
}


void CML::hashedWordList::transfer(List<word>& lst)
{
    List<word>::transfer(lst);
    rehash();
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, hashedWordList& lst)
{
    is  >> static_cast<List<word>&>(lst);
    lst.rehash();

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const hashedWordList& lst)
{
    os  << static_cast<const List<word>&>(lst);
    return os;
}


// ************************************************************************* //
