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

Class
    CML::UPtrList

Description
    A templated 1D list of pointers to objects of type \<T\>, where the
    size of the array is known and used for subscript bounds checking, etc.

    The element operator [] returns a reference to the object rather than a
    pointer.  Storage is not allocated during construction or use but is
    supplied to the constructor as an argument.

SourceFiles
    UPtrList.cpp
    UPtrListIO.cpp

\*---------------------------------------------------------------------------*/

#ifndef UPtrList_H
#define UPtrList_H

#include "List.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class T> class UPtrList;

template<class T>
inline typename UPtrList<T>::iterator operator+
(
    const typename UPtrList<T>::iterator&,
    label
);

template<class T>
inline typename UPtrList<T>::iterator operator+
(
    label,
    const typename UPtrList<T>::iterator&
);

template<class T>
inline typename UPtrList<T>::iterator operator-
(
    const typename UPtrList<T>::iterator&,
    label
);

template<class T>
inline label operator-
(
    const typename UPtrList<T>::iterator&,
    const typename UPtrList<T>::iterator&
);

template<class T>
Istream& operator>>(Istream&, UPtrList<T>&);

template<class T>
Ostream& operator<<(Ostream&, const UPtrList<T>&);


/*---------------------------------------------------------------------------*\
                           Class UPtrList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class UPtrList
{
    // Private data

        List<T*> ptrs_;


public:

    // Constructors

        //- Null Constructor.
        UPtrList();

        //- Construct with size specified.
        explicit UPtrList(const label);

        //- Construct by transferring the parameter contents
        UPtrList(const Xfer<UPtrList<T> >&);

        //- Construct as copy or re-use as specified.
        UPtrList(UPtrList<T>&, bool reUse);


    // Member functions

        // Access

            //- Return the number of elements in the UPtrList
            inline label size() const;

            //- Return true if the UPtrList is empty (ie, size() is zero).
            inline bool empty() const;

            //- Return reference to the first element of the list.
            inline T& first();

            //- Return reference to first element of the list.
            inline const T& first() const;

            //- Return reference to the last element of the list.
            inline T& last();

            //- Return reference to the last element of the list.
            inline const T& last() const;


        // Edit

            //- Reset size of UPtrList.  This can only be used to set the size
            //  of an empty UPtrList, extend a UPtrList, remove entries from
            //  the end of a UPtrList.
            void setSize(const label);

            //- Reset size of UPtrList.  This can only be used to set the size
            //  of an empty UPtrList, extend a UPtrList, remove entries from
            //  the end of a UPtrList.
            inline void resize(const label);

            //- Clear the UPtrList, i.e. set size to zero
            void clear();

            //- Transfer the contents of the argument UPtrList into this
            //  UPtrList and annul the argument list.
            void transfer(UPtrList<T>&);

            //- Transfer contents to the Xfer container
            inline Xfer<UPtrList<T> > xfer();

            //- Is element set
            inline bool set(const label) const;

            //- Set element. Return old element (can be nullptr).
            //  No checks on new element.
            inline T* set(const label, T*);

            //- Reorders elements. Ordering does not have to be done in
            //  ascending or descending order. Reordering has to be unique.
            //  (is shuffle)
            void reorder(const labelUList&);


    // Member operators

        //- Return element const reference.
        inline const T& operator[](const label) const;

        //- Return element reference.
        inline T& operator[](const label);

        //- Return element const pointer.
        inline const T* operator()(const label) const;


    // STL type definitions

        //- Type of values the UPtrList contains.
        typedef T value_type;

        //- Type that can be used for storing into UPtrList::value_type objects.
        typedef T& reference;

        //- Type that can be used for storing into constant UPtrList::value_type
        //  objects.
        typedef const T& const_reference;


    // STL iterator
    // Random access iterator for traversing UPtrList.

        class iterator;
        friend class iterator;

        //- An STL iterator
        class iterator
        {
            T** ptr_;

        public:

            //- Construct for a given UPtrList entry
            inline iterator(T**);

            // Member operators

                inline bool operator==(const iterator&) const;
                inline bool operator!=(const iterator&) const;

                inline T& operator*();
                inline T& operator()();

                inline iterator operator++();
                inline iterator operator++(int);

                inline iterator operator--();
                inline iterator operator--(int);

                inline iterator operator+=(label);

                friend iterator operator+ <T>(const iterator&, label);
                friend iterator operator+ <T>(label, const iterator&);

                inline iterator operator-=(label);

                friend iterator operator- <T>(const iterator&, label);

                friend label operator- <T>
                (
                    const iterator&,
                    const iterator&
                );

                inline T& operator[](label);

                inline bool operator<(const iterator&) const;
                inline bool operator>(const iterator&) const;

                inline bool operator<=(const iterator&) const;
                inline bool operator>=(const iterator&) const;
        };

        //- Return an iterator to begin traversing the UPtrList.
        inline iterator begin();

        //- Return an iterator to end traversing the UPtrList.
        inline iterator end();


    // IOstream operator

        // Write List to Ostream.
        friend Ostream& operator<< <T>(Ostream&, const UPtrList<T>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "error.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline CML::label CML::UPtrList<T>::size() const
{
    return ptrs_.size();
}


template<class T>
inline bool CML::UPtrList<T>::empty() const
{
    return ptrs_.empty();
}


template<class T>
inline T& CML::UPtrList<T>::first()
{
    return this->operator[](0);
}


template<class T>
inline const T& CML::UPtrList<T>::first() const
{
    return this->operator[](0);
}


template<class T>
inline T& CML::UPtrList<T>::last()
{
    return this->operator[](this->size()-1);
}


template<class T>
inline const T& CML::UPtrList<T>::last() const
{
    return this->operator[](this->size()-1);
}


template<class T>
inline void CML::UPtrList<T>::resize(const label newSize)
{
    this->setSize(newSize);
}


template<class T>
inline bool CML::UPtrList<T>::set(const label i) const
{
    return ptrs_[i] != nullptr;
}


template<class T>
inline T* CML::UPtrList<T>::set(const label i, T* ptr)
{
    T* old = ptrs_[i];
    ptrs_[i] = ptr;
    return old;
}


template<class T>
inline CML::Xfer<CML::UPtrList<T> > CML::UPtrList<T>::xfer()
{
    return xferMove(*this);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline const T& CML::UPtrList<T>::operator[](const label i) const
{
    if (!ptrs_[i])
    {
        FatalErrorInFunction
            << "hanging pointer, cannot dereference"
            << abort(FatalError);
    }

    return *(ptrs_[i]);
}


template<class T>
inline T& CML::UPtrList<T>::operator[](const label i)
{
    if (!ptrs_[i])
    {
        FatalErrorInFunction
            << "hanging pointer, cannot dereference"
            << abort(FatalError);
    }

    return *(ptrs_[i]);
}


template<class T>
inline const T* CML::UPtrList<T>::operator()(const label i) const
{
    return ptrs_[i];
}


// * * * * * * * * * * * * * * * * STL iterator  * * * * * * * * * * * * * * //

template<class T>
inline CML::UPtrList<T>::iterator::iterator(T** ptr)
:
    ptr_(ptr)
{}

template<class T>
inline bool CML::UPtrList<T>::iterator::operator==(const iterator& iter) const
{
    return ptr_ == iter.ptr_;
}

template<class T>
inline bool CML::UPtrList<T>::iterator::operator!=(const iterator& iter) const
{
    return ptr_ != iter.ptr_;
}

template<class T>
inline T& CML::UPtrList<T>::iterator::operator*()
{
    return **ptr_;
}

template<class T>
inline T& CML::UPtrList<T>::iterator::operator()()
{
    return operator*();
}

template<class T>
inline typename CML::UPtrList<T>::iterator
CML::UPtrList<T>::iterator::operator++()
{
    ++ptr_;
    return *this;
}

template<class T>
inline typename CML::UPtrList<T>::iterator
CML::UPtrList<T>::iterator::operator++(int)
{
    iterator tmp = *this;
    ++ptr_;
    return tmp;
}

template<class T>
inline typename CML::UPtrList<T>::iterator
CML::UPtrList<T>::iterator::operator--()
{
    --ptr_;
    return *this;
}

template<class T>
inline typename CML::UPtrList<T>::iterator
CML::UPtrList<T>::iterator::operator--(int)
{
    iterator tmp = *this;
    --ptr_;
    return tmp;
}

template<class T>
inline typename CML::UPtrList<T>::iterator
CML::UPtrList<T>::iterator::operator+=(label n)
{
    ptr_ += n;
    return *this;
}

template<class T>
inline typename CML::UPtrList<T>::iterator
CML::operator+(const typename UPtrList<T>::iterator& iter, label n)
{
    typename UPtrList<T>::iterator tmp = iter;
    return tmp += n;
}

template<class T>
inline typename CML::UPtrList<T>::iterator
CML::operator+(label n, const typename UPtrList<T>::iterator& iter)
{
    typename UPtrList<T>::iterator tmp = iter;
    return tmp += n;
}

template<class T>
inline typename CML::UPtrList<T>::iterator
CML::UPtrList<T>::iterator::operator-=(label n)
{
    ptr_ -= n;
    return *this;
}

template<class T>
inline typename CML::UPtrList<T>::iterator
CML::operator-(const typename UPtrList<T>::iterator& iter, label n)
{
    typename UPtrList<T>::iterator tmp = iter;
    return tmp -= n;
}

template<class T>
inline CML::label CML::operator-
(
    const typename UPtrList<T>::iterator& iter1,
    const typename UPtrList<T>::iterator& iter2
)
{
    return (iter1.ptr_ - iter2.ptr_)/sizeof(T*);
}

template<class T>
inline T& CML::UPtrList<T>::iterator::operator[](label n)
{
    return *(*this + n);
}

template<class T>
inline bool CML::UPtrList<T>::iterator::operator<(const iterator& iter) const
{
    return ptr_ < iter.ptr_;
}

template<class T>
inline bool CML::UPtrList<T>::iterator::operator>(const iterator& iter) const
{
    return ptr_ > iter.ptr_;
}

template<class T>
inline bool CML::UPtrList<T>::iterator::operator<=(const iterator& iter) const
{
    return ptr_ <= iter.ptr_;
}

template<class T>
inline bool CML::UPtrList<T>::iterator::operator>=(const iterator& iter) const
{
    return ptr_ >= iter.ptr_;
}

template<class T>
inline typename CML::UPtrList<T>::iterator
CML::UPtrList<T>::begin()
{
    return ptrs_.begin();
}

template<class T>
inline typename CML::UPtrList<T>::iterator
CML::UPtrList<T>::end()
{
    return ptrs_.end();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "error.hpp"


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class T>
CML::UPtrList<T>::UPtrList()
:
    ptrs_()
{}


template<class T>
CML::UPtrList<T>::UPtrList(const label s)
:
    ptrs_(s, reinterpret_cast<T*>(0))
{}


template<class T>
CML::UPtrList<T>::UPtrList(const Xfer<UPtrList<T> >& lst)
{
    transfer(lst());
}


template<class T>
CML::UPtrList<T>::UPtrList(UPtrList<T>& a, bool reUse)
:
    ptrs_(a.ptrs_, reUse)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
void CML::UPtrList<T>::setSize(const label newSize)
{
    label oldSize = size();

    if (newSize <= 0)
    {
        clear();
    }
    else if (newSize < oldSize)
    {
        ptrs_.setSize(newSize);
    }
    else if (newSize > oldSize)
    {
        ptrs_.setSize(newSize);

        // set new elements to nullptr
        for (register label i=oldSize; i<newSize; i++)
        {
            ptrs_[i] = nullptr;
        }
    }
}


template<class T>
void CML::UPtrList<T>::clear()
{
    ptrs_.clear();
}


template<class T>
void CML::UPtrList<T>::transfer(UPtrList<T>& a)
{
    ptrs_.transfer(a.ptrs_);
}


template<class T>
void CML::UPtrList<T>::reorder(const labelUList& oldToNew)
{
    if (oldToNew.size() != size())
    {
        FatalErrorInFunction
            << "Size of map (" << oldToNew.size()
            << ") not equal to list size (" << size()
            << ")." << abort(FatalError);
    }

    List<T*> newPtrs_(ptrs_.size(), reinterpret_cast<T*>(0));

    forAll(*this, i)
    {
        label newI = oldToNew[i];

        if (newI < 0 || newI >= size())
        {
            FatalErrorInFunction
                << "Illegal index " << newI << nl
                << "Valid indices are 0.." << size()-1
                << abort(FatalError);
        }

        if (newPtrs_[newI])
        {
            FatalErrorInFunction
                << "reorder map is not unique; element " << newI
                << " already set." << abort(FatalError);
        }
        newPtrs_[newI] = ptrs_[i];
    }

    forAll(newPtrs_, i)
    {
        if (!newPtrs_[i])
        {
            FatalErrorInFunction
                << "Element " << i << " not set after reordering." << nl
                << abort(FatalError);
        }
    }

    ptrs_.transfer(newPtrs_);
}


#include "Ostream.hpp"

// * * * * * * * * * * * * * * * Ostream Operators * * * * * * * * * * * * * //

template<class T>
CML::Ostream& CML::operator<<(Ostream& os, const UPtrList<T>& L)
{
    // Write size and start delimiter
    os << nl << L.size() << nl << token::BEGIN_LIST;

    // Write contents
    forAll(L, i)
    {
        os << nl << L[i];
    }

    // Write end delimiter
    os << nl << token::END_LIST << nl;

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const UPtrList&)");

    return os;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
