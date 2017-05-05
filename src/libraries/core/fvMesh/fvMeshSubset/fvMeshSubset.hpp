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

    - if a user supplied patch is used the mapping becomes a problem.
    Do the new faces get the value of the internal face they came from?
    What if e.g. the user supplied patch is a fixedValue 0? So for now
    they get the face of existing patch face 0.

SourceFiles
    fvMeshSubset.cpp

\*---------------------------------------------------------------------------*/

#ifndef fvMeshSubset_H
#define fvMeshSubset_H

#include "fvMesh.hpp"
#include "pointMesh.hpp"
#include "fvPatchFieldMapper.hpp"
#include "pointPatchFieldMapper.hpp"
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

public:

    //- Patch-field subset interpolation class
    class patchFieldSubset
    :
        public fvPatchFieldMapper
    {
        const labelList& directAddressing_;

    public:

        // Constructors

            //- Construct given addressing
            patchFieldSubset(const labelList& directAddressing)
            :
                directAddressing_(directAddressing)
            {}

        //- Destructor
        virtual ~patchFieldSubset()
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

            const labelUList& directAddressing() const
            {
                return directAddressing_;
            }
    };


    //- Patch-field subset interpolation class
    class pointPatchFieldSubset
    :
        public pointPatchFieldMapper
    {
        const labelList& directAddressing_;

    public:

        // Constructors

            //- Construct given addressing
            pointPatchFieldSubset(const labelList& directAddressing)
            :
                directAddressing_(directAddressing)
            {}

        //- Destructor
        virtual ~pointPatchFieldSubset()
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

            const labelUList& directAddressing() const
            {
                return directAddressing_;
            }
    };


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
            //  Handles coupled patches by if nessecary making coupled patch
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

            //- Return reference to subset mesh
            const fvMesh& subMesh() const;

            fvMesh& subMesh();

            //- Return point map
            const labelList& pointMap() const;

            //- Return face map
            const labelList& faceMap() const;

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

            //- Map surface field
            template<class Type>
            static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
            interpolate
            (
                const GeometricField<Type, fvsPatchField, surfaceMesh>&,
                const fvMesh& sMesh,
                const labelList& patchMap,
                const labelList& faceMap
            );

            template<class Type>
            tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
            interpolate
            (
                const GeometricField<Type, fvsPatchField, surfaceMesh>&
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
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "emptyFvsPatchField.hpp"
#include "emptyPointPatchField.hpp"
#include "emptyFvPatchFields.hpp"

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
    // Create and map the internal-field values
    Field<Type> internalField(vf.internalField(), cellMap);

    // Create and map the patch field values
    PtrList<fvPatchField<Type> > patchFields(patchMap.size());

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
                new emptyFvPatchField<Type>
                (
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, volMesh>::null()
                )
            );
        }
        else
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
                    // Mapped from internal face. Do what? Map from element
                    // 0 for now.
                    directAddressing[i] = 0;
                }
            }

            patchFields.set
            (
                patchI,
                fvPatchField<Type>::New
                (
                    vf.boundaryField()[patchMap[patchI]],
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, volMesh>::null(),
                    patchFieldSubset(directAddressing)
                )
            );

            // What to do with exposed internal faces if put into this patch?
        }
    }


    // Create the complete field from the pieces
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
            internalField,
            patchFields
        )
    );

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
    const labelList& faceMap
)
{
    // Create and map the internal-field values
    Field<Type> internalField
    (
        vf.internalField(),
        SubList<label>
        (
            faceMap,
            sMesh.nInternalFaces()
        )
    );

    // Create and map the patch field values
    PtrList<fvsPatchField<Type> > patchFields(patchMap.size());

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
                new emptyFvsPatchField<Type>
                (
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, surfaceMesh>::null()
                )
            );
        }
        else
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
                    // Mapped from internal face. Do what? Map from element
                    // 0 for now.
                    directAddressing[i] = 0;
                }
            }

            patchFields.set
            (
                patchI,
                fvsPatchField<Type>::New
                (
                    vf.boundaryField()[patchMap[patchI]],
                    sMesh.boundary()[patchI],
                    DimensionedField<Type, surfaceMesh>::null(),
                    patchFieldSubset(directAddressing)
                )
            );
        }
    }


    // Map exposed internal faces. Note: Only nessecary if exposed faces added
    // into existing patch but since we don't know that at this point...
    forAll(patchFields, patchI)
    {
        fvsPatchField<Type>& pfld = patchFields[patchI];

        label meshFaceI = pfld.patch().start();

        forAll(pfld, i)
        {
            label oldFaceI = faceMap[meshFaceI++];

            if (oldFaceI < vf.internalField().size())
            {
                pfld[i] = vf.internalField()[oldFaceI];
            }
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
            internalField,
            patchFields
        )
    );

    return tresF;
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > fvMeshSubset::interpolate
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& sf
) const
{
    return interpolate
    (
        sf,
        subMesh(),
        patchMap(),
        faceMap()
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
    // Create and map the internal-field values
    Field<Type> internalField(vf.internalField(), pointMap);

    // Create and map the patch field values
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

            // If mapped from outside patch use point 0 for lack of better.
            labelList directAddressing(subPatch.size(), 0);

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

            patchFields.set
            (
                patchI,
                pointPatchField<Type>::New
                (
                    vf.boundaryField()[patchMap[patchI]],
                    subPatch,
                    DimensionedField<Type, pointMesh>::null(),
                    pointPatchFieldSubset(directAddressing)
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
                vf.time().timeName(),
                sMesh.thisDb(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            sMesh,
            vf.dimensions(),
            internalField,
            patchFields
        )
    );

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


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
