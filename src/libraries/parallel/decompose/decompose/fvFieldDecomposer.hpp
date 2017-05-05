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
    CML::fvFieldDecomposer

Description
    Finite Volume volume and surface field decomposer.

SourceFiles
    fvFieldDecomposer.cpp

\*---------------------------------------------------------------------------*/

#ifndef fvFieldDecomposer_H
#define fvFieldDecomposer_H

#include "fvMesh.hpp"
#include "fvPatchFieldMapper.hpp"
#include "surfaceFields.hpp"

#include "processorFvPatchField.hpp"
#include "processorFvsPatchField.hpp"
#include "processorCyclicFvPatchField.hpp"
#include "processorCyclicFvsPatchField.hpp"
#include "emptyFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class IOobjectList;

/*---------------------------------------------------------------------------*\
                    Class fvFieldDecomposer Declaration
\*---------------------------------------------------------------------------*/

class fvFieldDecomposer
{
public:

        //- Patch field decomposer class
        class patchFieldDecomposer
        :
            public fvPatchFieldMapper
        {
            // Private data

                labelList directAddressing_;

        public:

            // Constructors

                //- Construct given addressing
                patchFieldDecomposer
                (
                    const labelUList& addressingSlice,
                    const label addressingOffset
                );


            // Member functions

                label size() const
                {
                    return directAddressing_.size();
                }

                bool direct() const
                {
                    return true;
                }

                const labelUList& directAddressing() const
                {
                    return directAddressing_;
                }
        };


        //- Processor patch field decomposer class. Maps either owner or
        //  neighbour data (no interpolate anymore - processorFvPatchField
        //  holds neighbour data)
        class processorVolPatchFieldDecomposer
        :
            public fvPatchFieldMapper
        {
            // Private data

                labelList directAddressing_;

        public:

            //- Construct given addressing
            processorVolPatchFieldDecomposer
            (
                const fvMesh& mesh,
                const labelUList& addressingSlice
            );


            // Member functions

                label size() const
                {
                    return directAddressing_.size();
                }

                bool direct() const
                {
                    return true;
                }

                const labelUList& directAddressing() const
                {
                    return directAddressing_;
                }
        };


        //- Processor patch field decomposer class. Surface field is assumed
        //  to have direction (so manipulates sign when mapping)
        class processorSurfacePatchFieldDecomposer
        :
            public fvPatchFieldMapper
        {
            labelListList addressing_;
            scalarListList weights_;

        public:

            //- Construct given addressing
            processorSurfacePatchFieldDecomposer
            (
                const labelUList& addressingSlice
            );


            // Member functions

                label size() const
                {
                    return addressing_.size();
                }

                bool direct() const
                {
                    return false;
                }

                const labelListList& addressing() const
                {
                    return addressing_;
                }

                const scalarListList& weights() const
                {
                    return weights_;
                }
        };


private:

    // Private data

        //- Reference to complete mesh
        const fvMesh& completeMesh_;

        //- Reference to processor mesh
        const fvMesh& procMesh_;

        //- Reference to face addressing
        const labelList& faceAddressing_;

        //- Reference to cell addressing
        const labelList& cellAddressing_;

        //- Reference to boundary addressing
        const labelList& boundaryAddressing_;

        //- List of patch field decomposers
        List<patchFieldDecomposer*> patchFieldDecomposerPtrs_;

        List<processorVolPatchFieldDecomposer*>
            processorVolPatchFieldDecomposerPtrs_;

        List<processorSurfacePatchFieldDecomposer*>
            processorSurfacePatchFieldDecomposerPtrs_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        fvFieldDecomposer(const fvFieldDecomposer&);

        //- Disallow default bitwise assignment
        void operator=(const fvFieldDecomposer&);


public:

    // Constructors

        //- Construct from components
        fvFieldDecomposer
        (
            const fvMesh& completeMesh,
            const fvMesh& procMesh,
            const labelList& faceAddressing,
            const labelList& cellAddressing,
            const labelList& boundaryAddressing
        );


    //- Destructor
    ~fvFieldDecomposer();


    // Member Functions

        //- Decompose volume field
        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> >
        decomposeField
        (
            const GeometricField<Type, fvPatchField, volMesh>& field,
            const bool allowUnknownPatchFields = false
        ) const;

        //- Decompose surface field
        template<class Type>
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        decomposeField
        (
            const GeometricField<Type, fvsPatchField, surfaceMesh>& field
        ) const;

        template<class GeoField>
        void decomposeFields(const PtrList<GeoField>& fields) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::fvFieldDecomposer::decomposeField
(
    const GeometricField<Type, fvPatchField, volMesh>& field,
    const bool allowUnknownPatchFields
) const
{
    // Create and map the internal field values
    Field<Type> internalField(field.internalField(), cellAddressing_);

    // Create and map the patch field values
    PtrList<fvPatchField<Type> > patchFields(boundaryAddressing_.size());

    forAll(boundaryAddressing_, patchi)
    {
        if (patchFieldDecomposerPtrs_[patchi])
        {
            patchFields.set
            (
                patchi,
                fvPatchField<Type>::New
                (
                    field.boundaryField()[boundaryAddressing_[patchi]],
                    procMesh_.boundary()[patchi],
                    DimensionedField<Type, volMesh>::null(),
                    *patchFieldDecomposerPtrs_[patchi]
                )
            );
        }
        else if (isA<processorCyclicFvPatch>(procMesh_.boundary()[patchi]))
        {
            patchFields.set
            (
                patchi,
                new processorCyclicFvPatchField<Type>
                (
                    procMesh_.boundary()[patchi],
                    DimensionedField<Type, volMesh>::null(),
                    Field<Type>
                    (
                        field.internalField(),
                        *processorVolPatchFieldDecomposerPtrs_[patchi]
                    )
                )
            );
        }
        else if (isA<processorFvPatch>(procMesh_.boundary()[patchi]))
        {
            patchFields.set
            (
                patchi,
                new processorFvPatchField<Type>
                (
                    procMesh_.boundary()[patchi],
                    DimensionedField<Type, volMesh>::null(),
                    Field<Type>
                    (
                        field.internalField(),
                        *processorVolPatchFieldDecomposerPtrs_[patchi]
                    )
                )
            );
        }
        else if (allowUnknownPatchFields)
        {
            patchFields.set
            (
                patchi,
                new emptyFvPatchField<Type>
                (
                    procMesh_.boundary()[patchi],
                    DimensionedField<Type, volMesh>::null()
                )
            );
        }
        else
        {
            FatalErrorIn("fvFieldDecomposer::decomposeField()")
                << "Unknown type." << abort(FatalError);
        }
    }

    // Create the field for the processor
    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                field.name(),
                procMesh_.time().timeName(),
                procMesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            procMesh_,
            field.dimensions(),
            internalField,
            patchFields
        )
    );
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvsPatchField, CML::surfaceMesh> >
CML::fvFieldDecomposer::decomposeField
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& field
) const
{
    labelList mapAddr
    (
        labelList::subList
        (
            faceAddressing_,
            procMesh_.nInternalFaces()
        )
    );
    forAll(mapAddr, i)
    {
        mapAddr[i] -= 1;
    }

    // Create and map the internal field values
    Field<Type> internalField
    (
        field.internalField(),
        mapAddr
    );

    // Problem with addressing when a processor patch picks up both internal
    // faces and faces from cyclic boundaries. This is a bit of a hack, but
    // I cannot find a better solution without making the internal storage
    // mechanism for surfaceFields correspond to the one of faces in polyMesh
    // (i.e. using slices)
    Field<Type> allFaceField(field.mesh().nFaces());

    forAll(field.internalField(), i)
    {
        allFaceField[i] = field.internalField()[i];
    }

    forAll(field.boundaryField(), patchi)
    {
        const Field<Type> & p = field.boundaryField()[patchi];

        const label patchStart = field.mesh().boundaryMesh()[patchi].start();

        forAll(p, i)
        {
            allFaceField[patchStart + i] = p[i];
        }
    }

    // Create and map the patch field values
    PtrList<fvsPatchField<Type> > patchFields(boundaryAddressing_.size());

    forAll(boundaryAddressing_, patchi)
    {
        if (patchFieldDecomposerPtrs_[patchi])
        {
            patchFields.set
            (
                patchi,
                fvsPatchField<Type>::New
                (
                    field.boundaryField()[boundaryAddressing_[patchi]],
                    procMesh_.boundary()[patchi],
                    DimensionedField<Type, surfaceMesh>::null(),
                    *patchFieldDecomposerPtrs_[patchi]
                )
            );
        }
        else if (isA<processorCyclicFvPatch>(procMesh_.boundary()[patchi]))
        {
            patchFields.set
            (
                patchi,
                new processorCyclicFvsPatchField<Type>
                (
                    procMesh_.boundary()[patchi],
                    DimensionedField<Type, surfaceMesh>::null(),
                    Field<Type>
                    (
                        allFaceField,
                        *processorSurfacePatchFieldDecomposerPtrs_[patchi]
                    )
                )
            );
        }
        else if (isA<processorFvPatch>(procMesh_.boundary()[patchi]))
        {
            patchFields.set
            (
                patchi,
                new processorFvsPatchField<Type>
                (
                    procMesh_.boundary()[patchi],
                    DimensionedField<Type, surfaceMesh>::null(),
                    Field<Type>
                    (
                        allFaceField,
                        *processorSurfacePatchFieldDecomposerPtrs_[patchi]
                    )
                )
            );
        }
        else
        {
            FatalErrorIn("fvFieldDecomposer::decomposeField()")
                << "Unknown type." << abort(FatalError);
        }
    }

    // Create the field for the processor
    return tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                field.name(),
                procMesh_.time().timeName(),
                procMesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            procMesh_,
            field.dimensions(),
            internalField,
            patchFields
        )
    );
}


template<class GeoField>
void CML::fvFieldDecomposer::decomposeFields
(
    const PtrList<GeoField>& fields
) const
{
    forAll(fields, fieldI)
    {
        decomposeField(fields[fieldI])().write();
    }
}


#endif

// ************************************************************************* //
