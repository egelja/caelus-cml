/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::HashTable

Description
    An STL-conforming hash table.

Note
    Hashing index collisions are handled via chaining using a singly-linked
    list with the colliding entry being added to the head of the linked
    list. Thus copying the hash table (or indeed even resizing it) will
    often result in a different hash order. Use a sorted table-of-contents
    when the hash order is important.


\*---------------------------------------------------------------------------*/

#ifndef HashTable_H
#define HashTable_H

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
template<class T> class UList;
template<class T, class Key, class Hash> class HashTable;
template<class T, class Key, class Hash> class HashPtrTable;

template<class T, class Key, class Hash>
Istream& operator>>(Istream&, HashTable<T, Key, Hash>&);

template<class T, class Key, class Hash>
Ostream& operator<<(Ostream&, const HashTable<T, Key, Hash>&);


/*---------------------------------------------------------------------------*\
                         Class HashTableCore Declaration
\*---------------------------------------------------------------------------*/

//- Template-invariant bits for HashTable
struct HashTableCore
{
    //- Return a canonical (power-of-two) size
    static label canonicalSize(const label);

    //- Maximum allowable table size
    static const label maxTableSize;

    //- Construct null
    HashTableCore()
    {}

    //- Define template name and debug
    ClassName("HashTable");

    //- A zero-sized end iterator
    struct iteratorEnd
    {
        //- Construct null
        iteratorEnd()
        {}
    };

    //- iteratorEnd set to beyond the end of any HashTable
    inline static iteratorEnd cend()
    {
        return iteratorEnd();
    }

    //- iteratorEnd set to beyond the end of any HashTable
    inline static iteratorEnd end()
    {
        return iteratorEnd();
    }
};


/*---------------------------------------------------------------------------*\
                           Class HashTable Declaration
\*---------------------------------------------------------------------------*/

template<class T, class Key=word, class Hash=string::hash>
class HashTable
:
    public HashTableCore
{
    // Private data type for table entries

        //- Structure to hold a hashed entry with SLList for collisions
        struct hashedEntry
        {
            //- The lookup key
            Key key_;

            //- Pointer to next hashedEntry in sub-list
            hashedEntry* next_;

            //- The data object
            T obj_;

            //- Construct from key, next pointer and object
            inline hashedEntry(const Key&, hashedEntry* next, const T&);


        private:
            //- Disallow default bitwise copy construct
            hashedEntry(const hashedEntry&);

            //- Disallow default bitwise assignment
            void operator=(const hashedEntry&);
        };


    // Private data: size of table, the table and current number of elements

        //- The current number of elements in table
        label nElmts_;

        //- Number of primary entries allocated in table
        label tableSize_;

        //- The table of primary entries
        hashedEntry** table_;


    // Private Member Functions

        //- Return a canonical (power-of-two) size
        static label canonicalSize(const label);

        //- Return the hash index of the Key within the current table size.
        //  No checks for zero-sized tables.
        inline label hashKeyIndex(const Key&) const;

        //- Assign a new hashedEntry to a possibly already existing key
        bool set(const Key&, const T& newElmt, bool protect);

public:

    // Forward declaration of iterators

        class iteratorBase;
        class iterator;
        class const_iterator;

        //- Declare friendship with the HashPtrTable class
        template<class T2, class Key2, class Hash2>
        friend class HashPtrTable;

        //- Declare friendship with the iteratorBase
        friend class iteratorBase;

        //- Declare friendship with the iterator
        friend class iterator;

        //- Declare friendship with the const_iterator
        friend class const_iterator;


    // Constructors

        //- Construct given initial table size
        HashTable(const label size = 128);

        //- Construct from Istream
        HashTable(Istream&, const label size = 128);

        //- Construct as copy
        HashTable(const HashTable<T, Key, Hash>&);

        //- Construct by transferring the parameter contents
        HashTable(const Xfer<HashTable<T, Key, Hash> >&);


    //- Destructor
    ~HashTable();


    // Member Functions

        // Access

            //- The size of the underlying table
            inline label capacity() const;

            //- Return number of elements in table
            inline label size() const;

            //- Return true if the hash table is empty
            inline bool empty() const;

            //- Return true if hashedEntry is found in table
            bool found(const Key&) const;

            //- Find and return an iterator set at the hashedEntry
            //  If not found iterator = end()
            iterator find(const Key&);

            //- Find and return an const_iterator set at the hashedEntry
            //  If not found iterator = end()
            const_iterator find(const Key&) const;

            //- Return the table of contents
            List<Key> toc() const;

            //- Return the table of contents as a sorted list
            List<Key> sortedToc() const;

            //- Print information
            Ostream& printInfo(Ostream&) const;


        // Edit

            //- Insert a new hashedEntry
            inline bool insert(const Key&, const T& newElmt);

            //- Assign a new hashedEntry, overwriting existing entries
            inline bool set(const Key&, const T& newElmt);

            //- Erase a hashedEntry specified by given iterator
            //  This invalidates the iterator until the next operator++
            bool erase(const iterator&);

            //- Erase a hashedEntry specified by the given key
            bool erase(const Key&);

            //- Remove entries given by the listed keys from this HashTable
            //  Return the number of elements removed
            label erase(const UList<Key>&);

            //- Remove entries given by the given keys from this HashTable
            //  Return the number of elements removed.
            //  The parameter HashTable needs the same type of key, but the
            //  type of values held and the hashing function are arbitrary.
            template<class AnyType, class AnyHash>
            label erase(const HashTable<AnyType, Key, AnyHash>&);

            //- Resize the hash table for efficiency
            void resize(const label newSize);

            //- Clear all entries from table
            void clear();

            //- Clear the table entries and the table itself.
            //  Equivalent to clear() followed by resize(0)
            void clearStorage();

            //- Shrink the allocated table to approx. twice number of elements
            void shrink();

            //- Transfer the contents of the argument table into this table
            //  and annul the argument table.
            void transfer(HashTable<T, Key, Hash>&);

            //- Transfer contents to the Xfer container
            inline Xfer<HashTable<T, Key, Hash> > xfer();


    // Member Operators

        //- Find and return a hashedEntry
        inline T& operator[](const Key&);

        //- Find and return a hashedEntry
        inline const T& operator[](const Key&) const;

        //- Find and return a hashedEntry, create it null if not present
        inline T& operator()(const Key&);

        //- Assignment
        void operator=(const HashTable<T, Key, Hash>&);

        //- Equality. Hash tables are equal if the keys and values are equal.
        //  Independent of table storage size and table order.
        bool operator==(const HashTable<T, Key, Hash>&) const;

        //- The opposite of the equality operation. Takes linear time.
        bool operator!=(const HashTable<T, Key, Hash>&) const;



    // STL type definitions

        //- Type of values the HashTable contains.
        typedef T value_type;

        //- Type that can be used for storing into HashTable::value_type
        //  objects.  This type is usually List::value_type&.
        typedef T& reference;

        //- Type that can be used for storing into constant
        //  HashTable::value_type objects.  This type is usually const
        //  HashTable::value_type&.
        typedef const T& const_reference;

        //- The type that can represent the size of a HashTable.
        typedef label size_type;


    // Iterators and helpers

        //- The iterator base for HashTable
        //  Note: data and functions are protected, to allow reuse by iterator
        //  and prevent most external usage.
        //  iterator and const_iterator have the same size, allowing
        //  us to reinterpret_cast between them (if desired)
        class iteratorBase
        {
            // Private Data

                //- Pointer to the HashTable for which this is an iterator
                //  This also lets us use the default bitwise copy/assignment
                HashTable<T, Key, Hash>* hashTable_;

                //- Current element
                hashedEntry* entryPtr_;

                //- Current hash index
                label hashIndex_;


        protected:

            // Constructors

                //- Construct null - equivalent to an 'end' position
                inline iteratorBase();

                //- Construct from hash table, moving to its 'begin' position
                inline explicit iteratorBase
                (
                    const HashTable<T, Key, Hash>* curHashTable
                );

                //- Construct from hash table, element and hash index
                inline explicit iteratorBase
                (
                    const HashTable<T, Key, Hash>* curHashTable,
                    const hashedEntry* elmt,
                    const label hashIndex
                );


            // Protected Member Functions

                //- Increment to the next position
                inline void increment();

                //- Erase the HashTable element at the current position
                bool erase();

                //- Return non-const access to referenced object
                inline T& object();

                //- Return const access to referenced object
                inline const T& cobject() const;


        public:

            // Member operators

                // Access

                //- Return the Key corresponding to the iterator
                inline const Key& key() const;

                //- Compare hashedEntry element pointers
                inline bool operator==(const iteratorBase&) const;
                inline bool operator!=(const iteratorBase&) const;

                //- Compare hashedEntry to iteratorEnd pointers
                inline bool operator==(const iteratorEnd& unused) const;
                inline bool operator!=(const iteratorEnd& unused) const;
        };


        //- An STL-conforming iterator
        class iterator
        :
            public iteratorBase
        {
            friend class HashTable;

            // Private Member Functions

                //- Construct from hash table, moving to its 'begin' position
                inline explicit iterator
                (
                    HashTable<T, Key, Hash>* curHashTable
                );

                //- Construct from hash table, element and hash index
                inline explicit iterator
                (
                    HashTable<T, Key, Hash>* curHashTable,
                    hashedEntry* elmt,
                    const label hashIndex
                );


        public:

            // Constructors

                //- Construct null (end iterator)
                inline iterator();

                //- Construct end iterator
                inline iterator(const iteratorEnd& unused);


            // Member operators

                //- Return referenced hash value
                inline T& operator*();
                inline T& operator()();

                //- Return referenced hash value
                inline const T& operator*() const;
                inline const T& operator()() const;

                inline iterator& operator++();
                inline iterator operator++(int);
        };

        //- iterator set to the beginning of the HashTable
        inline iterator begin();


    // STL const_iterator

        //- An STL-conforming const_iterator
        class const_iterator
        :
            public iteratorBase
        {
            friend class HashTable;

            // Private Member Functions

                //- Construct from hash table, moving to its 'begin' position
                inline explicit const_iterator
                (
                    const HashTable<T, Key, Hash>* curHashTable
                );

                //- Construct from hash table, element and hash index
                inline explicit const_iterator
                (
                    const HashTable<T, Key, Hash>* curHashTable,
                    const hashedEntry* elmt,
                    const label hashIndex
                );


        public:

            // Constructors

                //- Construct null (end iterator)
                inline const_iterator();

                //- Construct from iterator
                inline const_iterator(const iterator&);

                //- Construct end iterator
                inline const_iterator(const iteratorEnd& unused);


            // Member operators

                //- Return referenced hash value
                inline const T& operator*() const;
                inline const T& operator()() const;

                inline const_iterator& operator++();
                inline const_iterator operator++(int);

        };


        //- const_iterator set to the beginning of the HashTable
        inline const_iterator cbegin() const;

        //- const_iterator set to the beginning of the HashTable
        inline const_iterator begin() const;


    // IOstream Operator

        friend Istream& operator>> <T, Key, Hash>
        (
            Istream&,
            HashTable<T, Key, Hash>&
        );

        friend Ostream& operator<< <T, Key, Hash>
        (
            Ostream&,
            const HashTable<T, Key, Hash>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "error.hpp"

// * * * * * * * * * * * * * Private Member Classes * * * * * * * * * * * * //

template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::hashedEntry::hashedEntry
(
    const Key& key,
    hashedEntry* next,
    const T& obj
)
:
    key_(key),
    next_(next),
    obj_(obj)
{}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class T, class Key, class Hash>
inline CML::label
CML::HashTable<T, Key, Hash>::hashKeyIndex(const Key& key) const
{
    // size is power of two - this is the modulus
    return Hash()(key) & (tableSize_ - 1);
}


// * * * * * * * * * * * * * * * Member Functions * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
inline CML::label CML::HashTable<T, Key, Hash>::capacity() const
{
    return tableSize_;
}


template<class T, class Key, class Hash>
inline CML::label CML::HashTable<T, Key, Hash>::size() const
{
    return nElmts_;
}


template<class T, class Key, class Hash>
inline bool CML::HashTable<T, Key, Hash>::empty() const
{
    return !nElmts_;
}


template<class T, class Key, class Hash>
inline bool CML::HashTable<T, Key, Hash>::insert
(
    const Key& key,
    const T& newEntry
)
{
    return this->set(key, newEntry, true);
}


template<class T, class Key, class Hash>
inline bool CML::HashTable<T, Key, Hash>::set
(
    const Key& key,
    const T& newEntry
)
{
    return this->set(key, newEntry, false);
}


template<class T, class Key, class Hash>
inline CML::Xfer<CML::HashTable<T, Key, Hash> >
CML::HashTable<T, Key, Hash>::xfer()
{
    return xferMove(*this);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
inline T& CML::HashTable<T, Key, Hash>::operator[](const Key& key)
{
    iterator iter = this->find(key);

    if (iter == this->end())
    {
        FatalErrorInFunction
            << key << " not found in table.  Valid entries: "
            << toc()
            << exit(FatalError);
    }

    return *iter;
}


template<class T, class Key, class Hash>
inline const T& CML::HashTable<T, Key, Hash>::operator[](const Key& key) const
{
    const_iterator iter = this->find(key);

    if (iter == this->cend())
    {
        FatalErrorInFunction
            << key << " not found in table.  Valid entries: "
            << toc()
            << exit(FatalError);
    }

    return *iter;
}


template<class T, class Key, class Hash>
inline T& CML::HashTable<T, Key, Hash>::operator()(const Key& key)
{
    iterator iter = this->find(key);

    if (iter == this->end())
    {
        this->insert(key, T());
        return *find(key);
    }
    else
    {
        return *iter;
    }
}


// * * * * * * * * * * * * * * * iterator base * * * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::iteratorBase::iteratorBase()
:
    hashTable_(0),
    entryPtr_(0),
    hashIndex_(0)
{}


template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::iteratorBase::iteratorBase
(
    const HashTable<T, Key, Hash>* hashTbl
)
:
    hashTable_(const_cast<HashTable<T, Key, Hash>*>(hashTbl)),
    entryPtr_(0),
    hashIndex_(0)
{
    if (hashTable_->nElmts_)
    {
        // find first non-nullptr table entry
        while
        (
            !(entryPtr_ = hashTable_->table_[hashIndex_])
         && ++hashIndex_ < hashTable_->tableSize_
        )
        {}

        if (hashIndex_ >= hashTable_->tableSize_)
        {
            // make into an end iterator
            entryPtr_ = 0;
            hashIndex_ = 0;
        }
    }
}


template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::iteratorBase::iteratorBase
(
    const HashTable<T, Key, Hash>* hashTbl,
    const hashedEntry* elmt,
    const label hashIndex
)
:
    hashTable_(const_cast<HashTable<T, Key, Hash>*>(hashTbl)),
    entryPtr_(const_cast<hashedEntry*>(elmt)),
    hashIndex_(hashIndex)
{}


template<class T, class Key, class Hash>
inline void
CML::HashTable<T, Key, Hash>::iteratorBase::increment()
{
    // A negative index is a special value from erase
    if (hashIndex_ < 0)
    {
        // the markPos='-curPos-1', but we wish to continue at 'curPos-1'
        // thus use '-(markPos+1) -1'
        hashIndex_ = -(hashIndex_+1) - 1;
    }
    else if (entryPtr_)
    {
        if (entryPtr_->next_)
        {
            // Move to next element on the SLList
            entryPtr_ = entryPtr_->next_;
            return;
        }
    }
    // else
    // {
    //     // if we reach here (entryPtr_ is nullptr) it is already at the end()
    //     // we should probably stop
    // }


    // Step to the next table entry
    while
    (
        ++hashIndex_ < hashTable_->tableSize_
     && !(entryPtr_ = hashTable_->table_[hashIndex_])
    )
    {}

    if (hashIndex_ >= hashTable_->tableSize_)
    {
        // make into an end iterator
        entryPtr_ = 0;
        hashIndex_ = 0;
    }
}


template<class T, class Key, class Hash>
inline
const Key& CML::HashTable<T, Key, Hash>::iteratorBase::key() const
{
    return entryPtr_->key_;
}


template<class T, class Key, class Hash>
inline T&
CML::HashTable<T, Key, Hash>::iteratorBase::object()
{
    return entryPtr_->obj_;
}


template<class T, class Key, class Hash>
inline const T&
CML::HashTable<T, Key, Hash>::iteratorBase::cobject() const
{
    return entryPtr_->obj_;
}


template<class T, class Key, class Hash>
inline bool CML::HashTable<T, Key, Hash>::iteratorBase::operator==
(
    const iteratorBase& iter
) const
{
    return entryPtr_ == iter.entryPtr_;
}


template<class T, class Key, class Hash>
inline bool CML::HashTable<T, Key, Hash>::iteratorBase::operator!=
(
    const iteratorBase& iter
) const
{
    return entryPtr_ != iter.entryPtr_;
}


template<class T, class Key, class Hash>
inline bool CML::HashTable<T, Key, Hash>::iteratorBase::operator==
(
    const iteratorEnd&
) const
{
    return !entryPtr_;
}


template<class T, class Key, class Hash>
inline bool CML::HashTable<T, Key, Hash>::iteratorBase::operator!=
(
    const iteratorEnd&
) const
{
    return entryPtr_;
}


// * * * * * * * * * * * * * * * * STL iterator  * * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::iterator::iterator()
:
    iteratorBase()
{}


template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::iterator::iterator
(
    const iteratorEnd&
)
:
    iteratorBase()
{}


template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::iterator::iterator
(
    HashTable<T, Key, Hash>* hashTbl
)
:
    iteratorBase(hashTbl)
{}


template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::iterator::iterator
(
    HashTable<T, Key, Hash>* hashTbl,
    hashedEntry* elmt,
    const label hashIndex
)
:
    iteratorBase(hashTbl, elmt, hashIndex)
{}


template<class T, class Key, class Hash>
inline T&
CML::HashTable<T, Key, Hash>::iterator::operator*()
{
    return this->object();
}


template<class T, class Key, class Hash>
inline T&
CML::HashTable<T, Key, Hash>::iterator::operator()()
{
    return this->object();
}


template<class T, class Key, class Hash>
inline const T&
CML::HashTable<T, Key, Hash>::iterator::operator*() const
{
    return this->cobject();
}


template<class T, class Key, class Hash>
inline const T&
CML::HashTable<T, Key, Hash>::iterator::operator()() const
{
    return this->cobject();
}


template<class T, class Key, class Hash>
inline
typename CML::HashTable<T, Key, Hash>::iterator&
CML::HashTable<T, Key, Hash>::iterator::operator++()
{
    this->increment();
    return *this;
}


template<class T, class Key, class Hash>
inline typename CML::HashTable<T, Key, Hash>::iterator
CML::HashTable<T, Key, Hash>::iterator::operator++(int)
{
    iterator old = *this;
    this->increment();
    return old;
}


template<class T, class Key, class Hash>
inline typename CML::HashTable<T, Key, Hash>::iterator
CML::HashTable<T, Key, Hash>::begin()
{
    return iterator(this);
}


// * * * * * * * * * * * * * * * STL const_iterator * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::const_iterator::const_iterator()
:
    iteratorBase()
{}


template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::const_iterator::const_iterator
(
    const HashTable<T, Key, Hash>::iterator& iter
)
:
    iteratorBase(iter)
{}


template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::const_iterator::const_iterator
(
    const iteratorEnd&
)
:
    iteratorBase()
{}


template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::const_iterator::const_iterator
(
    const HashTable<T, Key, Hash>* hashTbl
)
:
    iteratorBase(hashTbl)
{}


template<class T, class Key, class Hash>
inline CML::HashTable<T, Key, Hash>::const_iterator::const_iterator
(
    const HashTable<T, Key, Hash>* hashTbl,
    const hashedEntry* elmt,
    const label hashIndex
)
:
    iteratorBase(hashTbl, elmt, hashIndex)
{}


template<class T, class Key, class Hash>
inline const T&
CML::HashTable<T, Key, Hash>::const_iterator::operator*() const
{
    return this->cobject();
}


template<class T, class Key, class Hash>
inline const T&
CML::HashTable<T, Key, Hash>::const_iterator::operator()() const
{
    return this->cobject();
}


template<class T, class Key, class Hash>
inline
typename CML::HashTable<T, Key, Hash>::const_iterator&
CML::HashTable<T, Key, Hash>::const_iterator::operator++()
{
    this->increment();
    return *this;
}


template<class T, class Key, class Hash>
inline typename CML::HashTable<T, Key, Hash>::const_iterator
CML::HashTable<T, Key, Hash>::const_iterator::operator++(int)
{
    const_iterator old = *this;
    this->increment();
    return old;
}


template<class T, class Key, class Hash>
inline typename CML::HashTable<T, Key, Hash>::const_iterator
CML::HashTable<T, Key, Hash>::cbegin() const
{
    return const_iterator(this);
}


template<class T, class Key, class Hash>
inline typename CML::HashTable<T, Key, Hash>::const_iterator
CML::HashTable<T, Key, Hash>::begin() const
{
    return this->cbegin();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifndef NoHashTableC
#ifndef HashTable_C
#define HashTable_C

#include "HashTable.hpp"
#include "List.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
CML::HashTable<T, Key, Hash>::HashTable(const label size)
:
    HashTableCore(),
    nElmts_(0),
    tableSize_(HashTableCore::canonicalSize(size)),
    table_(nullptr)
{
    if (tableSize_)
    {
        table_ = new hashedEntry*[tableSize_];

        for (label hashIdx = 0; hashIdx < tableSize_; hashIdx++)
        {
            table_[hashIdx] = 0;
        }
    }
}


template<class T, class Key, class Hash>
CML::HashTable<T, Key, Hash>::HashTable(const HashTable<T, Key, Hash>& ht)
:
    HashTableCore(),
    nElmts_(0),
    tableSize_(ht.tableSize_),
    table_(nullptr)
{
    if (tableSize_)
    {
        table_ = new hashedEntry*[tableSize_];

        for (label hashIdx = 0; hashIdx < tableSize_; hashIdx++)
        {
            table_[hashIdx] = 0;
        }

        for (const_iterator iter = ht.cbegin(); iter != ht.cend(); ++iter)
        {
            insert(iter.key(), *iter);
        }
    }
}

template<class T, class Key, class Hash>
CML::HashTable<T, Key, Hash>::HashTable
(
    const Xfer<HashTable<T, Key, Hash> >& ht
)
:
    HashTableCore(),
    nElmts_(0),
    tableSize_(0),
    table_(nullptr)
{
    transfer(ht());
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
CML::HashTable<T, Key, Hash>::~HashTable()
{
    if (table_)
    {
        clear();
        delete[] table_;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
bool CML::HashTable<T, Key, Hash>::found(const Key& key) const
{
    if (nElmts_)
    {
        const label hashIdx = hashKeyIndex(key);

        for (hashedEntry* ep = table_[hashIdx]; ep; ep = ep->next_)
        {
            if (key == ep->key_)
            {
                return true;
            }
        }
    }

#   ifdef FULLDEBUG
    if (debug)
    {
        Info<< "HashTable<T, Key, Hash>::found(const Key& key) : "
            << "Entry " << key << " not found in hash table\n";
    }
#   endif

    return false;
}


template<class T, class Key, class Hash>
typename CML::HashTable<T, Key, Hash>::iterator
CML::HashTable<T, Key, Hash>::find
(
    const Key& key
)
{
    if (nElmts_)
    {
        const label hashIdx = hashKeyIndex(key);

        for (hashedEntry* ep = table_[hashIdx]; ep; ep = ep->next_)
        {
            if (key == ep->key_)
            {
                return iterator(this, ep, hashIdx);
            }
        }
    }

#   ifdef FULLDEBUG
    if (debug)
    {
        Info<< "HashTable<T, Key, Hash>::find(const Key& key) : "
            << "Entry " << key << " not found in hash table\n";
    }
#   endif

    return iterator();
}


template<class T, class Key, class Hash>
typename CML::HashTable<T, Key, Hash>::const_iterator
CML::HashTable<T, Key, Hash>::find
(
    const Key& key
) const
{
    if (nElmts_)
    {
        const label hashIdx = hashKeyIndex(key);

        for (hashedEntry* ep = table_[hashIdx]; ep; ep = ep->next_)
        {
            if (key == ep->key_)
            {
                return const_iterator(this, ep, hashIdx);
            }
        }
    }

#   ifdef FULLDEBUG
    if (debug)
    {
        Info<< "HashTable<T, Key, Hash>::find(const Key& key) const : "
            << "Entry " << key << " not found in hash table\n";
    }
#   endif

    return const_iterator();
}


template<class T, class Key, class Hash>
CML::List<Key> CML::HashTable<T, Key, Hash>::toc() const
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
CML::List<Key> CML::HashTable<T, Key, Hash>::sortedToc() const
{
    List<Key> sortedLst = this->toc();
    sort(sortedLst);

    return sortedLst;
}


template<class T, class Key, class Hash>
bool CML::HashTable<T, Key, Hash>::set
(
    const Key& key,
    const T& newEntry,
    const bool protect
)
{
    if (!tableSize_)
    {
        resize(2);
    }

    const label hashIdx = hashKeyIndex(key);

    hashedEntry* existing = 0;
    hashedEntry* prev = 0;

    for (hashedEntry* ep = table_[hashIdx]; ep; ep = ep->next_)
    {
        if (key == ep->key_)
        {
            existing = ep;
            break;
        }
        prev = ep;
    }

    // not found, insert it at the head
    if (!existing)
    {
        table_[hashIdx] = new hashedEntry(key, table_[hashIdx], newEntry);
        nElmts_++;

        if (double(nElmts_)/tableSize_ > 0.8 && tableSize_ < maxTableSize)
        {
#           ifdef FULLDEBUG
            if (debug)
            {
                Info<< "HashTable<T, Key, Hash>::set"
                    "(const Key& key, T newEntry) : "
                    "Doubling table size\n";
            }
#           endif

            resize(2*tableSize_);
        }
    }
    else if (protect)
    {
        // found - but protected from overwriting
        // this corresponds to the STL 'insert' convention
#       ifdef FULLDEBUG
        if (debug)
        {
            Info<< "HashTable<T, Key, Hash>::set"
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
        hashedEntry* ep = new hashedEntry(key, existing->next_, newEntry);

        // replace existing element - within list or insert at the head
        if (prev)
        {
            prev->next_ = ep;
        }
        else
        {
            table_[hashIdx] = ep;
        }

        delete existing;
    }

    return true;
}


template<class T, class Key, class Hash>
bool CML::HashTable<T, Key, Hash>::iteratorBase::erase()
{
    // note: entryPtr_ is nullptr for end(), so this catches that too
    if (entryPtr_)
    {
        // Search element before entryPtr_
        hashedEntry* prev = 0;

        for
        (
            hashedEntry* ep = hashTable_->table_[hashIndex_];
            ep;
            ep = ep->next_
        )
        {
            if (ep == entryPtr_)
            {
                break;
            }
            prev = ep;
        }

        if (prev)
        {
            // has an element before entryPtr - reposition to there
            prev->next_ = entryPtr_->next_;
            delete entryPtr_;
            entryPtr_ = prev;
        }
        else
        {
            // entryPtr was first element on SLList
            hashTable_->table_[hashIndex_] = entryPtr_->next_;
            delete entryPtr_;

            // assign any non-nullptr pointer value so it doesn't look
            // like end()/cend()
            entryPtr_ = reinterpret_cast<hashedEntry*>(this);

            // Mark with special hashIndex value to signal it has been rewound.
            // The next increment will bring it back to the present location.
            //
            // From the current position 'curPos', we wish to continue at
            // prevPos='curPos-1', which we mark as markPos='-curPos-1'.
            // The negative lets us notice it is special, the extra '-1'
            // is needed to avoid ambiguity for position '0'.
            // To retrieve prevPos, we would later use '-(markPos+1) - 1'
            hashIndex_ = -hashIndex_ - 1;
        }

        hashTable_->nElmts_--;

        return true;
    }
    else
    {
        return false;
    }
}



// NOTE:
// We use (const iterator&) here, but manipulate its contents anyhow.
// The parameter should be (iterator&), but then the compiler doesn't find
// it correctly and tries to call as (iterator) instead.
//
template<class T, class Key, class Hash>
bool CML::HashTable<T, Key, Hash>::erase(const iterator& iter)
{
    // adjust iterator after erase
    return const_cast<iterator&>(iter).erase();
}


template<class T, class Key, class Hash>
bool CML::HashTable<T, Key, Hash>::erase(const Key& key)
{
    return erase(find(key));
}


template<class T, class Key, class Hash>
CML::label CML::HashTable<T, Key, Hash>::erase(const UList<Key>& keys)
{
    const label nTotal = nElmts_;
    label count = 0;

    // Remove listed keys from this table - terminates early if possible
    for (label keyI = 0; count < nTotal && keyI < keys.size(); ++keyI)
    {
        if (erase(keys[keyI]))
        {
            count++;
        }
    }

    return count;
}


template<class T, class Key, class Hash>
template<class AnyType, class AnyHash>
CML::label CML::HashTable<T, Key, Hash>::erase
(
    const HashTable<AnyType, Key, AnyHash>& rhs
)
{
    label count = 0;

    // Remove rhs keys from this table - terminates early if possible
    // Could optimize depending on which hash is smaller ...
    for (iterator iter = begin(); iter != end(); ++iter)
    {
        if (rhs.found(iter.key()) && erase(iter))
        {
            count++;
        }
    }

    return count;
}


template<class T, class Key, class Hash>
void CML::HashTable<T, Key, Hash>::resize(const label sz)
{
    label newSize = HashTableCore::canonicalSize(sz);

    if (newSize == tableSize_)
    {
#       ifdef FULLDEBUG
        if (debug)
        {
            Info<< "HashTable<T, Key, Hash>::resize(const label) : "
                << "new table size == old table size\n";
        }
#       endif

        return;
    }

    HashTable<T, Key, Hash>* tmpTable = new HashTable<T, Key, Hash>(newSize);

    for (const_iterator iter = cbegin(); iter != cend(); ++iter)
    {
        tmpTable->insert(iter.key(), *iter);
    }

    label oldSize = tableSize_;
    tableSize_ = tmpTable->tableSize_;
    tmpTable->tableSize_ = oldSize;

    hashedEntry** oldTable = table_;
    table_ = tmpTable->table_;
    tmpTable->table_ = oldTable;

    delete tmpTable;
}


template<class T, class Key, class Hash>
void CML::HashTable<T, Key, Hash>::clear()
{
    if (nElmts_)
    {
        for (label hashIdx = 0; hashIdx < tableSize_; hashIdx++)
        {
            if (table_[hashIdx])
            {
                hashedEntry* ep = table_[hashIdx];
                while (hashedEntry* next = ep->next_)
                {
                    delete ep;
                    ep = next;
                }
                delete ep;
                table_[hashIdx] = 0;
            }
        }
        nElmts_ = 0;
    }
}


template<class T, class Key, class Hash>
void CML::HashTable<T, Key, Hash>::clearStorage()
{
    clear();
    resize(0);
}


template<class T, class Key, class Hash>
void CML::HashTable<T, Key, Hash>::shrink()
{
    const label newSize = HashTableCore::canonicalSize(nElmts_);

    if (newSize < tableSize_)
    {
        // avoid having the table disappear on us
        resize(newSize ? newSize : 2);
    }
}


template<class T, class Key, class Hash>
void CML::HashTable<T, Key, Hash>::transfer(HashTable<T, Key, Hash>& ht)
{
    // as per the Destructor
    if (table_)
    {
        clear();
        delete[] table_;
    }

    tableSize_ = ht.tableSize_;
    ht.tableSize_ = 0;

    table_ = ht.table_;
    ht.table_ = nullptr;

    nElmts_ = ht.nElmts_;
    ht.nElmts_ = 0;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
void CML::HashTable<T, Key, Hash>::operator=
(
    const HashTable<T, Key, Hash>& rhs
)
{
    // Check for assignment to self
    if (this == &rhs)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    // could be zero-sized from a previous transfer()
    if (!tableSize_)
    {
        resize(rhs.tableSize_);
    }
    else
    {
        clear();
    }

    for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter)
    {
        insert(iter.key(), *iter);
    }
}


template<class T, class Key, class Hash>
bool CML::HashTable<T, Key, Hash>::operator==
(
    const HashTable<T, Key, Hash>& rhs
) const
{
    // sizes (number of keys) must match
    if (size() != rhs.size())
    {
        return false;
    }

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
bool CML::HashTable<T, Key, Hash>::operator!=
(
    const HashTable<T, Key, Hash>& rhs
) const
{
    return !(operator==(rhs));
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

#include "Istream.hpp"
#include "Ostream.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
CML::HashTable<T, Key, Hash>::HashTable(Istream& is, const label size)
:
    HashTableCore(),
    nElmts_(0),
    tableSize_(HashTableCore::canonicalSize(size)),
    table_(nullptr)
{
    if (tableSize_)
    {
        table_ = new hashedEntry*[tableSize_];

        for (label hashIdx = 0; hashIdx < tableSize_; hashIdx++)
        {
            table_[hashIdx] = 0;
        }
    }

    operator>>(is, *this);
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
CML::Ostream&
CML::HashTable<T, Key, Hash>::printInfo(Ostream& os) const
{
    label used = 0;
    label maxChain = 0;
    unsigned avgChain = 0;

    for (label hashIdx = 0; hashIdx < tableSize_; ++hashIdx)
    {
        label count = 0;
        for (hashedEntry* ep = table_[hashIdx]; ep; ep = ep->next_)
        {
            ++count;
        }

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

    os  << "HashTable<T,Key,Hash>"
        << " elements:" << size() << " slots:" << used << "/" << tableSize_
        << " chaining(avg/max):" << (used ? (float(avgChain)/used) : 0)
        << "/" << maxChain << endl;

    return os;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class T, class Key, class Hash>
CML::Istream& CML::operator>>
(
    Istream& is,
    HashTable<T, Key, Hash>& L
)
{
    is.fatalCheck("operator>>(Istream&, HashTable<T, Key, Hash>&)");

    // Anull list
    L.clear();

    is.fatalCheck("operator>>(Istream&, HashTable<T, Key, Hash>&)");

    token firstToken(is);

    is.fatalCheck
    (
        "operator>>(Istream&, HashTable<T, Key, Hash>&) : "
        "reading first token"
    );

    if (firstToken.isLabel())
    {
        label s = firstToken.labelToken();

        // Read beginning of contents
        char delimiter = is.readBeginList("HashTable<T, Key, Hash>");

        if (s)
        {
            if (2*s > L.tableSize_)
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
                        "operator>>(Istream&, HashTable<T, Key, Hash>&) : "
                        "reading entry"
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
        is.readEndList("HashTable");
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
                "operator>>(Istream&, HashTable<T, Key, Hash>&) : "
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

    is.fatalCheck("operator>>(Istream&, HashTable<T, Key, Hash>&)");

    return is;
}


template<class T, class Key, class Hash>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const HashTable<T, Key, Hash>& L
)
{
    // Write size and start delimiter
    os << nl << L.size() << nl << token::BEGIN_LIST << nl;

    // Write contents
    for
    (
        typename HashTable<T, Key, Hash>::const_iterator iter = L.cbegin();
        iter != L.cend();
        ++iter
    )
    {
        os << iter.key() << token::SPACE << iter() << nl;
    }

    // Write end delimiter
    os << token::END_LIST;

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const HashTable&)");

    return os;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
