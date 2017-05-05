/*---------------------------------------------------------------------------*\
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
    CML::MeshObject

Description
    Templated abstract base-class for dynamic mesh objects used to automate
    their allocation to the mesh database and the mesh-modifier event-loop.

SourceFiles
    MeshObject.cpp

\*---------------------------------------------------------------------------*/

#ifndef MeshObject_H
#define MeshObject_H

#include "regIOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class MeshObject Declaration
\*---------------------------------------------------------------------------*/


template<class Mesh, class Type>
class MeshObject
:
    public regIOobject
{

protected:

        // Reference to Mesh
        const Mesh& mesh_;


public:

    // Constructors

        explicit MeshObject(const Mesh& mesh);

        static const Type& New(const Mesh& mesh);

        template<class Data1>
        static const Type& New
        (
            const Mesh& mesh,
            const Data1& d
        );

        template<class Data1, class Data2>
        static const Type& New
        (
            const Mesh& mesh,
            const Data1&,
            const Data2&
        );

        template<class Data1, class Data2, class Data3>
        static const Type& New
        (
            const Mesh& mesh,
            const Data1&,
            const Data2&,
            const Data3&
        );

        template<class Data1, class Data2, class Data3, class Data4>
        static const Type& New
        (
            const Mesh& mesh,
            const Data1&,
            const Data2&,
            const Data3&,
            const Data4&
        );


    // Destructors

        virtual ~MeshObject();

        static bool Delete(const Mesh& mesh);


    // Member Functions

        const Mesh& mesh() const
        {
            return mesh_;
        }

        virtual bool writeData(CML::Ostream&) const
        {
            return true;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "objectRegistry.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Mesh, class Type>
CML::MeshObject<Mesh, Type>::MeshObject(const Mesh& mesh)
:
    regIOobject
    (
        IOobject
        (
            Type::typeName,
            mesh.thisDb().instance(),
            mesh.thisDb()
        )
    ),
    mesh_(mesh)
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

template<class Mesh, class Type>
const Type& CML::MeshObject<Mesh, Type>::New
(
    const Mesh& mesh
)
{
    if
    (
        mesh.thisDb().objectRegistry::template foundObject<Type>
        (
            Type::typeName
        )
    )
    {
        return mesh.thisDb().objectRegistry::template lookupObject<Type>
        (
            Type::typeName
        );
    }
    else
    {
        return store(new Type(mesh));
    }
}


template<class Mesh, class Type>
template<class Data1>
const Type& CML::MeshObject<Mesh, Type>::New
(
    const Mesh& mesh,
    const Data1& d
)
{
    if
    (
        mesh.thisDb().objectRegistry::template foundObject<Type>
        (
            Type::typeName
        )
    )
    {
        return mesh.thisDb().objectRegistry::template lookupObject<Type>
        (
            Type::typeName
        );
    }
    else
    {
        return store(new Type(mesh, d));
    }
}


template<class Mesh, class Type>
template<class Data1, class Data2>
const Type& CML::MeshObject<Mesh, Type>::New
(
    const Mesh& mesh,
    const Data1& d1,
    const Data2& d2
)
{
    if
    (
        mesh.thisDb().objectRegistry::template foundObject<Type>
        (
            Type::typeName
        )
    )
    {
        return mesh.thisDb().objectRegistry::template lookupObject<Type>
        (
            Type::typeName
        );
    }
    else
    {
        return store(new Type(mesh, d1, d2));
    }
}


template<class Mesh, class Type>
template<class Data1, class Data2, class Data3>
const Type& CML::MeshObject<Mesh, Type>::New
(
    const Mesh& mesh,
    const Data1& d1,
    const Data2& d2,
    const Data3& d3
)
{
    if
    (
        mesh.thisDb().objectRegistry::template foundObject<Type>
        (
            Type::typeName
        )
    )
    {
        return mesh.thisDb().objectRegistry::template lookupObject<Type>
        (
            Type::typeName
        );
    }
    else
    {
        return store(new Type(mesh, d1, d2, d3));
    }
}


template<class Mesh, class Type>
template<class Data1, class Data2, class Data3, class Data4>
const Type& CML::MeshObject<Mesh, Type>::New
(
    const Mesh& mesh,
    const Data1& d1,
    const Data2& d2,
    const Data3& d3,
    const Data4& d4
)
{
    if
    (
        mesh.thisDb().objectRegistry::template foundObject<Type>
        (
            Type::typeName
        )
    )
    {
        return mesh.thisDb().objectRegistry::template lookupObject<Type>
        (
            Type::typeName
        );
    }
    else
    {
        return store(new Type(mesh, d1, d2, d3, d4));
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class Mesh, class Type>
bool CML::MeshObject<Mesh, Type>::Delete(const Mesh& mesh)
{
    if
    (
        mesh.thisDb().objectRegistry::template foundObject<Type>
        (
            Type::typeName
        )
    )
    {
        return mesh.thisDb().checkOut
        (
            const_cast<Type&>
            (
                mesh.thisDb().objectRegistry::template lookupObject<Type>
                (
                    Type::typeName
                )
            )
        );
    }
    else
    {
        return false;
    }
}


template<class Mesh, class Type>
CML::MeshObject<Mesh, Type>::~MeshObject()
{
    release();
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
