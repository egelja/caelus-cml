/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::IOMap

Description
    A Map of objects of type \<T\> with automated input and output.

SourceFiles
    IOMap.cpp

\*---------------------------------------------------------------------------*/

#ifndef IOMap_H
#define IOMap_H

#include "Map.hpp"
#include "regIOobject.hpp"
//CAELUSFIX
#include "dictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class IOMap Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class IOMap
:
    public regIOobject,
    public Map<T>
{

public:

    //- Runtime type information
    TypeName("Map");


    // Constructors

        //- Construct from IOobject
        IOMap(const IOobject&);

        //- Construct from IOobject and size of Map
        IOMap(const IOobject&, const label);

        //- Construct from IOobject and a Map
        IOMap(const IOobject&, const Map<T>&);

        //- Construct by transferring the Map contents
        IOMap(const IOobject&, const Xfer<Map<T> >&);


    //- Destructor
    virtual ~IOMap();


    // Member functions

        bool writeData(Ostream&) const;


    // Member operators

        //- Assignment of other IOMap's entries to this IOMap
        void operator=(const IOMap<T>&);

        //- Assignment of other Map's entries to this IOMap
        void operator=(const Map<T>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class T>
CML::IOMap<T>::IOMap(const IOobject& io)
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
        readStream(typeName) >> *this;
        close();
    }
}

template<class T>
CML::IOMap<T>::IOMap(const IOobject& io, const label size)
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
        readStream(typeName) >> *this;
        close();
    }
    else
    {
        Map<T>::setSize(size);
    }
}


template<class T>
CML::IOMap<T>::IOMap(const IOobject& io, const Map<T>& map)
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
        readStream(typeName) >> *this;
        close();
    }
    else
    {
        Map<T>::operator=(map);
    }
}


template<class T>
CML::IOMap<T>::IOMap(const IOobject& io, const Xfer<Map<T> >& map)
:
    regIOobject(io)
{
    Map<T>::transfer(map());

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
CML::IOMap<T>::~IOMap()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
bool CML::IOMap<T>::writeData(Ostream& os) const
{
    return (os << *this).good();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
void CML::IOMap<T>::operator=(const IOMap<T>& rhs)
{
    Map<T>::operator=(rhs);
}


template<class T>
void CML::IOMap<T>::operator=(const Map<T>& rhs)
{
    Map<T>::operator=(rhs);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
