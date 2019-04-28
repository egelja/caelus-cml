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
    CML::Istream

Description
    An Istream is an abstract base class for all input systems
    (streams, files, token lists etc).  The basic operations
    are construct, close, read token, read primitive and read binary
    block.

    In addition, version control and line number counting is incorporated.
    Usually one would use the read primitive member functions, but if one
    were reading a stream on unknown data sequence one can read token by
    token, and then analyse.

SourceFiles
    Istream.cpp

\*---------------------------------------------------------------------------*/

#ifndef Istream_H
#define Istream_H

#include "IOstream.hpp"
#include "token.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class Istream Declaration
\*---------------------------------------------------------------------------*/

class Istream
:
    public IOstream
{
    // Private data

        //- Has a token been put back on the stream?
        bool putBack_;

        //- The last token put back on the stream
        token putBackToken_;


public:

    // Constructors

        //- Set stream status
        Istream
        (
            streamFormat format=ASCII,
            versionNumber version=currentVersion,
            compressionType compression=UNCOMPRESSED
        )
        :
            IOstream(format, version, compression),
            putBack_(false)
        {}


    //- Destructor
    virtual ~Istream()
    {}


    // Member functions

        // Read functions

            //- Put back token
            //  Only a single put back is permitted
            void putBack(const token&);

            //- Get the put back token if there is one and return true.
            //  Return false if no put back token is available.
            bool getBack(token&);

            //- Peek at the put back token without removing it.
            //  Returns false if no put back token is available and set the
            //  token to undefined.
            bool peekBack(token&);

            //- Return next token from stream
            virtual Istream& read(token&) = 0;

            //- Read a character
            virtual Istream& read(char&) = 0;

            //- Read a word
            virtual Istream& read(word&) = 0;

            // Read a string (including enclosing double-quotes)
            virtual Istream& read(string&) = 0;

            //- Read a label
            virtual Istream& read(label&) = 0;

            //- Read a floatScalar
            virtual Istream& read(floatScalar&) = 0;

            //- Read a doubleScalar
            virtual Istream& read(doubleScalar&) = 0;

            //- Read binary block
            virtual Istream& read(char*, std::streamsize) = 0;

            //- Rewind and return the stream so that it may be read again
            virtual Istream& rewind() = 0;


        // Read List punctuation tokens

            Istream& readBegin(const char* funcName);
            Istream& readEnd(const char* funcName);
            Istream& readEndBegin(const char* funcName);

            char readBeginList(const char* funcName);
            char readEndList(const char* funcName);


    // Member operators

        //- Return a non-const reference to const Istream
        //  Needed for read-constructors where the stream argument is temporary:
        //  e.g. thing thisThing(IFstream("thingFileName")());
        Istream& operator()() const;
};


// --------------------------------------------------------------------
// ------ Manipulators (not taking arguments)
// --------------------------------------------------------------------

typedef Istream& (*IstreamManip)(Istream&);

//- operator>> handling for manipulators without arguments
inline Istream& operator>>(Istream& is, IstreamManip f)
{
    return f(is);
}

//- operator>> handling for manipulators without arguments
inline Istream& operator>>(Istream& is, IOstreamManip f)
{
    f(is);
    return is;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
