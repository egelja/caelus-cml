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
    CML::pointFieldReconstructor

Description
    Point field reconstructor.

SourceFiles
    pointFieldReconstructor.cpp

\*---------------------------------------------------------------------------*/

#ifndef pointFieldReconstructor_H
#define pointFieldReconstructor_H

#include "pointMesh.hpp"
#include "pointFields.hpp"
#include "pointPatchFieldMapperPatchRef.hpp"
#include "IOobjectList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class pointFieldReconstructor Declaration
\*---------------------------------------------------------------------------*/

class pointFieldReconstructor
{
    // Private data

        //- Reconstructed mesh reference
        const pointMesh& mesh_;

        //- List of processor meshes
        const PtrList<pointMesh>& procMeshes_;

        //- List of processor point addressing lists
        const PtrList<labelIOList>& pointProcAddressing_;

        //- List of processor boundary addressing lists
        const PtrList<labelIOList>& boundaryProcAddressing_;

        //- Point patch addressing
        labelListListList patchPointAddressing_;

        //- Number of fields reconstructed
        label nReconstructed_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        pointFieldReconstructor(const pointFieldReconstructor&);

        //- Disallow default bitwise assignment
        void operator=(const pointFieldReconstructor&);


public:

        class pointPatchFieldReconstructor
        :
            public pointPatchFieldMapper
        {
            label size_;

        public:

            // Constructors

                //- Construct given size
                pointPatchFieldReconstructor(const label size)
                :
                    size_(size)
                {}


            // Member functions

                label size() const
                {
                    return size_;
                }

                bool direct() const
                {
                    return true;
                }

                const labelUList& directAddressing() const
                {
                    return labelUList::null();
                }
        };


    // Constructors

        //- Construct from components
        pointFieldReconstructor
        (
            const pointMesh& mesh,
            const PtrList<pointMesh>& procMeshes,
            const PtrList<labelIOList>& pointProcAddressing,
            const PtrList<labelIOList>& boundaryProcAddressing
        );


    // Member Functions

        //- Return number of fields reconstructed
        label nReconstructed() const
        {
            return nReconstructed_;
        }

        //- Reconstruct field
        template<class Type>
        tmp<GeometricField<Type, pointPatchField, pointMesh> >
        reconstructField(const IOobject& fieldIoObject);

        //- Reconstruct and write all fields
        template<class Type>
        void reconstructFields
        (
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::GeometricField<Type, CML::pointPatchField, CML::pointMesh> >
CML::pointFieldReconstructor::reconstructField(const IOobject& fieldIoObject)
{
    // Read the field for all the processors
    PtrList<GeometricField<Type, pointPatchField, pointMesh> > procFields
    (
        procMeshes_.size()
    );

    forAll(procMeshes_, proci)
    {
        procFields.set
        (
            proci,
            new GeometricField<Type, pointPatchField, pointMesh>
            (
                IOobject
                (
                    fieldIoObject.name(),
                    procMeshes_[proci]().time().timeName(),
                    procMeshes_[proci](),
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                ),
                procMeshes_[proci]
            )
        );
    }


    // Create the internalField
    Field<Type> internalField(mesh_.size());

    // Create the patch fields
    PtrList<pointPatchField<Type> > patchFields(mesh_.boundary().size());


    forAll(procMeshes_, proci)
    {
        const GeometricField<Type, pointPatchField, pointMesh>&
            procField = procFields[proci];

        // Get processor-to-global addressing for use in rmap
        const labelList& procToGlobalAddr = pointProcAddressing_[proci];

        // Set the cell values in the reconstructed field
        internalField.rmap
        (
            procField.internalField(),
            procToGlobalAddr
        );

        // Set the boundary patch values in the reconstructed field
        forAll(boundaryProcAddressing_[proci], patchi)
        {
            // Get patch index of the original patch
            const label curBPatch = boundaryProcAddressing_[proci][patchi];

            // check if the boundary patch is not a processor patch
            if (curBPatch >= 0)
            {
                if (!patchFields(curBPatch))
                {
                    patchFields.set(
                        curBPatch,
                        pointPatchField<Type>::New
                        (
                            procField.boundaryField()[patchi],
                            mesh_.boundary()[curBPatch],
                            DimensionedField<Type, pointMesh>::null(),
                            pointPatchFieldReconstructor
                            (
                                mesh_.boundary()[curBPatch].size()
                            )
                        )
                    );
                }

                patchFields[curBPatch].rmap
                (
                    procField.boundaryField()[patchi],
                    patchPointAddressing_[proci][patchi]
                );
            }
        }
    }

    // Construct and write the field
    // setting the internalField and patchFields
    return tmp<GeometricField<Type, pointPatchField, pointMesh> >
    (
        new GeometricField<Type, pointPatchField, pointMesh>
        (
            IOobject
            (
                fieldIoObject.name(),
                mesh_().time().timeName(),
                mesh_(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            procFields[0].dimensions(),
            internalField,
            patchFields
        )
    );
}


// Reconstruct and write all point fields
template<class Type>
void CML::pointFieldReconstructor::reconstructFields
(
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
)
{
    word fieldClassName
    (
        GeometricField<Type, pointPatchField, pointMesh>::typeName
    );

    IOobjectList fields = objects.lookupClass(fieldClassName);

    if (fields.size())
    {
        Info<< "    Reconstructing " << fieldClassName << "s\n" << endl;

        forAllConstIter(IOobjectList, fields, fieldIter)
        {
            if
            (
                !selectedFields.size()
             || selectedFields.found(fieldIter()->name())
            )
            {
                Info<< "        " << fieldIter()->name() << endl;

                reconstructField<Type>(*fieldIter())().write();
            }
        }

        Info<< endl;
    }
}


#endif

// ************************************************************************* //
