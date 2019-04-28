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
    CML::CompactIOList

Description
    A List of objects of type \<T\> with automated input and output using
    a compact storage. Behaves like IOList except when binary output in
    case it writes a CompactListList.

    Useful for lists of small sublists e.g. faceList, cellList.


\*---------------------------------------------------------------------------*/

#ifndef CompactIOList_H
#define CompactIOList_H

#include "IOList.hpp"
#include "regIOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class Istream;
class Ostream;

// Forward declaration of friend functions and operators
template<class T, class BaseType> class CompactIOList;

template<class T, class BaseType> Istream& operator>>
(
    Istream&,
    CompactIOList<T, BaseType>&
);
template<class T, class BaseType> Ostream& operator<<
(
    Ostream&,
    const CompactIOList<T, BaseType>&
);

/*---------------------------------------------------------------------------*\
                           Class CompactIOList Declaration
\*---------------------------------------------------------------------------*/

template<class T, class BaseType>
class CompactIOList
:
    public regIOobject,
    public List<T>
{
    // Private Member Functions

        //- Read according to header type
        void readFromStream();

public:

    //- Runtime type information
    TypeName("CompactList");


    // Constructors

        //- Construct from IOobject
        CompactIOList(const IOobject&);

        //- Construct from IOobject and size of CompactIOList
        CompactIOList(const IOobject&, const label);

        //- Construct from IOobject and a List
        CompactIOList(const IOobject&, const List<T>&);

        //- Construct by transferring the List contents
        CompactIOList(const IOobject&, const Xfer<List<T> >&);


    // Destructor

        virtual ~CompactIOList();


    // Member functions

        virtual bool writeObject
        (
            IOstream::streamFormat,
            IOstream::versionNumber,
            IOstream::compressionType
        ) const;

        virtual bool writeData(Ostream&) const;


    // Member operators

        void operator=(const CompactIOList<T, BaseType>&);

        void operator=(const List<T>&);


    // IOstream operators

        //- Read List from Istream, discarding contents of existing List.
        friend Istream& operator>> <T, BaseType>
        (
            Istream&,
            CompactIOList<T, BaseType>&
        );

        // Write List to Ostream.
        friend Ostream& operator<< <T, BaseType>
        (
            Ostream&,
            const CompactIOList<T, BaseType>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "labelList.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class T, class BaseType>
void CML::CompactIOList<T, BaseType>::readFromStream()
{
    Istream& is = readStream(word::null);

    if (headerClassName() == IOList<T>::typeName)
    {
        is >> static_cast<List<T>&>(*this);
        close();
    }
    else if (headerClassName() == typeName)
    {
        is >> *this;
        close();
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "unexpected class name " << headerClassName()
            << " expected " << typeName << " or " << IOList<T>::typeName
            << endl
            << "    while reading object " << name()
            << exit(FatalIOError);
    }
}


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class T, class BaseType>
CML::CompactIOList<T, BaseType>::CompactIOList(const IOobject& io)
:
    regIOobject(io)
{
    if
    (
        io.readOpt() == IOobject::MUST_READ
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readFromStream();
    }
}


template<class T, class BaseType>
CML::CompactIOList<T, BaseType>::CompactIOList
(
    const IOobject& io,
    const label size
)
:
    regIOobject(io)
{
    if
    (
        io.readOpt() == IOobject::MUST_READ
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readFromStream();
    }
    else
    {
        List<T>::setSize(size);
    }
}


template<class T, class BaseType>
CML::CompactIOList<T, BaseType>::CompactIOList
(
    const IOobject& io,
    const List<T>& list
)
:
    regIOobject(io)
{
    if
    (
        io.readOpt() == IOobject::MUST_READ
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readFromStream();
    }
    else
    {
        List<T>::operator=(list);
    }
}


template<class T, class BaseType>
CML::CompactIOList<T, BaseType>::CompactIOList
(
    const IOobject& io,
    const Xfer<List<T> >& list
)
:
    regIOobject(io)
{
    List<T>::transfer(list());

    if
    (
        io.readOpt() == IOobject::MUST_READ
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readFromStream();
    }
}


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

template<class T, class BaseType>
CML::CompactIOList<T, BaseType>::~CompactIOList()
{}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T, class BaseType>
bool CML::CompactIOList<T, BaseType>::writeObject
(
    IOstream::streamFormat fmt,
    IOstream::versionNumber ver,
    IOstream::compressionType cmp
) const
{
    if (fmt == IOstream::ASCII)
    {
        // Change type to be non-compact format type
        const word oldTypeName = typeName;

        const_cast<word&>(typeName) = IOList<T>::typeName;

        bool good = regIOobject::writeObject(fmt, ver, cmp);

        // Change type back
        const_cast<word&>(typeName) = oldTypeName;

        return good;
    }
    else
    {
        return regIOobject::writeObject(fmt, ver, cmp);
    }
}


template<class T, class BaseType>
bool CML::CompactIOList<T, BaseType>::writeData(Ostream& os) const
{
    return (os << *this).good();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, class BaseType>
void CML::CompactIOList<T, BaseType>::operator=
(
    const CompactIOList<T, BaseType>& rhs
)
{
    List<T>::operator=(rhs);
}


template<class T, class BaseType>
void CML::CompactIOList<T, BaseType>::operator=(const List<T>& rhs)
{
    List<T>::operator=(rhs);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class T, class BaseType>
CML::Istream& CML::operator>>
(
    CML::Istream& is,
    CML::CompactIOList<T, BaseType>& L
)
{
    // Read compact
    const labelList start(is);
    const List<BaseType> elems(is);

    // Convert
    L.setSize(start.size()-1);

    forAll(L, i)
    {
        T& subList = L[i];

        label index = start[i];
        subList.setSize(start[i+1] - index);

        forAll(subList, j)
        {
            subList[j] = elems[index++];
        }
    }

    return is;
}


template<class T, class BaseType>
CML::Ostream& CML::operator<<
(
    CML::Ostream& os,
    const CML::CompactIOList<T, BaseType>& L
)
{
    // Keep ascii writing same.
    if (os.format() == IOstream::ASCII)
    {
        os << static_cast<const List<T>&>(L);
    }
    else
    {
        // Convert to compact format
        labelList start(L.size()+1);

        start[0] = 0;
        for (label i = 1; i < start.size(); i++)
        {
            start[i] = start[i-1]+L[i-1].size();
        }

        List<BaseType> elems(start[start.size()-1]);

        label elemI = 0;
        forAll(L, i)
        {
            const T& subList = L[i];

            forAll(subList, j)
            {
                elems[elemI++] = subList[j];
            }
        }
        os << start << elems;
    }

    return os;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
