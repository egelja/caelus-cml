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
    CML::IOList

Description
    A List of objects of type \<T\> with automated input and output.


\*---------------------------------------------------------------------------*/

#ifndef IOList_H
#define IOList_H

#include "List.hpp"
#include "regIOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class IOList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class IOList
:
    public regIOobject,
    public List<T>
{

public:

    //- Runtime type information
    TypeName("List");


    // Constructors

        //- Construct from IOobject
        IOList(const IOobject&);

        //- Construct from IOobject and size of IOList
        IOList(const IOobject&, const label);

        //- Construct from IOobject and a List
        IOList(const IOobject&, const List<T>&);

        //- Construct by transferring the List contents
        IOList(const IOobject&, const Xfer<List<T> >&);


    //- Destructor
    virtual ~IOList();


    // Member functions

        bool writeData(Ostream&) const;


    // Member operators

        void operator=(const IOList<T>&);

        void operator=(const List<T>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class T>
CML::IOList<T>::IOList(const IOobject& io)
:
    regIOobject(io)
{
    // Temporary warning
    if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    {
        WarningInFunction
            << "IOList " << name()
            << " constructed with IOobject::MUST_READ_IF_MODIFIED"
            " but IOList does not support automatic rereading."
            << endl;
    }
    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }
}


template<class T>
CML::IOList<T>::IOList(const IOobject& io, const label size)
:
    regIOobject(io)
{
    // Temporary warning
    if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    {
        WarningInFunction
            << "IOList " << name()
            << " constructed with IOobject::MUST_READ_IF_MODIFIED"
            " but IOList does not support automatic rereading."
            << endl;
    }
    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }
    else
    {
        List<T>::setSize(size);
    }
}


template<class T>
CML::IOList<T>::IOList(const IOobject& io, const List<T>& list)
:
    regIOobject(io)
{
    // Temporary warning
    if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    {
        WarningInFunction
            << "IOList " << name()
            << " constructed with IOobject::MUST_READ_IF_MODIFIED"
            " but IOList does not support automatic rereading."
            << endl;
    }

    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }
    else
    {
        List<T>::operator=(list);
    }
}


template<class T>
CML::IOList<T>::IOList(const IOobject& io, const Xfer<List<T> >& list)
:
    regIOobject(io)
{
    // Temporary warning
    if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    {
        WarningInFunction
            << "IOList " << name()
            << " constructed with IOobject::MUST_READ_IF_MODIFIED"
            " but IOList does not support automatic rereading."
            << endl;
    }

    List<T>::transfer(list());

    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }
}


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

template<class T>
CML::IOList<T>::~IOList()
{}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
bool CML::IOList<T>::writeData(Ostream& os) const
{
    return (os << *this).good();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
void CML::IOList<T>::operator=(const IOList<T>& rhs)
{
    List<T>::operator=(rhs);
}


template<class T>
void CML::IOList<T>::operator=(const List<T>& rhs)
{
    List<T>::operator=(rhs);
}




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
