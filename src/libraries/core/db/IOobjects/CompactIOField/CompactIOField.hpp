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
    CML::CompactIOField

Description
    A Field of objects of type \<T\> with automated input and output using
    a compact storage. Behaves like IOField except when binary output in
    case it writes a CompactListList.

    Useful for fields of small subfields e.g. in lagrangian


\*---------------------------------------------------------------------------*/

#ifndef CompactIOField_H
#define CompactIOField_H

#include "IOField.hpp"
#include "regIOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators
template<class T, class BaseType> class CompactIOField;

template<class T, class BaseType> Istream& operator>>
(
    Istream&,
    CompactIOField<T, BaseType>&
);

template<class T, class BaseType> Ostream& operator<<
(
    Ostream&,
    const CompactIOField<T, BaseType>&
);

/*---------------------------------------------------------------------------*\
                        Class CompactIOField Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class BaseType>
class CompactIOField
:
    public regIOobject,
    public Field<Type>
{
    // Private Member Functions

        //- Read according to header type
        void readFromStream();

public:

    //- Runtime type information
    TypeName("FieldField");

    // Constructors

        //- Construct from IOobject
        CompactIOField(const IOobject&);

        //- Construct from IOobject and size
        CompactIOField(const IOobject&, const label);

        //- Construct from IOobject and a Field
        CompactIOField(const IOobject&, const Field<Type>&);

        //- Construct by transferring the Field contents
        CompactIOField(const IOobject&, const Xfer<Field<Type> >&);


    // Destructor

        virtual ~CompactIOField();


    // Member functions

        virtual bool writeObject
        (
            IOstream::streamFormat,
            IOstream::versionNumber,
            IOstream::compressionType
        ) const;

        virtual bool writeData(Ostream&) const;


    // Member operators

        void operator=(const CompactIOField<Type, BaseType>&);

        void operator=(const Field<Type>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "labelList.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type, class BaseType>
void CML::CompactIOField<Type, BaseType>::readFromStream()
{
    Istream& is = readStream(word::null);

    if (headerClassName() == IOField<Type>::typeName)
    {
        is >> static_cast<Field<Type>&>(*this);
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
            << " expected " << typeName << " or " << IOField<Type>::typeName
            << endl
            << "    while reading object " << name()
            << exit(FatalIOError);
    }
}


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class Type, class BaseType>
CML::CompactIOField<Type, BaseType>::CompactIOField(const IOobject& io)
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


template<class Type, class BaseType>
CML::CompactIOField<Type, BaseType>::CompactIOField
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
        Field<Type>::setSize(size);
    }
}


template<class Type, class BaseType>
CML::CompactIOField<Type, BaseType>::CompactIOField
(
    const IOobject& io,
    const Field<Type>& list
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
        Field<Type>::operator=(list);
    }
}


template<class Type, class BaseType>
CML::CompactIOField<Type, BaseType>::CompactIOField
(
    const IOobject& io,
    const Xfer<Field<Type> >& list
)
:
    regIOobject(io)
{
    Field<Type>::transfer(list());

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

template<class Type, class BaseType>
CML::CompactIOField<Type, BaseType>::~CompactIOField()
{}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, class BaseType>
bool CML::CompactIOField<Type, BaseType>::writeObject
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

        const_cast<word&>(typeName) = IOField<Type>::typeName;

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


template<class Type, class BaseType>
bool CML::CompactIOField<Type, BaseType>::writeData(Ostream& os) const
{
    return (os << *this).good();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type, class BaseType>
void CML::CompactIOField<Type, BaseType>::operator=
(
    const CompactIOField<Type, BaseType>& rhs
)
{
    Field<Type>::operator=(rhs);
}


template<class Type, class BaseType>
void CML::CompactIOField<Type, BaseType>::operator=(const Field<Type>& rhs)
{
    Field<Type>::operator=(rhs);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Type, class BaseType>
CML::Istream& CML::operator>>
(
    CML::Istream& is,
    CML::CompactIOField<Type, BaseType>& L
)
{
    // Read compact
    const labelList start(is);
    const Field<BaseType> elems(is);

    // Convert
    L.setSize(start.size()-1);

    forAll(L, i)
    {
        Type& subField = L[i];

        label index = start[i];
        subField.setSize(start[i+1] - index);

        forAll(subField, j)
        {
            subField[j] = elems[index++];
        }
    }

    return is;
}


template<class Type, class BaseType>
CML::Ostream& CML::operator<<
(
    CML::Ostream& os,
    const CML::CompactIOField<Type, BaseType>& L
)
{
    // Keep ascii writing same.
    if (os.format() == IOstream::ASCII)
    {
        os << static_cast<const Field<Type>&>(L);
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

        Field<BaseType> elems(start[start.size()-1]);

        label elemI = 0;
        forAll(L, i)
        {
            const Type& subField = L[i];

            forAll(subField, j)
            {
                elems[elemI++] = subField[j];
            }
        }
        os << start << elems;
    }

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
