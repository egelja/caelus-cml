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
    CML::PtrList

Description
    A templated 1D list of pointers to objects of type \<T\>, where the
    size of the array is known and used for subscript bounds checking, etc.

    The element operator [] returns a reference to the object rather than a
    pointer.

SourceFiles
    PtrList.cpp
    PtrListIO.cpp

\*---------------------------------------------------------------------------*/

#ifndef PtrList_H
#define PtrList_H

#include "List.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

template<class T> class SLPtrList;
template<class T> class autoPtr;
template<class T> class tmp;

// Forward declaration of friend functions and operators
template<class T> class PtrList;

template<class T>
inline typename PtrList<T>::iterator operator+
(
    const typename PtrList<T>::iterator&,
    label
);

template<class T>
inline typename PtrList<T>::iterator operator+
(
    label,
    const typename PtrList<T>::iterator&
);

template<class T>
inline typename PtrList<T>::iterator operator-
(
    const typename PtrList<T>::iterator&,
    label
);

template<class T>
inline label operator-
(
    const typename PtrList<T>::iterator&,
    const typename PtrList<T>::iterator&
);

template<class T>
Istream& operator>>(Istream&, PtrList<T>&);

template<class T>
Ostream& operator<<(Ostream&, const PtrList<T>&);




/*---------------------------------------------------------------------------*\
                           Class PtrList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class PtrList
{
    // Private data

        List<T*> ptrs_;


protected:

    // Protected Member Functions

        //- Read from Istream using given Istream constructor class
        template<class INew>
        void read(Istream&, const INew& inewt);


public:

    // Constructors

        //- Null Constructor
        PtrList();

        //- Construct with size specified
        explicit PtrList(const label);

        //- Copy constructor
        PtrList(const PtrList<T>&);

        //- Copy constructor with additional argument for clone
        template<class CloneArg>
        PtrList(const PtrList<T>&, const CloneArg&);

        //- Construct by transferring the parameter contents
        PtrList(const Xfer<PtrList<T> >&);

        //- Construct as copy or re-use as specified.
        PtrList(PtrList<T>&, bool reUse);

        //- Construct as copy of SLPtrList<T>
        explicit PtrList(const SLPtrList<T>&);

        //- Construct from Istream using given Istream constructor class
        template<class INew>
        PtrList(Istream&, const INew&);

        //- Construct from Istream using default Istream constructor class
        PtrList(Istream&);


    //- Destructor
    ~PtrList();


    // Member functions

        // Access

            //- Return the number of elements in the PtrList
            inline label size() const;

            //- Return true if the PtrList is empty (ie, size() is zero).
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

            //- Reset size of PtrList.  This can only be used to set the size
            //  of an empty PtrList, extend a PtrList, remove entries from
            //  the end of a PtrList. If the entries are non-empty they are
            //  deleted.
            void setSize(const label);

            //- Reset size of PtrList.  This can only be used to set the size
            //  of an empty PtrList, extend a PtrList, remove entries from
            //  the end of a PtrList. If the entries are non-empty they are
            //  deleted.
            inline void resize(const label);

            //- Clear the PtrList, i.e. set size to zero deleting all the
            //  allocated entries.
            void clear();

            //- Append an element at the end of the list
            inline void append(T*);
            inline void append(const autoPtr<T>&);
            inline void append(const tmp<T>&);

            //- Transfer the contents of the argument PtrList into this PtrList
            //  and annul the argument list.
            void transfer(PtrList<T>&);

            //- Transfer contents to the Xfer container
            inline Xfer<PtrList<T> > xfer();

            //- Is element set
            inline bool set(const label) const;

            //- Set element. Return old element (can be nullptr).
            //  No checks on new element.
            inline autoPtr<T> set(const label, T*);

            //- Set element to given autoPtr<T> and return old element
            inline autoPtr<T> set(const label, const autoPtr<T>&);

            //- Set element to given tmp<T> and return old element
            inline autoPtr<T> set(const label, const tmp<T>&);

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


        //- Assignment.
        PtrList<T>& operator=(const PtrList<T>&);


    // STL type definitions

        //- Type of values the PtrList contains.
        typedef T value_type;

        //- Type that can be used for storing into PtrList::value_type objects.
        typedef T& reference;

        //- Type that can be used for storing into constant PtrList::value_type
        //  objects.
        typedef const T& const_reference;


    // STL iterator
    // Random access iterator for traversing PtrList.

        class iterator;
        friend class iterator;

        //- An STL-conforming iterator
        class iterator
        {
            T** ptr_;

        public:

            //- Construct for a given PtrList entry
            inline iterator(T**);

            // Member operators

                inline bool operator==(const iterator&) const;
                inline bool operator!=(const iterator&) const;

                typedef T& Tref;
                inline Tref operator*();
                inline Tref operator()();

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

        //- Return an iterator to begin traversing the PtrList.
        inline iterator begin();

        //- Return an iterator to end traversing the PtrList.
        inline iterator end();


    // IOstream operator

        //- Read List from Istream, discarding contents of existing List.
        friend Istream& operator>> <T>(Istream&, PtrList<T>&);

        // Write List to Ostream.
        friend Ostream& operator<< <T>(Ostream&, const PtrList<T>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "error.hpp"

#include "autoPtr.hpp"
#include "tmp.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


template<class T>
inline CML::label CML::PtrList<T>::size() const
{
    return ptrs_.size();
}


template<class T>
inline bool CML::PtrList<T>::empty() const
{
    return ptrs_.empty();
}


template<class T>
inline T& CML::PtrList<T>::first()
{
    return this->operator[](0);
}


template<class T>
inline const T& CML::PtrList<T>::first() const
{
    return this->operator[](0);
}


template<class T>
inline T& CML::PtrList<T>::last()
{
    return this->operator[](this->size()-1);
}


template<class T>
inline const T& CML::PtrList<T>::last() const
{
    return this->operator[](this->size()-1);
}


template<class T>
inline void CML::PtrList<T>::resize(const label newSize)
{
    this->setSize(newSize);
}

template<class T>
inline void CML::PtrList<T>::append(T* ptr)
{
    label sz = this->size();
    this->setSize(sz+1);
    this->ptrs_[sz] = ptr;
}


template<class T>
inline void CML::PtrList<T>::append(const autoPtr<T>& aptr)
{
    return append(const_cast<autoPtr<T>&>(aptr).ptr());
}


template<class T>
inline void CML::PtrList<T>::append
(
    const tmp<T>& t
)
{
    return append(const_cast<tmp<T>&>(t).ptr());
}

template<class T>
inline bool CML::PtrList<T>::set(const label i) const
{
    return ptrs_[i] != nullptr;
}


template<class T>
inline CML::autoPtr<T> CML::PtrList<T>::set(const label i, T* ptr)
{
    autoPtr<T> old(ptrs_[i]);
    ptrs_[i] = ptr;
    return old;
}


template<class T>
inline CML::autoPtr<T> CML::PtrList<T>::set
(
    const label i,
    const autoPtr<T>& aptr
)
{
    return set(i, const_cast<autoPtr<T>&>(aptr).ptr());
}


template<class T>
inline CML::autoPtr<T> CML::PtrList<T>::set
(
    const label i,
    const tmp<T>& t
)
{
    return set(i, const_cast<tmp<T>&>(t).ptr());
}


template<class T>
inline CML::Xfer<CML::PtrList<T> > CML::PtrList<T>::xfer()
{
    return xferMove(*this);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline const T& CML::PtrList<T>::operator[](const label i) const
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
inline T& CML::PtrList<T>::operator[](const label i)
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
inline const T* CML::PtrList<T>::operator()(const label i) const
{
    return ptrs_[i];
}


// * * * * * * * * * * * * * * * * STL iterator  * * * * * * * * * * * * * * //

template<class T>
inline CML::PtrList<T>::iterator::iterator(T** ptr)
:
    ptr_(ptr)
{}


template<class T>
inline bool CML::PtrList<T>::iterator::operator==(const iterator& iter) const
{
    return ptr_ == iter.ptr_;
}


template<class T>
inline bool CML::PtrList<T>::iterator::operator!=(const iterator& iter) const
{
    return ptr_ != iter.ptr_;
}


template<class T>
inline T& CML::PtrList<T>::iterator::operator*()
{
    return **ptr_;
}


template<class T>
inline T& CML::PtrList<T>::iterator::operator()()
{
    return operator*();
}


template<class T>
inline typename CML::PtrList<T>::iterator
CML::PtrList<T>::iterator::operator++()
{
    ++ptr_;
    return *this;
}


template<class T>
inline typename CML::PtrList<T>::iterator
CML::PtrList<T>::iterator::operator++(int)
{
    iterator tmp = *this;
    ++ptr_;
    return tmp;
}


template<class T>
inline typename CML::PtrList<T>::iterator
CML::PtrList<T>::iterator::operator--()
{
    --ptr_;
    return *this;
}


template<class T>
inline typename CML::PtrList<T>::iterator
CML::PtrList<T>::iterator::operator--(int)
{
    iterator tmp = *this;
    --ptr_;
    return tmp;
}


template<class T>
inline typename CML::PtrList<T>::iterator
CML::PtrList<T>::iterator::operator+=(label n)
{
    ptr_ += n;
    return *this;
}


template<class T>
inline typename CML::PtrList<T>::iterator
CML::operator+(const typename PtrList<T>::iterator& iter, label n)
{
    typename PtrList<T>::iterator tmp = iter;
    return tmp += n;
}


template<class T>
inline typename CML::PtrList<T>::iterator
CML::operator+(label n, const typename PtrList<T>::iterator& iter)
{
    typename PtrList<T>::iterator tmp = iter;
    return tmp += n;
}


template<class T>
inline typename CML::PtrList<T>::iterator
CML::PtrList<T>::iterator::operator-=(label n)
{
    ptr_ -= n;
    return *this;
}


template<class T>
inline typename CML::PtrList<T>::iterator
CML::operator-(const typename PtrList<T>::iterator& iter, label n)
{
    typename PtrList<T>::iterator tmp = iter;
    return tmp -= n;
}


template<class T>
inline CML::label CML::operator-
(
    const typename PtrList<T>::iterator& iter1,
    const typename PtrList<T>::iterator& iter2
)
{
    return (iter1.ptr_ - iter2.ptr_)/sizeof(T*);
}


template<class T>
inline T& CML::PtrList<T>::iterator::operator[](label n)
{
    return *(*this + n);
}


template<class T>
inline bool CML::PtrList<T>::iterator::operator<(const iterator& iter) const
{
    return ptr_ < iter.ptr_;
}


template<class T>
inline bool CML::PtrList<T>::iterator::operator>(const iterator& iter) const
{
    return ptr_ > iter.ptr_;
}


template<class T>
inline bool CML::PtrList<T>::iterator::operator<=(const iterator& iter) const
{
    return ptr_ <= iter.ptr_;
}


template<class T>
inline bool CML::PtrList<T>::iterator::operator>=(const iterator& iter) const
{
    return ptr_ >= iter.ptr_;
}


template<class T>
inline typename CML::PtrList<T>::iterator CML::PtrList<T>::begin()
{
    return ptrs_.begin();
}


template<class T>
inline typename CML::PtrList<T>::iterator CML::PtrList<T>::end()
{
    return ptrs_.end();
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "error.hpp"

#include "PtrList.hpp"
#include "SLPtrList.hpp"

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class T>
CML::PtrList<T>::PtrList()
:
    ptrs_()
{}


template<class T>
CML::PtrList<T>::PtrList(const label s)
:
    ptrs_(s, reinterpret_cast<T*>(0))
{}


template<class T>
CML::PtrList<T>::PtrList(const PtrList<T>& a)
:
    ptrs_(a.size())
{
    forAll(*this, i)
    {
        ptrs_[i] = (a[i]).clone().ptr();
    }
}


template<class T>
template<class CloneArg>
CML::PtrList<T>::PtrList(const PtrList<T>& a, const CloneArg& cloneArg)
:
    ptrs_(a.size())
{
    forAll(*this, i)
    {
        ptrs_[i] = (a[i]).clone(cloneArg).ptr();
    }
}


template<class T>
CML::PtrList<T>::PtrList(const Xfer<PtrList<T> >& lst)
{
    transfer(lst());
}


template<class T>
CML::PtrList<T>::PtrList(PtrList<T>& a, bool reUse)
:
    ptrs_(a.size())
{
    if (reUse)
    {
        forAll(*this, i)
        {
            ptrs_[i] = a.ptrs_[i];
            a.ptrs_[i] = nullptr;
        }
        a.setSize(0);
    }
    else
    {
        forAll(*this, i)
        {
            ptrs_[i] = (a[i]).clone().ptr();
        }
    }
}


template<class T>
CML::PtrList<T>::PtrList(const SLPtrList<T>& sll)
:
    ptrs_(sll.size())
{
    if (sll.size())
    {
        label i = 0;
        for
        (
            typename SLPtrList<T>::const_iterator iter = sll.begin();
            iter != sll.end();
            ++iter
        )
        {
            ptrs_[i++] = (iter()).clone().ptr();
        }
    }
}


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

template<class T>
CML::PtrList<T>::~PtrList()
{
    forAll(*this, i)
    {
        if (ptrs_[i])
        {
            delete ptrs_[i];
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
void CML::PtrList<T>::setSize(const label newSize)
{
    if (newSize < 0)
    {
        FatalErrorInFunction
            << "bad set size " << newSize
            << abort(FatalError);
    }

    label oldSize = size();

    if (newSize == 0)
    {
        clear();
    }
    else if (newSize < oldSize)
    {
        register label i;
        for (i=newSize; i<oldSize; i++)
        {
            if (ptrs_[i])
            {
                delete ptrs_[i];
            }
        }

        ptrs_.setSize(newSize);
    }
    else // newSize > oldSize
    {
        ptrs_.setSize(newSize);

        register label i;
        for (i=oldSize; i<newSize; i++)
        {
            ptrs_[i] = nullptr;
        }
    }
}


template<class T>
void CML::PtrList<T>::clear()
{
    forAll(*this, i)
    {
        if (ptrs_[i])
        {
            delete ptrs_[i];
        }
    }

    ptrs_.clear();
}


template<class T>
void CML::PtrList<T>::transfer(PtrList<T>& a)
{
    clear();
    ptrs_.transfer(a.ptrs_);
}


template<class T>
void CML::PtrList<T>::reorder(const labelUList& oldToNew)
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


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
CML::PtrList<T>& CML::PtrList<T>::operator=(const PtrList<T>& a)
{
    if (this == &a)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    if (size() == 0)
    {
        setSize(a.size());

        forAll(*this, i)
        {
            ptrs_[i] = (a[i]).clone().ptr();
        }
    }
    else if (a.size() == size())
    {
        forAll(*this, i)
        {
            (*this)[i] = a[i];
        }
    }
    else
    {
        FatalErrorInFunction
            << "bad size: " << a.size()
            << abort(FatalError);
    }


    return *this;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "SLList.hpp"
#include "Istream.hpp"
#include "Ostream.hpp"
#include "INew.hpp"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class T>
template<class INew>
void CML::PtrList<T>::read(Istream& is, const INew& inewt)
{
    is.fatalCheck("PtrList<T>::read(Istream&, const INew&)");

    token firstToken(is);

    is.fatalCheck
    (
        "PtrList<T>::read(Istream&, const INew&) : "
        "reading first token"
    );

    if (firstToken.isLabel())
    {
        // Read size of list
        label s = firstToken.labelToken();

        setSize(s);

        // Read beginning of contents
        char delimiter = is.readBeginList("PtrList");

        if (s)
        {
            if (delimiter == token::BEGIN_LIST)
            {
                forAll(*this, i)
                {
                    set(i, inewt(is));

                    is.fatalCheck
                    (
                        "PtrList<T>::read(Istream&, const INew&) : "
                        "reading entry"
                    );
                }
            }
            else
            {
                T* tPtr = inewt(is).ptr();
                set(0, tPtr);

                is.fatalCheck
                (
                    "PtrList<T>::read(Istream&, const INew&) : "
                    "reading the single entry"
                );

                for (label i=1; i<s; i++)
                {
                    set(i, tPtr->clone());
                }
            }
        }

        // Read end of contents
        is.readEndList("PtrList");
    }
    else if (firstToken.isPunctuation())
    {
        if (firstToken.pToken() != token::BEGIN_LIST)
        {
            FatalIOErrorInFunction(is)
                << "incorrect first token, '(', found " << firstToken.info()
                << exit(FatalIOError);
        }

        SLList<T*> sllPtrs;

        token lastToken(is);
        while
        (
           !(
                lastToken.isPunctuation()
             && lastToken.pToken() == token::END_LIST
            )
        )
        {
            is.putBack(lastToken);
            sllPtrs.append(inewt(is).ptr());
            is >> lastToken;
        }

        setSize(sllPtrs.size());

        label i = 0;
        for
        (
            typename SLList<T*>::iterator iter = sllPtrs.begin();
            iter != sllPtrs.end();
            ++iter
        )
        {
            set(i++, iter());
        }
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "incorrect first token, expected <int> or '(', found "
            << firstToken.info()
            << exit(FatalIOError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
template<class INew>
CML::PtrList<T>::PtrList(Istream& is, const INew& inewt)
{
    read(is, inewt);
}


template<class T>
CML::PtrList<T>::PtrList(Istream& is)
{
    read(is, INew<T>());
}


// * * * * * * * * * * * * * * * Istream Operator  * * * * * * * * * * * * * //

template<class T>
CML::Istream& CML::operator>>(Istream& is, PtrList<T>& L)
{
    L.clear();
    L.read(is, INew<T>());

    return is;
}


// * * * * * * * * * * * * * * * Ostream Operators * * * * * * * * * * * * * //

template<class T>
CML::Ostream& CML::operator<<(Ostream& os, const PtrList<T>& L)
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
    os.check("Ostream& operator<<(Ostream&, const PtrList&)");

    return os;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
