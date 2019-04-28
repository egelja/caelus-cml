/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::isoSurface

Description
    A surface formed by the iso value.
    After "Regularised Marching Tetrahedra: improved iso-surface extraction",
    G.M. Treece, R.W. Prager and A.H. Gee.

    Note:
    - does tets without using cell centres/cell values. Not tested.
    - regularisation can create duplicate triangles/non-manifold surfaces.
    Current handling of those is bit ad-hoc for now and not perfect.
    - regularisation does not do boundary points so as to preserve the
      boundary perfectly.
    - uses geometric merge with fraction of bounding box as distance.
    - triangles can be between two cell centres so constant sampling
      does not make sense.
    - on empty patches behaves like zero gradient.
    - does not do 2D correctly, creates non-flat iso surface.
    - on processor boundaries might two overlapping (identical) triangles
      (one from either side)

    The handling on coupled patches is a bit complex. All fields
    (values and coordinates) get rewritten so
    - empty patches get zerogradient (value) and facecentre (coordinates)
    - separated processor patch faces get interpolate (value) and facecentre
      (coordinates). (this is already the default for cyclics)
    - non-separated processor patch faces keep opposite value and cell centre

    Now the triangle generation on non-separated processor patch faces
    can use the neighbouring value. Any separated processor face or cyclic
    face gets handled just like any boundary face.


SourceFiles
    isoSurface.cpp

\*---------------------------------------------------------------------------*/

#ifndef isoSurface_H
#define isoSurface_H

#include "triSurface.hpp"
#include "labelPair.hpp"
#include "pointIndexHit.hpp"
#include "PackedBoolList.hpp"
#include "volFields.hpp"
#include "slicedVolFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;

/*---------------------------------------------------------------------------*\
                       Class isoSurface Declaration
\*---------------------------------------------------------------------------*/

class isoSurface
:
    public triSurface
{
    // Private data

        enum segmentCutType
        {
            NEARFIRST,      // intersection close to e.first()
            NEARSECOND,     //  ,,                   e.second()
            NOTNEAR         // no intersection
        };

        enum cellCutType
        {
            NOTCUT,     // not cut
            SPHERE,     // all edges to cell centre cut
            CUT         // normal cut
        };


        //- Reference to mesh
        const fvMesh& mesh_;

        const scalarField& pVals_;

        //- Input volScalarField with separated coupled patches rewritten
        autoPtr<slicedVolScalarField> cValsPtr_;

        //- Isosurface value
        const scalar iso_;

        //- Regularise?
        const Switch regularise_;

        //- When to merge points
        const scalar mergeDistance_;


        //- Whether face might be cut
        List<cellCutType> faceCutType_;

        //- Whether cell might be cut
        List<cellCutType> cellCutType_;

        //- Estimated number of cut cells
        label nCutCells_;

        //- For every triangle the original cell in mesh
        labelList meshCells_;

        //- For every unmerged triangle point the point in the triSurface
        labelList triPointMergeMap_;


    // Private Member Functions

        // Point synchronisation

            //- Does tensor differ (to within mergeTolerance) from identity
            bool noTransform(const tensor& tt) const;

            //- Is patch a collocated (i.e. non-separated) coupled patch?
            static bool collocatedPatch(const polyPatch&);

            //- Per face whether is collocated
            PackedBoolList collocatedFaces(const coupledPolyPatch&) const;

            //- Synchonise points on all non-separated coupled patches
            void syncUnseparatedPoints
            (
                pointField& collapsedPoint,
                const point& nullValue
            ) const;


        //- Get location of iso value as fraction inbetween s0,s1
        scalar isoFraction
        (
            const scalar s0,
            const scalar s1
        ) const;

        //- Check if any edge of a face is cut
        bool isEdgeOfFaceCut
        (
            const scalarField& pVals,
            const face& f,
            const bool ownLower,
            const bool neiLower
        ) const;

        void getNeighbour
        (
            const labelList& boundaryRegion,
            const volVectorField& meshC,
            const volScalarField& cVals,
            const label cellI,
            const label faceI,
            scalar& nbrValue,
            point& nbrPoint
        ) const;

        //- Set faceCutType,cellCutType.
        void calcCutTypes
        (
            const labelList& boundaryRegion,
            const volVectorField& meshC,
            const volScalarField& cVals,
            const scalarField& pVals
        );

        static labelPair findCommonPoints
        (
            const labelledTri&,
            const labelledTri&
        );

        static point calcCentre(const triSurface&);

        static pointIndexHit collapseSurface
        (
            pointField& localPoints,
            DynamicList<labelledTri, 64>& localTris
        );

        //- Determine per cc whether all near cuts can be snapped to single
        //  point.
        void calcSnappedCc
        (
            const labelList& boundaryRegion,
            const volVectorField& meshC,
            const volScalarField& cVals,
            const scalarField& pVals,
            DynamicList<point>& snappedPoints,
            labelList& snappedCc
        ) const;

        //- Determine per point whether all near cuts can be snapped to single
        //  point.
        void calcSnappedPoint
        (
            const PackedBoolList& isBoundaryPoint,
            const labelList& boundaryRegion,
            const volVectorField& meshC,
            const volScalarField& cVals,
            const scalarField& pVals,
            DynamicList<point>& snappedPoints,
            labelList& snappedPoint
        ) const;


        //- Return input field with coupled (and empty) patch values rewritten
        template<class Type>
        tmp<SlicedGeometricField
        <Type, fvPatchField, slicedFvPatchField, volMesh> >
        adaptPatchFields
        (
            const GeometricField<Type, fvPatchField, volMesh>& fld
        ) const;

        //- Generate single point by interpolation or snapping
        template<class Type>
        Type generatePoint
        (
            const scalar s0,
            const Type& p0,
            const bool hasSnap0,
            const Type& snapP0,

            const scalar s1,
            const Type& p1,
            const bool hasSnap1,
            const Type& snapP1
        ) const;

        template<class Type>
        void generateTriPoints
        (
            const scalar s0,
            const Type& p0,
            const bool hasSnap0,
            const Type& snapP0,

            const scalar s1,
            const Type& p1,
            const bool hasSnap1,
            const Type& snapP1,

            const scalar s2,
            const Type& p2,
            const bool hasSnap2,
            const Type& snapP2,

            const scalar s3,
            const Type& p3,
            const bool hasSnap3,
            const Type& snapP3,

            DynamicList<Type>& points
        ) const;

        template<class Type>
        label generateFaceTriPoints
        (
            const volScalarField& cVals,
            const scalarField& pVals,

            const GeometricField<Type, fvPatchField, volMesh>& cCoords,
            const Field<Type>& pCoords,

            const DynamicList<Type>& snappedPoints,
            const labelList& snappedCc,
            const labelList& snappedPoint,
            const label faceI,

            const scalar neiVal,
            const Type& neiPt,
            const bool hasNeiSnap,
            const Type& neiSnapPt,

            DynamicList<Type>& triPoints,
            DynamicList<label>& triMeshCells
        ) const;

        template<class Type>
        void generateTriPoints
        (
            const volScalarField& cVals,
            const scalarField& pVals,

            const GeometricField<Type, fvPatchField, volMesh>& cCoords,
            const Field<Type>& pCoords,

            const DynamicList<Type>& snappedPoints,
            const labelList& snappedCc,
            const labelList& snappedPoint,

            DynamicList<Type>& triPoints,
            DynamicList<label>& triMeshCells
        ) const;

        triSurface stitchTriPoints
        (
            const bool checkDuplicates,
            const List<point>& triPoints,
            labelList& triPointReverseMap,  // unmerged to merged point
            labelList& triMap               // merged to unmerged triangle
        ) const;

        //- Check single triangle for (topological) validity
        static bool validTri(const triSurface&, const label);

        //- Determine edge-face addressing
        void calcAddressing
        (
            const triSurface& surf,
            List<FixedList<label, 3> >& faceEdges,
            labelList& edgeFace0,
            labelList& edgeFace1,
            Map<labelList>& edgeFacesRest
        ) const;

        //- Determine orientation
        static void walkOrientation
        (
            const triSurface& surf,
            const List<FixedList<label, 3> >& faceEdges,
            const labelList& edgeFace0,
            const labelList& edgeFace1,
            const label seedTriI,
            labelList& flipState
        );

        //- Orient surface
        static void orientSurface
        (
            triSurface&,
            const List<FixedList<label, 3> >& faceEdges,
            const labelList& edgeFace0,
            const labelList& edgeFace1,
            const Map<labelList>& edgeFacesRest
        );

        //- Is triangle (given by 3 edges) not fully connected?
        static bool danglingTriangle
        (
            const FixedList<label, 3>& fEdges,
            const labelList& edgeFace1
        );

        //- Mark all non-fully connected triangles
        static label markDanglingTriangles
        (
            const List<FixedList<label, 3> >& faceEdges,
            const labelList& edgeFace0,
            const labelList& edgeFace1,
            const Map<labelList>& edgeFacesRest,
            boolList& keepTriangles
        );

        static triSurface subsetMesh
        (
            const triSurface& s,
            const labelList& newToOldFaces,
            labelList& oldToNewPoints,
            labelList& newToOldPoints
        );

public:

    //- Runtime type information
    TypeName("isoSurface");


    // Constructors

        //- Construct from cell values and point values. Uses boundaryField
        //  for boundary values. Holds reference to cellIsoVals and
        //  pointIsoVals.
        isoSurface
        (
            const volScalarField& cellIsoVals,
            const scalarField& pointIsoVals,
            const scalar iso,
            const bool regularise,
            const scalar mergeTol = 1e-6    // fraction of bounding box
        );


    // Member Functions

        //- For every face original cell in mesh
        const labelList& meshCells() const
        {
            return meshCells_;
        }

        //- For every unmerged triangle point the point in the triSurface
        const labelList& triPointMergeMap() const
        {
            return triPointMergeMap_;
        }

        //- Interpolates cCoords,pCoords. Uses the references to the original
        //  fields used to create the iso surface.
        template<class Type>
        tmp<Field<Type> > interpolate
        (
            const GeometricField<Type, fvPatchField, volMesh>& cCoords,
            const Field<Type>& pCoords
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "polyMesh.hpp"
#include "syncTools.hpp"
#include "surfaceFields.hpp"
#include "OFstream.hpp"
#include "meshTools.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::SlicedGeometricField
<
    Type,
    CML::fvPatchField,
    CML::slicedFvPatchField,
    CML::volMesh
> >
CML::isoSurface::adaptPatchFields
(
    const GeometricField<Type, fvPatchField, volMesh>& fld
) const
{
    typedef SlicedGeometricField
    <
        Type,
        fvPatchField,
        slicedFvPatchField,
        volMesh
    > FieldType;

    tmp<FieldType> tsliceFld
    (
        new FieldType
        (
            IOobject
            (
                fld.name(),
                fld.instance(),
                fld.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            fld,        // internal field
            true        // preserveCouples
        )
    );
    FieldType& sliceFld = tsliceFld();

    const fvMesh& mesh = fld.mesh();

    const polyBoundaryMesh& patches = mesh.boundaryMesh();

    forAll(patches, patchI)
    {
        const polyPatch& pp = patches[patchI];

        if
        (
            isA<emptyPolyPatch>(pp)
         && pp.size() != sliceFld.boundaryField()[patchI].size()
        )
        {
            // Clear old value. Cannot resize it since is a slice.
            sliceFld.boundaryField().set(patchI, nullptr);

            // Set new value we can change
            sliceFld.boundaryField().set
            (
                patchI,
                new calculatedFvPatchField<Type>
                (
                    mesh.boundary()[patchI],
                    sliceFld
                )
            );

            // Note: cannot use patchInternalField since uses emptyFvPatch::size
            // Do our own internalField instead.
            const labelUList& faceCells =
                mesh.boundary()[patchI].patch().faceCells();

            Field<Type>& pfld = sliceFld.boundaryField()[patchI];
            pfld.setSize(faceCells.size());
            forAll(faceCells, i)
            {
                pfld[i] = sliceFld[faceCells[i]];
            }
        }
        else if (isA<cyclicPolyPatch>(pp))
        {
            // Already has interpolate as value
        }
        else if (isA<processorPolyPatch>(pp))
        {
            fvPatchField<Type>& pfld = const_cast<fvPatchField<Type>&>
            (
                fld.boundaryField()[patchI]
            );

            const scalarField& w = mesh.weights().boundaryField()[patchI];

            tmp<Field<Type> > f =
                w*pfld.patchInternalField()
              + (1.0-w)*pfld.patchNeighbourField();

            PackedBoolList isCollocated
            (
                collocatedFaces(refCast<const processorPolyPatch>(pp))
            );

            forAll(isCollocated, i)
            {
                if (!isCollocated[i])
                {
                    pfld[i] = f()[i];
                }
            }
        }
    }
    return tsliceFld;
}


template<class Type>
Type CML::isoSurface::generatePoint
(
    const scalar s0,
    const Type& p0,
    const bool hasSnap0,
    const Type& snapP0,

    const scalar s1,
    const Type& p1,
    const bool hasSnap1,
    const Type& snapP1
) const
{
    scalar d = s1-s0;

    if (mag(d) > VSMALL)
    {
        scalar s = (iso_-s0)/d;

        if (hasSnap1 && s >= 0.5 && s <= 1)
        {
            return snapP1;
        }
        else if (hasSnap0 && s >= 0.0 && s <= 0.5)
        {
            return snapP0;
        }
        else
        {
            return s*p1 + (1.0-s)*p0;
        }
    }
    else
    {
        scalar s = 0.4999;

        return s*p1 + (1.0-s)*p0;
    }
}


template<class Type>
void CML::isoSurface::generateTriPoints
(
    const scalar s0,
    const Type& p0,
    const bool hasSnap0,
    const Type& snapP0,

    const scalar s1,
    const Type& p1,
    const bool hasSnap1,
    const Type& snapP1,

    const scalar s2,
    const Type& p2,
    const bool hasSnap2,
    const Type& snapP2,

    const scalar s3,
    const Type& p3,
    const bool hasSnap3,
    const Type& snapP3,

    DynamicList<Type>& points
) const
{
    int triIndex = 0;
    if (s0 < iso_)
    {
        triIndex |= 1;
    }
    if (s1 < iso_)
    {
        triIndex |= 2;
    }
    if (s2 < iso_)
    {
        triIndex |= 4;
    }
    if (s3 < iso_)
    {
        triIndex |= 8;
    }

    /* Form the vertices of the triangles for each case */
    switch (triIndex)
    {
        case 0x00:
        case 0x0F:
        break;

        case 0x0E:
        case 0x01:
            points.append
            (
                generatePoint(s0,p0,hasSnap0,snapP0,s1,p1,hasSnap1,snapP1)
            );
            points.append
            (
                generatePoint(s0,p0,hasSnap0,snapP0,s2,p2,hasSnap2,snapP2)
            );
            points.append
            (
                generatePoint(s0,p0,hasSnap0,snapP0,s3,p3,hasSnap3,snapP3)
            );
        break;

        case 0x0D:
        case 0x02:
            points.append
            (
                generatePoint(s1,p1,hasSnap1,snapP1,s0,p0,hasSnap0,snapP0)
            );
            points.append
            (
                generatePoint(s1,p1,hasSnap1,snapP1,s3,p3,hasSnap3,snapP3)
            );
            points.append
            (
                generatePoint(s1,p1,hasSnap1,snapP1,s2,p2,hasSnap2,snapP2)
            );
        break;

        case 0x0C:
        case 0x03:
        {
            Type tp1 =
                generatePoint(s0,p0,hasSnap0,snapP0,s2,p2,hasSnap2,snapP2);
            Type tp2 =
                generatePoint(s1,p1,hasSnap1,snapP1,s3,p3,hasSnap3,snapP3);

            points.append
            (
                generatePoint(s0,p0,hasSnap0,snapP0,s3,p3,hasSnap3,snapP3)
            );
            points.append(tp1);
            points.append(tp2);
            points.append(tp2);
            points.append
            (
                generatePoint(s1,p1,hasSnap1,snapP1,s2,p2,hasSnap2,snapP2)
            );
            points.append(tp1);
        }
        break;

        case 0x0B:
        case 0x04:
        {
            points.append
            (
                generatePoint(s2,p2,hasSnap2,snapP2,s0,p0,hasSnap0,snapP0)
            );
            points.append
            (
                generatePoint(s2,p2,hasSnap2,snapP2,s1,p1,hasSnap1,snapP1)
            );
            points.append
            (
                generatePoint(s2,p2,hasSnap2,snapP2,s3,p3,hasSnap3,snapP3)
            );
        }
        break;

        case 0x0A:
        case 0x05:
        {
            Type tp0 =
                generatePoint(s0,p0,hasSnap0,snapP0,s1,p1,hasSnap1,snapP1);
            Type tp1 =
                generatePoint(s2,p2,hasSnap2,snapP2,s3,p3,hasSnap3,snapP3);

            points.append(tp0);
            points.append(tp1);
            points.append
            (
                generatePoint(s0,p0,hasSnap0,snapP0,s3,p3,hasSnap3,snapP3)
            );
            points.append(tp0);
            points.append
            (
                generatePoint(s1,p1,hasSnap1,snapP1,s2,p2,hasSnap2,snapP2)
            );
            points.append(tp1);
        }
        break;

        case 0x09:
        case 0x06:
        {
            Type tp0 =
                generatePoint(s0,p0,hasSnap0,snapP0,s1,p1,hasSnap1,snapP1);
            Type tp1 =
                generatePoint(s2,p2,hasSnap2,snapP2,s3,p3,hasSnap3,snapP3);

            points.append(tp0);
            points.append
            (
                generatePoint(s1,p1,hasSnap1,snapP1,s3,p3,hasSnap3,snapP3)
            );
            points.append(tp1);
            points.append(tp0);
            points.append
            (
                generatePoint(s0,p0,hasSnap0,snapP0,s2,p2,hasSnap2,snapP2)
            );
            points.append(tp1);
        }
        break;

        case 0x07:
        case 0x08:
            points.append
            (
                generatePoint(s3,p3,hasSnap3,snapP3,s0,p0,hasSnap0,snapP0)
            );
            points.append
            (
                generatePoint(s3,p3,hasSnap3,snapP3,s2,p2,hasSnap2,snapP2)
            );
            points.append
            (
                generatePoint(s3,p3,hasSnap3,snapP3,s1,p1,hasSnap1,snapP1)
            );
        break;
    }
}


template<class Type>
CML::label CML::isoSurface::generateFaceTriPoints
(
    const volScalarField& cVals,
    const scalarField& pVals,

    const GeometricField<Type, fvPatchField, volMesh>& cCoords,
    const Field<Type>& pCoords,

    const DynamicList<Type>& snappedPoints,
    const labelList& snappedCc,
    const labelList& snappedPoint,
    const label faceI,

    const scalar neiVal,
    const Type& neiPt,
    const bool hasNeiSnap,
    const Type& neiSnapPt,

    DynamicList<Type>& triPoints,
    DynamicList<label>& triMeshCells
) const
{
    label own = mesh_.faceOwner()[faceI];

    label oldNPoints = triPoints.size();

    const face& f = mesh_.faces()[faceI];

    forAll(f, fp)
    {
        label pointI = f[fp];
        label nextPointI = f[f.fcIndex(fp)];

        generateTriPoints
        (
            pVals[pointI],
            pCoords[pointI],
            snappedPoint[pointI] != -1,
            (
                snappedPoint[pointI] != -1
              ? snappedPoints[snappedPoint[pointI]]
              : pTraits<Type>::zero
            ),

            pVals[nextPointI],
            pCoords[nextPointI],
            snappedPoint[nextPointI] != -1,
            (
                snappedPoint[nextPointI] != -1
              ? snappedPoints[snappedPoint[nextPointI]]
              : pTraits<Type>::zero
            ),

            cVals[own],
            cCoords[own],
            snappedCc[own] != -1,
            (
                snappedCc[own] != -1
              ? snappedPoints[snappedCc[own]]
              : pTraits<Type>::zero
            ),

            neiVal,
            neiPt,
            hasNeiSnap,
            neiSnapPt,

            triPoints
        );
    }

    // Every three triPoints is a triangle
    label nTris = (triPoints.size()-oldNPoints)/3;
    for (label i = 0; i < nTris; i++)
    {
        triMeshCells.append(own);
    }

    return nTris;
}


template<class Type>
void CML::isoSurface::generateTriPoints
(
    const volScalarField& cVals,
    const scalarField& pVals,

    const GeometricField<Type, fvPatchField, volMesh>& cCoords,
    const Field<Type>& pCoords,

    const DynamicList<Type>& snappedPoints,
    const labelList& snappedCc,
    const labelList& snappedPoint,

    DynamicList<Type>& triPoints,
    DynamicList<label>& triMeshCells
) const
{
    const polyBoundaryMesh& patches = mesh_.boundaryMesh();
    const labelList& own = mesh_.faceOwner();
    const labelList& nei = mesh_.faceNeighbour();

    if
    (
        (cVals.size() != mesh_.nCells())
     || (pVals.size() != mesh_.nPoints())
     || (cCoords.size() != mesh_.nCells())
     || (pCoords.size() != mesh_.nPoints())
     || (snappedCc.size() != mesh_.nCells())
     || (snappedPoint.size() != mesh_.nPoints())
    )
    {
        FatalErrorInFunction
            << "Incorrect size." << endl
            << "mesh: nCells:" << mesh_.nCells()
            << " points:" << mesh_.nPoints() << endl
            << "cVals:" << cVals.size() << endl
            << "cCoords:" << cCoords.size() << endl
            << "snappedCc:" << snappedCc.size() << endl
            << "pVals:" << pVals.size() << endl
            << "pCoords:" << pCoords.size() << endl
            << "snappedPoint:" << snappedPoint.size() << endl
            << abort(FatalError);
    }


    // Generate triangle points

    triPoints.clear();
    triMeshCells.clear();

    for (label faceI = 0; faceI < mesh_.nInternalFaces(); faceI++)
    {
        if (faceCutType_[faceI] != NOTCUT)
        {
            generateFaceTriPoints
            (
                cVals,
                pVals,

                cCoords,
                pCoords,

                snappedPoints,
                snappedCc,
                snappedPoint,
                faceI,

                cVals[nei[faceI]],
                cCoords[nei[faceI]],
                snappedCc[nei[faceI]] != -1,
                (
                    snappedCc[nei[faceI]] != -1
                  ? snappedPoints[snappedCc[nei[faceI]]]
                  : pTraits<Type>::zero
                ),

                triPoints,
                triMeshCells
            );
        }
    }


    // Determine neighbouring snap status
    boolList neiSnapped(mesh_.nFaces()-mesh_.nInternalFaces(), false);
    List<Type> neiSnappedPoint(neiSnapped.size(), pTraits<Type>::zero);
    forAll(patches, patchI)
    {
        const polyPatch& pp = patches[patchI];

        if (pp.coupled())
        {
            label faceI = pp.start();
            forAll(pp, i)
            {
                label bFaceI = faceI-mesh_.nInternalFaces();
                label snappedIndex = snappedCc[own[faceI]];

                if (snappedIndex != -1)
                {
                    neiSnapped[bFaceI] = true;
                    neiSnappedPoint[bFaceI] = snappedPoints[snappedIndex];
                }
                faceI++;
            }
        }
    }
    syncTools::swapBoundaryFaceList(mesh_, neiSnapped);
    syncTools::swapBoundaryFaceList(mesh_, neiSnappedPoint);



    forAll(patches, patchI)
    {
        const polyPatch& pp = patches[patchI];

        if (isA<processorPolyPatch>(pp))
        {
            const processorPolyPatch& cpp =
                refCast<const processorPolyPatch>(pp);

            PackedBoolList isCollocated(collocatedFaces(cpp));

            forAll(isCollocated, i)
            {
                label faceI = pp.start()+i;

                if (faceCutType_[faceI] != NOTCUT)
                {
                    if (isCollocated[i])
                    {
                        generateFaceTriPoints
                        (
                            cVals,
                            pVals,

                            cCoords,
                            pCoords,

                            snappedPoints,
                            snappedCc,
                            snappedPoint,
                            faceI,

                            cVals.boundaryField()[patchI][i],
                            cCoords.boundaryField()[patchI][i],
                            neiSnapped[faceI-mesh_.nInternalFaces()],
                            neiSnappedPoint[faceI-mesh_.nInternalFaces()],

                            triPoints,
                            triMeshCells
                        );
                    }
                    else
                    {
                        generateFaceTriPoints
                        (
                            cVals,
                            pVals,

                            cCoords,
                            pCoords,

                            snappedPoints,
                            snappedCc,
                            snappedPoint,
                            faceI,

                            cVals.boundaryField()[patchI][i],
                            cCoords.boundaryField()[patchI][i],
                            false,
                            pTraits<Type>::zero,

                            triPoints,
                            triMeshCells
                        );
                    }
                }
            }
        }
        else
        {
            label faceI = pp.start();

            forAll(pp, i)
            {
                if (faceCutType_[faceI] != NOTCUT)
                {
                    generateFaceTriPoints
                    (
                        cVals,
                        pVals,

                        cCoords,
                        pCoords,

                        snappedPoints,
                        snappedCc,
                        snappedPoint,
                        faceI,

                        cVals.boundaryField()[patchI][i],
                        cCoords.boundaryField()[patchI][i],
                        false,  // fc not snapped
                        pTraits<Type>::zero,

                        triPoints,
                        triMeshCells
                    );
                }
                faceI++;
            }
        }
    }

    triPoints.shrink();
    triMeshCells.shrink();
}


//template<class Type>
//CML::tmp<CML::Field<Type> >
//CML::isoSurface::sample(const Field<Type>& vField) const
//{
//    return tmp<Field<Type> >(new Field<Type>(vField, meshCells()));
//}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::isoSurface::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& cCoords,
    const Field<Type>& pCoords
) const
{
    // Recalculate boundary values
    tmp<SlicedGeometricField
    <
        Type,
        fvPatchField,
        slicedFvPatchField,
        volMesh
    > > c2(adaptPatchFields(cCoords));


    DynamicList<Type> triPoints(nCutCells_);
    DynamicList<label> triMeshCells(nCutCells_);

    // Dummy snap data
    DynamicList<Type> snappedPoints;
    labelList snappedCc(mesh_.nCells(), -1);
    labelList snappedPoint(mesh_.nPoints(), -1);

    generateTriPoints
    (
        cValsPtr_(),
        pVals_,

        c2(),
        pCoords,

        snappedPoints,
        snappedCc,
        snappedPoint,

        triPoints,
        triMeshCells
    );


    // One value per point
    tmp<Field<Type> > tvalues
    (
        new Field<Type>(points().size(), pTraits<Type>::zero)
    );
    Field<Type>& values = tvalues();
    labelList nValues(values.size(), 0);

    forAll(triPoints, i)
    {
        label mergedPointI = triPointMergeMap_[i];

        if (mergedPointI >= 0)
        {
            values[mergedPointI] += triPoints[i];
            nValues[mergedPointI]++;
        }
    }

    if (debug)
    {
        Pout<< "nValues:" << values.size() << endl;
        label nMult = 0;
        forAll(nValues, i)
        {
            if (nValues[i] == 0)
            {
                FatalErrorInFunction
                    << "point:" << i << " nValues:" << nValues[i]
                    << abort(FatalError);
            }
            else if (nValues[i] > 1)
            {
                nMult++;
            }
        }
        Pout<< "Of which mult:" << nMult << endl;
    }

    forAll(values, i)
    {
        values[i] /= scalar(nValues[i]);
    }

    return tvalues;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
