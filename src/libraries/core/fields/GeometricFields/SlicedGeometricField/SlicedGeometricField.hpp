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
    CML::SlicedGeometricField

Description
    Specialization of GeometricField which holds slices of given complete
    fields in a form that they act as a GeometricField.

    The destructor is wrapped to avoid deallocation of the storage of the
    complete fields when this is destroyed.

    SlicedGeometricField can only be instantiated with a valid form of
    SlicedPatchField to handle the slicing and storage deallocation of the
    boundary field.


\*---------------------------------------------------------------------------*/

#ifndef SlicedGeometricField_H
#define SlicedGeometricField_H

#include "GeometricField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class SlicedGeometricField Declaration
\*---------------------------------------------------------------------------*/

template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
class SlicedGeometricField
:
    public GeometricField<Type, PatchField, GeoMesh>
{
public:

    typedef typename GeoMesh::Mesh Mesh;
    typedef typename GeoMesh::BoundaryMesh BoundaryMesh;

    class DimensionedInternalField;


private:

    // Private Member Functions

        //- Slice the given field and a create a PtrList of SlicedPatchField
        //  from which the boundary field is built
        tmp<FieldField<PatchField, Type> >  slicedBoundaryField
        (
            const Mesh& mesh,
            const Field<Type>& completeField,
            const bool preserveCouples,
            const bool preserveProcessorOnly = false
        );

        //- Slice the given field and a create a PtrList of SlicedPatchField
        //  from which the boundary field is built
        tmp<FieldField<PatchField, Type> >  slicedBoundaryField
        (
            const Mesh& mesh,
            const FieldField<PatchField, Type>& bField,
            const bool preserveCouples
        );

        ////- Disallow default bitwise copy construct
        //SlicedGeometricField(const SlicedGeometricField&);

        //- Disallow default bitwise assignment
        void operator=(const SlicedGeometricField&);

        //- Disallow standard assignment to GeometricField,
        //  == assignment is allowed.
        void operator=(const GeometricField<Type, PatchField, GeoMesh>&);

        //- Disallow standard assignment to tmp<GeometricField>,
        //  == assignment is allowed.
        void operator=(const tmp<GeometricField<Type, PatchField, GeoMesh> >&);


public:

    // Constructors

        //- Construct from components and field to slice
        SlicedGeometricField
        (
            const IOobject&,
            const Mesh&,
            const dimensionSet&,
            const Field<Type>& completeField,
            const bool preserveCouples=true
        );

        //- Construct from components and separate fields to slice for the
        //  internal field and boundary field
        SlicedGeometricField
        (
            const IOobject&,
            const Mesh&,
            const dimensionSet&,
            const Field<Type>& completeIField,
            const Field<Type>& completeBField,
            const bool preserveCouples=true,
            const bool preserveProcessorOnly = false
        );

        //- Construct from GeometricField. Reuses full internal and
        //  patch fields except on couples (preserveCouples=true).
        SlicedGeometricField
        (
            const IOobject&,
            const GeometricField<Type, PatchField, GeoMesh>&,
            const bool preserveCouples=true
        );

        //- Construct as copy
        SlicedGeometricField
        (
            const SlicedGeometricField
            <
                Type,
                PatchField,
                SlicedPatchField,
                GeoMesh
            >&
        );


    //- Destructor
    ~SlicedGeometricField();


    // Member functions

        //- Correct boundary field
        void correctBoundaryConditions();
};


/*---------------------------------------------------------------------------*\
       Class SlicedGeometricField::DimensionedInternalField Declaration
\*---------------------------------------------------------------------------*/

//- The internalField of a SlicedGeometricField
template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
class SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>::
DimensionedInternalField
:
    public GeometricField<Type, PatchField, GeoMesh>::DimensionedInternalField
{

public:

    // Constructors

        //- Construct from components and field to slice
        DimensionedInternalField
        (
            const IOobject&,
            const Mesh&,
            const dimensionSet&,
            const Field<Type>& iField
        );


    //- Destructor
    ~DimensionedInternalField();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "processorFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
CML::tmp<CML::FieldField<PatchField, Type> >
CML::SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>::
slicedBoundaryField
(
    const Mesh& mesh,
    const Field<Type>& completeField,
    const bool preserveCouples,
    const bool preserveProcessorOnly
)
{
    tmp<FieldField<PatchField, Type> > tbf
    (
        new FieldField<PatchField, Type>(mesh.boundary().size())
    );

    FieldField<PatchField, Type>& bf = tbf();

    forAll(mesh.boundary(), patchi)
    {
        if
        (
            preserveCouples
         && mesh.boundary()[patchi].coupled()
         && (
               !preserveProcessorOnly
            || isA<processorFvPatch>(mesh.boundary()[patchi])
            )
        )
        {
            // For coupled patched construct the correct patch field type
            bf.set
            (
                patchi,
                PatchField<Type>::New
                (
                    mesh.boundary()[patchi].type(),
                    mesh.boundary()[patchi],
                    *this
                )
            );

            // Initialize the values on the coupled patch to those of the slice
            // of the given field.
            // Note: these will usually be over-ridden by the boundary field
            // evaluation e.g. in the case of processor and cyclic patches.
            bf[patchi] = SlicedPatchField<Type>
            (
                mesh.boundary()[patchi],
                DimensionedField<Type, GeoMesh>::null(),
                completeField
            );
        }
        else
        {
            bf.set
            (
                patchi,
                new SlicedPatchField<Type>
                (
                    mesh.boundary()[patchi],
                    DimensionedField<Type, GeoMesh>::null(),
                    completeField
                )
            );
        }
    }

    return tbf;
}


template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
CML::tmp<CML::FieldField<PatchField, Type> >
CML::SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>::
slicedBoundaryField
(
    const Mesh& mesh,
    const FieldField<PatchField, Type>& bField,
    const bool preserveCouples
)
{
    tmp<FieldField<PatchField, Type> > tbf
    (
        new FieldField<PatchField, Type>(mesh.boundary().size())
    );

    FieldField<PatchField, Type>& bf = tbf();

    forAll(mesh.boundary(), patchi)
    {
        if (preserveCouples && mesh.boundary()[patchi].coupled())
        {
            // For coupled patched construct the correct patch field type
            bf.set
            (
                patchi,
                PatchField<Type>::New
                (
                    mesh.boundary()[patchi].type(),
                    mesh.boundary()[patchi],
                    *this
                )
            );

            // Assign field
            bf[patchi] == bField[patchi];
        }
        else
        {
            // Create unallocated copy of patch field
            bf.set
            (
                patchi,
                new SlicedPatchField<Type>
                (
                    mesh.boundary()[patchi],
                    DimensionedField<Type, GeoMesh>::null()
                )
            );
            bf[patchi].UList<Type>::operator=(bField[patchi]);
        }
    }

    return tbf;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
CML::SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>::
DimensionedInternalField::DimensionedInternalField
(
    const IOobject& io,
    const Mesh& mesh,
    const dimensionSet& ds,
    const Field<Type>& iField
)
:
    DimensionedField<Type, GeoMesh>
    (
        io,
        mesh,
        ds,
        Field<Type>()
    )
{
    // Set the internalField to the slice of the complete field
    UList<Type>::operator=
    (
        typename Field<Type>::subField(iField, GeoMesh::size(mesh))
    );
}


template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
CML::SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>::
SlicedGeometricField
(
    const IOobject& io,
    const Mesh& mesh,
    const dimensionSet& ds,
    const Field<Type>& completeField,
    const bool preserveCouples
)
:
    GeometricField<Type, PatchField, GeoMesh>
    (
        io,
        mesh,
        ds,
        Field<Type>(),
        slicedBoundaryField(mesh, completeField, preserveCouples)
    )
{
    // Set the internalField to the slice of the complete field
    UList<Type>::operator=
    (
        typename Field<Type>::subField(completeField, GeoMesh::size(mesh))
    );

    correctBoundaryConditions();
}


template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
CML::SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>::
SlicedGeometricField
(
    const IOobject& io,
    const Mesh& mesh,
    const dimensionSet& ds,
    const Field<Type>& completeIField,
    const Field<Type>& completeBField,
    const bool preserveCouples,
    const bool preserveProcessorOnly
)
:
    GeometricField<Type, PatchField, GeoMesh>
    (
        io,
        mesh,
        ds,
        Field<Type>(),
        slicedBoundaryField
        (
            mesh,
            completeBField,
            preserveCouples,
            preserveProcessorOnly
        )
    )
{
    // Set the internalField to the slice of the complete field
    UList<Type>::operator=
    (
        typename Field<Type>::subField(completeIField, GeoMesh::size(mesh))
    );

    correctBoundaryConditions();
}


template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
CML::SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>::
SlicedGeometricField
(
    const IOobject& io,
    const GeometricField<Type, PatchField, GeoMesh>& gf,
    const bool preserveCouples
)
:
    GeometricField<Type, PatchField, GeoMesh>
    (
        io,
        gf.mesh(),
        gf.dimensions(),
        Field<Type>(),
        slicedBoundaryField(gf.mesh(), gf.boundaryField(), preserveCouples)
    )
{
    // Set the internalField to the supplied internal field
    UList<Type>::operator=(gf.internalField());

    correctBoundaryConditions();
}


template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
CML::SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>::
SlicedGeometricField
(
    const SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>& gf
)
:
    GeometricField<Type, PatchField, GeoMesh>
    (
        gf,
        gf.mesh(),
        gf.dimensions(),
        Field<Type>(),
        slicedBoundaryField(gf.mesh(), gf.boundaryField(), true)
    )
{
    // Set the internalField to the supplied internal field
    UList<Type>::operator=(gf.internalField());
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
CML::SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>::
~SlicedGeometricField()
{
    // Set the internalField storage pointer to nullptr before its destruction
    // to protect the field it a slice of.
    UList<Type>::operator=(UList<Type>(nullptr, 0));
}


template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
CML::SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>::
DimensionedInternalField::~DimensionedInternalField()
{
    // Set the internalField storage pointer to nullptr before its destruction
    // to protect the field it a slice of.
    UList<Type>::operator=(UList<Type>(nullptr, 0));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
void CML::SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>::
correctBoundaryConditions()
{
    GeometricField<Type, PatchField, GeoMesh>::correctBoundaryConditions();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
