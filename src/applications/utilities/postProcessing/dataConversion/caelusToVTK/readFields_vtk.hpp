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

InClass
    CML::readFields

Description

SourceFiles
    readFields.cpp

\*---------------------------------------------------------------------------*/

#ifndef readFields_H
#define readFields_H

#include "fvMesh.hpp"
#include "PtrList.hpp"
#include "IOobjectList.hpp"
#include "HashSet.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Read the fields and optionally subset and put on the pointer list
template<class GeoField>
void readFields
(
    const vtkMesh& vMesh,
    const typename GeoField::Mesh& mesh,
    const IOobjectList& objects,
    const HashSet<word>& selectedFields,
    PtrList<GeoField>& fields
);

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IOobjectList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class GeoField>
void readFields
(
    const vtkMesh& vMesh,
    const typename GeoField::Mesh& mesh,
    const IOobjectList& objects,
    const HashSet<word>& selectedFields,
    PtrList<GeoField>& fields
)
{
    // Search list of objects for volScalarFields
    IOobjectList fieldObjects(objects.lookupClass(GeoField::typeName));

    // Construct the vol scalar fields
    fields.setSize(fieldObjects.size());
    label nFields = 0;

    forAllIter(IOobjectList, fieldObjects, iter)
    {
        if (selectedFields.empty() || selectedFields.found(iter()->name()))
        {
            fields.set
            (
                nFields,
                vMesh.interpolate
                (
                    GeoField
                    (
                        *iter(),
                        mesh
                    )
                )
            );
            nFields++;
        }
    }

    fields.setSize(nFields);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
