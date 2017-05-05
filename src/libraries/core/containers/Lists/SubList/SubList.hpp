/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM 
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

Class
    CML::SubList

Description
    A List obtained as a section of another List.

    Since the SubList is itself unallocated, no storage is allocated or
    de-allocated during its use.  To achieve this behaviour, SubList is
    derived from UList rather than List.


\*---------------------------------------------------------------------------*/

#ifndef SubList_H
#define SubList_H

#include "List.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class SubList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class SubList
:
    public UList<T>
{

public:

    // Static Member Functions

        //- Return a null SubList
        inline static const SubList<T>& null();


    // Constructors

        //- Construct from UList and sub-list size
        inline SubList
        (
            const UList<T>& list,
            const label subSize
        );

        //- Construct from UList, sub-list size and start index
        inline SubList
        (
            const UList<T>& list,
            const label subSize,
            const label startIndex
        );


    // Member operators

        //- Allow cast to a const List<T>&
        inline operator const CML::List<T>&() const;

        //- Assignment of all entries to the given value
        inline void operator=(const T&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
inline CML::SubList<T>::SubList
(
 const UList<T>& list,
 const label subSize
 )
:
UList<T>(list.v_, subSize)
{
#   ifdef FULLDEBUG
    list.checkSize(subSize);
#   endif
}


template<class T>
inline CML::SubList<T>::SubList
(
 const UList<T>& list,
 const label subSize,
 const label startIndex
 )
:
UList<T>(&(list.v_[startIndex]), subSize)
{
#   ifdef FULLDEBUG
    
    // Artificially allow the start of a zero-sized subList to be
    // one past the end of the original list.
    if (subSize)
    {
        list.checkStart(startIndex);
        list.checkSize(startIndex + subSize);
    }
    else
    {
        // Start index needs to fall between 0 and size.  One position
        // behind the last element is allowed
        list.checkSize(startIndex);
    }
#   endif
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline const CML::SubList<T>& CML::SubList<T>::null()
{
    return NullSingletonRef< SubList<T> >();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline CML::SubList<T>::operator const CML::List<T>&() const
{
    return *reinterpret_cast< const List<T>* >(this);
}


template<class T>
inline void CML::SubList<T>::operator=(const T& t)
{
    UList<T>::operator=(t);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
