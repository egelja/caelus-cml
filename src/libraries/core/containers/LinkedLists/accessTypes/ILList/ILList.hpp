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
    CML::ILList

Description
    Template class for intrusive linked lists.

SourceFiles
    ILList.cpp
    ILListIO.cpp

\*---------------------------------------------------------------------------*/

#ifndef ILList_H
#define ILList_H

#include "UILList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class Istream;
class Ostream;

// Forward declaration of friend functions and operators

template<class LListBase, class T> class ILList;

template<class LListBase, class T> Istream& operator>>
(
    Istream&,
    ILList<LListBase, T>&
);


/*---------------------------------------------------------------------------*\
                           Class ILList Declaration
\*---------------------------------------------------------------------------*/

template<class LListBase, class T>
class ILList
:
    public UILList<LListBase, T>
{
    // Private Member Functions

        //- Read from Istream using given Istream constructor class
        template<class INew>
        void read(Istream&, const INew&);


public:

    // Constructors

        //- Null construct
        ILList()
        {}

        //- Construct given initial T
        ILList(T* a)
        :
            UILList<LListBase, T>(a)
        {}

        //- Construct from Istream
        ILList(Istream&);

        //- Construct as copy
        ILList(const ILList<LListBase, T>&);

        //- Copy constructor with additional argument for clone
        template<class CloneArg>
        ILList(const ILList<LListBase, T>& lst, const CloneArg& cloneArg)
		
        #ifdef __INTEL_COMPILER
        :
            UILList<LListBase, T>()
        {
            for
            (
                typename UILList<LListBase, T>::const_iterator iter =
                    lst.begin();
                iter != lst.end();
                ++iter
            )
            {
                this->append(iter().clone(cloneArg).ptr());
            }
        }
        #else
        ;
        #endif

        //- Construct from Istream using given Istream constructor class
        template<class INew>
        ILList(Istream&, const INew&);


    //- Destructor
    ~ILList();


    // Member Functions

        // Edit

            //- Remove the head element specified from the list and delete it
            bool eraseHead();

            //- Remove the specified element from the list and delete it
            bool erase(T* p);

            //- Clear the contents of the list
            void clear();

            //- Transfer the contents of the argument into this List
            //  and annul the argument list.
            void transfer(ILList<LListBase, T>&);


    // Member operators

        void operator=(const ILList<LListBase, T>&);


    // Istream operator

        //- Read List from Istream, discarding contents of existing List.
        friend Istream& operator>> <LListBase, T>
        (
            Istream&,
            ILList<LListBase, T>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class LListBase, class T>
CML::ILList<LListBase, T>::ILList(const ILList<LListBase, T>& lst)
:
    UILList<LListBase, T>()
{
    for
    (
        typename UILList<LListBase, T>::const_iterator iter = lst.begin();
        iter != lst.end();
        ++iter
    )
    {
        this->append(iter().clone().ptr());
    }
}


#ifndef __INTEL_COMPILER
template<class LListBase, class T>
template<class CloneArg>
CML::ILList<LListBase, T>::ILList
(
    const ILList<LListBase, T>& lst,
    const CloneArg& cloneArg
)
:
    UILList<LListBase, T>()
{
    for
    (
        typename UILList<LListBase, T>::const_iterator iter = lst.begin();
        iter != lst.end();
        ++iter
    )
    {
        this->append(iter().clone(cloneArg).ptr());
    }
}
#endif


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

template<class LListBase, class T>
CML::ILList<LListBase, T>::~ILList()
{
    this->clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class LListBase, class T>
bool CML::ILList<LListBase, T>::eraseHead()
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
bool CML::ILList<LListBase, T>::erase(T* p)
{
    T* tPtr;
    if ((tPtr = remove(p)))
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
void CML::ILList<LListBase, T>::clear()
{
    label oldSize = this->size();
    for (label i=0; i<oldSize; ++i)
    {
        eraseHead();
    }

    LListBase::clear();
}


template<class LListBase, class T>
void CML::ILList<LListBase, T>::transfer(ILList<LListBase, T>& lst)
{
    clear();
    LListBase::transfer(lst);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class LListBase, class T>
void CML::ILList<LListBase, T>::operator=(const ILList<LListBase, T>& lst)
{
    this->clear();

    for
    (
        typename UILList<LListBase, T>::const_iterator iter = lst.begin();
        iter != lst.end();
        ++iter
    )
    {
        this->append(iter().clone().ptr());
    }
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

#include "Istream.hpp"
#include "INew.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class LListBase, class T>
template<class INew>
void CML::ILList<LListBase, T>::read(Istream& is, const INew& iNew)
{
    is.fatalCheck("operator>>(Istream&, ILList<LListBase, T>&)");

    token firstToken(is);

    is.fatalCheck
    (
        "operator>>(Istream&, ILList<LListBase, T>&) : reading first token"
    );

    if (firstToken.isLabel())
    {
        label s = firstToken.labelToken();

        // Read beginning of contents
        char delimiter = is.readBeginList("ILList<LListBase, T>");

        if (s)
        {
            if (delimiter == token::BEGIN_LIST)
            {
                for (label i=0; i<s; ++i)
                {
                    this->append(iNew(is).ptr());

                    is.fatalCheck
                    (
                        "operator>>(Istream&, ILList<LListBase, T>&) : "
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
                    "operator>>(Istream&, ILList<LListBase, T>&) : "
                    "reading entry"
                );

                for (label i=1; i<s; ++i)
                {
                    this->append(new T(*tPtr));
                }
            }
        }

        // Read end of contents
        is.readEndList("ILList<LListBase, T>");
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
        is.fatalCheck("operator>>(Istream&, ILList<LListBase, T>&)");

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
            is.fatalCheck("operator>>(Istream&, ILList<LListBase, T>&)");
        }
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "incorrect first token, expected <int> or '(', found "
            << firstToken.info()
            << exit(FatalIOError);
    }

    is.fatalCheck("operator>>(Istream&, ILList<LListBase, T>&)");
}


template<class LListBase, class T>
template<class INew>
CML::ILList<LListBase, T>::ILList(Istream& is, const INew& iNew)
{
    this->read(is, iNew);
}


template<class LListBase, class T>
CML::ILList<LListBase, T>::ILList(Istream& is)
{
    this->read(is, INew<T>());
}


// * * * * * * * * * * * * * * * Istream Operator  * * * * * * * * * * * * * //

template<class LListBase, class T>
CML::Istream& CML::operator>>(Istream& is, ILList<LListBase, T>& L)
{
    L.clear();
    L.read(is, INew<T>());

    return is;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
