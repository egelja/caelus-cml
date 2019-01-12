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
    CML::UList

Description
    A 1D vector of objects of type \<T\>, where the size of the vector is
    known and can be used for subscript bounds checking, etc.

    Storage is not allocated during construction or use but is supplied to
    the constructor as an argument.  This type of list is particularly useful
    for lists that refer to parts of existing lists such as SubList.


\*---------------------------------------------------------------------------*/

#ifndef UList_H
#define UList_H

#include "bool.hpp"
#include "label.hpp"
#include "uLabel.hpp"
#include "restrict.hpp"
#include "nullSingleton.hpp"
#include "zero.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend classes
template<class T> class List;
template<class T> class SubList;

// Forward declaration of friend functions and operators
template<class T> class SLList;
template<class T> class UList;
template<class T> Ostream& operator<<(Ostream&, const UList<T>&);
template<class T> Istream& operator>>(Istream&, UList<T>&);

typedef UList<label> labelUList;

/*---------------------------------------------------------------------------*\
                           Class UList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class UList
{
    // Private data

        //- Number of elements in UList.
        label size_;

        //- Vector of values of type T.
        T* RESTRICT v_;


public:

    // Related types

        //- Declare friendship with the List class
        friend class List<T>;

        //- Declare friendship with the SubList class
        friend class SubList<T>;

    // Static Member Functions

        //- Return a null UList
        inline static const UList<T>& null();

    // Public classes

        //- Less function class that can be used for sorting
        class less
        {
            const UList<T>& values_;

        public:

            less(const UList<T>& values)
            :
                values_(values)
            {}

            bool operator()(const label a, const label b)
            {
                return values_[a] < values_[b];
            }
        };

        //- Greater function class that can be used for sorting
        class greater
        {
            const UList<T>& values_;

        public:

            greater(const UList<T>& values)
            :
                values_(values)
            {}

            bool operator()(const label a, const label b)
            {
                return values_[a] > values_[b];
            }
        };


    // Constructors

        //- Null constructor.
        inline UList();

        //- Construct from components
        inline UList(T* RESTRICT v, label size);


    // Member Functions


        // Access

            //- Return the forward circular index, i.e. the next index
            //  which returns to the first at the end of the list
            inline label fcIndex(const label i) const;

            //- Return the reverse circular index, i.e. the previous index
            //  which returns to the last at the beginning of the list
            inline label rcIndex(const label i) const;

            //- Return the binary size in number of characters of the UList
            //  if the element is a primitive type
            //  i.e. contiguous<T>() == true.
            //  Note that is of type streamsize since used in stream ops
            std::streamsize byteSize() const;


            //- Return a const pointer to the first data element,
            //  similar to the STL front() method and the string::data() method
            //  This can be used (with caution) when interfacing with C code.
            inline const T* cdata() const;

            //- Return a pointer to the first data element,
            //  similar to the STL front() method and the string::data() method
            //  This can be used (with caution) when interfacing with C code.
            inline T* data();

            //- Return the first element of the list.
            inline T& first();

            //- Return first element of the list.
            inline const T& first() const;

            //- Return the last element of the list.
            inline T& last();

            //- Return the last element of the list.
            inline const T& last() const;


        // Check

            //- Check start is within valid range (0 ... size-1).
            inline void checkStart(const label start) const;

            //- Check size is within valid range (0 ... size).
            inline void checkSize(const label size) const;

            //- Check index i is within valid range (0 ... size-1).
            inline void checkIndex(const label i) const;


        //- Assign elements to those from UList.
        void assign(const UList<T>&);


    // Member operators

        //- Return element of UList.
        inline T& operator[](const label);

        //- Return element of constant UList.
        //  Note that the bool specialization adds lazy evaluation so reading
        //  an out-of-range element returns false without any ill-effects
        inline const T& operator[](const label) const;

        //- Allow cast to a const List<T>&
        inline operator const CML::List<T>&() const;

        //- Assignment of all entries to the given value
        void operator=(const T& val);

        //- Assignment of all entries to zero
        void operator=(const zero);


    // STL type definitions

        //- Type of values the UList contains.
        typedef T value_type;

        //- Type that can be used for storing into
        //  UList::value_type objects.
        typedef T& reference;

        //- Type that can be used for storing into
        //  constant UList::value_type objects
        typedef const T& const_reference;

        //- The type that can represent the difference between any two
        //  UList iterator objects.
        typedef label difference_type;

        //- The type that can represent the size of a UList.
        typedef label size_type;


    // STL iterator

        //- Random access iterator for traversing UList.
        typedef T* iterator;

        //- Return an iterator to begin traversing the UList.
        inline iterator begin();

        //- Return an iterator to end traversing the UList.
        inline iterator end();


    // STL const_iterator

        //- Random access iterator for traversing UList.
        typedef const T* const_iterator;

        //- Return const_iterator to begin traversing the constant UList.
        inline const_iterator cbegin() const;

        //- Return const_iterator to end traversing the constant UList.
        inline const_iterator cend() const;

        //- Return const_iterator to begin traversing the constant UList.
        inline const_iterator begin() const;

        //- Return const_iterator to end traversing the constant UList.
        inline const_iterator end() const;


    // STL reverse_iterator

        //- Reverse iterator for reverse traversal of UList.
        typedef T* reverse_iterator;

        //- Return reverse_iterator to begin reverse traversing the UList.
        inline reverse_iterator rbegin();

        //- Return reverse_iterator to end reverse traversing the UList.
        inline reverse_iterator rend();


    // STL const_reverse_iterator

        //- Reverse iterator for reverse traversal of constant UList.
        typedef const T* const_reverse_iterator;

        //- Return const_reverse_iterator to begin reverse traversing the UList.
        inline const_reverse_iterator crbegin() const;

        //- Return const_reverse_iterator to end reverse traversing the UList.
        inline const_reverse_iterator crend() const;

        //- Return const_reverse_iterator to begin reverse traversing the UList.
        inline const_reverse_iterator rbegin() const;

        //- Return const_reverse_iterator to end reverse traversing the UList.
        inline const_reverse_iterator rend() const;


    // STL member functions

        //- Return the number of elements in the UList.
        inline label size() const;

        //- Return size of the largest possible UList.
        inline label max_size() const;

        //- Return true if the UList is empty (ie, size() is zero).
        inline bool empty() const;

        //- Swap two ULists of the same type in constant time.
        void swap(UList<T>&);


    // STL member operators

        //- Equality operation on ULists of the same type.
        //  Returns true when the ULists are elementwise equal
        //  (using UList::value_type::operator==).  Takes linear time.
        bool operator==(const UList<T>&) const;

        //- The opposite of the equality operation. Takes linear time.
        bool operator!=(const UList<T>&) const;

        //- Compare two ULists lexicographically. Takes linear time.
        bool operator<(const UList<T>&) const;

        //- Compare two ULists lexicographically. Takes linear time.
        bool operator>(const UList<T>&) const;

        //- Return true if !(a > b). Takes linear time.
        bool operator<=(const UList<T>&) const;

        //- Return true if !(a < b). Takes linear time.
        bool operator>=(const UList<T>&) const;


    // Writing

        //- Write the UList as a dictionary entry.
        void writeEntry(Ostream&) const;

        //- Write the List as a dictionary entry with keyword
        void writeEntry(const word& keyword, Ostream& os) const;

        //- Write the List, with line-breaks in ASCII if the list length
        //- exceeds shortListLen.
        //  Using '0' suppresses line-breaks entirely.
        Ostream& writeList(Ostream& os, const label shortListLen=0) const;

    // Ostream operator

        // Write UList to Ostream.
        friend Ostream& operator<< <T>
        (
            Ostream&,
            const UList<T>&
        );

        //- Read UList contents from Istream. Requires size to have been set
        //  before.
        friend Istream& operator>> <T>
        (
            Istream&,
            UList<T>&
        );
};

template<class T>
void sort(UList<T>&);

template<class T, class Cmp>
void sort(UList<T>&, const Cmp&);

template<class T>
void stableSort(UList<T>&);

template<class T, class Cmp>
void stableSort(UList<T>&, const Cmp&);

template<class T>
void shuffle(UList<T>&);

// Reverse the first n elements of the list
template<class T>
inline void reverse(UList<T>&, const label n);

// Reverse all the elements of the list
template<class T>
inline void reverse(UList<T>&);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "error.hpp"
#include "pTraits.hpp"
#include "Swap.hpp"
#include "restrict.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
inline CML::UList<T>::UList()
:
    size_(0),
    v_(0)
{}


template<class T>
inline CML::UList<T>::UList(T* RESTRICT v, label size)
:
    size_(size),
    v_(v)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline const CML::UList<T>& CML::UList<T>::null()
{
    return NullSingletonRef< UList<T> >();
}


template<class T>
inline CML::label CML::UList<T>::fcIndex(const label i) const
{
    return (i == size()-1 ? 0 : i+1);
}


template<class T>
inline CML::label CML::UList<T>::rcIndex(const label i) const
{
    return (i ? i-1 : size()-1);
}


// Check start is within valid range (0 ... size-1).
template<class T>
inline void CML::UList<T>::checkStart(const label start) const
{
    if (start<0 || (start && start>=size_))
    {
        FatalErrorInFunction
            << "start " << start << " out of range 0 ... " << max(size_-1, 0)
            << abort(FatalError);
    }
}


// Check size is within valid range (0 ... size).
template<class T>
inline void CML::UList<T>::checkSize(const label size) const
{
    if (size<0 || size>size_)
    {
        FatalErrorInFunction
            << "size " << size << " out of range 0 ... " << size_
            << abort(FatalError);
    }
}


// Check index i is within valid range (0 ... size-1).
template<class T>
inline void CML::UList<T>::checkIndex(const label i) const
{
    if (!size_)
    {
        FatalErrorInFunction
            << "attempt to access element from zero sized list"
            << abort(FatalError);
    }
    else if (i<0 || i>=size_)
    {
        FatalErrorInFunction
            << "index " << i << " out of range 0 ... " << size_-1
            << abort(FatalError);
    }
}


template<class T>
inline T& CML::UList<T>::first()
{
    return this->operator[](0);
}


template<class T>
inline const T& CML::UList<T>::first() const
{
    return this->operator[](0);
}


template<class T>
inline T& CML::UList<T>::last()
{
    return this->operator[](this->size()-1);
}


template<class T>
inline const T& CML::UList<T>::last() const
{
    return this->operator[](this->size()-1);
}


template<class T>
inline const T* CML::UList<T>::cdata() const
{
    return v_;
}


template<class T>
inline T* CML::UList<T>::data()
{
    return v_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// element access
template<class T>
inline T& CML::UList<T>::operator[](const label i)
{
#   ifdef FULLDEBUG
    checkIndex(i);
#   endif
    return v_[i];
}


namespace CML
{

    // Template specialization for bool
    template<>
    inline const bool& CML::UList<bool>::operator[](const label i) const
    {
        // lazy evaluation - return false for out-of-range
        if (i < size_)
        {
            return v_[i];
        }
        else
        {
            return CML::pTraits<bool>::zero;
        }
    }

} // end of namespace CML


// const element access
template<class T>
inline const T& CML::UList<T>::operator[](const label i) const
{
#   ifdef FULLDEBUG
    checkIndex(i);
#   endif
    return v_[i];
}


// Allow cast to a const List<T>&
template<class T>
inline CML::UList<T>::operator const CML::List<T>&() const
{
    return *reinterpret_cast<const List<T>*>(this);
}


// * * * * * * * * * * * * * * STL Member Functions  * * * * * * * * * * * * //

template<class T>
inline typename CML::UList<T>::iterator
CML::UList<T>::begin()
{
    return v_;
}

template<class T>
inline typename CML::UList<T>::const_iterator
CML::UList<T>::begin() const
{
    return v_;
}

template<class T>
inline typename CML::UList<T>::const_iterator
CML::UList<T>::cbegin() const
{
    return v_;
}

template<class T>
inline typename CML::UList<T>::iterator
CML::UList<T>::end()
{
    return &v_[size_];
}

template<class T>
inline typename CML::UList<T>::const_iterator
CML::UList<T>::end() const
{
    return &v_[size_];
}

template<class T>
inline typename CML::UList<T>::const_iterator
CML::UList<T>::cend() const
{
    return &v_[size_];
}

template<class T>
inline typename CML::UList<T>::iterator
CML::UList<T>::rbegin()
{
    return &v_[size_-1];
}

template<class T>
inline typename CML::UList<T>::const_iterator
CML::UList<T>::rbegin() const
{
    return &v_[size_-1];
}

template<class T>
inline typename CML::UList<T>::const_iterator
CML::UList<T>::crbegin() const
{
    return &v_[size_-1];
}

template<class T>
inline typename CML::UList<T>::iterator
CML::UList<T>::rend()
{
    return &v_[-1];
}

template<class T>
inline typename CML::UList<T>::const_iterator
CML::UList<T>::rend() const
{
    return &v_[-1];
}

template<class T>
inline typename CML::UList<T>::const_iterator
CML::UList<T>::crend() const
{
    return &v_[-1];
}

template<class T>
inline CML::label CML::UList<T>::size() const
{
    return size_;
}


template<class T>
inline CML::label CML::UList<T>::max_size() const
{
    return labelMax;
}


template<class T>
inline bool CML::UList<T>::empty() const
{
    return !size_;
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class T>
inline void CML::reverse(UList<T>& ul, const label n)
{
    for (int i=0; i<n/2; i++)
    {
        Swap(ul[i], ul[n-1-i]);
    }
}

template<class T>
inline void CML::reverse(UList<T>& ul)
{
    reverse(ul, ul.size());
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/**
 * \def forAll(list, i)
 * Loop across all elements in \a list
 * \par Usage
 * \code
 * forAll(anyList, i)
 * {
 *      statements;
 * }
 * \endcode
 * \sa forAllReverse
*/
/**
 * \def forAllReverse(list, i)
 * Reverse loop across all elements in \a list
 * \par Usage
 * \code
 * forAllReverse(anyList, i)
 * {
 *      statements;
 * }
 * \endcode
 * \sa forAll
*/
#define forAll(list, i) \
    for (CML::label i=0; i<(list).size(); i++)

#define forAllReverse(list, i) \
    for (CML::label i=(list).size()-1; i>=0; i--)

/**
 * \def forAllIter(Container, container, iter)
 * Iterate across all elements in the \a container object of type
 * \a Container.
 * \par Usage
 * \code
 * forAll(ContainerType, container, iter)
 * {
 *     statements;
 * }
 * \endcode
 * \sa forAllConstIter
*/
#define forAllIter(Container,container,iter)                                   \
    for                                                                        \
    (                                                                          \
        Container::iterator iter = (container).begin();                        \
        iter != (container).end();                                             \
        ++iter                                                                 \
    )

/**
 * \def forAllConstIter(Container, container, iter)
 * Iterate across all elements in the \a container object of type
 * \a Container with const access.
 * \par Usage
 * \code
 * forAllConstIter(ContainerType, container, iter)
 * {
 *     statements;
 * }
 * \endcode
 * \sa forAllIter
*/
#define forAllConstIter(Container,container,iter)                              \
    for                                                                        \
    (                                                                          \
        Container::const_iterator iter = (container).begin();                  \
        iter != (container).end();                                             \
        ++iter                                                                 \
    )


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "error.hpp"

#include "ListLoopM.hpp"
#include "contiguous.hpp"

#include <algorithm>

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
void CML::UList<T>::assign(const UList<T>& a)
{
    if (a.size_ != this->size_)
    {
        FatalErrorInFunction
            << "ULists have different sizes: "
            << this->size_ << " " << a.size_
            << abort(FatalError);
    }

    if (this->size_)
    {
        #ifdef USEMEMCPY
        if (contiguous<T>())
        {
            memcpy(this->v_, a.v_, this->byteSize());
        }
        else
        #endif
        {
            List_ACCESS(T, (*this), vp);
            List_CONST_ACCESS(T, a, ap);
            List_FOR_ALL((*this), i)
                List_ELEM((*this), vp, i) = List_ELEM(a, ap, i);
            List_END_FOR_ALL
        }
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
void CML::UList<T>::operator=(const T& t)
{
    List_ACCESS(T, (*this), vp);
    List_FOR_ALL((*this), i)
        List_ELEM((*this), vp, i) = t;
    List_END_FOR_ALL
}

template<class T>
void CML::UList<T>::operator=(const zero)
{
    List_ACCESS(T, (*this), vp);
    List_FOR_ALL((*this), i)
        List_ELEM((*this), vp, i) = Zero;
    List_END_FOR_ALL
}

// * * * * * * * * * * * * * * STL Member Functions  * * * * * * * * * * * * //

template<class T>
void CML::UList<T>::swap(UList<T>& a)
{
    if (a.size_ != this->size_)
    {
        FatalErrorInFunction
            << "ULists have different sizes: "
            << this->size_ << " " << a.size_
            << abort(FatalError);
    }

    List_ACCESS(T, (*this), vp);
    List_ACCESS(T, a, ap);
    T tmp;
    List_FOR_ALL((*this), i)
        tmp = List_CELEM((*this), vp, i);
        List_ELEM((*this), vp, i) = List_CELEM(a, ap, i);
        List_ELEM(a, ap, i) = tmp;
    List_END_FOR_ALL
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
std::streamsize CML::UList<T>::byteSize() const
{
    if (!contiguous<T>())
    {
        FatalErrorInFunction
            << "Cannot return the binary size of a list of "
               "non-primitive elements"
            << abort(FatalError);
    }

    return this->size_*sizeof(T);
}


template<class T>
void CML::sort(UList<T>& a)
{
    std::sort(a.begin(), a.end());
}


template<class T, class Cmp>
void CML::sort(UList<T>& a, const Cmp& cmp)
{
    std::sort(a.begin(), a.end(), cmp);
}


template<class T>
void CML::stableSort(UList<T>& a)
{
    std::stable_sort(a.begin(), a.end());
}


template<class T, class Cmp>
void CML::stableSort(UList<T>& a, const Cmp& cmp)
{
    std::stable_sort(a.begin(), a.end(), cmp);
}


template<class T>
void CML::shuffle(UList<T>& a)
{
    std::random_shuffle(a.begin(), a.end());
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
bool CML::UList<T>::operator==(const UList<T>& a) const
{
    if (this->size_ != a.size_)
    {
        return false;
    }

    bool equal = true;

    List_CONST_ACCESS(T, (*this), vp);
    List_CONST_ACCESS(T, (a), ap);

    List_FOR_ALL((*this), i)
        equal = equal && (List_ELEM((*this), vp, i) == List_ELEM((a), ap, i));
    List_END_FOR_ALL

    return equal;
}


template<class T>
bool CML::UList<T>::operator!=(const UList<T>& a) const
{
    return !operator==(a);
}


template<class T>
bool CML::UList<T>::operator<(const UList<T>& a) const
{
    for
    (
        const_iterator vi = begin(), ai = a.begin();
        vi < end() && ai < a.end();
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

    if (this->size_ < a.size_)
    {
        return true;
    }
    else
    {
        return false;
    }
}


template<class T>
bool CML::UList<T>::operator>(const UList<T>& a) const
{
    return a.operator<(*this);
}


template<class T>
bool CML::UList<T>::operator<=(const UList<T>& a) const
{
    return !operator>(a);
}


template<class T>
bool CML::UList<T>::operator>=(const UList<T>& a) const
{
    return !operator<(a);
}


// * * * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * //

#include "Ostream.hpp"
#include "token.hpp"
#include "SLList.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * Ostream Operator *  * * * * * * * * * * * * //

template<class T>
void CML::UList<T>::writeEntry(Ostream& os) const
{
    if
    (
        size()
     && token::compound::isCompound
        (
            "List<" + word(pTraits<T>::typeName) + '>'
        )
    )
    {
        os  << word("List<" + word(pTraits<T>::typeName) + '>') << " ";
    }

    os << *this;
}


template<class T>
void CML::UList<T>::writeEntry(const word& keyword, Ostream& os) const
{
    os.writeKeyword(keyword);
    writeEntry(os);
    os << token::END_STATEMENT << endl;
}


template<class T>
CML::Ostream& CML::UList<T>::writeList
(
    Ostream& os,
    const label shortListLen
) const
{
    const UList<T>& list = *this;

    const label len = list.size();

    // Write list contents depending on data format
    if (os.format() == IOstream::ASCII || !contiguous<T>())
    {
        if (contiguous<T>() && list.uniform())
        {
            // Two or more entries, and all entries have identical values.
            os  << len << token::BEGIN_BLOCK << list[0] << token::END_BLOCK;
        }
        else if
        (
            len <= 1 || !shortListLen
         || (len <= shortListLen && contiguous<T>())
        )
        {
            // Size and start delimiter
            os << len << token::BEGIN_LIST;

            // Contents
            for (label i=0; i < len; ++i)
            {
                if (i) os << token::SPACE;
                os << list[i];
            }

            // End delimiter
            os << token::END_LIST;
        }
        else
        {
            // Size and start delimiter
            os << nl << len << nl << token::BEGIN_LIST << nl;

            // Contents
            for (label i=0; i < len; ++i)
            {
                os << list[i] << nl;
            }

            // End delimiter
            os << token::END_LIST << nl;
        }
    }
    else
    {
        // Contents are binary and contiguous
        os << nl << len << nl;

        if (len)
        {
            // write(...) includes surrounding start/end delimiters
            os.write
            (
                reinterpret_cast<const char*>(list.cdata()),
                list.byteSize()
            );
        }
    }

    os.check(FUNCTION_NAME);
    return os;
}


template<class T>
CML::Ostream& CML::operator<<(CML::Ostream& os, const CML::UList<T>& L)
{
    // Write list contents depending on data format
    if (os.format() == IOstream::ASCII || !contiguous<T>())
    {
        bool uniform = false;

        if (L.size() > 1 && contiguous<T>())
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
            // Write size and start delimiter
            os << L.size() << token::BEGIN_BLOCK;

            // Write contents
            os << L[0];

            // Write end delimiter
            os << token::END_BLOCK;
        }
        else if (L.size() <= 1 || (L.size() < 11 && contiguous<T>()))
        {
            // Write size and start delimiter
            os << L.size() << token::BEGIN_LIST;

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
            // Write size and start delimiter
            os << nl << L.size() << nl << token::BEGIN_LIST;

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
        os << nl << L.size() << nl;
        if (L.size())
        {
            os.write(reinterpret_cast<const char*>(L.v_), L.byteSize());
        }
    }

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const UList&)");

    return os;
}


template<class T>
CML::Istream& CML::operator>>(Istream& is, UList<T>& L)
{
    is.fatalCheck("operator>>(Istream&, UList<T>&)");

    token firstToken(is);

    is.fatalCheck("operator>>(Istream&, UList<T>&) : reading first token");

    if (firstToken.isCompound())
    {
        List<T> elems;
        elems.transfer
        (
            dynamicCast<token::Compound<List<T> > >
            (
                firstToken.transferCompoundToken(is)
            )
        );
        // Check list length
        label s = elems.size();

        if (s != L.size())
        {
            FatalIOErrorInFunction(is)
                << "incorrect length for UList. Read " << s
                << " expected " << L.size()
                << exit(FatalIOError);
        }
        for (register label i=0; i<s; i++)
        {
            L[i] = elems[i];
        }
    }
    else if (firstToken.isLabel())
    {
        label s = firstToken.labelToken();

        // Set list length to that read
        if (s != L.size())
        {
            FatalIOErrorInFunction(is)
                << "incorrect length for UList. Read " << s
                << " expected " << L.size()
                << exit(FatalIOError);
        }

        // Read list contents depending on data format

        if (is.format() == IOstream::ASCII || !contiguous<T>())
        {
            // Read beginning of contents
            char delimiter = is.readBeginList("List");

            if (s)
            {
                if (delimiter == token::BEGIN_LIST)
                {
                    for (register label i=0; i<s; i++)
                    {
                        is >> L[i];

                        is.fatalCheck
                        (
                            "operator>>(Istream&, UList<T>&) : reading entry"
                        );
                    }
                }
                else
                {
                    T element;
                    is >> element;

                    is.fatalCheck
                    (
                        "operator>>(Istream&, UList<T>&) : "
                        "reading the single entry"
                    );

                    for (register label i=0; i<s; i++)
                    {
                        L[i] = element;
                    }
                }
            }

            // Read end of contents
            is.readEndList("List");
        }
        else
        {
            if (s)
            {
                is.read(reinterpret_cast<char*>(L.data()), s*sizeof(T));

                is.fatalCheck
                (
                    "operator>>(Istream&, UList<T>&) : reading the binary block"
                );
            }
        }
    }
    else if (firstToken.isPunctuation())
    {
        if (firstToken.pToken() != token::BEGIN_LIST)
        {
            FatalIOErrorInFunction(is)
                << "incorrect first token, expected '(', found "
                << firstToken.info()
                << exit(FatalIOError);
        }

        // Putback the opening bracket
        is.putBack(firstToken);

        // Now read as a singly-linked list
        SLList<T> sll(is);

        if (sll.size() != L.size())
        {
            FatalIOErrorInFunction(is)
                << "incorrect length for UList. Read " << sll.size()
                << " expected " << L.size()
                << exit(FatalIOError);
        }

        // Convert the singly-linked list to this list
        label i = 0;
        for
        (
            typename SLList<T>::const_iterator iter = sll.begin();
            iter != sll.end();
            ++iter
        )
        {
            L[i] = iter();
        }

    }
    else
    {
        FatalIOErrorInFunction(is)
            << "incorrect first token, expected <int> or '(', found "
            << firstToken.info()
            << exit(FatalIOError);
    }

    return is;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
