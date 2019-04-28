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

Class
    CML::DynamicList

Description
    A 1D vector of objects of type \<T\> that resizes itself as necessary to
    accept the new objects.

    Internal storage is a compact array and the list can be shrunk to compact
    storage. The increase of list size is controlled by three template
    parameters, which allows the list storage to either increase by the given
    increment or by the given multiplier and divider (allowing non-integer
    multiples).

SourceFiles
    DynamicListI.hpp
    DynamicList.cpp

\*---------------------------------------------------------------------------*/

#ifndef DynamicList_H
#define DynamicList_H

#include "List.hpp"
#include "StaticAssert.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
class DynamicList;

template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
Ostream& operator<<
(
    Ostream&,
    const DynamicList<T, SizeInc, SizeMult, SizeDiv>&
);
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
Istream& operator>>
(
    Istream&,
    DynamicList<T, SizeInc, SizeMult, SizeDiv>&
);


/*---------------------------------------------------------------------------*\
                           Class DynamicList Declaration
\*---------------------------------------------------------------------------*/

template<class T, unsigned SizeInc=0, unsigned SizeMult=2, unsigned SizeDiv=1>
class DynamicList
:
    public List<T>
{
    //- Avoid invalid sizing parameters
    StaticAssert((SizeInc || SizeMult) && SizeDiv);

    // Private data

        //- The capacity (allocated size) of the underlying list.
        label capacity_;


public:

    // Related types

        //- Declare friendship with the List class
        friend class List<T>;


    // Constructors

        //- Construct null
        inline DynamicList();

        //- Construct given size.
        explicit inline DynamicList(const label);

        //- Construct copy.
        inline DynamicList(const DynamicList<T, SizeInc, SizeMult, SizeDiv>&);

        //- Construct from UList. Size set to UList size.
        //  Also constructs from DynamicList with different sizing parameters.
        explicit inline DynamicList(const UList<T>&);

        //- Construct from UIndirectList. Size set to UIndirectList size.
        explicit inline DynamicList(const UIndirectList<T>&);

        //- Construct by transferring the parameter contents
        explicit inline DynamicList(const Xfer<List<T> >&);

        //- Construct from Istream. Size set to size of list read.
        explicit DynamicList(Istream&);


    // Member Functions

        // Access

            //- Size of the underlying storage.
            inline label capacity() const;

        // Edit

            //- Alter the size of the underlying storage.
            //  The addressed size will be truncated if needed to fit, but will
            //  remain otherwise untouched.
            //  Use this or reserve() in combination with append().
            inline void setCapacity(const label);

            //- Alter the addressed list size.
            //  New space will be allocated if required.
            //  Use this to resize the list prior to using the operator[] for
            //  setting values (as per List usage).
            inline void setSize(const label);

            //- Alter the addressed list size and fill new space with a
            //  constant.
            inline void setSize(const label, const T&);

            //- Alter the addressed list size.
            //  New space will be allocated if required.
            //  Use this to resize the list prior to using the operator[] for
            //  setting values (as per List usage).
            inline void resize(const label);

            //- Alter the addressed list size and fill new space with a
            //  constant.
            inline void resize(const label, const T&);

            //- Reserve allocation space for at least this size.
            //  Never shrinks the allocated size, use setCapacity() for that.
            inline void reserve(const label);

            //- Clear the addressed list, i.e. set the size to zero.
            //  Allocated size does not change
            inline void clear();

            //- Clear the list and delete storage.
            inline void clearStorage();

            //- Shrink the allocated space to the number of elements used.
            //  Returns a reference to the DynamicList.
            inline DynamicList<T, SizeInc, SizeMult, SizeDiv>& shrink();

            //- Transfer contents of the argument List into this.
            inline void transfer(List<T>&);

            //- Transfer contents of the argument DynamicList into this.
            inline void transfer(DynamicList<T, SizeInc, SizeMult, SizeDiv>&);

            //- Transfer contents to the Xfer container as a plain List
            inline Xfer<List<T> > xfer();


        // Member Operators

            //- Append an element at the end of the list
            inline DynamicList<T, SizeInc, SizeMult, SizeDiv>& append
            (
                const T&
            );

            //- Append a List at the end of this list
            inline DynamicList<T, SizeInc, SizeMult, SizeDiv>& append
            (
                const UList<T>&
            );

            //- Append a UIndirectList at the end of this list
            inline DynamicList<T, SizeInc, SizeMult, SizeDiv>& append
            (
                const UIndirectList<T>&
            );

            //- Remove and return the top element
            inline T remove();

            //- Return non-const access to an element, resizing list if
            //  necessary
            inline T& operator()(const label);

            //- Assignment of all addressed entries to the given value
            inline void operator=(const T&);

            //- Assignment from DynamicList
            inline void operator=
            (
                const DynamicList<T, SizeInc, SizeMult, SizeDiv>&
            );

            //- Assignment from UList
            inline void operator=(const UList<T>&);

            //- Assignment from UIndirectList
            inline void operator=(const UIndirectList<T>&);



        // IOstream operators

            // Write DynamicList to Ostream.
            friend Ostream& operator<< <T, SizeInc, SizeMult, SizeDiv>
            (
                Ostream&,
                const DynamicList<T, SizeInc, SizeMult, SizeDiv>&
            );

            //- Read from Istream, discarding contents of existing DynamicList.
            friend Istream& operator>> <T, SizeInc, SizeMult, SizeDiv>
            (
                Istream&,
                DynamicList<T, SizeInc, SizeMult, SizeDiv>&
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::DynamicList()
:
    List<T>(0),
    capacity_(0)
{
    List<T>::size(0);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::DynamicList
(
    const label nElem
)
:
    List<T>(nElem),
    capacity_(nElem)
{
    // we could also enforce SizeInc granularity when (!SizeMult || !SizeDiv)
    List<T>::size(0);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::DynamicList
(
    const DynamicList<T, SizeInc, SizeMult, SizeDiv>& lst
)
:
    List<T>(lst),
    capacity_(lst.size())
{}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::DynamicList
(
    const UList<T>& lst
)
:
    List<T>(lst),
    capacity_(lst.size())
{}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::DynamicList
(
    const UIndirectList<T>& lst
)
:
    List<T>(lst),
    capacity_(lst.size())
{}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::DynamicList
(
    const Xfer<List<T> >& lst
)
:
    List<T>(lst),
    capacity_(List<T>::size())
{}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::label CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::capacity()
const
{
    return capacity_;
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::setCapacity
(
    const label nElem
)
{
    label nextFree = List<T>::size();
    capacity_ = nElem;

    if (nextFree > capacity_)
    {
        // truncate addressed sizes too
        nextFree = capacity_;
    }
    // we could also enforce SizeInc granularity when (!SizeMult || !SizeDiv)

    List<T>::setSize(capacity_);
    List<T>::size(nextFree);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::reserve
(
    const label nElem
)
{
    // allocate more capacity?
    if (nElem > capacity_)
    {
// TODO: convince the compiler that division by zero does not occur
//        if (SizeInc && (!SizeMult || !SizeDiv))
//        {
//            // resize with SizeInc as the granularity
//            capacity_ = nElem;
//            unsigned pad = SizeInc - (capacity_ % SizeInc);
//            if (pad != SizeInc)
//            {
//                capacity_ += pad;
//            }
//        }
//        else
        {
            capacity_ = max
            (
                nElem,
                label(SizeInc + capacity_ * SizeMult / SizeDiv)
            );
        }

        // adjust allocated size, leave addressed size untouched
        label nextFree = List<T>::size();
        List<T>::setSize(capacity_);
        List<T>::size(nextFree);
    }
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::setSize
(
    const label nElem
)
{
    // allocate more capacity?
    if (nElem > capacity_)
    {
// TODO: convince the compiler that division by zero does not occur
//        if (SizeInc && (!SizeMult || !SizeDiv))
//        {
//            // resize with SizeInc as the granularity
//            capacity_ = nElem;
//            unsigned pad = SizeInc - (capacity_ % SizeInc);
//            if (pad != SizeInc)
//            {
//                capacity_ += pad;
//            }
//        }
//        else
        {
            capacity_ = max
            (
                nElem,
                label(SizeInc + capacity_ * SizeMult / SizeDiv)
            );
        }

        List<T>::setSize(capacity_);
    }

    // adjust addressed size
    List<T>::size(nElem);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::setSize
(
    const label nElem,
    const T& t
)
{
    label nextFree = List<T>::size();
    setSize(nElem);

    // set new elements to constant value
    while (nextFree < nElem)
    {
        this->operator[](nextFree++) = t;
    }
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::resize
(
    const label nElem
)
{
    this->setSize(nElem);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::resize
(
    const label nElem,
    const T& t
)
{
    this->setSize(nElem, t);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::clear()
{
    List<T>::size(0);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::clearStorage()
{
    List<T>::clear();
    capacity_ = 0;
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>&
CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::shrink()
{
    label nextFree = List<T>::size();
    if (capacity_ > nextFree)
    {
        // use the full list when resizing
        List<T>::size(capacity_);

        // the new size
        capacity_ = nextFree;
        List<T>::setSize(capacity_);
        List<T>::size(nextFree);
    }
    return *this;
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void
CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::transfer(List<T>& lst)
{
    capacity_ = lst.size();
    List<T>::transfer(lst);   // take over storage, clear addressing for lst.
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void
CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::transfer
(
    DynamicList<T, SizeInc, SizeMult, SizeDiv>& lst
)
{
    // take over storage as-is (without shrink), clear addressing for lst.
    capacity_ = lst.capacity_;
    lst.capacity_ = 0;

    List<T>::transfer(static_cast<List<T>&>(lst));
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::Xfer<CML::List<T> >
CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::xfer()
{
    return xferMoveTo< List<T> >(*this);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>&
CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::append
(
    const T& t
)
{
    const label elemI = List<T>::size();
    setSize(elemI + 1);

    this->operator[](elemI) = t;
    return *this;
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>&
CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::append
(
    const UList<T>& lst
)
{
    if (this == &lst)
    {
        FatalErrorInFunction
            << "attempted appending to self" << abort(FatalError);
    }

    label nextFree = List<T>::size();
    setSize(nextFree + lst.size());

    forAll(lst, elemI)
    {
        this->operator[](nextFree++) = lst[elemI];
    }
    return *this;
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>&
CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::append
(
    const UIndirectList<T>& lst
)
{
    label nextFree = List<T>::size();
    setSize(nextFree + lst.size());

    forAll(lst, elemI)
    {
        this->operator[](nextFree++) = lst[elemI];
    }
    return *this;
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline T CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::remove()
{
    const label elemI = List<T>::size() - 1;

    if (elemI < 0)
    {
        FatalErrorInFunction
            << "List is empty" << abort(FatalError);
    }

    const T& val = List<T>::operator[](elemI);

    List<T>::size(elemI);

    return val;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline T& CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::operator()
(
    const label elemI
)
{
    if (elemI >= List<T>::size())
    {
        setSize(elemI + 1);
    }

    return this->operator[](elemI);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::operator=
(
    const T& t
)
{
    UList<T>::operator=(t);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::operator=
(
    const DynamicList<T, SizeInc, SizeMult, SizeDiv>& lst
)
{
    if (this == &lst)
    {
        FatalErrorInFunction
            << "attempted assignment to self" << abort(FatalError);
    }

    if (capacity_ >= lst.size())
    {
        // can copy w/o reallocating, match initial size to avoid reallocation
        List<T>::size(lst.size());
        List<T>::operator=(lst);
    }
    else
    {
        // make everything available for the copy operation
        List<T>::size(capacity_);

        List<T>::operator=(lst);
        capacity_ = List<T>::size();
    }
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::operator=
(
    const UList<T>& lst
)
{
    if (capacity_ >= lst.size())
    {
        // can copy w/o reallocating, match initial size to avoid reallocation
        List<T>::size(lst.size());
        List<T>::operator=(lst);
    }
    else
    {
        // make everything available for the copy operation
        List<T>::size(capacity_);

        List<T>::operator=(lst);
        capacity_ = List<T>::size();
    }
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::operator=
(
    const UIndirectList<T>& lst
)
{
    if (capacity_ >= lst.size())
    {
        // can copy w/o reallocating, match initial size to avoid reallocation
        List<T>::size(lst.size());
        List<T>::operator=(lst);
    }
    else
    {
        // make everything available for the copy operation
        List<T>::size(capacity_);

        List<T>::operator=(lst);
        capacity_ = List<T>::size();
    }
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
CML::DynamicList<T, SizeInc, SizeMult, SizeDiv>::DynamicList(Istream& is)
:
    List<T>(is),
    capacity_(List<T>::size())
{}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const DynamicList<T, SizeInc, SizeMult, SizeDiv>& lst
)
{
    os << static_cast<const List<T>&>(lst);
    return os;
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
CML::Istream& CML::operator>>
(
    Istream& is,
    DynamicList<T, SizeInc, SizeMult, SizeDiv>& lst
)
{
    is >> static_cast<List<T>&>(lst);
    lst.capacity_ = lst.List<T>::size();

    return is;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
