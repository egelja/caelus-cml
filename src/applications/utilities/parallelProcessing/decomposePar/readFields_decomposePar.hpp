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

Global
    readFields

Description

SourceFiles
    readFields.cpp

\*---------------------------------------------------------------------------*/

#ifndef readFields_H
#define readFields_H

#include "IOobjectList.hpp"
#include "PtrList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    // Read the fields and hold on the pointer list
    template<class Type, template<class> class PatchField, class GeoMesh>
    void readFields
    (
        const typename GeoMesh::Mesh& mesh,
        const IOobjectList& objects,
        PtrList<GeometricField<Type, PatchField, GeoMesh> >& fields,
        const bool readOldTime
    );

    // Read the fields and hold on the pointer list
    template<class Mesh, class GeoField>
    void readFields
    (
        const Mesh& mesh,
        const IOobjectList& objects,
        PtrList<GeoField>& fields
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
void CML::readFields
(
    const typename GeoMesh::Mesh& mesh,
    const IOobjectList& objects,
    PtrList<GeometricField<Type, PatchField, GeoMesh> >& fields,
    const bool readOldTime
)
{
    typedef GeometricField<Type, PatchField, GeoMesh> GeoField;

    // Search list of objects for fields of type GeomField
    IOobjectList fieldObjects(objects.lookupClass(GeoField::typeName));

    // Remove the cellDist field
    IOobjectList::iterator celDistIter = fieldObjects.find("cellDist");
    if (celDistIter != fieldObjects.end())
    {
        fieldObjects.erase(celDistIter);
    }

    // Get sorted set of names (different processors might read objects in
    // different order)
    const wordList masterNames(fieldObjects.sortedNames());

    // Construct the fields
    fields.setSize(masterNames.size());

    forAll(masterNames, i)
    {
        const IOobject& io = *fieldObjects[masterNames[i]];

        fields.set(i, new GeoField(io, mesh, readOldTime));
    }
}


template<class Mesh, class GeoField>
void CML::readFields
(
    const Mesh& mesh,
    const IOobjectList& objects,
    PtrList<GeoField>& fields
)
{
    // Search list of objects for fields of type GeomField
    IOobjectList fieldObjects(objects.lookupClass(GeoField::typeName));

    // Construct the fields
    fields.setSize(fieldObjects.size());

    // Get sorted set of names (different processors might read objects in
    // different order)
    const wordList masterNames(fieldObjects.sortedNames());

    // Construct the fields
    fields.setSize(masterNames.size());

    forAll(masterNames, i)
    {
        const IOobject& io = *fieldObjects[masterNames[i]];

        fields.set(i, new GeoField(io, mesh));
    }
}
#endif

// ************************************************************************* //
