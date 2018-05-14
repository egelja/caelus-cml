/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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
    CML::fvMeshTools

Description
    A collection of tools for operating on an fvMesh.

SourceFiles
    fvMeshTools.C

\*---------------------------------------------------------------------------*/

#ifndef fvMeshTools_H
#define fvMeshTools_H

#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class fvMeshTools Declaration
\*---------------------------------------------------------------------------*/

class fvMeshTools
{
    template<class GeoField>
    static void addPatchFields
    (
        fvMesh&,
        const dictionary& patchFieldDict,
        const word& defaultPatchFieldType,
        const typename GeoField::value_type& defaultPatchValue
    );

    //- Set patchFields according to dictionary
    template<class GeoField>
    static void setPatchFields
    (
        fvMesh& mesh,
        const label patchI,
        const dictionary& patchFieldDict
    );

    //- Set patchFields to value
    template<class GeoField>
    static void setPatchFields
    (
        fvMesh& mesh,
        const label patchI,
        const typename GeoField::value_type& value
    );

    // Remove last patch fields
    template<class GeoField>
    static void trimPatchFields(fvMesh&, const label nPatches);

    template<class GeoField>
    static void reorderPatchFields(fvMesh&, const labelList& oldToNew);

    // Remove trialing patches
    static void trimPatches(fvMesh&, const label nPatches);


public:

    //- Add patch. Inserts patch before all processor patches.
    //  Supply per field the new patchField per field as a
    //  subdictionary or a default type. If validBoundary call is parallel
    //  synced and all add the same patch with same settings.
    static label addPatch
    (
        fvMesh& mesh,
        const polyPatch& patch,
        const dictionary& patchFieldDict,
        const word& defaultPatchFieldType,
        const bool validBoundary
    );

    //- Change patchField on registered fields according to dictionary
    static void setPatchFields
    (
        fvMesh& mesh,
        const label patchI,
        const dictionary& patchFieldDict
    );

    //- Change patchField to zero on registered fields
    static void zeroPatchFields(fvMesh& mesh, const label patchI);

    //- Reorder and remove trailing patches. If validBoundary call is parallel
    //  synced
    static void reorderPatches
    (
        fvMesh&,
        const labelList& oldToNew,
        const label nPatches,
        const bool validBoundary
    );

    //- Remove zero sized patches. All but processor patches are
    //  assumed to be present on all processors (so size will be reduced
    //  if validBoundary). Return map from new
    //  to old patches
    static labelList removeEmptyPatches(fvMesh&, const bool validBoundary);

    //- Read mesh or create dummy mesh (0 cells, >0 patches). Works in two
    //  modes according to masterOnlyReading:
    //  true : all slaves create dummy mesh
    //  false: checks locally for mesh directories and only creates dummy mesh
    //         if not present
    static autoPtr<fvMesh> newMesh
    (
        const IOobject& io,
        const bool masterOnlyReading
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class GeoField>
void CML::fvMeshTools::addPatchFields
(
    fvMesh& mesh,
    const dictionary& patchFieldDict,
    const word& defaultPatchFieldType,
    const typename GeoField::value_type& defaultPatchValue
)
{
    HashTable<GeoField*> flds
    (
        mesh.objectRegistry::lookupClass<GeoField>()
    );

    forAllIter(typename HashTable<GeoField*>, flds, iter)
    {
        GeoField& fld = *iter();

        typename GeoField::GeometricBoundaryField& bfld =
            fld.boundaryField();

        label sz = bfld.size();
        bfld.setSize(sz+1);

        if (patchFieldDict.found(fld.name()))
        {
            bfld.set
            (
                sz,
                GeoField::PatchFieldType::New
                (
                    mesh.boundary()[sz],
                    fld.dimensionedInternalField(),
                    patchFieldDict.subDict(fld.name())
                )
            );
        }
        else
        {
            bfld.set
            (
                sz,
                GeoField::PatchFieldType::New
                (
                    defaultPatchFieldType,
                    mesh.boundary()[sz],
                    fld.dimensionedInternalField()
                )
            );
            bfld[sz] == defaultPatchValue;
        }
    }
}


template<class GeoField>
void CML::fvMeshTools::setPatchFields
(
    fvMesh& mesh,
    const label patchI,
    const dictionary& patchFieldDict
)
{
    HashTable<GeoField*> flds
    (
        mesh.objectRegistry::lookupClass<GeoField>()
    );

    forAllIter(typename HashTable<GeoField*>, flds, iter)
    {
        GeoField& fld = *iter();

        typename GeoField::GeometricBoundaryField& bfld =
            fld.boundaryField();

        if (patchFieldDict.found(fld.name()))
        {
            bfld.set
            (
                patchI,
                GeoField::PatchFieldType::New
                (
                    mesh.boundary()[patchI],
                    fld.dimensionedInternalField(),
                    patchFieldDict.subDict(fld.name())
                )
            );
        }
    }
}




template<class GeoField>
void CML::fvMeshTools::setPatchFields
(
    fvMesh& mesh,
    const label patchI,
    const typename GeoField::value_type& value
)
{
    HashTable<GeoField*> flds
    (
        mesh.objectRegistry::lookupClass<GeoField>()
    );

    forAllIter(typename HashTable<GeoField*>, flds, iter)
    {
        GeoField& fld = *iter();

        typename GeoField::GeometricBoundaryField& bfld =
            fld.boundaryField();

        bfld[patchI] == value;
    }
}


// Remove last patch field
template<class GeoField>
void CML::fvMeshTools::trimPatchFields(fvMesh& mesh, const label nPatches)
{
    HashTable<GeoField*> flds
    (
        mesh.objectRegistry::lookupClass<GeoField>()
    );

    forAllIter(typename HashTable<GeoField*>, flds, iter)
    {
        GeoField& fld = *iter();
        fld.boundaryField().setSize(nPatches);
    }
}


// Reorder patch field
template<class GeoField>
void CML::fvMeshTools::reorderPatchFields
(
    fvMesh& mesh,
    const labelList& oldToNew
)
{
    HashTable<GeoField*> flds
    (
        mesh.objectRegistry::lookupClass<GeoField>()
    );

    forAllIter(typename HashTable<GeoField*>, flds, iter)
    {
        GeoField& fld = *iter();

        typename GeoField::GeometricBoundaryField& bfld =
            fld.boundaryField();

        bfld.reorder(oldToNew);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
