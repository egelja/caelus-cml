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
    CML::HashSet

Description
    A HashTable with keys but without contents.

Typedef
    CML::wordHashSet

Description
    A HashSet with (the default) word keys.

Typedef
    CML::labelHashSet

Description
    A HashSet with label keys.

\*---------------------------------------------------------------------------*/

#ifndef HashSet_H
#define HashSet_H

#include "HashTable.hpp"
#include "nil.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class HashSet Declaration
\*---------------------------------------------------------------------------*/

template<class Key=word, class Hash=string::hash>
class HashSet
:
    public HashTable<nil, Key, Hash>
{

public:

    typedef typename HashTable<nil, Key, Hash>::iterator iterator;
    typedef typename HashTable<nil, Key, Hash>::const_iterator const_iterator;


    // Constructors

        //- Construct given initial size
        HashSet(const label size = 128)
        :
            HashTable<nil, Key, Hash>(size)
        {}

        //- Construct from Istream
        HashSet(Istream& is)
        :
            HashTable<nil, Key, Hash>(is)
        {}

        //- Construct from UList of Key
        HashSet(const UList<Key>&);

        //- Construct as copy
        HashSet(const HashSet<Key, Hash>& hs)
        :
            HashTable<nil, Key, Hash>(hs)
        {}

        //- Construct by transferring the parameter contents
        HashSet(const Xfer<HashSet<Key, Hash> >& hs)
        :
            HashTable<nil, Key, Hash>(hs)
        {}

        //- Construct by transferring the parameter contents
        HashSet(const Xfer<HashTable<nil, Key, Hash> >& hs)
        :
            HashTable<nil, Key, Hash>(hs)
        {}

        //- Construct from the keys of another HashTable,
        //  the type of values held is arbitrary.
        template<class AnyType, class AnyHash>
        HashSet(const HashTable<AnyType, Key, AnyHash>&);


    // Member Functions

        // Edit

        //- Insert a new entry
        bool insert(const Key& key)
        {
            return HashTable<nil, Key, Hash>::insert(key, nil());
        }

        //- Insert keys from a UList of Key
        //  Return the number of new elements inserted
        label insert(const UList<Key>&);

        //- Same as insert (cannot overwrite nil content)
        bool set(const Key& key)
        {
            return insert(key);
        }

        //- Same as insert (cannot overwrite nil content)
        label set(const UList<Key>& lst)
        {
            return insert(lst);
        }

        //- Unset the specified key - same as erase
        bool unset(const Key& key)
        {
            return HashTable<nil, Key, Hash>::erase(key);
        }


    // Member Operators

        //- Return true if the entry exists, same as found()
        inline bool operator[](const Key&) const;

        //- Equality. Two hashtables are equal when their contents are equal.
        //  Independent of table size or order.
        bool operator==(const HashSet<Key, Hash>&) const;

        //- The opposite of the equality operation.
        bool operator!=(const HashSet<Key, Hash>&) const;


        //- Combine entries from HashSets
        void operator|=(const HashSet<Key, Hash>&);

        //- Only retain entries found in both HashSets
        void operator&=(const HashSet<Key, Hash>&);

        //- Only retain unique entries (xor)
        void operator^=(const HashSet<Key, Hash>&);

        //- Add entries listed in the given HashSet to this HashSet
        inline void operator+=(const HashSet<Key, Hash>& rhs)
        {
            this->operator|=(rhs);
        }

        //- Remove entries listed in the given HashSet from this HashSet
        void operator-=(const HashSet<Key, Hash>&);
};


// Global Operators

//- Combine entries from HashSets
template<class Key, class Hash>
HashSet<Key,Hash> operator|
(
    const HashSet<Key,Hash>& hash1,
    const HashSet<Key,Hash>& hash2
);


//- Create a HashSet that only contains entries found in both HashSets
template<class Key, class Hash>
HashSet<Key,Hash> operator&
(
    const HashSet<Key,Hash>& hash1,
    const HashSet<Key,Hash>& hash2
);


//- Create a HashSet that only contains unique entries (xor)
template<class Key, class Hash>
HashSet<Key,Hash> operator^
(
    const HashSet<Key,Hash>& hash1,
    const HashSet<Key,Hash>& hash2
);


//- A HashSet with word keys.
typedef HashSet<> wordHashSet;

//- A HashSet with label keys.
typedef HashSet<label, Hash<label> > labelHashSet;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifndef HashSet_C
#define HashSet_C

#include "HashSet.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Key, class Hash>
CML::HashSet<Key, Hash>::HashSet(const UList<Key>& lst)
:
    HashTable<nil, Key, Hash>(2*lst.size())
{
    forAll(lst, elemI)
    {
        this->insert(lst[elemI]);
    }
}


template<class Key, class Hash>
template<class AnyType, class AnyHash>
CML::HashSet<Key, Hash>::HashSet
(
    const HashTable<AnyType, Key, AnyHash>& h
)
:
    HashTable<nil, Key, Hash>(h.size())
{
    for
    (
        typename HashTable<AnyType, Key, AnyHash>::const_iterator
        cit = h.cbegin();
        cit != h.cend();
        ++cit
    )
    {
        this->insert(cit.key());
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Key, class Hash>
CML::label CML::HashSet<Key, Hash>::insert(const UList<Key>& lst)
{
    label count = 0;
    forAll(lst, elemI)
    {
        if (this->insert(lst[elemI]))
        {
            ++count;
        }
    }

    return count;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Key, class Hash>
inline bool CML::HashSet<Key, Hash>::operator[](const Key& key) const
{
    return this->found(key);
}


template<class Key, class Hash>
bool CML::HashSet<Key, Hash>::operator==(const HashSet<Key, Hash>& rhs) const
{
    // Are all lhs elements in rhs?
    for (const_iterator iter = this->cbegin(); iter != this->cend(); ++iter)
    {
        if (!rhs.found(iter.key()))
        {
            return false;
        }
    }

    // Are all rhs elements in lhs?
    for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter)
    {
        if (!this->found(iter.key()))
        {
            return false;
        }
    }

    return true;
}


template<class Key, class Hash>
bool CML::HashSet<Key, Hash>::operator!=(const HashSet<Key, Hash>& rhs) const
{
    return !(this->operator==(rhs));
}


template<class Key, class Hash>
void CML::HashSet<Key, Hash>::operator|=(const HashSet<Key, Hash>& rhs)
{
    // Add rhs elements into lhs
    for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter)
    {
        this->insert(iter.key());
    }
}


template<class Key, class Hash>
void CML::HashSet<Key, Hash>::operator&=(const HashSet<Key, Hash>& rhs)
{
    // Remove elements not also found in rhs
    for (iterator iter = this->begin(); iter != this->end(); ++iter)
    {
        if (!rhs.found(iter.key()))
        {
            this->erase(iter);
        }
    }
}


template<class Key, class Hash>
void CML::HashSet<Key, Hash>::operator^=(const HashSet<Key, Hash>& rhs)
{
    // Add missed rhs elements, remove duplicate elements
    for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter)
    {
        if (this->found(iter.key()))
        {
            this->erase(iter.key());
        }
        else
        {
            this->insert(iter.key());
        }
    }
}


// same as HashTable::erase()
template<class Key, class Hash>
void CML::HashSet<Key, Hash>::operator-=(const HashSet<Key, Hash>& rhs)
{
    // Remove rhs elements from lhs
    for (const_iterator iter = rhs.cbegin(); iter != rhs.cend(); ++iter)
    {
        this->erase(iter.key());
    }
}


/* * * * * * * * * * * * * * * * Global operators  * * * * * * * * * * * * * */

template<class Key, class Hash>
CML::HashSet<Key, Hash>
CML::operator|
(
    const HashSet<Key, Hash>& hash1,
    const HashSet<Key, Hash>& hash2
)
{
    HashSet<Key, Hash> out(hash1);
    out |= hash2;
    return out;
}


template<class Key, class Hash>
CML::HashSet<Key, Hash>
CML::operator&
(
    const HashSet<Key, Hash>& hash1,
    const HashSet<Key, Hash>& hash2
)
{
    HashSet<Key, Hash> out(hash1);
    out &= hash2;
    return out;
}


template<class Key, class Hash>
CML::HashSet<Key, Hash>
CML::operator^
(
    const HashSet<Key, Hash>& hash1,
    const HashSet<Key, Hash>& hash2
)
{
    HashSet<Key, Hash> out(hash1);
    out ^= hash2;
    return out;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
