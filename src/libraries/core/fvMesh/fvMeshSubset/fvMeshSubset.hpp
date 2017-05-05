/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::fvMeshSubset

Description
    Post-processing mesh subset tool.  Given the original mesh and the
    list of selected cells, it creates the mesh consisting only of the
    desired cells, with the mapping list for points, faces, and cells.

    Puts all exposed internal faces into either
    - a user supplied patch
    - a newly created patch "oldInternalFaces"

    - setCellSubset is for small subsets. Uses Maps to minimize memory.
    - setLargeCellSubset is for largish subsets (>10% of mesh).
      Uses labelLists instead.

    - setLargeCellSubset does coupled patch subsetting as well. If it detects
      a face on a coupled patch 'losing' its neighbour it will move the
      face into the oldInternalFaces patch.

    - if a user supplied patch is used it is up to the destination
      patchField to handle exposed internal faces (mapping from face -1).
      If not provided the default is to assign the internalField. All the
      basic patch field types (e.g. fixedValue) will give a warning and
      preferably derived patch field types should be used that know how to
      handle exposed faces (e.g. use uniformFixedValue instead of fixedValue)

SourceFiles
    fvMeshSubset.cpp

\*---------------------------------------------------------------------------*/

#ifndef fvMeshSubset_H
#define fvMeshSubset_H

#include "fvMesh.hpp"
#include "pointMesh.hpp"
#include "GeometricField.hpp"
#include "HashSet.hpp"
#include "surfaceMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class fvMeshSubset Declaration
\*---------------------------------------------------------------------------*/

class fvMeshSubset
{

private:

    // Private data

        //- Mesh to subset from
        const fvMesh& baseMesh_;

        //- Subset mesh pointer
        autoPtr<fvMesh> fvMeshSubsetPtr_;

        //- Point mapping array
        labelList pointMap_;

        //- Face mapping array
        labelList faceMap_;

        //- Cell mapping array
        labelList cellMap_;

        //- Patch mapping array
        labelList patchMap_;

        mutable autoPtr<labelList> faceFlipMapPtr_;


    // Private Member Functions

        //- Check if subset has been performed
        bool checkCellSubset() const;

        //- Mark points in Map
        static void markPoints(const labelList&, Map<label>&);

        //- Mark points (with 0) in labelList
        static void markPoints(const labelList&, labelList&);

        //- Adapt nCellsUsingFace for coupled faces becoming 'uncoupled'.
        void doCoupledPatches
        (
            const bool syncPar,
            labelList& nCellsUsingFace
        ) const;

        //- Subset of subset
        static labelList subset
        (
            const label nElems,
            const labelList& selectedElements,
            const labelList& subsetMap
        );

        //- Create zones for submesh
        void subsetZones();

        //- Disallow default bitwise copy construct
        fvMeshSubset(const fvMeshSubset&);

        //- Disallow default bitwise assignment
        void operator=(const fvMeshSubset&);

public:

    // Constructors

        //- Construct given a mesh to subset
        explicit fvMeshSubset(const fvMesh&);


    // Member Functions

        // Edit

            //- Set the subset. Create "oldInternalFaces" patch for exposed
            //  internal faces (patchID==-1) or use supplied patch.
            //  Does not handle coupled patches correctly if only one side
            //  gets deleted.
            void setCellSubset
            (
                const labelHashSet& globalCellMap,
                const label patchID = -1
            );

            //- Set the subset from all cells with region == currentRegion.
            //  Create "oldInternalFaces" patch for exposed
            //  internal faces (patchID==-1) or use supplied patch.
            //  Handles coupled patches by if necessary making coupled patch
            //  face part of patchID (so uncoupled)
            void setLargeCellSubset
            (
                const labelList& region,
                const label currentRegion,
                const label patchID = -1,
                const bool syncCouples = true
            );

            //- setLargeCellSubset but with labelHashSet.
            void setLargeCellSubset
            (
                const labelHashSet& globalCellMap,
                const label patchID = -1,
                const bool syncPar = true
            );


        // Access

            //- Original mesh
            const fvMesh& baseMesh() const
            {
                return baseMesh_;
            }

            //- Have subMesh?
            bool hasSubMesh() const;

            //- Return reference to subset mesh
            const fvMesh& subMesh() const;

            fvMesh& subMesh();

            //- Return point map
            const labelList& pointMap() const;

            //- Return face map
            const labelList& faceMap() const;

            //- Return face map with sign to encode flipped faces
            const labelList& faceFlipMap() const;

            //- Return cell map
            const labelList& cellMap() const;

            //- Return patch map
            const labelList& patchMap() const;


        // Field mapping

            //- Map volume field
            template<class Type>
            static tmp<GeometricField<Type, fvPatchField, volMesh> >
            interpolate
            (
                const GeometricField<Type, fvPatchField, volMesh>&,
                const fvMesh& sMesh,
                const labelList& patchMap,
                const labelList& cellMap,
                const labelList& faceMap
            );

            template<class Type>
            tmp<GeometricField<Type, fvPatchField, volMesh> >
            interpolate
            (
                const GeometricField<Type, fvPatchField, volMesh>&
            ) const;

            //- Map surface field. Optionally negates value if flipping
            //  a face (from exposing an internal face)
            template<class Type>
            static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
            interpolate
            (
                const GeometricField<Type, fvsPatchField, surfaceMesh>&,
                const fvMesh& sMesh,
                const labelList& patchMap,
                const labelList& cellMap,
                const labelList& faceMap,
                const bool negateIfFlipped = true
            );

            template<class Type>
            tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
            interpolate
            (
                const GeometricField<Type, fvsPatchField, surfaceMesh>&,
                const bool negateIfFlipped = true
            ) const;

            //- Map point field
            template<class Type>
            static tmp<GeometricField<Type, pointPatchField, pointMesh> >
            interpolate
            (
                const GeometricField<Type, pointPatchField, pointMesh>&,
                const pointMesh& sMesh,
                const labelList& patchMap,
                const labelList& pointMap
            );

            template<class Type>
            tmp<GeometricField<Type, pointPatchField, pointMesh> >
            interpolate
            (
                const GeometricField<Type, pointPatchField, pointMesh>&
            ) const;

            //- Map dimensioned fields
            template<class Type>
            static tmp<DimensionedField<Type, volMesh> >
            interpolate
            (
                const DimensionedField<Type, volMesh>&,
                const fvMesh& sMesh,
                const labelList& cellMap
            );

            template<class Type>
            tmp<DimensionedField<Type, volMesh> >
            interpolate
            (
                const DimensionedField<Type, volMesh>&
            ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "emptyFvsPatchField.hpp"
#include "emptyPointPatchField.hpp"
#include "emptyFvPatchFields.hpp"
#include "directFvPatchFieldMapper.hpp"
#include "directPointPatchFieldMapper.hpp"
#include "flipOp.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > fvMeshSubset::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const fvMesh& sMesh,
    const labelList& patchMap,
    const labelList& cellMap,
    const labelList& faceMap
)
{
    // 1. Create the complete field with dummy patch fields
    PtrList<fvPatchField<Type> > patchFields(patchMap.size());

    forAll(patchFields, patchI)
    {
        // Set the first one by hand as it corresponds to the
        // exposed internal faces. Additional interpolation can be put here
        // as necessary.
        if (patchMap[patchI] == -1)
        {
            patchFields.set
            (
                patchI,
                new emptyFvPatchField<Type>
                (
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, volMesh>::null()
                )
            );
        }
        else
        {
            patchFields.set
            (
                patchI,
                fvPatchField<Type>::New
                (
                    calculatedFvPatchField<Type>::typeName,
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, volMesh>::null()
                )
            );
        }
    }

    tmp<GeometricField<Type, fvPatchField, volMesh> > tresF
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            IOobject
            (
                "subset"+vf.name(),
                sMesh.time().timeName(),
                sMesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            sMesh,
            vf.dimensions(),
            Field<Type>(vf.internalField(), cellMap),
            patchFields
        )
    );
    GeometricField<Type, fvPatchField, volMesh>& resF = tresF();


    // 2. Change the fvPatchFields to the correct type using a mapper
    //  constructor (with reference to the now correct internal field)

    typename GeometricField<Type, fvPatchField, volMesh>::
        GeometricBoundaryField& bf = resF.boundaryField();

    forAll(bf, patchI)
    {
        if (patchMap[patchI] != -1)
        {
            // Construct addressing
            const fvPatch& subPatch = sMesh.boundary()[patchI];
            const fvPatch& basePatch = vf.mesh().boundary()[patchMap[patchI]];
            const label baseStart = basePatch.start();
            const label baseSize = basePatch.size();

            labelList directAddressing(subPatch.size());

            forAll(directAddressing, i)
            {
                label baseFaceI = faceMap[subPatch.start()+i];

                if (baseFaceI >= baseStart && baseFaceI < baseStart+baseSize)
                {
                    directAddressing[i] = baseFaceI-baseStart;
                }
                else
                {
                    // Mapped from internal face. Do what? Leave up to
                    // fvPatchField
                    directAddressing[i] = -1;
                }
            }

            bf.set
            (
                patchI,
                fvPatchField<Type>::New
                (
                    vf.boundaryField()[patchMap[patchI]],
                    subPatch,
                    resF.dimensionedInternalField(),
                    directFvPatchFieldMapper(directAddressing)
                )
            );
        }
    }

    return tresF;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > fvMeshSubset::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return interpolate
    (
        vf,
        subMesh(),
        patchMap(),
        cellMap(),
        faceMap()
    );
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > fvMeshSubset::interpolate
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& vf,
    const fvMesh& sMesh,
    const labelList& patchMap,
    const labelList& cellMap,
    const labelList& faceMap,
    const bool negateIfFlipped
)
{
    // 1. Create the complete field with dummy patch fields
    PtrList<fvsPatchField<Type> > patchFields(patchMap.size());

    forAll(patchFields, patchI)
    {
        // Set the first one by hand as it corresponds to the
        // exposed internal faces. Additional interpolation can be put here
        // as necessary.
        if (patchMap[patchI] == -1)
        {
            patchFields.set
            (
                patchI,
                new emptyFvsPatchField<Type>
                (
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, surfaceMesh>::null()
                )
            );
        }
        else
        {
            patchFields.set
            (
                patchI,
                fvsPatchField<Type>::New
                (
                    calculatedFvsPatchField<Type>::typeName,
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, surfaceMesh>::null()
                )
            );
        }
    }

    // Create the complete field from the pieces
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tresF
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "subset"+vf.name(),
                sMesh.time().timeName(),
                sMesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            sMesh,
            vf.dimensions(),
            Field<Type>
            (
                vf.internalField(),
                SubList<label>
                (
                    faceMap,
                    sMesh.nInternalFaces()
                )
            ),
            patchFields
        )
    );
    GeometricField<Type, fvsPatchField, surfaceMesh>& resF = tresF();


    // 2. Change the fvsPatchFields to the correct type using a mapper
    //  constructor (with reference to the now correct internal field)

    typename GeometricField<Type, fvsPatchField, surfaceMesh>::
        GeometricBoundaryField& bf = resF.boundaryField();

    forAll(bf, patchI)
    {
        if (patchMap[patchI] != -1)
        {
            // Construct addressing
            const fvPatch& subPatch = sMesh.boundary()[patchI];
            const fvPatch& basePatch = vf.mesh().boundary()[patchMap[patchI]];
            const label baseStart = basePatch.start();
            const label baseSize = basePatch.size();

            labelList directAddressing(subPatch.size());

            forAll(directAddressing, i)
            {
                label baseFaceI = faceMap[subPatch.start()+i];

                if (baseFaceI >= baseStart && baseFaceI < baseStart+baseSize)
                {
                    directAddressing[i] = baseFaceI-baseStart;
                }
                else
                {
                    // Mapped from internal face. Do what? Leave up to
                    // patchField. This would require also to pass in
                    // original internal field so for now do as postprocessing
                    directAddressing[i] = -1;
                }
            }

            bf.set
            (
                patchI,
                fvsPatchField<Type>::New
                (
                    vf.boundaryField()[patchMap[patchI]],
                    subPatch,
                    resF.dimensionedInternalField(),
                    directFvPatchFieldMapper(directAddressing)
                )
            );


            // Postprocess patch field for exposed faces

            fvsPatchField<Type>& pfld = bf[patchI];
            const labelUList& fc = bf[patchI].patch().faceCells();
            const labelList& own = vf.mesh().faceOwner();

            forAll(pfld, i)
            {
                label baseFaceI = faceMap[subPatch.start()+i];
                if (baseFaceI < vf.internalField().size())
                {
                    Type val = vf.internalField()[baseFaceI];

                    if (cellMap[fc[i]] == own[baseFaceI] || !negateIfFlipped)
                    {
                        pfld[i] = val;
                    }
                    else
                    {
                        pfld[i] = flipOp()(val);
                    }
                }
                else
                {
                    // Exposed face from other patch.
                    // Only possible in case of a coupled boundary
                    label patchI = vf.mesh().boundaryMesh().whichPatch
                    (
                        baseFaceI
                    );
                    const fvPatch& otherPatch = vf.mesh().boundary()[patchI];
                    label patchFaceI = otherPatch.patch().whichFace(baseFaceI);
                    pfld[i] = vf.boundaryField()[patchI][patchFaceI];
                }
            }
        }
    }

    return tresF;
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > fvMeshSubset::interpolate
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& sf,
    const bool negateIfFlipped
) const
{
    return interpolate
    (
        sf,
        subMesh(),
        patchMap(),
        cellMap(),
        faceMap(),
        negateIfFlipped
    );
}


template<class Type>
tmp<GeometricField<Type, pointPatchField, pointMesh> >
fvMeshSubset::interpolate
(
    const GeometricField<Type, pointPatchField, pointMesh>& vf,
    const pointMesh& sMesh,
    const labelList& patchMap,
    const labelList& pointMap
)
{
    // 1. Create the complete field with dummy patch fields
    PtrList<pointPatchField<Type> > patchFields(patchMap.size());

    forAll(patchFields, patchI)
    {
        // Set the first one by hand as it corresponds to the
        // exposed internal faces.  Additional interpolation can be put here
        // as necessary.
        if (patchMap[patchI] == -1)
        {
            patchFields.set
            (
                patchI,
                new emptyPointPatchField<Type>
                (
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, pointMesh>::null()
                )
            );
        }
        else
        {
            patchFields.set
            (
                patchI,
                pointPatchField<Type>::New
                (
                    calculatedPointPatchField<Type>::typeName,
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, pointMesh>::null()
                )
            );
        }
    }

    // Create the complete field from the pieces
    tmp<GeometricField<Type, pointPatchField, pointMesh> > tresF
    (
        new GeometricField<Type, pointPatchField, pointMesh>
        (
            IOobject
            (
                "subset"+vf.name(),
                sMesh.time().timeName(),
                sMesh.thisDb(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            sMesh,
            vf.dimensions(),
            Field<Type>(vf.internalField(), pointMap),
            patchFields
        )
    );
    GeometricField<Type, pointPatchField, pointMesh>& resF = tresF();


    // 2. Change the pointPatchFields to the correct type using a mapper
    //  constructor (with reference to the now correct internal field)

    typename GeometricField<Type, pointPatchField, pointMesh>::
        GeometricBoundaryField& bf = resF.boundaryField();

    forAll(bf, patchI)
    {
        // Set the first one by hand as it corresponds to the
        // exposed internal faces.  Additional interpolation can be put here
        // as necessary.
        if (patchMap[patchI] != -1)
        {
            // Construct addressing
            const pointPatch& basePatch =
                vf.mesh().boundary()[patchMap[patchI]];

            const labelList& meshPoints = basePatch.meshPoints();

            // Make addressing from mesh to patch point
            Map<label> meshPointMap(2*meshPoints.size());
            forAll(meshPoints, localI)
            {
                meshPointMap.insert(meshPoints[localI], localI);
            }

            // Find which subpatch points originate from which patch point
            const pointPatch& subPatch = sMesh.boundary()[patchI];
            const labelList& subMeshPoints = subPatch.meshPoints();

            // If mapped from outside patch leave handling up to patchField
            labelList directAddressing(subPatch.size(), -1);

            forAll(subMeshPoints, localI)
            {
                // Get mesh point on original mesh.
                label meshPointI = pointMap[subMeshPoints[localI]];

                Map<label>::const_iterator iter = meshPointMap.find(meshPointI);

                if (iter != meshPointMap.end())
                {
                    directAddressing[localI] = iter();
                }
            }

            bf.set
            (
                patchI,
                pointPatchField<Type>::New
                (
                    vf.boundaryField()[patchMap[patchI]],
                    subPatch,
                    resF.dimensionedInternalField(),
                    directPointPatchFieldMapper(directAddressing)
                )
            );
        }
    }

    return tresF;
}


template<class Type>
tmp<GeometricField<Type, pointPatchField, pointMesh> > fvMeshSubset::interpolate
(
    const GeometricField<Type, pointPatchField, pointMesh>& sf
) const
{
    return interpolate
    (
        sf,
        pointMesh::New(subMesh()),     // subsetted point mesh
        patchMap(),
        pointMap()
    );
}


template<class Type>
tmp<DimensionedField<Type, volMesh> > fvMeshSubset::interpolate
(
    const DimensionedField<Type, volMesh>& df,
    const fvMesh& sMesh,
    const labelList& cellMap
)
{
    // Create the complete field from the pieces
    tmp<DimensionedField<Type, volMesh> > tresF
    (
        new DimensionedField<Type, volMesh>
        (
            IOobject
            (
                "subset"+df.name(),
                sMesh.time().timeName(),
                sMesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            sMesh,
            df.dimensions(),
            Field<Type>(df, cellMap)
        )
    );

    return tresF;
}


template<class Type>
tmp<DimensionedField<Type, volMesh> > fvMeshSubset::interpolate
(
    const DimensionedField<Type, volMesh>& df
) const
{
    return interpolate(df, subMesh(), cellMap());
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#endif

// ************************************************************************* //
