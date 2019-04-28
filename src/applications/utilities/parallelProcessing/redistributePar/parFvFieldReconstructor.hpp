/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::parFvFieldReconstructor

Description
    Finite volume reconstructor for volume and surface fields.

    Runs in parallel. Reconstructs from procMesh to baseMesh. baseMesh
    is non-zero cells on processor0 only.

SourceFiles
    parFvFieldReconstructor.cpp

\*---------------------------------------------------------------------------*/

#ifndef parFvFieldReconstructor_HPP
#define parFvFieldReconstructor_HPP

#include "PtrList.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class mapDistributePolyMesh;
class mapDistributeBase;
class IOobjectList;

/*---------------------------------------------------------------------------*\
                    Class parFvFieldReconstructor Declaration
\*---------------------------------------------------------------------------*/

class parFvFieldReconstructor
{
    // Private data

        //- Reconstructed mesh reference
        fvMesh& baseMesh_;

        //- Processor mesh reference
        const fvMesh& procMesh_;

        //- Distribution map reference
        const mapDistributePolyMesh& distMap_;

        //- Do I need to to write (usually master only)
        const bool isWriteProc_;

        //- Patch mappers
        PtrList<mapDistributeBase> patchFaceMaps_;


    // Private Member Functions

        //- Construct per-patch addressing
        void createPatchFaceMaps();

        //- Disallow default bitwise copy construct
        parFvFieldReconstructor(const parFvFieldReconstructor&);

        //- Disallow default bitwise assignment
        void operator=(const parFvFieldReconstructor&);


public:

    // Constructors

        //- Construct from components
        parFvFieldReconstructor
        (
            fvMesh& baseMesh,
            const fvMesh& procMesh,
            const mapDistributePolyMesh& distMap,
            const bool isWriteProc
        );


    // Member Functions

        //- Reconstruct volume internal field
        template<class Type>
        tmp<DimensionedField<Type, volMesh> >
        reconstructFvVolumeInternalField
        (
            const DimensionedField<Type, volMesh>&
        ) const;

        //- Read and reconstruct volume internal field
        template<class Type>
        tmp<DimensionedField<Type, volMesh> >
        reconstructFvVolumeInternalField(const IOobject& fieldIoObject) const;



        //- Reconstruct volume field
        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> >
        reconstructFvVolumeField
        (
            const GeometricField<Type, fvPatchField, volMesh>& fld
        ) const;

        //- Read and reconstruct volume field
        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> >
        reconstructFvVolumeField(const IOobject& fieldIoObject) const;



        //- Reconstruct surface field
        template<class Type>
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        reconstructFvSurfaceField
        (
            const GeometricField<Type, fvsPatchField, surfaceMesh>&
        ) const;

        //- Read and reconstruct surface field
        template<class Type>
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        reconstructFvSurfaceField(const IOobject& fieldIoObject) const;


        //- Read, reconstruct and write all/selected volume internal fields
        template<class Type>
        void reconstructFvVolumeInternalFields
        (
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        ) const;

        //- Read, reconstruct and write all/selected volume fields
        template<class Type>
        void reconstructFvVolumeFields
        (
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        ) const;

        //- Read, reconstruct and write all/selected surface fields
        template<class Type>
        void reconstructFvSurfaceFields
        (
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        ) const;

        //- Helper: reconstruct and write mesh points
        //  (note: should be moved to something like processorMeshes class)
        void reconstructPoints();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Time.hpp"
#include "PtrList.hpp"
#include "fvPatchFields.hpp"
#include "emptyFvPatch.hpp"
#include "emptyFvPatchField.hpp"
#include "emptyFvsPatchField.hpp"
#include "IOobjectList.hpp"
#include "mapDistributePolyMesh.hpp"
#include "processorFvPatch.hpp"
#include "directFvPatchFieldMapper.hpp"
#include "distributedUnallocatedDirectFieldMapper.hpp"
#include "distributedUnallocatedDirectFvPatchFieldMapper.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::DimensionedField<Type, CML::volMesh> >
CML::parFvFieldReconstructor::reconstructFvVolumeInternalField
(
    const DimensionedField<Type, volMesh>& fld
) const
{
    distributedUnallocatedDirectFieldMapper mapper
    (
        labelUList::null(),
        distMap_.cellMap()
    );

    Field<Type> internalField(fld, mapper);

    // Construct a volField
    IOobject baseIO
    (
        fld.name(),
        baseMesh_.time().timeName(),
        fld.local(),
        baseMesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    );

    return tmp<DimensionedField<Type, volMesh> >
    (
        new DimensionedField<Type, volMesh>
        (
            baseIO,
            baseMesh_,
            fld.dimensions(),
            internalField
        )
    );
}


template<class Type>
CML::tmp<CML::DimensionedField<Type, CML::volMesh> >
CML::parFvFieldReconstructor::reconstructFvVolumeInternalField
(
    const IOobject& fieldIoObject
) const
{
    // Read the field
    DimensionedField<Type, volMesh> fld
    (
        fieldIoObject,
        procMesh_
    );

    // Distribute onto baseMesh
    return reconstructFvVolumeInternalField(fld);
}


// Reconstruct a field onto the baseMesh
template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::parFvFieldReconstructor::reconstructFvVolumeField
(
    const GeometricField<Type, fvPatchField, volMesh>& fld
) const
{
    // Create the internalField by remote mapping
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    distributedUnallocatedDirectFieldMapper mapper
    (
        labelUList::null(),
        distMap_.cellMap()
    );

    Field<Type> internalField(fld.internalField(), mapper);



    // Create the patchFields by remote mapping
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Note: patchFields still on mesh, not baseMesh

    PtrList<fvPatchField<Type> > patchFields(fld.mesh().boundary().size());

    const typename GeometricField
    <
        Type,
        fvPatchField,
        volMesh
    >::GeometricBoundaryField& bfld = fld.boundaryField();

    forAll(bfld, patchI)
    {
        if (patchFaceMaps_.set(patchI))
        {
            // Clone local patch field
            patchFields.set(patchI, bfld[patchI].clone());

            distributedUnallocatedDirectFvPatchFieldMapper mapper
            (
                labelUList::null(),
                patchFaceMaps_[patchI]
            );

            // Map into local copy
            patchFields[patchI].autoMap(mapper);
        }
    }


    PtrList<fvPatchField<Type> > basePatchFields
    (
        baseMesh_.boundary().size()
    );

    // Clone the patchFields onto the base patches. This is just to reset
    // the reference to the patch, size and content stay the same.
    forAll(patchFields, patchI)
    {
        if (patchFields.set(patchI))
        {
            const fvPatch& basePatch = baseMesh_.boundary()[patchI];

            const fvPatchField<Type>& pfld = patchFields[patchI];

            labelList dummyMap(identity(pfld.size()));
            directFvPatchFieldMapper dummyMapper(dummyMap);

            basePatchFields.set
            (
                patchI,
                fvPatchField<Type>::New
                (
                    pfld,
                    basePatch,
                    DimensionedField<Type, volMesh>::null(),
                    dummyMapper
                )
            );
        }
    }

    // Add some empty patches on remaining patches (tbd.probably processor
    // patches)
    forAll(basePatchFields, patchI)
    {
        if (patchI >= patchFields.size() || !patchFields.set(patchI))
        {
            basePatchFields.set
            (
                patchI,
                fvPatchField<Type>::New
                (
                    emptyFvPatchField<Type>::typeName,
                    baseMesh_.boundary()[patchI],
                    DimensionedField<Type, volMesh>::null()
                )
            );
        }
    }

    // Construct a volField
    IOobject baseIO
    (
        fld.name(),
        baseMesh_.time().timeName(),
        fld.local(),
        baseMesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    );

    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            baseIO,
            baseMesh_,
            fld.dimensions(),
            internalField,
            basePatchFields
        )
    );
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::parFvFieldReconstructor::reconstructFvVolumeField
(
    const IOobject& fieldIoObject
) const
{
    // Read the field
    GeometricField<Type, fvPatchField, volMesh> fld
    (
        fieldIoObject,
        procMesh_
    );

    // Distribute onto baseMesh
    return reconstructFvVolumeField(fld);
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvsPatchField, CML::surfaceMesh> >
CML::parFvFieldReconstructor::reconstructFvSurfaceField
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& fld
) const
{
    // Create the internalField by remote mapping
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    distributedUnallocatedDirectFieldMapper mapper
    (
        labelUList::null(),
        distMap_.faceMap()
    );

    // Create flat field of internalField + all patch fields
    Field<Type> flatFld(fld.mesh().nFaces(), pTraits<Type>::zero);
    SubList<Type>(flatFld, fld.internalField().size()).assign
    (
        fld.internalField()
    );
    forAll(fld.boundaryField(), patchI)
    {
        const fvsPatchField<Type>& fvp = fld.boundaryField()[patchI];

        SubList<Type>(flatFld, fvp.size(), fvp.patch().start()).assign(fvp);
    }

    // Map all faces
    Field<Type> internalField(flatFld, mapper);

    // Trim to internal faces (note: could also have special mapper)
    internalField.setSize
    (
        min
        (
            internalField.size(),
            baseMesh_.nInternalFaces()
        )
    );


    // Create the patchFields by remote mapping
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Note: patchFields still on mesh, not baseMesh

    PtrList<fvsPatchField<Type> > patchFields(fld.mesh().boundary().size());

    const typename GeometricField
    <
        Type,
        fvsPatchField,
        surfaceMesh
    >::GeometricBoundaryField& bfld = fld.boundaryField();

    forAll(bfld, patchI)
    {
        if (patchFaceMaps_.set(patchI))
        {
            // Clone local patch field
            patchFields.set(patchI, bfld[patchI].clone());

            distributedUnallocatedDirectFvPatchFieldMapper mapper
            (
                labelUList::null(),
                patchFaceMaps_[patchI]
            );

            // Map into local copy
            patchFields[patchI].autoMap(mapper);
        }
    }


    PtrList<fvsPatchField<Type> > basePatchFields
    (
        baseMesh_.boundary().size()
    );

    // Clone the patchFields onto the base patches. This is just to reset
    // the reference to the patch, size and content stay the same.
    forAll(patchFields, patchI)
    {
        if (patchFields.set(patchI))
        {
            const fvPatch& basePatch = baseMesh_.boundary()[patchI];

            const fvsPatchField<Type>& pfld = patchFields[patchI];

            labelList dummyMap(identity(pfld.size()));
            directFvPatchFieldMapper dummyMapper(dummyMap);

            basePatchFields.set
            (
                patchI,
                fvsPatchField<Type>::New
                (
                    pfld,
                    basePatch,
                    DimensionedField<Type, surfaceMesh>::null(),
                    dummyMapper
                )
            );
        }
    }

    // Add some empty patches on remaining patches (tbd.probably processor
    // patches)
    forAll(basePatchFields, patchI)
    {
        if (patchI >= patchFields.size() || !patchFields.set(patchI))
        {
            basePatchFields.set
            (
                patchI,
                fvsPatchField<Type>::New
                (
                    emptyFvsPatchField<Type>::typeName,
                    baseMesh_.boundary()[patchI],
                    DimensionedField<Type, surfaceMesh>::null()
                )
            );
        }
    }

    // Construct a volField
    IOobject baseIO
    (
        fld.name(),
        baseMesh_.time().timeName(),
        fld.local(),
        baseMesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    );

    return tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            baseIO,
            baseMesh_,
            fld.dimensions(),
            internalField,
            basePatchFields
        )
    );
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvsPatchField, CML::surfaceMesh> >
CML::parFvFieldReconstructor::reconstructFvSurfaceField
(
    const IOobject& fieldIoObject
) const
{
    // Read the field
    GeometricField<Type, fvsPatchField, surfaceMesh> fld
    (
        fieldIoObject,
        procMesh_
    );

    return reconstructFvSurfaceField(fld);
}


template<class Type>
void CML::parFvFieldReconstructor::reconstructFvVolumeInternalFields
(
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
) const
{
    const word& fieldClassName = DimensionedField<Type, volMesh>::typeName;

    IOobjectList fields = objects.lookupClass(fieldClassName);

    if (fields.size())
    {
        Info<< "    Reconstructing " << fieldClassName << "s\n" << endl;

        forAllConstIter(IOobjectList, fields, fieldIter)
        {
            if
            (
                selectedFields.empty()
             || selectedFields.found(fieldIter()->name())
            )
            {
                Info<< "        " << fieldIter()->name() << endl;

                tmp<DimensionedField<Type, volMesh> > tfld
                (
                    reconstructFvVolumeInternalField<Type>(*fieldIter())
                );

                if (isWriteProc_)
                {
                    tfld().write();
                }
            }
        }
        Info<< endl;
    }
}


template<class Type>
void CML::parFvFieldReconstructor::reconstructFvVolumeFields
(
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
) const
{
    const word& fieldClassName =
        GeometricField<Type, fvPatchField, volMesh>::typeName;

    IOobjectList fields = objects.lookupClass(fieldClassName);

    if (fields.size())
    {
        Info<< "    Reconstructing " << fieldClassName << "s\n" << endl;

        forAllConstIter(IOobjectList, fields, fieldIter)
        {
            const word& name = fieldIter()->name();

            if
            (
                (selectedFields.empty() || selectedFields.found(name))
             && name != "cellDist"
            )
            {
                Info<< "        " << name << endl;

                tmp<GeometricField<Type, fvPatchField, volMesh> > tfld
                (
                    reconstructFvVolumeField<Type>(*fieldIter())
                );
                if (isWriteProc_)
                {
                    tfld().write();
                }
            }
        }
        Info<< endl;
    }
}


template<class Type>
void CML::parFvFieldReconstructor::reconstructFvSurfaceFields
(
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
) const
{
    const word& fieldClassName =
        GeometricField<Type, fvsPatchField, surfaceMesh>::typeName;

    IOobjectList fields = objects.lookupClass(fieldClassName);

    if (fields.size())
    {
        Info<< "    Reconstructing " << fieldClassName << "s\n" << endl;

        forAllConstIter(IOobjectList, fields, fieldIter)
        {
            if
            (
                selectedFields.empty()
             || selectedFields.found(fieldIter()->name())
            )
            {
                Info<< "        " << fieldIter()->name() << endl;

                tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tfld
                (
                    reconstructFvSurfaceField<Type>(*fieldIter())
                );
                if (isWriteProc_)
                {
                    tfld().write();
                }
            }
        }
        Info<< endl;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
