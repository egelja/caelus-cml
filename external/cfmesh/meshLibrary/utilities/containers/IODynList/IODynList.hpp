/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Class
    IODynList

Description
    An IODynList of a given type is a DynList of that type which supports
    automated input and output.

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    IODynList.cpp

\*---------------------------------------------------------------------------*/

#ifndef IODynList_HPP
#define IODynList_HPP

#include "DynList.hpp"
#include "regIOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class IODynList Declaration
\*---------------------------------------------------------------------------*/

template<class T, class IndexType=label>
class IODynList
:
    public regIOobject,
    public DynList<T, IndexType>
{

public:

    //- Runtime type information
    TypeName("DynList");


    // Constructors

        //- Construct from IOobject
        IODynList(const IOobject&);

        //- Construct from IOobject and size of IODynList
        IODynList(const IOobject&, const IndexType);

        //- Construct from IOobject and a List
        IODynList(const IOobject&, const DynList<T, IndexType>&);


    // Member functions

        bool writeData(Ostream&) const;


    // Member operators

        void operator=(const IODynList<T, IndexType>&);

        void operator=(const DynList<T, IndexType>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class T, class IndexType>
IODynList<T, IndexType>::IODynList(const IOobject& io)
:
    regIOobject(io),
    DynList<T, IndexType>()
{
    if
    (
        io.readOpt() == IOobject::MUST_READ
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }
}


template<class T, class IndexType>
IODynList<T, IndexType>::IODynList
(
    const IOobject& io,
    const IndexType size
)
:
    regIOobject(io),
    DynList<T, IndexType>(size)
{}


template<class T, class IndexType>
IODynList<T, IndexType>::IODynList
(
    const IOobject& io,
    const DynList<T, IndexType>& list
)
:
    regIOobject(io),
    DynList<T, IndexType>()
{
    if (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    {
        readStream(typeName) >> *this;
        close();
    }
    
    DynList<T, IndexType>::operator=(list);
}


template<class T, class IndexType>
void IODynList<T, IndexType>::operator=
(
    const IODynList<T, IndexType>& rhs
)
{
    DynList<T, IndexType>::operator=(rhs);
}


template<class T, class IndexType>
void IODynList<T, IndexType>::operator=
(
    const DynList<T, IndexType>& rhs
)
{
    DynList<T, IndexType>::operator=(rhs);
}


template<class T, class IndexType>
bool IODynList<T, IndexType>::writeData(Ostream& os) const
{
    return (os << *this).good();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
