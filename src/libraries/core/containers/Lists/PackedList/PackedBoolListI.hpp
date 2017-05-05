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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::PackedBoolList::PackedBoolList()
:
    PackedList<1>()
{}


inline CML::PackedBoolList::PackedBoolList(const label size)
:
    PackedList<1>(size)
{}


inline CML::PackedBoolList::PackedBoolList
(
    const label size,
    const bool val
)
:
    PackedList<1>(size, (val ? 1u : 0u))
{}


inline CML::PackedBoolList::PackedBoolList(const PackedBoolList& lst)
:
    PackedList<1>(lst)
{}


inline CML::PackedBoolList::PackedBoolList(const PackedList<1>& lst)
:
    PackedList<1>(lst)
{}


inline CML::PackedBoolList::PackedBoolList(const Xfer<PackedBoolList>& lst)
:
    PackedList<1>()
{
    transfer(lst());
}


inline CML::PackedBoolList::PackedBoolList(const Xfer<PackedList<1> >& lst)
:
    PackedList<1>(lst)
{}


inline CML::PackedBoolList::PackedBoolList(const CML::UList<bool>& lst)
:
    PackedList<1>()
{
    operator=(lst);
}


inline CML::PackedBoolList::PackedBoolList(const labelUList& indices)
:
    PackedList<1>(indices.size(), 0u)
{
    set(indices);
}


inline CML::PackedBoolList::PackedBoolList(const UIndirectList<label>& indices)
:
    PackedList<1>(indices.size(), 0u)
{
    set(indices);
}


inline CML::autoPtr<CML::PackedBoolList>
CML::PackedBoolList::clone() const
{
    return autoPtr<PackedBoolList>(new PackedBoolList(*this));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline void CML::PackedBoolList::transfer(PackedBoolList& lst)
{
    PackedList<1>::transfer(static_cast<PackedList<1>&>(lst));
}


inline void CML::PackedBoolList::transfer(PackedList<1>& lst)
{
    PackedList<1>::transfer(lst);
}


inline CML::Xfer<CML::PackedBoolList> CML::PackedBoolList::xfer()
{
    return xferMove(*this);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline CML::PackedBoolList&
CML::PackedBoolList::operator=(const bool val)
{
    PackedList<1>::operator=(val);
    return *this;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator=(const PackedBoolList& lst)
{
    PackedList<1>::operator=(lst);
    return *this;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator=(const PackedList<1>& lst)
{
    PackedList<1>::operator=(lst);
    return *this;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator=(const labelUList& indices)
{
    clear();
    set(indices);

    return *this;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator=(const UIndirectList<label>& indices)
{
    clear();
    set(indices);

    return *this;
}


inline CML::PackedBoolList
CML::PackedBoolList::operator~() const
{
    PackedBoolList result(*this);
    result.flip();

    return result;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator&=(const PackedList<1>& lst)
{
    subset(lst);
    return *this;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator&=(const labelUList& indices)
{
    subset(indices);
    return *this;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator&=(const UIndirectList<label>& indices)
{
    subset(indices);
    return *this;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator|=(const PackedList<1>& lst)
{
    set(lst);
    return *this;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator|=(const labelUList& indices)
{
    set(indices);
    return *this;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator|=(const UIndirectList<label>& indices)
{
    set(indices);
    return *this;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator+=(const PackedList<1>& lst)
{
    return operator|=(lst);
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator+=(const labelUList& indices)
{
    return operator|=(indices);
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator+=(const UIndirectList<label>& indices)
{
    return operator|=(indices);
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator-=(const PackedList<1>& lst)
{
    unset(lst);
    return *this;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator-=(const labelUList& indices)
{
    unset(indices);
    return *this;
}


inline CML::PackedBoolList&
CML::PackedBoolList::operator-=(const UIndirectList<label>& indices)
{
    unset(indices);
    return *this;
}


// ************************************************************************* //
