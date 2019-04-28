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
    CML::DictionaryBase

Description
    Base dictionary class templated on both the form of doubly-linked list
    it uses as well as the type it holds.

    The double templating allows for the instantiation of forms with or
    without storage management.

Note
    The IDLListType parameter should itself be a template but this confused
    gcc 2.95.2 so it has to be instantiated for T when an instantiation of
    DictionaryBase is requested

See Also
    Dictionary and UDictionary


\*---------------------------------------------------------------------------*/

#ifndef DictionaryBase_H
#define DictionaryBase_H

#include "HashTable.hpp"
#include "wordList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class IDLListType, class T>
class DictionaryBase;

template<class IDLListType, class T>
Ostream& operator<<(Ostream&, const DictionaryBase<IDLListType, T>&);


/*---------------------------------------------------------------------------*\
                      Class DictionaryBase Declaration
\*---------------------------------------------------------------------------*/

template<class IDLListType, class T>
class DictionaryBase
:
    public IDLListType
{
protected:

    // Protected data

        //- HashTable of the entries held on the IDLListType for quick lookup
        HashTable<T*> hashedTs_;


    // Protected Member functions

        // Add the IDLListType entries into the HashTable
        void addEntries();


public:

    // Constructors

        //- Construct given initial table size
        DictionaryBase(const label size = 128);

        //- Copy construct
        DictionaryBase(const DictionaryBase&);

        //- Construct from Istream using given Istream constructor class
        template<class INew>
        DictionaryBase(Istream&, const INew&);

        //- Construct from Istream using default Istream constructor class
        DictionaryBase(Istream&);


    // Member functions

        // Search and lookup

            //- Search DictionaryBase for given keyword
            bool found(const word&) const;

            //- Find and return an entry if present, otherwise return nullptr
            const T* lookupPtr(const word&) const;

            //- Find and return an entry if present, otherwise return nullptr
            T* lookupPtr(const word&);

            //- Find and return entry
            const T* lookup(const word&) const;

            //- Find and return entry
            T* lookup(const word&);

            //- Return the table of contents
            wordList toc() const;

            //- Return the table of contents as a sorted list
            wordList sortedToc() const;


        // Editing

            //- Add at head of dictionary
            void insert(const word&, T*);

            //- Add at tail of dictionary
            void append(const word&, T*);

            //- Remove and return entry specified by keyword.
            //  Return nullptr if the keyword was not found.
            T* remove(const word&);

            //- Clear the dictionary
            void clear();

            //- Transfer the contents of the argument into this DictionaryBase
            //  and annul the argument.
            void transfer(DictionaryBase<IDLListType, T>&);


    // Member operators

        void operator=(const DictionaryBase&);

        //- Find and return entry
        const T* operator[](const word& key) const
        {
            return lookup(key);
        }

        //- Find and return entry
        T* operator[](const word& key)
        {
            return lookup(key);
        }


    // Ostream operator

        friend Ostream& operator<< <IDLListType, T>
        (
            Ostream&,
            const DictionaryBase<IDLListType, T>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class IDLListType, class T>
void CML::DictionaryBase<IDLListType, T>::addEntries()
{
    for
    (
        typename IDLListType::iterator iter = this->begin();
        iter != this->end();
        ++iter
    )
    {
        this->hashedTs_.insert((*iter).keyword(), &(*iter));
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class IDLListType, class T>
CML::DictionaryBase<IDLListType, T>::DictionaryBase(const label size)
:
    hashedTs_(size)
{}


template<class IDLListType, class T>
CML::DictionaryBase<IDLListType, T>::DictionaryBase
(
    const DictionaryBase& dict
)
:
    IDLListType(dict)
{
    addEntries();
}


template<class IDLListType, class T>
template<class INew>
CML::DictionaryBase<IDLListType, T>::DictionaryBase
(
    Istream& is,
    const INew& iNew
)
:
    IDLListType(is, iNew)
{
    addEntries();
}


template<class IDLListType, class T>
CML::DictionaryBase<IDLListType, T>::DictionaryBase(Istream& is)
:
    IDLListType(is)
{
    addEntries();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Find and return T
template<class IDLListType, class T>
bool CML::DictionaryBase<IDLListType, T>::found(const word& keyword) const
{
    return hashedTs_.found(keyword);
}


// Find and return T*, return nullptr if not found
template<class IDLListType, class T>
const T* CML::DictionaryBase<IDLListType, T>::lookupPtr
(
    const word& keyword
) const
{
    typename HashTable<T*>::const_iterator iter = hashedTs_.find(keyword);

    if (iter != hashedTs_.end())
    {
        return *iter;
    }
    else
    {
        return nullptr;
    }
}


// Find and return T*, return nullptr if not found
template<class IDLListType, class T>
T* CML::DictionaryBase<IDLListType, T>::lookupPtr(const word& keyword)
{
    typename HashTable<T*>::iterator iter = hashedTs_.find(keyword);

    if (iter != hashedTs_.end())
    {
        return *iter;
    }
    else
    {
        return nullptr;
    }
}


// Find and return T*, FatalError if keyword not found
template<class IDLListType, class T>
const T* CML::DictionaryBase<IDLListType, T>::lookup(const word& keyword) const
{
    typename HashTable<T*>::const_iterator iter = hashedTs_.find(keyword);

    if (iter == hashedTs_.end())
    {
        FatalErrorInFunction
            << keyword << " is undefined"
            << exit(FatalError);
    }

    return *iter;
}


// Find and return T*, FatalError if keyword not found
template<class IDLListType, class T>
T* CML::DictionaryBase<IDLListType, T>::lookup(const word& keyword)
{
    typename HashTable<T*>::iterator iter = hashedTs_.find(keyword);

    if (iter == hashedTs_.end())
    {
        FatalErrorInFunction
            << keyword << " is undefined"
            << exit(FatalError);
    }

    return *iter;
}


// Return the table of contents
template<class IDLListType, class T>
CML::wordList CML::DictionaryBase<IDLListType, T>::toc() const
{
    wordList keywords(this->size());

    label i = 0;
    for
    (
        typename IDLListType::const_iterator iter = this->begin();
        iter != this->end();
        ++iter
    )
    {
        keywords[i++] = iter().keyword();
    }

    return keywords;
}


template<class IDLListType, class T>
CML::wordList CML::DictionaryBase<IDLListType, T>::sortedToc() const
{
    return hashedTs_.sortedToc();
}


// Add at head of dictionary
template<class IDLListType, class T>
void CML::DictionaryBase<IDLListType, T>::insert(const word& keyword, T* tPtr)
{
    // NOTE: we should probably check that HashTable::insert actually worked
    hashedTs_.insert(keyword, tPtr);
    IDLListType::insert(tPtr);
}


// Add at tail of dictionary
template<class IDLListType, class T>
void CML::DictionaryBase<IDLListType, T>::append(const word& keyword, T* tPtr)
{
    // NOTE: we should probably check that HashTable::insert actually worked
    hashedTs_.insert(keyword, tPtr);
    IDLListType::append(tPtr);
}


template<class IDLListType, class T>
T* CML::DictionaryBase<IDLListType, T>::remove(const word& keyword)
{
    typename HashTable<T*>::iterator iter = hashedTs_.find(keyword);

    if (iter != hashedTs_.end())
    {
        T* tPtr = IDLListType::remove(iter());
        hashedTs_.erase(iter);
        return tPtr;
    }
    else
    {
        return nullptr;
    }
}


template<class IDLListType, class T>
void CML::DictionaryBase<IDLListType, T>::clear()
{
    IDLListType::clear();
    hashedTs_.clear();
}


template<class IDLListType, class T>
void CML::DictionaryBase<IDLListType, T>::transfer
(
    DictionaryBase<IDLListType, T>& dict
)
{
    IDLListType::transfer(dict);
    hashedTs_.transfer(dict.hashedTs_);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class IDLListType, class T>
void CML::DictionaryBase<IDLListType, T>::operator=
(
    const DictionaryBase<IDLListType, T>& dict
)
{
    // Check for assignment to self
    if (this == &dict)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    IDLListType::operator=(dict);
    this->hashedTs_.clear();
    this->addEntries();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IOstreams.hpp"

// * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * * //

template<class IDLListType, class T>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const DictionaryBase<IDLListType, T>& dict)
{
    for
    (
        typename IDLListType::const_iterator iter = dict.begin();
        iter != dict.end();
        ++iter
    )
    {
        os << *iter;

        // Check stream before going to next entry.
        if (!os.good())
        {
            Info
                << "operator<<(Ostream&, const DictionaryBase&) : "
                << "Can't write entry for DictionaryBase"
                << endl;

            return os;
        }
    }

    return os;
}




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
