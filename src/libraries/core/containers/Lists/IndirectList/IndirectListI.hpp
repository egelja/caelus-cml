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


inline CML::IndirectListAddressing::IndirectListAddressing
(
    const labelUList& addr
)
:
    addressing_(addr)
{}


inline CML::IndirectListAddressing::IndirectListAddressing
(
    const Xfer<List<label> >& addr
)
:
    addressing_(addr)
{}


template<class T>
inline CML::IndirectList<T>::IndirectList
(
    const UList<T>& completeList,
    const labelUList& addr
)
:
    IndirectListAddressing(addr),
    UIndirectList<T>
    (
        completeList,
        IndirectListAddressing::addressing()
    )
{}


template<class T>
inline CML::IndirectList<T>::IndirectList
(
    const UList<T>& completeList,
    const Xfer<List<label> >& addr
)
:
    IndirectListAddressing(addr),
    UIndirectList<T>
    (
        completeList,
        IndirectListAddressing::addressing()
    )
{}


template<class T>
inline CML::IndirectList<T>::IndirectList
(
    const IndirectList<T>& lst
)
:
    IndirectListAddressing(lst.addressing()),
    UIndirectList<T>
    (
        lst.completeList(),
        IndirectListAddressing::addressing()
    )
{}


template<class T>
inline CML::IndirectList<T>::IndirectList
(
    const UIndirectList<T>& lst
)
:
    IndirectListAddressing(lst.addressing()),
    UIndirectList<T>
    (
        lst.completeList(),
        IndirectListAddressing::addressing()
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::List<CML::label>&
CML::IndirectListAddressing::addressing() const
{
    return addressing_;
}


inline void CML::IndirectListAddressing::resetAddressing
(
    const labelUList& addr
)
{
    addressing_ = addr;
}


inline void CML::IndirectListAddressing::resetAddressing
(
    const Xfer<List<label> >& addr
)
{
    addressing_.transfer(addr());
}


// ************************************************************************* //
