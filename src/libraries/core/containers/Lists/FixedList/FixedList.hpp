/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
Copyright (C) 2017-2018 OpenCFD Ltd.
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
    CML::FixedList

Description
    A 1D vector of objects of type \<T\> with a fixed size \<Size\>.


\*---------------------------------------------------------------------------*/

#ifndef FixedList_H
#define FixedList_H

#include "bool.hpp"
#include "label.hpp"
#include "uLabel.hpp"
#include "Hash.hpp"
#include "autoPtr.hpp"
#include "StaticAssert.hpp"
#include <type_traits>
#include <initializer_list>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class T, unsigned Size> class FixedList;

template<class T, unsigned Size>
Istream& operator>>(Istream&, FixedList<T, Size>&);

template<class T, unsigned Size>
Ostream& operator<<(Ostream&, const FixedList<T, Size>&);

template<class T> class UList;
template<class T> class SLList;


/*---------------------------------------------------------------------------*\
                           Class FixedList Declaration
\*---------------------------------------------------------------------------*/

template<class T, unsigned Size>
class FixedList
{
    //- Size must be positive (non-zero) and also fit as a signed value
    StaticAssert(Size && Size <= INT_MAX);

    // Private data

        //- Vector of values of type T of size Size.
        T v_[Size];


public:

    //- Hashing function class.
    //  Use Hasher directly for contiguous data. Otherwise hash incrementally.
    template< class HashT=Hash<T> >
    class Hash
    {
    public:
        Hash()
        {}

        inline unsigned operator()
        (
            const FixedList<T, Size>&,
            unsigned seed = 0
        ) const;
    };


    // Static Member Functions

        //- Return a null FixedList
        inline static const FixedList<T, Size>& null();


    // Constructors

        //- Null constructor
        inline FixedList();

        //- Construct from value
        explicit FixedList(const T&);

        //- Construct from C-array
        explicit inline FixedList(const T v[Size]);

        //- Construct given start and end iterators
        template<class InputIterator>
        inline FixedList(InputIterator first, InputIterator last);

        //- Construct from brace-enclosed values
        inline FixedList(std::initializer_list<T>);

        //- Construct from UList
        explicit inline FixedList(const UList<T>&);

        //- Construct from SLList
        explicit inline FixedList(const SLList<T>&);

        //- Copy constructor
        inline FixedList(const FixedList<T, Size>&);

        //- Construct from Istream
        FixedList(Istream&);

        //- Clone
        inline autoPtr< FixedList<T, Size> > clone() const;


    // Member Functions

        // Access

            //- Return the forward circular index, i.e. the next index
            //  which returns to the first at the end of the list
            inline label fcIndex(const label i) const;

            //- Return the reverse circular index, i.e. the previous index
            //  which returns to the last at the beginning of the list
            inline label rcIndex(const label i) const;


            //- Return a const pointer to the first data element,
            //  similar to the STL front() method and the string::data() method
            //  This can be used (with caution) when interfacing with C code
            inline const T* cdata() const;

            //- Return a pointer to the first data element,
            //  similar to the STL front() method and the string::data() method
            //  This can be used (with caution) when interfacing with C code
            inline T* data();

            //- Return the first element of the list
            inline T& first();

            //- Return first element of the list
            inline const T& first() const;

            //- Return the last element of the list
            inline T& last();

            //- Return the last element of the list
            inline const T& last() const;


        // Check

            //- Check start is within valid range (0 ... size-1)
            inline void checkStart(const label start) const;

            //- Check size is within valid range (0 ... size)
            inline void checkSize(const label size) const;

            //- Check index i is within valid range (0 ... size-1)
            inline void checkIndex(const label i) const;


        // Edit

            //- Dummy resize function
            //  needed to make FixedList consistent with List
            inline void resize(const label);

            //- Dummy setSize function
            //  needed to make FixedList consistent with List
            inline void setSize(const label);

            //- Copy (not transfer) the argument contents
            //  needed to make FixedList consistent with List
            void transfer(const FixedList<T, Size>&);


    // Member operators

        //- Return element of FixedList
        inline T& operator[](const label);

        //- Return element of constant FixedList
        inline const T& operator[](const label) const;

        //- Assignment to array operator. Takes linear time
        inline void operator=(const T v[Size]);

        //- Assignment to UList operator. Takes linear time
        inline void operator=(const UList<T>&);

        //- Assignment to SLList operator. Takes linear time
        inline void operator=(const SLList<T>&);

        //- Assignment of all entries to the given value
        inline void operator=(const T&);


    // STL type definitions

        //- Type of values the FixedList contains
        typedef T value_type;

        //- Type that can be used for storing into
        //  FixedList::value_type objects
        typedef T& reference;

        //- Type that can be used for storing into
        //  constant FixedList::value_type objects
        typedef const T& const_reference;

        //- The type that can represent the difference between any two
        //  FixedList iterator objects
        typedef label difference_type;

        //- The type that can represent the size of a FixedList
        typedef label size_type;


    // STL iterator

        //- Random access iterator for traversing FixedList
        typedef T* iterator;

        //- Return an iterator to begin traversing the FixedList
        inline iterator begin();

        //- Return an iterator to end traversing the FixedList
        inline iterator end();


    // STL const_iterator

        //- Random access iterator for traversing FixedList
        typedef const T* const_iterator;

        //- Return const_iterator to begin traversing the constant FixedList
        inline const_iterator cbegin() const;

        //- Return const_iterator to end traversing the constant FixedList
        inline const_iterator cend() const;

        //- Return const_iterator to begin traversing the constant FixedList
        inline const_iterator begin() const;

        //- Return const_iterator to end traversing the constant FixedList
        inline const_iterator end() const;


    // STL reverse_iterator

        //- Reverse iterator for reverse traversal of FixedList
        typedef T* reverse_iterator;

        //- Return reverse_iterator to begin reverse traversing the FixedList
        inline reverse_iterator rbegin();

        //- Return reverse_iterator to end reverse traversing the FixedList
        inline reverse_iterator rend();


    // STL const_reverse_iterator

        //- Reverse iterator for reverse traversal of constant FixedList
        typedef const T* const_reverse_iterator;

        //- Return const_reverse_iterator to begin reverse traversing FixedList
        inline const_reverse_iterator crbegin() const;

        //- Return const_reverse_iterator to end reverse traversing FixedList
        inline const_reverse_iterator crend() const;

        //- Return const_reverse_iterator to begin reverse traversing FixedList
        inline const_reverse_iterator rbegin() const;

        //- Return const_reverse_iterator to end reverse traversing FixedList
        inline const_reverse_iterator rend() const;


    // STL member functions

        //- Return the number of elements in the FixedList
        inline label size() const;

        //- Return size of the largest possible FixedList
        inline label max_size() const;

        //- Return true if the FixedList is empty (ie, size() is zero)
        inline bool empty() const;

        //- Swap two FixedLists of the same type in constant time
        void swap(FixedList<T, Size>&);


    // STL member operators

        //- Equality operation on FixedLists of the same type.
        //  Returns true when the FixedLists are elementwise equal
        //  (using FixedList::value_type::operator==).  Takes linear time
        bool operator==(const FixedList<T, Size>&) const;

        //- The opposite of the equality operation. Takes linear time
        bool operator!=(const FixedList<T, Size>&) const;

        //- Compare two FixedLists lexicographically. Takes linear time
        bool operator<(const FixedList<T, Size>&) const;

        //- Compare two FixedLists lexicographically. Takes linear time
        bool operator>(const FixedList<T, Size>&) const;

        //- Return true if !(a > b). Takes linear time
        bool operator<=(const FixedList<T, Size>&) const;

        //- Return true if !(a < b). Takes linear time
        bool operator>=(const FixedList<T, Size>&) const;

    // Writing

        //- Write the FixedList as a dictionary entry
        void writeEntry(Ostream&) const;

        //- Write the List as a dictionary entry with keyword
        void writeEntry(const word& keyword, Ostream& os) const;

        //- Write the List, with line-breaks in ASCII if the list length
        //- exceeds shortListLen.
        //  Using '0' suppresses line-breaks entirely.
        Ostream& writeList(Ostream& os, const label shortListLen=0) const;


    // IOstream operators

        //- Read List from Istream, discarding contents of existing List
        friend Istream& operator>> <T, Size>
        (Istream&, FixedList<T, Size>&);

        //- Write FixedList to Ostream
        friend Ostream& operator<< <T, Size>
        (
            Ostream&,
            const FixedList<T, Size>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "UList.hpp"
#include "SLList.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T, unsigned Size>
inline CML::FixedList<T, Size>::FixedList()
{}


template<class T, unsigned Size>
inline CML::FixedList<T, Size>::FixedList(const T& t)
{
    for (unsigned i=0; i<Size; i++)
    {
        v_[i] = t;
    }
}


template<class T, unsigned Size>
inline CML::FixedList<T, Size>::FixedList(const T v[Size])
{
    for (unsigned i=0; i<Size; i++)
    {
        v_[i] = v[i];
    }
}


template<class T, unsigned Size>
template<class InputIterator>
CML::FixedList<T, Size>::FixedList
(
    InputIterator first,
    InputIterator last
)
{
    checkSize(std::distance(first, last));

    InputIterator iter = first;
    for (unsigned i=0; i<Size; i++)
    {
        v_[i] = *iter++;
    }
}


template<class T, unsigned Size>
inline CML::FixedList<T, Size>::FixedList(std::initializer_list<T> lst)
:
    FixedList<T, Size>(lst.begin(), lst.end())
{}


template<class T, unsigned Size>
inline CML::FixedList<T, Size>::FixedList(const UList<T>& lst)
{
    checkSize(lst.size());

    for (unsigned i=0; i<Size; i++)
    {
        v_[i] = lst[i];
    }
}


template<class T, unsigned Size>
inline CML::FixedList<T, Size>::FixedList(const SLList<T>& lst)
{
    checkSize(lst.size());

    typename SLList<T>::const_iterator iter = lst.begin();
    for (unsigned i=0; i<Size; i++)
    {
        v_[i] = *iter++;
    }
}


template<class T, unsigned Size>
inline CML::FixedList<T, Size>::FixedList(const FixedList<T, Size>& lst)
{
    for (unsigned i=0; i<Size; i++)
    {
        v_[i] = lst[i];
    }
}


template<class T, unsigned Size>
inline CML::autoPtr< CML::FixedList<T, Size> >
CML::FixedList<T, Size>::clone() const
{
    return autoPtr< FixedList<T, Size> >(new FixedList<T, Size>(*this));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T, unsigned Size>
inline const CML::FixedList<T, Size>& CML::FixedList<T, Size>::null()
{
    return NullSingletonRef< FixedList<T, Size> >();
}


template<class T, unsigned Size>
inline CML::label CML::FixedList<T, Size>::fcIndex(const label i) const
{
    return (i == Size-1 ? 0 : i+1);
}


template<class T, unsigned Size>
inline CML::label CML::FixedList<T, Size>::rcIndex(const label i) const
{
    return (i ? i-1 : Size-1);
}


// Check start is within valid range (0 ... size-1).
template<class T, unsigned Size>
inline void CML::FixedList<T, Size>::checkStart(const label start) const
{
    if (start < 0 || (start && unsigned(start) >= Size))
    {
        FatalErrorInFunction
            << "start " << start << " out of range 0 ... " << (Size-1)
            << abort(FatalError);
    }
}


// Check size is within valid range (0 ... size).
template<class T, unsigned Size>
inline void CML::FixedList<T, Size>::checkSize(const label size) const
{
    if (size < 0 || unsigned(size) > Size)
    {
        FatalErrorInFunction
            << "size " << size << " out of range 0 ... " << (Size)
            << abort(FatalError);
    }
}


// Check index i is within valid range (0 ... size-1)
// The check for zero-sized list is already done in static assert
template<class T, unsigned Size>
inline void CML::FixedList<T, Size>::checkIndex(const label i) const
{
    if (i < 0 || unsigned(i) >= Size)
    {
        FatalErrorInFunction
            << "index " << i << " out of range 0 ... " << (Size-1)
            << abort(FatalError);
    }
}


template<class T, unsigned Size>
inline void CML::FixedList<T, Size>::resize(const label s)
{
    #ifdef FULLDEBUG
    checkSize(s);
    #endif
}

template<class T, unsigned Size>
inline void CML::FixedList<T, Size>::setSize(const label s)
{
    #ifdef FULLDEBUG
    checkSize(s);
    #endif
}

template<class T, unsigned Size>
inline void CML::FixedList<T, Size>::transfer(const FixedList<T, Size>& lst)
{
    for (unsigned i=0; i<Size; i++)
    {
        v_[i] = lst[i];
    }
}


template<class T, unsigned Size>
inline const T*
CML::FixedList<T, Size>::cdata() const
{
    return v_;
}


template<class T, unsigned Size>
inline T*
CML::FixedList<T, Size>::data()
{
    return v_;
}


template<class T, unsigned Size>
inline T& CML::FixedList<T, Size>::first()
{
    return v_[0];
}


template<class T, unsigned Size>
inline const T& CML::FixedList<T, Size>::first() const
{
    return v_[0];
}


template<class T, unsigned Size>
inline T& CML::FixedList<T, Size>::last()
{
    return v_[Size-1];
}


template<class T, unsigned Size>
inline const T& CML::FixedList<T, Size>::last() const
{
    return v_[Size-1];
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

// element access
template<class T, unsigned Size>
inline T& CML::FixedList<T, Size>::operator[](const label i)
{
    #ifdef FULLDEBUG
    checkIndex(i);
    #endif
    return v_[i];
}


// const element access
template<class T, unsigned Size>
inline const T& CML::FixedList<T, Size>::operator[](const label i) const
{
    #ifdef FULLDEBUG
    checkIndex(i);
    #endif
    return v_[i];
}


template<class T, unsigned Size>
inline void CML::FixedList<T, Size>::operator=(const T lst[Size])
{
    for (unsigned i=0; i<Size; i++)
    {
        v_[i] = lst[i];
    }
}

template<class T, unsigned Size>
inline void CML::FixedList<T, Size>::operator=(const UList<T>& lst)
{
    checkSize(lst.size());

    for (unsigned i=0; i<Size; i++)
    {
        v_[i] = lst[i];
    }
}

template<class T, unsigned Size>
inline void CML::FixedList<T, Size>::operator=(const SLList<T>& lst)
{
    checkSize(lst.size());

    label i = 0;
    for
    (
        typename SLList<T>::const_iterator iter = lst.begin();
        iter != lst.end();
        ++iter
    )
    {
        operator[](i++) = iter();
    }
}

template<class T, unsigned Size>
inline void CML::FixedList<T, Size>::operator=(const T& t)
{
    for (unsigned i=0; i<Size; i++)
    {
        v_[i] = t;
    }
}


// * * * * * * * * * * * * * * STL Member Functions  * * * * * * * * * * * * //

template<class T, unsigned Size>
inline typename CML::FixedList<T, Size>::iterator
CML::FixedList<T, Size>::begin()
{
    return v_;
}


template<class T, unsigned Size>
inline typename CML::FixedList<T, Size>::const_iterator
CML::FixedList<T, Size>::begin() const
{
    return v_;
}


template<class T, unsigned Size>
inline typename CML::FixedList<T, Size>::const_iterator
CML::FixedList<T, Size>::cbegin() const
{
    return v_;
}


template<class T, unsigned Size>
inline typename CML::FixedList<T, Size>::iterator
CML::FixedList<T, Size>::end()
{
    return &v_[Size];
}


template<class T, unsigned Size>
inline typename CML::FixedList<T, Size>::const_iterator
CML::FixedList<T, Size>::end() const
{
    return &v_[Size];
}


template<class T, unsigned Size>
inline typename CML::FixedList<T, Size>::const_iterator
CML::FixedList<T, Size>::cend() const
{
    return &v_[Size];
}


template<class T, unsigned Size>
inline typename CML::FixedList<T, Size>::iterator
CML::FixedList<T, Size>::rbegin()
{
    return &v_[Size-1];
}


template<class T, unsigned Size>
inline typename CML::FixedList<T, Size>::const_iterator
CML::FixedList<T, Size>::rbegin() const
{
    return &v_[Size-1];
}


template<class T, unsigned Size>
inline typename CML::FixedList<T, Size>::const_iterator
CML::FixedList<T, Size>::crbegin() const
{
    return &v_[Size-1];
}


template<class T, unsigned Size>
inline typename CML::FixedList<T, Size>::iterator
CML::FixedList<T, Size>::rend()
{
    return &v_[-1];
}


template<class T, unsigned Size>
inline typename CML::FixedList<T, Size>::const_iterator
CML::FixedList<T, Size>::rend() const
{
    return &v_[-1];
}


template<class T, unsigned Size>
inline typename CML::FixedList<T, Size>::const_iterator
CML::FixedList<T, Size>::crend() const
{
    return &v_[-1];
}


template<class T, unsigned Size>
inline CML::label CML::FixedList<T, Size>::size() const
{
    return Size;
}


template<class T, unsigned Size>
inline CML::label CML::FixedList<T, Size>::max_size() const
{
    return Size;
}


template<class T, unsigned Size>
inline bool CML::FixedList<T, Size>::empty() const
{
    return false;
}


template<class T, unsigned Size>
template<class HashT>
inline unsigned CML::FixedList<T, Size>::Hash<HashT>::operator()
(
    const FixedList<T, Size>& lst,
    unsigned seed
) const
{
    if (contiguous<T>())
    {
        // Hash directly
        return Hasher(lst.v_, sizeof(lst.v_), seed);
    }
    else
    {
        // Hash incrementally
        unsigned val = seed;

        for (unsigned i=0; i<Size; i++)
        {
            val = HashT()(lst[i], val);
        }

        return val;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "ListLoopM.hpp"


// * * * * * * * * * * * * * * STL Member Functions  * * * * * * * * * * * * //

template<class T, unsigned Size>
void CML::FixedList<T, Size>::swap(FixedList<T, Size>& a)
{
    List_ACCESS(T, (*this), vp);
    List_ACCESS(T, a, ap);
    T tmp;
    List_FOR_ALL((*this), i)
        tmp = List_CELEM((*this), vp, i);
        List_ELEM((*this), vp, i) = List_CELEM(a, ap, i);
        List_ELEM(a, ap, i) = tmp;
    List_END_FOR_ALL
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, unsigned Size>
bool CML::FixedList<T, Size>::operator==(const FixedList<T, Size>& a) const
{
    bool equal = true;

    List_CONST_ACCESS(T, (*this), vp);
    List_CONST_ACCESS(T, (a), ap);

    List_FOR_ALL((*this), i)
        equal = equal && (List_ELEM((*this), vp, i) == List_ELEM((a), ap, i));
    List_END_FOR_ALL

    return equal;
}


template<class T, unsigned Size>
bool CML::FixedList<T, Size>::operator!=(const FixedList<T, Size>& a) const
{
    return !operator==(a);
}


template<class T, unsigned Size>
bool CML::FixedList<T, Size>::operator<(const FixedList<T, Size>& a) const
{
    for
    (
        const_iterator vi = cbegin(), ai = a.cbegin();
        vi < cend() && ai < a.cend();
        vi++, ai++
    )
    {
        if (*vi < *ai)
        {
            return true;
        }
        else if (*vi > *ai)
        {
            return false;
        }
    }

    if (Size < a.Size)
    {
        return true;
    }
    else
    {
        return false;
    }
}


template<class T, unsigned Size>
bool CML::FixedList<T, Size>::operator>(const FixedList<T, Size>& a) const
{
    return a.operator<(*this);
}


template<class T, unsigned Size>
bool CML::FixedList<T, Size>::operator<=(const FixedList<T, Size>& a) const
{
    return !operator>(a);
}


template<class T, unsigned Size>
bool CML::FixedList<T, Size>::operator>=(const FixedList<T, Size>& a) const
{
    return !operator<(a);
}


// * * * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * //

#include "Istream.hpp"
#include "Ostream.hpp"
#include "token.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class T, unsigned Size>
CML::FixedList<T, Size>::FixedList(Istream& is)
{
    operator>>(is, *this);
}


template<class T, unsigned Size>
CML::Istream& CML::operator>>(CML::Istream& is, FixedList<T, Size>& L)
{
    is.fatalCheck("operator>>(Istream&, FixedList<T, Size>&)");

    if (is.format() == IOstream::ASCII || !contiguous<T>())
    {
        token firstToken(is);

        is.fatalCheck
        (
            "operator>>(Istream&, FixedList<T, Size>&) : reading first token"
        );

        if (firstToken.isCompound())
        {
            L = dynamicCast<token::Compound<List<T> > >
            (
                firstToken.transferCompoundToken(is)
            );
        }
        else if (firstToken.isLabel())
        {
            label s = firstToken.labelToken();

            // Set list length to that read
            L.checkSize(s);
        }
        else if (!firstToken.isPunctuation())
        {
            FatalIOErrorInFunction(is)
                << "incorrect first token, expected <label> "
                   "or '(' or '{', found "
                << firstToken.info()
                << exit(FatalIOError);
        }
        else
        {
            // Putback the opening bracket
            is.putBack(firstToken);
        }

        // Read beginning of contents
        char delimiter = is.readBeginList("FixedList");

        if (delimiter == token::BEGIN_LIST)
        {
            for (unsigned i=0; i<Size; i++)
            {
                is >> L[i];

                is.fatalCheck
                (
                    "operator>>(Istream&, FixedList<T, Size>&) : "
                    "reading entry"
                );
            }
        }
        else
        {
            T element;
            is >> element;

            is.fatalCheck
            (
                "operator>>(Istream&, FixedList<T, Size>&) : "
                "reading the single entry"
            );

            for (unsigned i=0; i<Size; i++)
            {
                L[i] = element;
            }
        }

        // Read end of contents
        is.readEndList("FixedList");
    }
    else
    {
        is.read(reinterpret_cast<char*>(L.data()), Size*sizeof(T));

        is.fatalCheck
        (
            "operator>>(Istream&, FixedList<T, Size>&) : "
            "reading the binary block"
        );
    }

    return is;
}


// * * * * * * * * * * * * * * * Ostream Operator *  * * * * * * * * * * * * //

template<class T, unsigned Size>
void CML::FixedList<T, Size>::writeEntry(Ostream& os) const
{
    if
    (
        token::compound::isCompound("List<" + word(pTraits<T>::typeName) + '>')
    )
    {
        os  << word("List<" + word(pTraits<T>::typeName) + '>') << " ";
    }

    os << *this;
}


template<class T, unsigned Size>
void CML::FixedList<T, Size>::writeEntry
(
    const word& keyword,
    Ostream& os
) const
{
    os.writeKeyword(keyword);
    writeEntry(os);
    os << token::END_STATEMENT << endl;
}


template<class T, unsigned Size>
CML::Ostream& CML::FixedList<T, Size>::writeList
(
    Ostream& os,
    const label shortListLen
) const
{
    const FixedList<T, Size>& list = *this;

    // Write list contents depending on data format
    if (os.format() == IOstream::ASCII || !contiguous<T>())
    {
        if (contiguous<T>() && list.uniform())
        {
            // Two or more entries, and all entries have identical values.

            os << Size << token::BEGIN_BLOCK << list[0] << token::END_BLOCK;
        }
        else if
        (
            Size <= 1 || !shortListLen
         || (Size <= unsigned(shortListLen) && contiguous<T>())
        )
        {
            // Start delimiter
            os << token::BEGIN_LIST;

            // Contents
            for (unsigned i=0; i<Size; ++i)
            {
                if (i) os << token::SPACE;
                os << list[i];
            }

            // End delimiter
            os << token::END_LIST;
        }
        else
        {
            // Start delimiter
            os << nl << token::BEGIN_LIST << nl;

            // Contents
            for (unsigned i=0; i<Size; ++i)
            {
                os << list[i] << nl;
            }

            // End delimiter
            os << token::END_LIST << nl;
        }
    }
    else
    {
        // Binary, contiguous

        // write(...) includes surrounding start/end delimiters
        os.write(reinterpret_cast<const char*>(list.cdata()), Size*sizeof(T));
    }

    os.check(FUNCTION_NAME);
    return os;
}


template<class T, unsigned Size>
CML::Ostream& CML::operator<<(Ostream& os, const FixedList<T, Size>& L)
{
    // Write list contents depending on data format
    if (os.format() == IOstream::ASCII || !contiguous<T>())
    {
        bool uniform = false;

        if (Size > 1 && contiguous<T>())
        {
            uniform = true;

            forAll(L, i)
            {
                if (L[i] != L[0])
                {
                    uniform = false;
                    break;
                }
            }
        }

        if (uniform)
        {
            // Write size (so it is valid dictionary entry) and start delimiter
            os << L.size() << token::BEGIN_BLOCK;

            // Write contents
            os << L[0];

            // Write end delimiter
            os << token::END_BLOCK;
        }
        else if (Size <= 1 ||(Size < 11 && contiguous<T>()))
        {
            // Write start delimiter
            os << token::BEGIN_LIST;

            // Write contents
            forAll(L, i)
            {
                if (i > 0) os << token::SPACE;
                os << L[i];
            }

            // Write end delimiter
            os << token::END_LIST;
        }
        else
        {
            // Write start delimiter
            os << nl << token::BEGIN_LIST;

            // Write contents
            forAll(L, i)
            {
                os << nl << L[i];
            }

            // Write end delimiter
            os << nl << token::END_LIST << nl;
        }
    }
    else
    {
        os.write(reinterpret_cast<const char*>(L.cdata()), Size*sizeof(T));
    }

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const FixedList&)");

    return os;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
