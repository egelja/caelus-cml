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
    CML::IOPtrList

Description
    A PtrList of objects of type \<T\> with automated input and output.


\*---------------------------------------------------------------------------*/

#ifndef IOPtrList_H
#define IOPtrList_H

#include "PtrList.hpp"
#include "regIOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class IOPtrList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class IOPtrList
:
    public regIOobject,
    public PtrList<T>
{

public:

    //- Runtime type information
    TypeName("PtrList");


    // Constructors

        //- Construct from IOobject using given Istream constructor class
        template<class INew>
        IOPtrList(const IOobject&, const INew&);

        //- Construct from IOobject
        IOPtrList(const IOobject&);

        //- Construct from IOobject with given size
        IOPtrList(const IOobject&, const label);

        //- Construct from IOobject and a PtrList
        IOPtrList(const IOobject&, const PtrList<T>&);

        //- Construct by transferring the PtrList contents
        IOPtrList(const IOobject&, const Xfer<PtrList<T> >&);


    //- Destructor
    virtual ~IOPtrList();


    // Member functions

        bool writeData(Ostream&) const;


    // Member operators

        void operator=(const IOPtrList<T>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class T>
template<class INew>
CML::IOPtrList<T>::IOPtrList(const IOobject& io, const INew& inewt)
:
    regIOobject(io)
{
    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        PtrList<T>::read(readStream(typeName), inewt);
        close();
    }
}


template<class T>
CML::IOPtrList<T>::IOPtrList(const IOobject& io)
:
    regIOobject(io)
{
    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        PtrList<T>::read(readStream(typeName), INew<T>());
        close();
    }
}


template<class T>
CML::IOPtrList<T>::IOPtrList(const IOobject& io, const label s)
:
    regIOobject(io),
    PtrList<T>(s)
{
    if (io.readOpt() != IOobject::NO_READ)
    {
        FatalErrorInFunction
            << "NO_READ must be set if specifying size" << nl
            << exit(FatalError);
    }
}


template<class T>
CML::IOPtrList<T>::IOPtrList(const IOobject& io, const PtrList<T>& list)
:
    regIOobject(io)
{
    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        PtrList<T>::read(readStream(typeName), INew<T>());
        close();
    }
    else
    {
        PtrList<T>::operator=(list);
    }
}


template<class T>
CML::IOPtrList<T>::IOPtrList(const IOobject& io, const Xfer<PtrList<T> >& list)
:
    regIOobject(io)
{
    PtrList<T>::transfer(list());

    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        PtrList<T>::read(readStream(typeName), INew<T>());
        close();
    }
}


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

template<class T>
CML::IOPtrList<T>::~IOPtrList()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
bool CML::IOPtrList<T>::writeData(Ostream& os) const
{
    return (os << *this).good();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
void CML::IOPtrList<T>::operator=(const IOPtrList<T>& rhs)
{
    PtrList<T>::operator=(rhs);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
