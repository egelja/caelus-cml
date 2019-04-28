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
    CML::LPtrList

Description
    Template class for non-intrusive linked PtrLists.

SourceFiles
    LPtrList.cpp
    LPtrListIO.cpp

\*---------------------------------------------------------------------------*/

#ifndef LPtrList_H
#define LPtrList_H

#include "LList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class LListBase, class T> class LPtrList;

template<class LListBase, class T>
Istream& operator>>
(
    Istream&,
    LPtrList<LListBase, T>&
);

template<class LListBase, class T>
Ostream& operator<<
(
    Ostream&,
    const LPtrList<LListBase, T>&
);


/*---------------------------------------------------------------------------*\
                          Class LPtrList Declaration
\*---------------------------------------------------------------------------*/

template<class LListBase, class T>
class LPtrList
:
    public LList<LListBase, T*>
{
    // Private Member Functions

        //- Read from Istream using given Istream constructor class
        template<class INew>
        void read(Istream&, const INew&);


public:

    // Forward declaration of STL iterators

        class iterator;
        friend class iterator;

        class const_iterator;
        friend class const_iterator;


    // Constructors

        //- Null construct
        LPtrList()
        {}

        //- Construct given initial T
        LPtrList(T* a)
        :
            LList<LListBase, T*>(a)
        {}

        //- Construct from Istream using given Istream constructor class
        template<class INew>
        LPtrList(Istream&, const INew&);

        //- Construct from Istream using default Istream constructor class
        LPtrList(Istream&);

        //- Construct as copy
        LPtrList(const LPtrList&);


    //- Destructor
    ~LPtrList();


    // Member Functions

        // Access

            //- Return the first entry added
            T& first()
            {
                return *LList<LListBase, T*>::first();
            }

            //- Return const access to the first entry added
            const T& first() const
            {
                return *LList<LListBase, T*>::first();
            }

            //- Return the last entry added
            T& last()
            {
                return *LList<LListBase, T*>::last();
            }

            //- Return const access to the last entry added
            const T& last() const
            {
                return *LList<LListBase, T*>::last();
            }


        // Edit

            //- Remove the head element from the list and delete the pointer
            bool eraseHead();

            //- Clear the contents of the list
            void clear();

            //- Transfer the contents of the argument into this List
            //  and annul the argument list.
            void transfer(LPtrList<LListBase, T>&);


    // Member operators

        //- Assign copy
        void operator=(const LPtrList<LListBase, T>&);


    // STL type definitions

        //- Type that can be used for storing into LPtrList::value_type
        //  objects.
        typedef T& reference;

        //- Type that can be used for storing into constant
        //  LPtrList::value_type objects.
        typedef T& const_reference;


    // STL iterator

        typedef typename LListBase::iterator LListBase_iterator;

        //- An STL-conforming iterator
        class iterator
        :
            public LList<LListBase, T*>::iterator
        {

        public:

            //- Construct from base iterator
            iterator(LListBase_iterator baseIter)
            :
                LList<LListBase, T*>::iterator(baseIter)
            {}


            // Member operators

                T& operator*()
                {
                    return *(LList<LListBase, T*>::iterator::operator*());
                }

                T& operator()()
                {
                    return operator*();
                }
        };


    // STL const_iterator

        typedef typename LListBase::const_iterator LListBase_const_iterator;

        //- An STL-conforming const_iterator
        class const_iterator
        :
            public LList<LListBase, T*>::const_iterator
        {

        public:

            //- Construct from base const_iterator
            const_iterator(LListBase_const_iterator baseIter)
            :
                LList<LListBase, T*>::const_iterator(baseIter)
            {}

            //- Construct from base iterator
            const_iterator(LListBase_iterator baseIter)
            :
                LList<LListBase, T*>::const_iterator(baseIter)
            {}


            // Member operators

                const T& operator*()
                {
                    return *(LList<LListBase, T*>::const_iterator::operator*());
                }

                const T& operator()()
                {
                    return operator*();
                }
        };


    // IOstream operators

        friend Istream& operator>> <LListBase, T>
        (
            Istream&,
            LPtrList<LListBase, T>&
        );

        friend Ostream& operator<< <LListBase, T>
        (
            Ostream&,
            const LPtrList<LListBase, T>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class LListBase, class T>
CML::LPtrList<LListBase, T>::LPtrList(const LPtrList<LListBase, T>& lst)
:
    LList<LListBase, T*>()
{
    for (const_iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        this->append(iter().clone().ptr());
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class LListBase, class T>
CML::LPtrList<LListBase, T>::~LPtrList()
{
    this->clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class LListBase, class T>
bool CML::LPtrList<LListBase, T>::eraseHead()
{
    T* tPtr;
    if ((tPtr = this->removeHead()))
    {
        delete tPtr;
        return true;
    }
    else
    {
        return false;
    }
}


template<class LListBase, class T>
void CML::LPtrList<LListBase, T>::clear()
{
    const label oldSize = this->size();
    for (label i=0; i<oldSize; ++i)
    {
        eraseHead();
    }

    LList<LListBase, T*>::clear();
}


template<class LListBase, class T>
void CML::LPtrList<LListBase, T>::transfer(LPtrList<LListBase, T>& lst)
{
    clear();
    LList<LListBase, T*>::transfer(lst);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class LListBase, class T>
void CML::LPtrList<LListBase, T>::operator=(const LPtrList<LListBase, T>& lst)
{
    clear();

    for (const_iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        this->append(iter().clone().ptr());
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

#include "Istream.hpp"
#include "Ostream.hpp"
#include "INew.hpp"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class LListBase, class T>
template<class INew>
void CML::LPtrList<LListBase, T>::read(Istream& is, const INew& iNew)
{
    is.fatalCheck
    (
        "LPtrList<LListBase, T>::read(Istream&, const INew&)"
    );

    token firstToken(is);

    is.fatalCheck
    (
        "LPtrList<LListBase, T>::read(Istream&, const INew&) : "
        "reading first token"
    );

    if (firstToken.isLabel())
    {
        label s = firstToken.labelToken();

        // Read beginning of contents
        char delimiter = is.readBeginList("LPtrList<LListBase, T>");

        if (s)
        {
            if (delimiter == token::BEGIN_LIST)
            {
                for (label i=0; i<s; ++i)
                {
                    this->append(iNew(is).ptr());

                    is.fatalCheck
                    (
                        "LPtrList<LListBase, T>::read(Istream&, const INew&) : "
                        "reading entry"
                    );
                }
            }
            else
            {
                T* tPtr = iNew(is).ptr();
                this->append(tPtr);

                is.fatalCheck
                (
                    "LPtrList<LListBase, T>::read(Istream&, const INew&) : "
                    "reading entry"
                );

                for (label i=1; i<s; ++i)
                {
                    this->append(tPtr->clone().ptr());
                }
            }
        }

        // Read end of contents
        is.readEndList("LPtrList<LListBase, T>");
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
        is.fatalCheck("LPtrList<LListBase, T>::read(Istream&, const INew&)");

        while
        (
           !(
                lastToken.isPunctuation()
             && lastToken.pToken() == token::END_LIST
            )
        )
        {
            is.putBack(lastToken);
            this->append(iNew(is).ptr());

            is >> lastToken;
            is.fatalCheck
            (
                "LPtrList<LListBase, T>::read(Istream&, const INew&)"
            );
        }
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "incorrect first token, expected <int> or '(', found "
            << firstToken.info()
            << exit(FatalIOError);
    }

    is.fatalCheck("LPtrList<LListBase, T>::read(Istream&, const INew&)");
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class LListBase, class T>
template<class INew>
CML::LPtrList<LListBase, T>::LPtrList(Istream& is, const INew& iNew)
{
    this->read(is, iNew);
}


template<class LListBase, class T>
CML::LPtrList<LListBase, T>::LPtrList(Istream& is)
{
    this->read(is, INew<T>());
}


// * * * * * * * * * * * * * * * Istream Operator  * * * * * * * * * * * * * //

template<class LListBase, class T>
CML::Istream& CML::operator>>(Istream& is, LPtrList<LListBase, T>& L)
{
    L.clear();
    L.read(is, INew<T>());

    return is;
}


// * * * * * * * * * * * * * * * Ostream Operators * * * * * * * * * * * * * //

template<class LListBase, class T>
CML::Ostream& CML::operator<<(Ostream& os, const LPtrList<LListBase, T>& lst)
{
    // Write size
    os << nl << lst.size();

    // Write beginning of contents
    os << nl << token::BEGIN_LIST << nl;

    // Write contents
    for
    (
        typename LPtrList<LListBase, T>::const_iterator iter = lst.begin();
        iter != lst.end();
        ++iter
    )
    {
        os << iter() << nl;
    }

    // Write end of contents
    os << token::END_LIST;

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const LPtrList<LListBase, T>&)");

    return os;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
