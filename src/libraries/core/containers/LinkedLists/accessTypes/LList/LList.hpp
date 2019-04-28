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
    CML::LList

Description
    Template class for non-intrusive linked lists.


\*---------------------------------------------------------------------------*/

#ifndef LList_H
#define LList_H

#include "label.hpp"
#include "uLabel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class Istream;
class Ostream;

// Forward declaration of friend functions and operators

template<class LListBase, class T> class LList;

template<class LListBase, class T>
Istream& operator>>
(
    Istream&,
    LList<LListBase, T>&
);

template<class LListBase, class T>
Ostream& operator<<
(
    Ostream&,
    const LList<LListBase, T>&
);


/*---------------------------------------------------------------------------*\
                            Class LList Declaration
\*---------------------------------------------------------------------------*/

template<class LListBase, class T>
class LList
:
    public LListBase
{

public:

    // Forward declaration of STL iterators

        class iterator;
        friend class iterator;

        class const_iterator;
        friend class const_iterator;


    //- Link structure
    struct link
    :
        public LListBase::link
    {
        //- Stored object
        T obj_;

        //- Construct given object
        link(T a)
        :
            obj_(a)
        {}
    };


    // Constructors

        //- Null construct
        LList()
        {}

        //- Construct given initial T
        LList(T a)
        :
            LListBase(new link(a))
        {}

        //- Construct from Istream
        LList(Istream&);

        //- Construct as copy
        LList(const LList<LListBase, T>&);


    //- Destructor
    ~LList();


    // Member Functions

        // Access

            //- Return the first entry added
            T& first()
            {
                return static_cast<link*>(LListBase::first())->obj_;
            }

            //- Return const access to the first entry added
            const T& first() const
            {
                return static_cast<const link*>(LListBase::first())->obj_;
            }

            //- Return the last entry added
            T& last()
            {
                return static_cast<link*>(LListBase::last())->obj_;
            }

            //- Return const access to the last entry added
            const T& last() const
            {
                return static_cast<const link*>(LListBase::last())->obj_;
            }


        // Edit

            //- Add at head of list
            void insert(const T& a)
            {
                LListBase::insert(new link(a));
            }

            //- Add at tail of list
            void append(const T& a)
            {
                LListBase::append(new link(a));
            }

            //- Remove and return head
            T removeHead()
            {
                link* elmtPtr = static_cast<link*>(LListBase::removeHead());
                T data = elmtPtr->obj_;
                delete elmtPtr;
                return data;
            }

            //- Remove and return element
            T remove(link* l)
            {
                link* elmtPtr = static_cast<link*>(LListBase::remove(l));
                T data = elmtPtr->obj_;
                delete elmtPtr;
                return data;
            }

            //- Remove and return element specified by iterator
            T remove(iterator& it)
            {
                link* elmtPtr = static_cast<link*>(LListBase::remove(it));
                T data = elmtPtr->obj_;
                delete elmtPtr;
                return data;
            }

            //- Delete contents of list
            void clear();

            //- Transfer the contents of the argument into this List
            //  and annul the argument list.
            void transfer(LList<LListBase, T>&);

    // Member operators

        void operator=(const LList<LListBase, T>&);


    // STL type definitions

        //- Type of values the LList contains.
        typedef T value_type;

        //- Type that can be used for storing into value_type
        //  objects.
        typedef T& reference;

        //- Type that can be used for storing into constant
        //  LList::value_type objects.
        typedef const T& const_reference;

        //- The type that can represent the size of a LList.
        typedef label size_type;


    // STL iterator

        typedef typename LListBase::iterator LListBase_iterator;

        //- An STL-conforming iterator
        class iterator
        :
            public LListBase_iterator
        {

        public:

            //- Construct from base iterator
            iterator(LListBase_iterator baseIter)
            :
                LListBase_iterator(baseIter)
            {}


            // Member operators

                T& operator*()
                {
                    return
                        static_cast<link&>
                        (LListBase_iterator::operator*()).obj_;
                }

                T& operator()()
                {
                    return operator*();
                }

                iterator& operator++()
                {
                    LListBase_iterator::operator++();
                    return *this;
                }
        };


    // STL const_iterator

        typedef typename LListBase::const_iterator LListBase_const_iterator;

        //- An STL-conforming const_iterator
        class const_iterator
        :
            public LListBase_const_iterator
        {

        public:

            //- Construct from base const_iterator
            const_iterator(LListBase_const_iterator baseIter)
            :
                LListBase_const_iterator(baseIter)
            {}


            //- Construct from base iterator
            const_iterator(LListBase_iterator baseIter)
            :
                LListBase_const_iterator(baseIter)
            {}


            // Member operators

                const T& operator*()
                {
                    return
                        static_cast<const link&>
                        (LListBase_const_iterator::operator*()).obj_;
                }

                const T& operator()()
                {
                    return operator*();
                }

                const_iterator& operator++()
                {
                    LListBase_const_iterator::operator++();
                    return *this;
                }
        };


    // IOstream operators

        friend Istream& operator>> <LListBase, T>
        (
            Istream&,
            LList<LListBase, T>&
        );

        friend Ostream& operator<< <LListBase, T>
        (
            Ostream&,
            const LList<LListBase, T>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "error.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class LListBase, class T>
CML::LList<LListBase, T>::LList(const LList<LListBase, T>& lst)
:
    LListBase()
{
    for (const_iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        this->append(iter());
    }
}


template<class LListBase, class T>
CML::LList<LListBase, T>::~LList()
{
    this->clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class LListBase, class T>
void CML::LList<LListBase, T>::clear()
{
    label oldSize = this->size();
    for (label i=0; i<oldSize; ++i)
    {
        this->removeHead();
    }

    LListBase::clear();
}


template<class LListBase, class T>
void CML::LList<LListBase, T>::transfer(LList<LListBase, T>& lst)
{
    clear();
    LListBase::transfer(lst);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class LListBase, class T>
void CML::LList<LListBase, T>::operator=(const LList<LListBase, T>& lst)
{
    this->clear();

    for (const_iterator iter = lst.begin(); iter != lst.end(); ++iter)
    {
        this->append(iter());
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

#include "Istream.hpp"
#include "Ostream.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class LListBase, class T>
CML::LList<LListBase, T>::LList(Istream& is)
{
    operator>>(is, *this);
}


// * * * * * * * * * * * * * * * Istream Operator  * * * * * * * * * * * * * //

template<class LListBase, class T>
CML::Istream& CML::operator>>(Istream& is, LList<LListBase, T>& L)
{
    // Anull list
    L.clear();

    is.fatalCheck(" operator>>(Istream&, LList<LListBase, T>&)");

    token firstToken(is);

    is.fatalCheck
    (
        " operator>>(Istream&, LList<LListBase, T>&) : reading first token"
    );

    if (firstToken.isLabel())
    {
        label s = firstToken.labelToken();

        // Read beginning of contents
        char delimiter = is.readBeginList("LList<LListBase, T>");

        if (s)
        {
            if (delimiter == token::BEGIN_LIST)
            {
                for (register label i=0; i<s; ++i)
                {
                    T element;
                    is >> element;
                    L.append(element);
                }
            }
            else
            {
                T element;
                is >> element;

                for (register label i=0; i<s; ++i)
                {
                    L.append(element);
                }
            }
        }

        // Read end of contents
        is.readEndList("LList");
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
        is.fatalCheck(" operator>>(Istream&, LList<LListBase, T>&)");

        while
        (
           !(
                lastToken.isPunctuation()
             && lastToken.pToken() == token::END_LIST
            )
        )
        {
            is.putBack(lastToken);
            T element;
            is >> element;
            L.append(element);

            is >> lastToken;
            is.fatalCheck(" operator>>(Istream&, LList<LListBase, T>&)");
        }
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "incorrect first token, expected <int> or '(', found "
            << firstToken.info()
            << exit(FatalIOError);
    }

    // Check state of IOstream
    is.fatalCheck(" operator>>(Istream&, LList<LListBase,>&)");

    return is;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class LListBase, class T>
CML::Ostream& CML::operator<<(Ostream& os, const LList<LListBase, T>& lst)
{
    // Write size
    os << nl << lst.size();

    // Write beginning of contents
    os << nl << token::BEGIN_LIST << nl;

    // Write contents
    for
    (
        typename LList<LListBase, T>::const_iterator iter = lst.begin();
        iter != lst.end();
        ++iter
    )
    {
        os << iter() << nl;
    }

    // Write end of contents
    os << token::END_LIST;

    // Check state of IOstream
    os.check("Ostream& operator<<(Ostream&, const LList<LListBase, T>&)");

    return os;
}




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
