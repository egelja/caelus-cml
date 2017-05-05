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
    CML::meshToMesh

Description
    mesh to mesh interpolation class.

SourceFiles
    meshToMesh.cpp
    calculateMeshToMeshAddressing.cpp
    calculateMeshToMeshWeights.cpp

\*---------------------------------------------------------------------------*/

#ifndef meshtoMesh_H
#define meshtoMesh_H

#include "fvMesh.hpp"
#include "HashTable.hpp"
#include "fvPatchMapper.hpp"
#include "scalarList.hpp"
#include "className.hpp"
#include "volFields.hpp"
#include "interpolationCellPoint.hpp"
#include "SubField.hpp"
#include "mixedFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
class indexedOctree;

class treeDataCell;

/*---------------------------------------------------------------------------*\
                         Class meshToMesh Declaration
\*---------------------------------------------------------------------------*/

class meshToMesh
{
    // Private data

        // mesh references

        const fvMesh& fromMesh_;
        const fvMesh& toMesh_;

        //- fromMesh patch labels
        HashTable<label> fromMeshPatches_;

        //- toMesh patch labels
        HashTable<label> toMeshPatches_;

        //- Patch map
        HashTable<word> patchMap_;

        //- toMesh patch labels which cut the from-mesh
        HashTable<label> cuttingPatches_;

        //- Cell addressing
        labelList cellAddressing_;

        //- Boundary addressing
        labelListList boundaryAddressing_;

        //- Inverse-distance interpolation weights
        mutable scalarListList* inverseDistanceWeightsPtr_;


    // Private Member Functions

        void calcAddressing();

        void cellAddresses
        (
            labelList& cells,
            const pointField& points,
            const fvMesh& fromMesh,
            const List<bool>& boundaryCell,
            const indexedOctree<treeDataCell>& oc
        ) const;

        void calculateInverseDistanceWeights() const;

        const scalarListList& inverseDistanceWeights() const;


    // Private static data members

        //- Direct hit tolerance
        static const scalar directHitTol;


public:

    // Declare name of the class and its debug switch
    ClassName("meshToMesh");


    //- Enumeration specifying required accuracy
    enum order
    {
        MAP,
        INTERPOLATE,
        CELL_POINT_INTERPOLATE
    };


    // Constructors

        //- Construct from the two meshes, the patch name map for the patches
        //  to be interpolated and the names of the toMesh-patches which
        //  cut the fromMesh
        meshToMesh
        (
            const fvMesh& fromMesh,
            const fvMesh& toMesh,
            const HashTable<word>& patchMap,
            const wordList& cuttingPatchNames
        );

        //- Construct from the two meshes assuming there is an exact mapping
        //  between the patches
        meshToMesh
        (
            const fvMesh& fromMesh,
            const fvMesh& toMesh
        );


    //- Destructor
    ~meshToMesh();


    //- Patch-field interpolation class
    class patchFieldInterpolator
    :
        public fvPatchFieldMapper
    {
        const labelList& directAddressing_;

    public:

        // Constructors

            //- Construct given addressing
            patchFieldInterpolator(const labelList& addr)
            :
                directAddressing_(addr)
            {}


        //- Destructor
        virtual ~patchFieldInterpolator()
        {}


        // Member Functions

            label size() const
            {
                return directAddressing_.size();
            }

            bool direct() const
            {
                return true;
            }

            const labelList& directAddressing() const
            {
                return directAddressing_;
            }
    };


    // Member Functions

        // Access

            const fvMesh& fromMesh() const
            {
                return fromMesh_;
            }

            const fvMesh& toMesh() const
            {
                return toMesh_;
            }

            //- From toMesh cells to fromMesh cells
            const labelList& cellAddressing() const
            {
                return cellAddressing_;
            }

        // Interpolation

            //- Map field
            template<class Type>
            void mapField
            (
                Field<Type>&,
                const Field<Type>&,
                const labelList& adr
            ) const;

            //- Interpolate field using inverse-distance weights
            template<class Type>
            void interpolateField
            (
                Field<Type>&,
                const GeometricField<Type, fvPatchField, volMesh>&,
                const labelList& adr,
                const scalarListList& weights
            ) const;

            //- Interpolate field using cell-point interpolation
            template<class Type>
            void interpolateField
            (
                Field<Type>&,
                const GeometricField<Type, fvPatchField, volMesh>&,
                const labelList& adr,
                const vectorField& centres
            ) const;


            //- Interpolate internal volume field
            template<class Type>
            void interpolateInternalField
            (
                Field<Type>&,
                const GeometricField<Type, fvPatchField, volMesh>&,
                order=INTERPOLATE
            ) const;

            template<class Type>
            void interpolateInternalField
            (
                Field<Type>&,
                const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
                order=INTERPOLATE
            ) const;


            //- Interpolate volume field
            template<class Type>
            void interpolate
            (
                GeometricField<Type, fvPatchField, volMesh>&,
                const GeometricField<Type, fvPatchField, volMesh>&,
                order=INTERPOLATE
            ) const;

            template<class Type>
            void interpolate
            (
                GeometricField<Type, fvPatchField, volMesh>&,
                const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
                order=INTERPOLATE
            ) const;


            //- Interpolate volume field
            template<class Type>
            tmp<GeometricField<Type, fvPatchField, volMesh> > interpolate
            (
                const GeometricField<Type, fvPatchField, volMesh>&,
                order=INTERPOLATE
            ) const;

            template<class Type>
            tmp<GeometricField<Type, fvPatchField, volMesh> > interpolate
            (
                const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
                order=INTERPOLATE
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::meshToMesh::mapField
(
    Field<Type>& toF,
    const Field<Type>& fromVf,
    const labelList& adr
) const
{
    // Direct mapping of nearest-cell values

    forAll(toF, celli)
    {
        if (adr[celli] != -1)
        {
            toF[celli] = fromVf[adr[celli]];
        }
    }

    //toF.map(fromVf, adr);
}


template<class Type>
void CML::meshToMesh::interpolateField
(
    Field<Type>& toF,
    const GeometricField<Type, fvPatchField, volMesh>& fromVf,
    const labelList& adr,
    const scalarListList& weights
) const
{
    // Inverse distance weighted interpolation

    // get reference to cellCells
    const labelListList& cc = fromMesh_.cellCells();

    forAll(toF, celli)
    {
        if (adr[celli] != -1)
        {
            const labelList& neighbours = cc[adr[celli]];
            const scalarList& w = weights[celli];

            toF[celli] = fromVf[adr[celli]]*w[0];

            for (label ni = 1; ni < w.size(); ni++)
            {
                toF[celli] += fromVf[neighbours[ni - 1]]*w[ni];
            }
        }
    }
}


template<class Type>
void CML::meshToMesh::interpolateField
(
    Field<Type>& toF,
    const GeometricField<Type, fvPatchField, volMesh>& fromVf,
    const labelList& adr,
    const vectorField& centres
) const
{
    // Cell-Point interpolation
    interpolationCellPoint<Type> interpolator(fromVf);

    forAll(toF, celli)
    {
        if (adr[celli] != -1)
        {
            toF[celli] = interpolator.interpolate
            (
                centres[celli],
                adr[celli]
            );
        }
    }
}


template<class Type>
void CML::meshToMesh::interpolateInternalField
(
    Field<Type>& toF,
    const GeometricField<Type, fvPatchField, volMesh>& fromVf,
    meshToMesh::order ord
) const
{
    if (fromVf.mesh() != fromMesh_)
    {
        FatalErrorIn
        (
            "meshToMesh::interpolateInternalField(Field<Type>& toF, "
            "const GeometricField<Type, fvPatchField, volMesh>& fromVf, "
            "meshToMesh::order ord) const"
        )   << "the argument field does not correspond to the right mesh. "
            << "Field size: " << fromVf.size()
            << " mesh size: " << fromMesh_.nCells()
            << exit(FatalError);
    }

    if (toF.size() != toMesh_.nCells())
    {
        FatalErrorIn
        (
            "meshToMesh::interpolateInternalField(Field<Type>& toF, "
            "const GeometricField<Type, fvPatchField, volMesh>& fromVf, "
            "meshToMesh::order ord) const"
        )   << "the argument field does not correspond to the right mesh. "
            << "Field size: " << toF.size()
            << " mesh size: " << toMesh_.nCells()
            << exit(FatalError);
    }

    switch(ord)
    {
        case MAP:
            mapField(toF, fromVf, cellAddressing_);
        break;

        case INTERPOLATE:
            interpolateField
            (
                toF,
                fromVf,
                cellAddressing_,
                inverseDistanceWeights()
            );
        break;

        case CELL_POINT_INTERPOLATE:
            interpolateField
            (
                toF,
                fromVf,
                cellAddressing_,
                toMesh_.cellCentres()
            );
        break;

        default:
            FatalErrorIn
            (
                "meshToMesh::interpolateInternalField(Field<Type>& toF, "
                "const GeometricField<Type, fvPatchField, volMesh>& fromVf, "
                "meshToMesh::order ord) const"
            )   << "unknown interpolation scheme " << ord
                << exit(FatalError);
    }
}


template<class Type>
void CML::meshToMesh::interpolateInternalField
(
    Field<Type>& toF,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tfromVf,
    meshToMesh::order ord
) const
{
    interpolateInternalField(toF, tfromVf(), ord);
    tfromVf.clear();
}


template<class Type>
void CML::meshToMesh::interpolate
(
    GeometricField<Type, fvPatchField, volMesh>& toVf,
    const GeometricField<Type, fvPatchField, volMesh>& fromVf,
    meshToMesh::order ord
) const
{
    interpolateInternalField(toVf, fromVf, ord);

    forAll(toMesh_.boundaryMesh(), patchi)
    {
        const fvPatch& toPatch = toMesh_.boundary()[patchi];

        if (cuttingPatches_.found(toPatch.name()))
        {
            switch(ord)
            {
                case MAP:
                    mapField
                    (
                        toVf.boundaryField()[patchi],
                        fromVf,
                        boundaryAddressing_[patchi]
                    );
                break;

                case INTERPOLATE:
                    interpolateField
                    (
                        toVf.boundaryField()[patchi],
                        fromVf,
                        boundaryAddressing_[patchi],
                        toPatch.Cf()
                    );
                break;

                case CELL_POINT_INTERPOLATE:
                    interpolateField
                    (
                        toVf.boundaryField()[patchi],
                        fromVf,
                        boundaryAddressing_[patchi],
                        toPatch.Cf()
                    );
                break;

                default:
                    FatalErrorIn
                    (
                        "meshToMesh::interpolate("
                        "GeometricField<Type, fvPatchField, volMesh>& toVf, "
                        "const GeometricField<Type, fvPatchField, volMesh>& "
                        "fromVf, meshToMesh::order ord) const"
                    )   << "unknown interpolation scheme " << ord
                        << exit(FatalError);
            }

            if (isA<mixedFvPatchField<Type> >(toVf.boundaryField()[patchi]))
            {
                refCast<mixedFvPatchField<Type> >
                (
                    toVf.boundaryField()[patchi]
                ).refValue() = toVf.boundaryField()[patchi];
            }
        }
        else if
        (
            patchMap_.found(toPatch.name())
         && fromMeshPatches_.found(patchMap_.find(toPatch.name())())
        )
        {
            /*
            toVf.boundaryField()[patchi].map
            (
                fromVf.boundaryField()
                [
                    fromMeshPatches_.find(patchMap_.find(toPatch.name())())()
                ],
                boundaryAddressing_[patchi]
            );
            */

            mapField
            (
                toVf.boundaryField()[patchi],
                fromVf.boundaryField()
                [
                    fromMeshPatches_.find(patchMap_.find(toPatch.name())())()
                ],
                boundaryAddressing_[patchi]
            );
        }
    }
}


template<class Type>
void CML::meshToMesh::interpolate
(
    GeometricField<Type, fvPatchField, volMesh>& toVf,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tfromVf,
    meshToMesh::order ord
) const
{
    interpolate(toVf, tfromVf(), ord);
    tfromVf.clear();
}


template<class Type>
CML::tmp< CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::meshToMesh::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& fromVf,
    meshToMesh::order ord
) const
{
    // Create and map the internal-field values
    Field<Type> internalField(toMesh_.nCells());
    interpolateInternalField(internalField, fromVf, ord);

    // check whether both meshes have got the same number
    // of boundary patches
    if (fromMesh_.boundary().size() != toMesh_.boundary().size())
    {
        FatalErrorIn
        (
            "meshToMesh::interpolate"
            "(const GeometricField<Type, fvPatchField, volMesh>& fromVf,"
            "meshToMesh::order ord) const"
        )   << "Incompatible meshes: different number of boundaries, "
               "only internal field may be interpolated"
            << exit(FatalError);
    }

    // Create and map the patch field values
    PtrList<fvPatchField<Type> > patchFields
    (
        boundaryAddressing_.size()
    );

    forAll(boundaryAddressing_, patchI)
    {
        patchFields.set
        (
            patchI,
            fvPatchField<Type>::New
            (
                fromVf.boundaryField()[patchI],
                toMesh_.boundary()[patchI],
                DimensionedField<Type, volMesh>::null(),
                patchFieldInterpolator
                (
                    boundaryAddressing_[patchI]
                )
            )
        );
    }


    // Create the complete field from the pieces
    tmp<GeometricField<Type, fvPatchField, volMesh> > ttoF
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "interpolated(" + fromVf.name() + ')',
                toMesh_.time().timeName(),
                toMesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            toMesh_,
            fromVf.dimensions(),
            internalField,
            patchFields
        )
    );

    return ttoF;
}


template<class Type>
CML::tmp< CML::GeometricField<Type, CML::fvPatchField, CML::volMesh> >
CML::meshToMesh::interpolate
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tfromVf,
    meshToMesh::order ord
) const
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > tint =
        interpolate(tfromVf(), ord);
    tfromVf.clear();

    return tint;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
