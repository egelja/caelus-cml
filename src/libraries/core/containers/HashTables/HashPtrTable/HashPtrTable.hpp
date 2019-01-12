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
    CML::HashPtrTable

Description
    A HashTable specialization for hashing pointers.

SourceFiles
    HashPtrTable.cpp
    HashPtrTableIO.cpp

\*---------------------------------------------------------------------------*/

#ifndef HashPtrTable_H
#define HashPtrTable_H

#include "HashTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class Istream;
class Ostream;

// Forward declaration of friend functions and operators

template<class T, class Key, class Hash> class HashPtrTable;

template<class T, class Key, class Hash>
Istream& operator>>(Istream&, HashPtrTable<T, Key, Hash>&);

template<class T, class Key, class Hash>
Ostream& operator<<(Ostream&, const HashPtrTable<T, Key, Hash>&);


/*---------------------------------------------------------------------------*\
                        Class HashPtrTable Declaration
\*---------------------------------------------------------------------------*/

template<class T, class Key=word, class Hash=string::hash>
class HashPtrTable
:
    public HashTable<T*, Key, Hash>
{
    // Private Member Functions

        //- Read from Istream using given Istream constructor class
        template<class INew>
        void read(Istream&, const INew& inewt);

        //- Read from dictionary using given dictionary constructor class
        template<class INew>
        void read(const dictionary& dict, const INew& inewt);



public:

    typedef typename HashTable<T*, Key, Hash>::iterator iterator;
    typedef typename HashTable<T*, Key, Hash>::const_iterator const_iterator;


    // Constructors

        //- Construct given initial table size
        HashPtrTable(const label size = 128);

        //- Construct from Istream using given Istream constructor class
        template<class INew>
        HashPtrTable(Istream&, const INew&);

        //- Construct from Istream using default Istream constructor class
        HashPtrTable(Istream&);

        //- Construct from dictionary using default dictionary constructor
        //  class
        HashPtrTable(const dictionary&);

        //- Construct as copy
        HashPtrTable(const HashPtrTable<T, Key, Hash>&);


    //- Destructor
    ~HashPtrTable();


    // Member Functions

        // Edit

            //- Remove and return the pointer specified by given iterator
            T* remove(iterator&);

            //- Erase an hashedEntry specified by given iterator
            bool erase(iterator&);

            //- Clear all entries from table
            void clear();

            //- Write
            void write(Ostream& os) const;


    // Member Operators

        void operator=(const HashPtrTable<T, Key, Hash>&);


    // IOstream Operators

        friend Istream& operator>> <T, Key, Hash>
        (
            Istream&,
            HashPtrTable<T, Key, Hash>&
        );

        friend Ostream& operator<< <T, Key, Hash>
        (
            Ostream&,
            const HashPtrTable<T, Key, Hash>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "error.hpp"
#include "HashPtrTable.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct given initial table size
template<class T, class Key, class Hash>
CML::HashPtrTable<T, Key, Hash>::HashPtrTable(const label size)
:
    HashTable<T*, Key, Hash>(size)
{}


// Construct as copy
template<class T, class Key, class Hash>
CML::HashPtrTable<T, Key, Hash>::HashPtrTable
(
    const HashPtrTable<T, Key, Hash>& ht
)
:
    HashTable<T*, Key, Hash>()
{
    for (const_iterator iter = ht.begin(); iter != ht.end(); ++iter)
    {
        this->insert(iter.key(), new T(**iter));
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
CML::HashPtrTable<T, Key, Hash>::~HashPtrTable()
{
    clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
T* CML::HashPtrTable<T, Key, Hash>::remove(iterator& it)
{
    T* elemPtr = *it;
    HashTable<T*, Key, Hash>::erase(it);
    return elemPtr;
}


template<class T, class Key, class Hash>
bool CML::HashPtrTable<T, Key, Hash>::erase(iterator& it)
{
    T* elemPtr = *it;

    if (HashTable<T*, Key, Hash>::erase(it))
    {
        if (elemPtr)
        {
            delete elemPtr;
        }

        return true;
    }
    else
    {
        return false;
    }
}


template<class T, class Key, class Hash>
void CML::HashPtrTable<T, Key, Hash>::clear()
{
    for
    (
        iterator iter = this->begin();
        iter != this->end();
        ++iter
    )
    {
        delete *iter;
    }

    HashTable<T*, Key, Hash>::clear();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
void CML::HashPtrTable<T, Key, Hash>::operator=
(
    const HashPtrTable<T, Key, Hash>& rhs
)
{
    // Check for assignment to self
    if (this == &rhs)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    this->clear();

    for (const_iterator iter = rhs.begin(); iter != rhs.end(); ++iter)
    {
        this->insert(iter.key(), new T(**iter));
    }
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

#include "Istream.hpp"
#include "Ostream.hpp"
#include "INew.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class T, class Key, class Hash>
template<class INew>
void CML::HashPtrTable<T, Key, Hash>::read(Istream& is, const INew& inewt)
{
    is.fatalCheck("HashPtrTable<T, Key, Hash>::read(Istream&, const INew&)");

    token firstToken(is);

    is.fatalCheck
    (
        "HashPtrTable<T, Key, Hash>::read(Istream&, const INew&) : "
        "reading first token"
    );

    if (firstToken.isLabel())
    {
        label s = firstToken.labelToken();

        // Read beginning of contents
        char delimiter = is.readBeginList("HashPtrTable<T, Key, Hash>");

        if (s)
        {
            if (2*s > this->tableSize_)
            {
                this->resize(2*s);
            }

            if (delimiter == token::BEGIN_LIST)
            {
                for (label i=0; i<s; i++)
                {
                    Key key;
                    is >> key;
                    this->insert(key, inewt(key, is).ptr());

                    is.fatalCheck
                    (
                        "HashPtrTable<T, Key, Hash>::"
                        "read(Istream&, const INew&) : reading entry"
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
        is.readEndList("HashPtrTable");
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
            this->insert(key, inewt(key, is).ptr());

            is.fatalCheck
            (
                "HashPtrTable<T, Key, Hash>::read(Istream&, const INew&) : "
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

    is.fatalCheck("HashPtrTable<T, Key, Hash>::read(Istream&, const INew&)");
}


template<class T, class Key, class Hash>
template<class INew>
void CML::HashPtrTable<T, Key, Hash>::read
(
    const dictionary& dict,
    const INew& inewt
)
{
    forAllConstIter(dictionary, dict, iter)
    {
        this->insert
        (
            iter().keyword(),
            inewt(dict.subDict(iter().keyword())).ptr()
        );
    }
}


template<class T, class Key, class Hash>
void CML::HashPtrTable<T, Key, Hash>::write(Ostream& os) const
{

    for
    (
        typename HashPtrTable<T, Key, Hash>::const_iterator
        iter = this->begin();
        iter != this->end();
        ++iter
    )
    {
        const T* ptr = iter();
        ptr->write(os);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T, class Key, class Hash>
template<class INew>
CML::HashPtrTable<T, Key, Hash>::HashPtrTable(Istream& is, const INew& inewt)
{
    this->read(is, inewt);
}


template<class T, class Key, class Hash>
CML::HashPtrTable<T, Key, Hash>::HashPtrTable(Istream& is)
{
    this->read(is, INew<T>());
}


template<class T, class Key, class Hash>
CML::HashPtrTable<T, Key, Hash>::HashPtrTable(const dictionary& dict)
{
    this->read(dict, INew<T>());
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class T, class Key, class Hash>
CML::Istream& CML::operator>>(Istream& is, HashPtrTable<T, Key, Hash>& L)
{
    L.clear();
    L.read(is, INew<T>());

    return is;
}


template<class T, class Key, class Hash>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const HashPtrTable<T, Key, Hash>& L
)
{
    // Write size and start delimiter
    os << nl << L.size() << nl << token::BEGIN_LIST << nl;

    // Write contents
    for
    (
        typename HashPtrTable<T, Key, Hash>::const_iterator iter = L.begin();
        iter != L.end();
        ++iter
    )
    {
        os << iter.key() << token::SPACE << *iter() << nl;
    }

    // Write end delimiter
    os << token::END_LIST;

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const HashPtrTable&)");

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
