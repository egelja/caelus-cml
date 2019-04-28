/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::UIndirectList

Description
    A List with indirect addressing.

    Like IndirectList but does not store addressing.


\*---------------------------------------------------------------------------*/

#ifndef UIndirectList_H
#define UIndirectList_H

#include "List.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators
template<class T> class UIndirectList;
template<class T> Ostream& operator<<(Ostream&, const UIndirectList<T>&);

/*---------------------------------------------------------------------------*\
                        Class UIndirectList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class UIndirectList
{
    // Private data

        UList<T>& completeList_;
        const labelUList& addressing_;


public:

    // Constructors

        //- Construct given the complete list and the addressing array
        inline UIndirectList(const UList<T>&, const labelUList&);


    // Member Functions

        // Access

            //- Return the number of elements in the list
            inline label size() const;

            //- Return true if the list is empty (ie, size() is zero).
            inline bool empty() const;

            //- Return the first element of the list.
            inline T& first();

            //- Return first element of the list.
            inline const T& first() const;

            //- Return the last element of the list.
            inline T& last();

            //- Return the last element of the list.
            inline const T& last() const;

            //- Return the complete list
            inline const UList<T>& completeList() const;

            //- Return the list addressing
            inline const List<label>& addressing() const;


        // Member Operators

            //- Return the addressed elements as a List
            inline List<T> operator()() const;

            //- Return non-const access to an element
            inline T& operator[](const label);

            //- Return const access to an element
            inline const T& operator[](const label) const;

            //- Assignment from UList of addressed elements
            inline void operator=(const UList<T>&);

            //- Assignment from UIndirectList of addressed elements
            inline void operator=(const UIndirectList<T>&);

            //- Assignment of all entries to the given value
            inline void operator=(const T&);


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


    // Writing

        //- Write the list, with line-breaks in ASCII if its length
        //- exceeds shortListLen.
        //  Using '0' suppresses line-breaks entirely.
        Ostream& writeList(Ostream& os, const label shortListLen=0) const;


    // Ostream operator

        //- Write UIndirectList to Ostream
        //  Binary output is currently still a bit of a problem
        friend Ostream& operator<< <T>
        (
            Ostream&,
            const UIndirectList<T>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
inline CML::UIndirectList<T>::UIndirectList
(
    const UList<T>& completeList,
    const labelUList& addr
)
:
    completeList_(const_cast<UList<T>&>(completeList)),
    addressing_(addr)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline CML::label CML::UIndirectList<T>::size() const
{
    return addressing_.size();
}


template<class T>
inline bool CML::UIndirectList<T>::empty() const
{
    return addressing_.empty();
}


template<class T>
inline T& CML::UIndirectList<T>::first()
{
    return completeList_[addressing_.first()];
}


template<class T>
inline const T& CML::UIndirectList<T>::first() const
{
    return completeList_[addressing_.first()];
}


template<class T>
inline T& CML::UIndirectList<T>::last()
{
    return completeList_[addressing_.last()];
}


template<class T>
inline const T& CML::UIndirectList<T>::last() const
{
    return completeList_[addressing_.last()];
}


template<class T>
inline const CML::UList<T>& CML::UIndirectList<T>::completeList() const
{
    return completeList_;
}


template<class T>
inline const CML::List<CML::label>& CML::UIndirectList<T>::addressing() const
{
    return addressing_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline CML::List<T> CML::UIndirectList<T>::operator()() const
{
    List<T> result(size());

    forAll(*this, i)
    {
        result[i] = operator[](i);
    }

    return result;
}


template<class T>
inline T& CML::UIndirectList<T>::operator[](const label i)
{
    return completeList_[addressing_[i]];
}


template<class T>
inline const T& CML::UIndirectList<T>::operator[](const label i) const
{
    return completeList_[addressing_[i]];
}


template<class T>
inline void CML::UIndirectList<T>::operator=(const UList<T>& ae)
{
    if (addressing_.size() != ae.size())
    {
        FatalErrorInFunction
            << "Addressing and list of addressed elements "
               "have different sizes: "
            << addressing_.size() << " " << ae.size()
            << abort(FatalError);
    }

    forAll(addressing_, i)
    {
        completeList_[addressing_[i]] = ae[i];
    }
}


template<class T>
inline void CML::UIndirectList<T>::operator=(const UIndirectList<T>& ae)
{
    if (addressing_.size() != ae.size())
    {
        FatalErrorInFunction
            << "Addressing and list of addressed elements "
               "have different sizes: "
            << addressing_.size() << " " << ae.size()
            << abort(FatalError);
    }

    forAll(addressing_, i)
    {
        completeList_[addressing_[i]] = ae[i];
    }
}


template<class T>
inline void CML::UIndirectList<T>::operator=(const T& t)
{
    forAll(addressing_, i)
    {
        completeList_[addressing_[i]] = t;
    }
}


#include "Ostream.hpp"
#include "token.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * Ostream Operator *  * * * * * * * * * * * * //

template<class T>
CML::Ostream& CML::UIndirectList<T>::writeList
(
    Ostream& os,
    const label shortListLen
) const
{
    const UIndirectList<T>& list = *this;

    const label len = list.size();

    // Write list contents depending on data format
    if (os.format() == IOstream::ASCII || !contiguous<T>())
    {
        if (contiguous<T>() && list.uniform())
        {
            // Two or more entries, and all entries have identical values.
            os << len << token::BEGIN_BLOCK << list[0] << token::END_BLOCK;
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
            // This is annoying, and wasteful, but currently no alternative
            List<T> lst = list();

            // write(...) includes surrounding start/end delimiters
            os.write
            (
                reinterpret_cast<const char*>(lst.cdata()),
                lst.byteSize()
            );
        }
    }

    os.check(FUNCTION_NAME);
    return os;
}


template<class T>
CML::Ostream& CML::operator<<
(
    CML::Ostream& os,
    const CML::UIndirectList<T>& L
)
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
        else if (L.size() < 11 && contiguous<T>())
        {
            // Write size and start delimiter
            os << L.size() << token::BEGIN_LIST;

            // Write contents
            forAll(L, i)
            {
                if (i) os << token::SPACE;
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
        // this is annoying, and wasteful, but there's currently no alternative

        os << nl << L.size() << nl;

        if (L.size())
        {
            List<T> lst = L();

            os.write
            (
                reinterpret_cast<const char*>(lst.cdata()),
                lst.byteSize()
            );
        }
    }

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const UIndirectList&)");

    return os;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
