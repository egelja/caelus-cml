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
    CML::StaticHashTable

Description
    STL conforming hash table.

Note
    Uses straight lists as underlying type.
    Is slower to insert than the standard HashTable, but should be more
    memory efficient and faster to access.

SourceFiles
    StaticHashTableI.hpp
    StaticHashTable.cpp
    StaticHashTableIO.cpp

\*---------------------------------------------------------------------------*/

#ifndef StaticHashTable_H
#define StaticHashTable_H

#include "label.hpp"
#include "uLabel.hpp"
#include "word.hpp"
#include "Xfer.hpp"
#include "className.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class T> class List;
template<class T, class Key, class Hash> class StaticHashTable;

template<class T, class Key, class Hash> Istream& operator>>
(
    Istream&,
    StaticHashTable<T, Key, Hash>&
);

template<class T, class Key, class Hash> Ostream& operator<<
(
    Ostream&,
    const StaticHashTable<T, Key, Hash>&
);


/*---------------------------------------------------------------------------*\
                     Class StaticHashTableCore Declaration
\*---------------------------------------------------------------------------*/

//- Template-invariant bits for StaticHashTable
struct StaticHashTableCore
{
    //- Return a canonical (power-of-two) size
    static label canonicalSize(const label);

    //- Construct null
    StaticHashTableCore()
    {}

    //- Define template name and debug
    ClassName("StaticHashTable");

    //- A zero-sized end iterator
    struct iteratorEnd
    {
        //- Construct null
        iteratorEnd()
        {}
    };
};



/*---------------------------------------------------------------------------*\
                       Class StaticHashTable Declaration
\*---------------------------------------------------------------------------*/

template<class T, class Key=word, class Hash=string::hash>
class StaticHashTable
:
    public StaticHashTableCore
{
    // Private data type for table entries

        //- The lookup keys, ordered per hash value
        List<List<Key> > keys_;

        //- For each key the corresponding object.
        List<List<T> > objects_;

        //- The current number of elements in table
        label nElmts_;

        //- Return a canonical (power-of-two) size
        static label canonicalSize(const label);

        //- Return the hash index of the Key within the current table size.
        //  No checks for zero-sized tables.
        inline label hashKeyIndex(const Key&) const;

        //- Assign a new hashed entry to a possibly already existing key
        bool set(const Key&, const T& newElmt, bool protect);


public:


    // Forward declaration of STL iterators

        template<class TRef, class TableRef>
        class Iterator;

        typedef Iterator
        <
            T&,
            StaticHashTable<T, Key, Hash>&
        > iterator;

        typedef Iterator
        <
            const T&,
            const StaticHashTable<T, Key, Hash>&
        > const_iterator;


    // Declare friendship with the iterators

        friend class Iterator
        <
            T&,
            StaticHashTable<T, Key, Hash>&
        >;

        friend class Iterator
        <
            const T&,
            const StaticHashTable<T, Key, Hash>&
        >;


    // Constructors

        //- Construct given initial table size
        StaticHashTable(const label size = 128);

        //- Construct from Istream
        StaticHashTable(Istream&, const label size = 128);

        //- Construct as copy
        StaticHashTable(const StaticHashTable<T, Key, Hash>&);

        //- Construct by transferring the parameter contents
        StaticHashTable(const Xfer<StaticHashTable<T, Key, Hash> >&);


    //- Destructor
    ~StaticHashTable();


    // Member Functions

        // Access

            //- Return number of elements in table.
            inline label size() const;

            //- Return true if the hash table is empty
            inline bool empty() const;

            //- Return true if hashed entry is found in table
            bool found(const Key& key) const;

            //- Find and return an iterator set at the hashed entry
            //  If not found iterator = end()
            iterator find(const Key& key);

            //- Find and return an const_iterator set at the hashed entry
            //  If not found iterator = end()
            const_iterator find(const Key& key) const;

            //- Return the table of contents
            List<Key> toc() const;

            //- Print information
            Ostream& printInfo(Ostream&) const;


        // Edit

            //- Insert a new hashed entry
            bool insert(const Key& key, const T& newElmt);

            //- Assign a new hashed entry, overwriting existing entries
            inline bool set(const Key&, const T& newElmt);

            //- Erase an hashed entry specified by given iterator
            bool erase(const iterator& it);

            //- Erase an hashed entry specified by given key if in table
            bool erase(const Key& key);

            //- Resize the hash table for efficiency
            void resize(const label newSize);

            //- Remove entries in the given hash table from this hash table
            //  Return the number of elements removed
            label erase(const StaticHashTable<T, Key, Hash>&);

            //- Clear all entries from table
            void clear();

            //- Clear the table entries and the table itself.
            //  Equivalent to clear() followed by resize(1)
            void clearStorage();

            //- Transfer the contents of the argument table into this table
            //  and annul the argument table.
            void transfer(StaticHashTable<T, Key, Hash>&);

            //- Transfer contents to the Xfer container
            inline Xfer<StaticHashTable<T, Key, Hash> > xfer();


    // Member Operators

        //- Find and return an hashed entry
        inline T& operator[](const Key&);

        //- Find and return an hashed entry
        inline const T& operator[](const Key&) const;

        //- Find and return an hashed entry, create it null if not present.
        inline T& operator()(const Key&);

        //- Assignment
        void operator=(const StaticHashTable<T, Key, Hash>&);

        //- Equality. Two hash tables are equal if all contents of first are
        //  also in second and vice versa.
        bool operator==(const StaticHashTable<T, Key, Hash>&) const;

        //- The opposite of the equality operation.
        bool operator!=(const StaticHashTable<T, Key, Hash>&) const;


    // STL type definitions

        //- Type of values the StaticHashTable contains.
        typedef T value_type;

        //- Type that can be used for storing into StaticHashTable::value_type
        //  objects.  This type is usually List::value_type&.
        typedef T& reference;

        //- Type that can be used for storing into constant
        //  StaticHashTable::value_type objects.  This type is usually const
        //  StaticHashTable::value_type&.
        typedef const T& const_reference;

        //- The type that can represent the size of a StaticHashTable.
        typedef label size_type;


    // STL iterator

        //- An STL iterator
        template<class TRef, class TableRef>
        class Iterator
        {
            friend class StaticHashTable;

#           ifndef __INTEL_COMPILER
            template<class TRef2, class TableRef2>
            friend class Iterator;
#           endif

            // Private data

                //- Reference to the StaticHashTable this is an iterator for
                TableRef hashTable_;

                //- Current hash index
                label hashIndex_;

                //- Index of current element at hashIndex
                label elemIndex_;


        public:

            // Constructors

                //- Construct from hash table, hash index and element index
                inline Iterator
                (
                    TableRef,
                    label hashIndex_,
                    label elemIndex_
                );

                //- Construct from the non-const iterator
                inline Iterator(const iterator&);


            // Member operators

                inline void operator=(const iterator&);

                inline bool operator==(const iterator&) const;
                inline bool operator==(const const_iterator&) const;

                inline bool operator!=(const iterator&) const;
                inline bool operator!=(const const_iterator&) const;

                inline TRef operator*();
                inline TRef operator()();
//CAELUSFIX : moved from staticHashTableI.hpp
                inline Iterator& operator++()
				{
                    // A negative index is a special value from erase
                    // (see notes in HashTable)
                    if (hashIndex_ < 0)
                    {
                        hashIndex_ = -(hashIndex_+1) - 1;
                    }
                    else
                    {
                        // Try the next element on the local list
                        elemIndex_++;

                        if (elemIndex_ < hashTable_.objects_[hashIndex_].size())
                        {
                            return *this;
                        }
                    }

                    // Step to the next table entry
                    elemIndex_ = 0;

                    while
                    (
                        ++hashIndex_ < hashTable_.objects_.size()
                        && !hashTable_.objects_[hashIndex_].size()
                    )
                    {}


                    if (hashIndex_ >= hashTable_.objects_.size())
                    {
                        // make end iterator
                        hashIndex_ = hashTable_.keys_.size();
                    }

                    return *this;
                }
//CAELUSFIX : Moved here from staticHashTableI.hpp
                inline Iterator operator++(int)
				{
                    iterator tmp = *this;
                    ++*this;
                    return tmp;
                }

                inline const Key& key() const;
        };


        //- iterator set to the beginning of the StaticHashTable
        inline iterator begin();

        //- iterator set to beyond the end of the StaticHashTable
        inline const iterator& end();

        //- const_iterator set to the beginning of the StaticHashTable
        inline const_iterator cbegin() const;

        //- const_iterator set to beyond the end of the StaticHashTable
        inline const const_iterator& cend() const;

        //- const_iterator set to the beginning of the StaticHashTable
        inline const_iterator begin() const;

        //- const_iterator set to beyond the end of the StaticHashTable
        inline const const_iterator& end() const;

    // IOstream Operator

        friend Istream& operator>> <T, Key, Hash>
        (
            Istream&,
            StaticHashTable<T, Key, Hash>&
        );

        friend Ostream& operator<< <T, Key, Hash>
        (
            Ostream&,
            const StaticHashTable<T, Key, Hash>&
        );


private:

        //- iterator returned by end()
        iterator endIter_;

        //- const_iterator returned by end()
        const_iterator endConstIter_;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "error.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * Private Member Classes * * * * * * * * * * * * //

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class T, class Key, class Hash>
inline CML::label
CML::StaticHashTable<T, Key, Hash>::hashKeyIndex(const Key& key) const
{
    // size is power of two - this is the modulus
    return Hash()(key) & (keys_.size() - 1);
}


// * * * * * * * * * * * * * * * Member Functions * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
inline CML::label CML::StaticHashTable<T, Key, Hash>::size() const
{
    return nElmts_;
}


template<class T, class Key, class Hash>
inline bool CML::StaticHashTable<T, Key, Hash>::empty() const
{
    return !nElmts_;
}


template<class T, class Key, class Hash>
inline bool CML::StaticHashTable<T, Key, Hash>::insert
(
    const Key& key,
    const T& newEntry
)
{
    return set(key, newEntry, true);
}


template<class T, class Key, class Hash>
inline bool CML::StaticHashTable<T, Key, Hash>::set
(
    const Key& key,
    const T& newEntry
)
{
    return set(key, newEntry, false);
}


template<class T, class Key, class Hash>
inline CML::Xfer<CML::StaticHashTable<T, Key, Hash> >
CML::StaticHashTable<T, Key, Hash>::xfer()
{
    return xferMove(*this);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
inline T& CML::StaticHashTable<T, Key, Hash>::operator[](const Key& key)
{
    iterator iter = find(key);

    if (iter == end())
    {
        FatalErrorInFunction
            << key << " not found in table.  Valid entries: "
            << toc()
            << exit(FatalError);
    }

    return *iter;
}


template<class T, class Key, class Hash>
inline const T& CML::StaticHashTable<T, Key, Hash>::operator[]
(
    const Key& key
) const
{
    const_iterator iter = find(key);

    if (iter == cend())
    {
        FatalErrorInFunction
            << key << " not found in table.  Valid entries: "
            << toc()
            << exit(FatalError);
    }

    return *iter;
}


template<class T, class Key, class Hash>
inline T& CML::StaticHashTable<T, Key, Hash>::operator()(const Key& key)
{
    iterator iter = find(key);

    if (iter == end())
    {
        insert(key, T());
        return *find(key);
    }
    else
    {
        return *iter;
    }
}


// * * * * * * * * * * * * * * * * STL iterator  * * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline CML::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::Iterator
(
    TableRef hashTbl,
    label hashIndex,
    label elemIndex
)
:
    hashTable_(hashTbl),
    hashIndex_(hashIndex),
    elemIndex_(elemIndex)
{}


template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline CML::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::Iterator
(
    const iterator& iter
)
:
    hashTable_(iter.hashTable_),
    hashIndex_(iter.hashIndex_),
    elemIndex_(iter.elemIndex_)
{}


template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline void
CML::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator=
(
    const iterator& iter
)
{
    this->hashIndex_ = iter.hashIndex_;
    this->elemIndex_ = iter.elemIndex_;
}


template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline bool
CML::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator==
(
    const iterator& iter
) const
{
    return hashIndex_ == iter.hashIndex_ && elemIndex_ == iter.elemIndex_;
}


template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline bool
CML::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator==
(
    const const_iterator& iter
) const
{
    return hashIndex_ == iter.hashIndex_ && elemIndex_ == iter.elemIndex_;
}


template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline bool
CML::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator!=
(
    const iterator& iter
) const
{
    return !operator==(iter);
}


template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline bool
CML::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator!=
(
    const const_iterator& iter
) const
{
    return !operator==(iter);
}


template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline TRef
CML::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator*()
{
    return hashTable_.objects_[hashIndex_][elemIndex_];
}


template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline TRef
CML::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::operator()()
{
    return operator*();
}


// template<class T, class Key, class Hash>
// template<class TRef, class TableRef>
// inline
// typename CML::StaticHashTable<T, Key, Hash>::template Iterator
// <
    // TRef,
    // TableRef
// >&
// CML::StaticHashTable<T, Key, Hash>::Iterator
// <
    // TRef,
    // TableRef
// >::operator++()
// {
    // // A negative index is a special value from erase
    // // (see notes in HashTable)
    // if (hashIndex_ < 0)
    // {
        // hashIndex_ = -(hashIndex_+1) - 1;
    // }
    // else
    // {
        // // Try the next element on the local list
        // elemIndex_++;

        // if (elemIndex_ < hashTable_.objects_[hashIndex_].size())
        // {
            // return *this;
        // }
    // }

    // // Step to the next table entry
    // elemIndex_ = 0;

    // while
    // (
        // ++hashIndex_ < hashTable_.objects_.size()
     // && !hashTable_.objects_[hashIndex_].size()
    // )
    // {}


    // if (hashIndex_ >= hashTable_.objects_.size())
    // {
        // // make end iterator
        // hashIndex_ = hashTable_.keys_.size();
    // }

    // return *this;
// }


// template<class T, class Key, class Hash>
// template<class TRef, class TableRef>
// inline
// typename CML::StaticHashTable<T, Key, Hash>::template Iterator
// <
    // TRef,
    // TableRef
// >
// CML::StaticHashTable<T, Key, Hash>::Iterator
// <
    // TRef,
    // TableRef
// >::operator++
// (
    // int
// )
// {
    // iterator tmp = *this;
    // ++*this;
    // return tmp;
// }


template<class T, class Key, class Hash>
template<class TRef, class TableRef>
inline const Key&
CML::StaticHashTable<T, Key, Hash>::Iterator<TRef, TableRef>::key() const
{
    return hashTable_.keys_[hashIndex_][elemIndex_];
}


template<class T, class Key, class Hash>
inline typename CML::StaticHashTable<T, Key, Hash>::iterator
CML::StaticHashTable<T, Key, Hash>::begin()
{
    // Find first non-empty entry
    forAll(keys_, hashIdx)
    {
        if (keys_[hashIdx].size())
        {
            return iterator(*this, hashIdx, 0);
        }
    }

#   ifdef FULLDEBUG
    if (debug)
    {
        Info<< "StaticHashTable is empty\n";
    }
#   endif

    return StaticHashTable<T, Key, Hash>::endIter_;
}


template<class T, class Key, class Hash>
inline const typename CML::StaticHashTable<T, Key, Hash>::iterator&
CML::StaticHashTable<T, Key, Hash>::end()
{
    return StaticHashTable<T, Key, Hash>::endIter_;
}


template<class T, class Key, class Hash>
inline typename CML::StaticHashTable<T, Key, Hash>::const_iterator
CML::StaticHashTable<T, Key, Hash>::cbegin() const
{
    // Find first non-empty entry
    forAll(keys_, hashIdx)
    {
        if (keys_[hashIdx].size())
        {
            return const_iterator(*this, hashIdx, 0);
        }
    }

#   ifdef FULLDEBUG
    if (debug)
    {
        Info<< "StaticHashTable is empty\n";
    }
#   endif

    return StaticHashTable<T, Key, Hash>::endConstIter_;
}


template<class T, class Key, class Hash>
inline const typename CML::StaticHashTable<T, Key, Hash>::const_iterator&
CML::StaticHashTable<T, Key, Hash>::cend() const
{
    return StaticHashTable<T, Key, Hash>::endConstIter_;
}


template<class T, class Key, class Hash>
inline typename CML::StaticHashTable<T, Key, Hash>::const_iterator
CML::StaticHashTable<T, Key, Hash>::begin() const
{
    return this->cbegin();
}


template<class T, class Key, class Hash>
inline const typename CML::StaticHashTable<T, Key, Hash>::const_iterator&
CML::StaticHashTable<T, Key, Hash>::end() const
{
    return StaticHashTable<T, Key, Hash>::endConstIter_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifndef NoStaticHashTableC
#ifndef StaticHashTable_C
#define StaticHashTable_C

#include "StaticHashTable.hpp"
#include "List.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

inline
CML::label CML::StaticHashTableCore::canonicalSize(const label size)
{
    if (size < 1)
    {
        return 0;
    }

    // enforce power of two
    unsigned int goodSize = size;

    if (goodSize & (goodSize - 1))
    {
        // brute-force is fast enough
        goodSize = 1;
        while (goodSize < unsigned(size))
        {
            goodSize <<= 1;
        }
    }

    return goodSize;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct given initial table size
template<class T, class Key, class Hash>
CML::StaticHashTable<T, Key, Hash>::StaticHashTable(const label size)
:
    StaticHashTableCore(),
    keys_(StaticHashTableCore::canonicalSize(size)),
    objects_(keys_.size()),
    nElmts_(0),
    endIter_(*this, keys_.size(), 0),
    endConstIter_(*this, keys_.size(), 0)
{
    if (size < 1)
    {
        FatalErrorInFunction
            << "Illegal size " << size << " for StaticHashTable."
            << " Minimum size is 1" << abort(FatalError);
    }
}


// Construct as copy
template<class T, class Key, class Hash>
CML::StaticHashTable<T, Key, Hash>::StaticHashTable
(
    const StaticHashTable<T, Key, Hash>& ht
)
:
    StaticHashTableCore(),
    keys_(ht.keys_),
    objects_(ht.objects_),
    nElmts_(ht.nElmts_),
    endIter_(*this, keys_.size(), 0),
    endConstIter_(*this, keys_.size(), 0)
{}


template<class T, class Key, class Hash>
CML::StaticHashTable<T, Key, Hash>::StaticHashTable
(
    const Xfer<StaticHashTable<T, Key, Hash> >& ht
)
:
    StaticHashTableCore(),
    keys_(0),
    objects_(0),
    nElmts_(0),
    endIter_(*this, 0, 0),
    endConstIter_(*this, 0, 0)
{
    transfer(ht());
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
CML::StaticHashTable<T, Key, Hash>::~StaticHashTable()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
bool CML::StaticHashTable<T, Key, Hash>::found(const Key& key) const
{
    if (nElmts_)
    {
        const label hashIdx = hashKeyIndex(key);
        const List<Key>& localKeys = keys_[hashIdx];

        forAll(localKeys, elemIdx)
        {
            if (key == localKeys[elemIdx])
            {
                return true;
            }
        }
    }

#   ifdef FULLDEBUG
    if (debug)
    {
        Info<< "StaticHashTable<T, Key, Hash>::found(const Key&) : "
            << "Entry " << key << " not found in hash table\n";
    }
#   endif

    return false;
}


template<class T, class Key, class Hash>
typename CML::StaticHashTable<T, Key, Hash>::iterator
CML::StaticHashTable<T, Key, Hash>::find
(
    const Key& key
)
{
    if (nElmts_)
    {
        const label hashIdx = hashKeyIndex(key);
        const List<Key>& localKeys = keys_[hashIdx];

        forAll(localKeys, elemIdx)
        {
            if (key == localKeys[elemIdx])
            {
                return iterator(*this, hashIdx, elemIdx);
            }
        }
    }

#   ifdef FULLDEBUG
    if (debug)
    {
        Info<< "StaticHashTable<T, Key, Hash>::find(const Key&) : "
            << "Entry " << key << " not found in hash table\n";
    }
#   endif

    return end();
}


template<class T, class Key, class Hash>
typename CML::StaticHashTable<T, Key, Hash>::const_iterator
CML::StaticHashTable<T, Key, Hash>::find
(
    const Key& key
) const
{
    if (nElmts_)
    {
        const label hashIdx = hashKeyIndex(key);
        const List<Key>& localKeys = keys_[hashIdx];

        forAll(localKeys, elemIdx)
        {
            if (key == localKeys[elemIdx])
            {
                return const_iterator(*this, hashIdx, elemIdx);
            }
        }
    }

#   ifdef FULLDEBUG
    if (debug)
    {
        Info<< "StaticHashTable<T, Key, Hash>::find(const Key&) const : "
            << "Entry " << key << " not found in hash table\n";
    }
#   endif

    return cend();
}


// Return the table of contents
template<class T, class Key, class Hash>
CML::List<Key> CML::StaticHashTable<T, Key, Hash>::toc() const
{
    List<Key> keys(nElmts_);
    label keyI = 0;

    for (const_iterator iter = cbegin(); iter != cend(); ++iter)
    {
        keys[keyI++] = iter.key();
    }

    return keys;
}


template<class T, class Key, class Hash>
bool CML::StaticHashTable<T, Key, Hash>::set
(
    const Key& key,
    const T& newEntry,
    const bool protect
)
{
    const label hashIdx = hashKeyIndex(key);
    List<Key>& localKeys = keys_[hashIdx];

    label existing = localKeys.size();
    forAll(localKeys, elemIdx)
    {
        if (key == localKeys[elemIdx])
        {
            existing = elemIdx;
            break;
        }
    }

    if (existing == localKeys.size())
    {
        // not found, append
        List<T>& localObjects = objects_[hashIdx];

        localKeys.setSize(existing+1);
        localObjects.setSize(existing+1);

        localKeys[existing] = key;
        localObjects[existing] = newEntry;

        nElmts_++;
    }
    else if (protect)
    {
        // found - but protected from overwriting
        // this corresponds to the STL 'insert' convention
#       ifdef FULLDEBUG
        if (debug)
        {
            Info<< "StaticHashTable<T, Key, Hash>::set"
                "(const Key& key, T newEntry, true) : "
                "Cannot insert " << key << " already in hash table\n";
        }
#       endif
        return false;
    }
    else
    {
        // found - overwrite existing entry
        // this corresponds to the Perl convention
        objects_[hashIdx][existing] = newEntry;
    }

    return true;
}


template<class T, class Key, class Hash>
bool CML::StaticHashTable<T, Key, Hash>::erase(const iterator& cit)
{
    if (cit != end())
    {
        List<Key>& localKeys = keys_[cit.hashIndex_];
        List<T>& localObjects = objects_[cit.hashIndex_];

        // Copy down
        for (label i = cit.elemIndex_+1; i < localKeys.size(); i++)
        {
            localKeys[i-1] = localKeys[i];
            localObjects[i-1] = localObjects[i];
        }
        localKeys.setSize(localKeys.size()-1);
        localObjects.setSize(localObjects.size()-1);

        // adjust iterator after erase
        iterator& it = const_cast<iterator&>(cit);

        it.elemIndex_--;
        if (it.elemIndex_ < 0)
        {
            // No previous element in the local list
            // Mark with as special value (see notes in HashTable)
            it.hashIndex_ = -it.hashIndex_ - 1;
            it.elemIndex_ = 0;
        }

        nElmts_--;

#       ifdef FULLDEBUG
        if (debug)
        {
            Info<< "StaticHashTable<T, Key, Hash>::erase(iterator&) : "
                << "hashedEntry removed.\n";
        }
#       endif

        return true;
    }
    else
    {
#       ifdef FULLDEBUG
        if (debug)
        {
            Info<< "StaticHashTable<T, Key, Hash>::erase(iterator&) : "
                << "cannot remove hashedEntry from hash table\n";
        }
#       endif

        return false;
    }
}


template<class T, class Key, class Hash>
bool CML::StaticHashTable<T, Key, Hash>::erase(const Key& key)
{
    iterator it = find(key);

    if (it != end())
    {
        return erase(it);
    }
    else
    {
        return false;
    }
}


template<class T, class Key, class Hash>
CML::label CML::StaticHashTable<T, Key, Hash>::erase
(
    const StaticHashTable<T, Key, Hash>& rhs
)
{
    label count = 0;

    // Remove rhs elements from this table
    // NOTE: could optimize depending on which hash is smaller
    for (iterator iter = this->begin(); iter != this->end(); ++iter)
    {
        if (rhs.found(iter.key()) && erase(iter))
        {
            count++;
        }
    }

   return count;
}


template<class T, class Key, class Hash>
void CML::StaticHashTable<T, Key, Hash>::resize(const label sz)
{
    label newSize = StaticHashTableCore::canonicalSize(sz);

    if (newSize == keys_.size())
    {
#       ifdef FULLDEBUG
        if (debug)
        {
            Info<< "StaticHashTable<T, Key, Hash>::resize(const label) : "
                << "new table size == old table size\n";
        }
#       endif

        return;
    }

    if (newSize < 1)
    {
        FatalErrorInFunction
            << "Illegal size " << newSize << " for StaticHashTable."
            << " Minimum size is 1" << abort(FatalError);
    }


    StaticHashTable<T, Key, Hash> newTable(newSize);

    for (const_iterator iter = cbegin(); iter != cend(); ++iter)
    {
        newTable.insert(iter.key(), *iter);
    }

    transfer(newTable);

    // Adapt end() iterators
    endIter_.hashIndex_ = keys_.size();
    endConstIter_.hashIndex_ = keys_.size();
}


template<class T, class Key, class Hash>
void CML::StaticHashTable<T, Key, Hash>::clear()
{
    forAll(keys_, hashIdx)
    {
        keys_[hashIdx].clear();
        objects_[hashIdx].clear();
    }

    nElmts_ = 0;
}


template<class T, class Key, class Hash>
void CML::StaticHashTable<T, Key, Hash>::clearStorage()
{
    clear();
    resize(1);
}


template<class T, class Key, class Hash>
void CML::StaticHashTable<T, Key, Hash>::transfer
(
    StaticHashTable<T, Key, Hash>& ht
)
{
    // Remove existing elements
    clear();

    // Copy data from ht
    keys_.transfer(ht.keys_);
    objects_.transfer(ht.objects_);

    nElmts_ = ht.nElmts_;
    ht.nElmts_ = 0;

    // Adapt end() iterators
    endIter_.hashIndex_ = keys_.size();
    endConstIter_.hashIndex_ = keys_.size();

    ht.endIter_.hashIndex_ = 0;
    ht.endConstIter_.hashIndex_ = 0;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
void CML::StaticHashTable<T, Key, Hash>::operator=
(
    const StaticHashTable<T, Key, Hash>& rhs
)
{
    // Check for assignment to self
    if (this == &rhs)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }


    // keys could be empty from a previous transfer()
    if (keys_.empty())
    {
        keys_.setSize(rhs.keys_.size());
        objects_.setSize(keys_.size());

        // Adapt end() iterators
        endIter_.hashIndex_ = keys_.size();
        endConstIter_.hashIndex_ = keys_.size();
    }
    else
    {
        clear();
        // keys_.size() does not change so neither does end() iterator.
    }


    for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter)
    {
        insert(iter.key(), *iter);
    }
}

template<class T, class Key, class Hash>
bool CML::StaticHashTable<T, Key, Hash>::operator==
(
    const StaticHashTable<T, Key, Hash>& rhs
) const
{
    // sizes (number of keys) must match

    for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter)
    {
        const_iterator fnd = find(iter.key());

        if (fnd == cend() || fnd() != iter())
        {
            return false;
        }
    }

    return true;
}


template<class T, class Key, class Hash>
bool CML::StaticHashTable<T, Key, Hash>::operator!=
(
    const StaticHashTable<T, Key, Hash>& rhs
) const
{
    return !(operator==(rhs));
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

#include "Istream.hpp"
#include "Ostream.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
CML::StaticHashTable<T, Key, Hash>::StaticHashTable
(
    Istream& is,
    const label size
)
:
    StaticHashTableCore(),
    keys_(StaticHashTableCore::canonicalSize(size)),
    objects_(StaticHashTableCore::canonicalSize(size)),
    nElmts_(0),
    endIter_(*this, keys_.size(), 0),
    endConstIter_(*this, keys_.size(), 0)
{
    if (size < 1)
    {
        FatalErrorInFunction
            << "Illegal size " << size << " for StaticHashTable."
            << " Minimum size is 1" << abort(FatalError);
    }

    operator>>(is, *this);
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
CML::Ostream&
CML::StaticHashTable<T, Key, Hash>::printInfo(Ostream& os) const
{
    label used = 0;
    label maxChain = 0;
    unsigned avgChain = 0;

    // Find first non-empty entry
    forAll(keys_, hashIdx)
    {
        const label count = keys_[hashIdx].size();
        if (count)
        {
            ++used;
            avgChain += count;

            if (maxChain < count)
            {
                maxChain = count;
            }
        }
    }

    os  << "StaticHashTable<T,Key,Hash>"
        << " elements:" << size() << " slots:" << used << "/" << keys_.size()
        << " chaining(avg/max):" << (used ? float(avgChain/used) : 0)
        << "/" << maxChain << endl;

    return os;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class T, class Key, class Hash>
CML::Istream& CML::operator>>(Istream& is, StaticHashTable<T, Key, Hash>& L)
{
    is.fatalCheck("operator>>(Istream&, StaticHashTable<T, Key, Hash>&)");

    // Anull list
    L.clear();

    is.fatalCheck("operator>>(Istream&, StaticHashTable<T, Key, Hash>&)");

    token firstToken(is);

    is.fatalCheck
    (
        "operator>>(Istream&, StaticHashTable<T, Key, Hash>&) : "
        "reading first token"
    );

    if (firstToken.isLabel())
    {
        label s = firstToken.labelToken();

        // Read beginning of contents
        char delimiter = is.readBeginList("StaticHashTable<T, Key, Hash>");

        if (s)
        {
            if (2*s > L.keys_.size())
            {
                L.resize(2*s);
            }

            if (delimiter == token::BEGIN_LIST)
            {
                for (label i=0; i<s; i++)
                {
                    Key key;
                    is >> key;
                    L.insert(key, pTraits<T>(is));

                    is.fatalCheck
                    (
                        "operator>>(Istream&, StaticHashTable<T, Key, Hash>&)"
                        " : reading entry"
                    );
                }
            }
            else
            {
                FatalIOErrorInFunction(is)
                    << "incorrect first token, '(', found " << firstToken.info()
                    << exit(FatalIOError);
            }
        }

        // Read end of contents
        is.readEndList("StaticHashTable");
    }
    else if (firstToken.isPunctuation())
    {
        if (firstToken.pToken() != token::BEGIN_LIST)
        {
            FatalIOErrorInFunction(is)
                << "incorrect first token, '(', found " << firstToken.info()
                << exit(FatalIOError);
        }

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

            Key key;
            is >> key;

            T element;
            is >> element;

            L.insert(key, element);

            is.fatalCheck
            (
                "operator>>(Istream&, StaticHashTable<T, Key, Hash>&) : "
                "reading entry"
            );

            is >> lastToken;
        }
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "incorrect first token, expected <int> or '(', found "
            << firstToken.info()
            << exit(FatalIOError);
    }

    is.fatalCheck("operator>>(Istream&, StaticHashTable<T, Key, Hash>&)");

    return is;
}


template<class T, class Key, class Hash>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const StaticHashTable<T, Key, Hash>& L)
{
    // Write size and start delimiter
    os << nl << L.size() << nl << token::BEGIN_LIST << nl;

    // Write contents
    for
    (
        typename StaticHashTable<T, Key, Hash>::const_iterator iter = L.begin();
        iter != L.end();
        ++iter
    )
    {
        os << iter.key() << token::SPACE << iter() << nl;
    }

    // Write end delimiter
    os << token::END_LIST;

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const StaticHashTable&)");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
