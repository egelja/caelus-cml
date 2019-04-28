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
    CML::meshRefinement

Description
    Helper class which maintains intersections of (changing) mesh with
    (static) surfaces.

    Maintains
    - per face any intersections of the cc-cc segment with any of the surfaces

SourceFiles
    meshRefinement.cpp
    meshRefinementBaffles.cpp
    meshRefinementMerge.cpp
    meshRefinementProblemCells.cpp
    meshRefinementRefine.cpp

\*---------------------------------------------------------------------------*/

#ifndef meshRefinement_H
#define meshRefinement_H

#include "hexRef8.hpp"
#include "mapPolyMesh.hpp"
#include "autoPtr.hpp"
#include "labelPair.hpp"
#include "indirectPrimitivePatch.hpp"
#include "pointFieldsFwd.hpp"
#include "Tuple2.hpp"
#include "pointIndexHit.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Class forward declarations
class fvMesh;
class mapDistributePolyMesh;
class decompositionMethod;
class refinementSurfaces;
class refinementFeatures;
class shellSurfaces;
class removeCells;
class fvMeshDistribute;
class searchableSurface;
class regionSplit;
class globalIndex;
class removePoints;
class localPointRegion;

/*---------------------------------------------------------------------------*\
                           Class meshRefinement Declaration
\*---------------------------------------------------------------------------*/

class meshRefinement
{

public:

    // Public data types

        //- Enumeration for debug dumping
        enum writeFlag
        {
            MESH = 1,
            SCALARLEVELS = 2,
            OBJINTERSECTIONS = 4
        };


        //- Enumeration for how the userdata is to be mapped upon refinement.
        enum mapType
        {
            MASTERONLY = 1, /*!< maintain master only */
            KEEPALL = 2,    /*!< have slaves (upon refinement) from master */
            REMOVE = 4      /*!< set value to -1 any face that was refined */
        };


private:

    // Private data

        //- Reference to mesh
        fvMesh& mesh_;

        //- Tolerance used for sorting coordinates (used in 'less' routine)
        const scalar mergeDistance_;

        //- Overwrite the mesh?
        const bool overwrite_;

        //- Instance of mesh upon construction. Used when in overwrite_ mode.
        const word oldInstance_;

        //- All surface-intersection interaction
        const refinementSurfaces& surfaces_;

        //- All feature-edge interaction
        const refinementFeatures& features_;

        //- All shell-refinement interaction
        const shellSurfaces& shells_;

        //- Refinement engine
        hexRef8 meshCutter_;

        //- Per cc-cc vector the index of the surface hit
        labelIOList surfaceIndex_;

        //- User supplied face based data.
        List<Tuple2<mapType, labelList>> userFaceData_;

        //- Meshed patches - are treated differently. Stored as wordList since
        //  order changes.
        wordList meshedPatches_;


    // Private Member Functions

        //- Add patchfield of given type to all fields on mesh
        template<class GeoField>
        static void addPatchFields(fvMesh&, const word& patchFieldType);

        //- Reorder patchfields of all fields on mesh
        template<class GeoField>
        static void reorderPatchFields(fvMesh&, const labelList& oldToNew);

        //- Find out which faces have changed given cells (old mesh labels)
        //  that were marked for refinement.
        static labelList getChangedFaces
        (
            const mapPolyMesh&,
            const labelList& oldCellsToRefine
        );

        //- Calculate coupled boundary end vector and refinement level
        void calcNeighbourData
        (
            labelList& neiLevel,
            pointField& neiCc
        ) const;

        //- Find any intersection of surface. Store in surfaceIndex_.
        void updateIntersections(const labelList& changedFaces);

        //- Remove cells. Put exposedFaces into exposedPatchIDs.
        autoPtr<mapPolyMesh> doRemoveCells
        (
            const labelList& cellsToRemove,
            const labelList& exposedFaces,
            const labelList& exposedPatchIDs,
            removeCells& cellRemover
        );

        // Get cells which are inside any closed surface. Note that
        // all closed surfaces
        // will have already been oriented to have keepPoint outside.
        labelList getInsideCells(const word&) const;

        // Do all to remove inside cells
        autoPtr<mapPolyMesh> removeInsideCells
        (
            const string& msg,
            const label exposedPatchi
        );


        // Refinement candidate selection

            //- Mark cell for refinement (if not already marked). Return false
            // if refinelimit hit. Keeps running count (in nRefine) of cells
            // marked for refinement
            static bool markForRefine
            (
                const label markValue,
                const label nAllowRefine,
                label& cellValue,
                label& nRefine
            );

            //- Mark every cell with level of feature passing through it
            //  (or -1 if not passed through). Uses tracking.
            void markFeatureCellLevel
            (
                const point& keepPoint,
                labelList& maxFeatureLevel
            ) const;

            //- Calculate list of cells to refine based on intersection of
            //  features.
            label markFeatureRefinement
            (
                const point& keepPoint,
                const label nAllowRefine,

                labelList& refineCell,
                label& nRefine
            ) const;

            //- Mark cells for refinement-shells based refinement.
            label markInternalRefinement
            (
                const label nAllowRefine,
                labelList& refineCell,
                label& nRefine
            ) const;

            //- Collect faces that are intersected and whose neighbours aren't
            //  yet marked  for refinement.
            labelList getRefineCandidateFaces
            (
                const labelList& refineCell
            ) const;

            //- Mark cells for surface intersection based refinement.
            label markSurfaceRefinement
            (
                const label nAllowRefine,
                const labelList& neiLevel,
                const pointField& neiCc,
                labelList& refineCell,
                label& nRefine
            ) const;

            //- Mark cell if local curvature > curvature or
            //  markDifferingRegions = true and intersections with different
            //  regions.
            bool checkCurvature
            (
                const scalar curvature,
                const label nAllowRefine,
                const label surfaceLevel,
                const vector& surfaceNormal,
                const label cellI,
                label& cellMaxLevel,
                vector& cellMaxNormal,
                labelList& refineCell,
                label& nRefine
            ) const;


            //- Mark cells for surface curvature based refinement. Marks if
            //  local curvature > curvature or if on different regions
            //  (markDifferingRegions)
            label markSurfaceCurvatureRefinement
            (
                const scalar curvature,
                const label nAllowRefine,
                const labelList& neiLevel,
                const pointField& neiCc,
                labelList& refineCell,
                label& nRefine
            ) const;

        // Baffle handling

            //- Get faces to repatch. Returns map from face to patch.
            Map<label> getZoneBafflePatches
            (
                const bool allowBoundary,
                const labelList& globalToPatch
            ) const;

            //- Determine patches for baffles
            void getBafflePatches
            (
                const labelList& globalToPatch,
                const labelList& neiLevel,
                const pointField& neiCc,
                labelList& ownPatch,
                labelList& neiPatch
            ) const;

            //- Determine patch for baffle using some heuristic (and not
            //  surface)
            label getBafflePatch
            (
                const labelList& facePatch,
                const label faceI
            ) const;

            //- Repatches external face or creates baffle for internal face
            //  with user specified patches (might be different for both sides).
            //  Returns label of added face.
            label createBaffle
            (
                const label facei,
                const label ownPatch,
                const label neiPatch,
                polyTopoChange& meshMod
            ) const;

        // Problem cell handling

            //- Helper function to mark face as being on 'boundary'. Used by
            //  markFacesOnProblemCells
            void markBoundaryFace
            (
                const label facei,
                boolList& isBoundaryFace,
                boolList& isBoundaryEdge,
                boolList& isBoundaryPoint
            ) const;

            void findNearest
            (
                const labelList& meshFaces,
                List<pointIndexHit>& nearestInfo,
                labelList& nearestSurface,
                labelList& nearestRegion,
                vectorField& nearestNormal
            ) const;

            Map<label> findEdgeConnectedProblemCells
            (
                const scalarField& perpendicularAngle,
                const labelList&
            ) const;

            bool isCollapsedFace
            (
                const pointField&,
                const pointField& neiCc,
                const scalar minFaceArea,
                const scalar maxNonOrtho,
                const label facei
            ) const;

            bool isCollapsedCell
            (
                const pointField&,
                const scalar volFraction,
                const label celli
            ) const;

            //- Returns list with for every internal face -1 or the patch
            //  they should be baffled into. If removeEdgeConnectedCells is set
            //  removes cells based on perpendicularAngle.
            labelList markFacesOnProblemCells
            (
                const dictionary& motionDict,
                const bool removeEdgeConnectedCells,
                const scalarField& perpendicularAngle,
                const labelList& globalToPatch
            ) const;


        // Baffle merging

            //- Extract those baffles (duplicate) faces that are on the edge
            //  of a baffle region. These are candidates for merging.
            List<labelPair> filterDuplicateFaces(const List<labelPair>&) const;


        // Zone handling

            //- Finds zone per cell for cells inside closed named surfaces.
            //  (uses geometric test for insideness)
            //  Adapts namedSurfaceIndex so all faces on boundary of cellZone
            //  have corresponding faceZone.
            void findCellZoneGeometric
            (
                const labelList& closedNamedSurfaces,
                labelList& namedSurfaceIndex,
                const labelList& surfaceToCellZone,
                labelList& cellToZone
            ) const;

            //- Finds zone per cell for cells inside named surfaces that have
            //  an inside point specified.
            void findCellZoneInsideWalk
            (
                const labelList& locationSurfaces,
                const labelList& namedSurfaceIndex,
                const labelList& surfaceToCellZone,
                labelList& cellToZone
            ) const;

            //- Determines cell zone from cell region information.
            bool calcRegionToZone
            (
                const label surfZoneI,
                const label ownRegion,
                const label neiRegion,

                labelList& regionToCellZone
            ) const;

            //- Finds zone per cell. Uses topological walk with all faces
            //  marked in namedSurfaceIndex regarded as blocked.
            void findCellZoneTopo
            (
                const point& keepPoint,
                const labelList& namedSurfaceIndex,
                const labelList& surfaceToCellZone,
                labelList& cellToZone
            ) const;

            void makeConsistentFaceIndex
            (
                const labelList& cellToZone,
                labelList& namedSurfaceIndex
            ) const;


        //- Disallow default bitwise copy construct
        meshRefinement(const meshRefinement&);

        //- Disallow default bitwise assignment
        void operator=(const meshRefinement&);

public:

    //- Runtime type information
    ClassName("meshRefinement");


    // Constructors

        //- Construct from components
        meshRefinement
        (
            fvMesh& mesh,
            const scalar mergeDistance,
            const bool overwrite,
            const refinementSurfaces&,
            const refinementFeatures&,
            const shellSurfaces&
        );


    // Member Functions

        // Access

            //- Reference to mesh
            const fvMesh& mesh() const
            {
                return mesh_;
            }
            fvMesh& mesh()
            {
                return mesh_;
            }

            scalar mergeDistance() const
            {
                return mergeDistance_;
            }

            //- Overwrite the mesh?
            bool overwrite() const
            {
                return overwrite_;
            }

            //- (points)instance of mesh upon construction
            const word& oldInstance() const
            {
                return oldInstance_;
            }

            //- Reference to surface search engines
            const refinementSurfaces& surfaces() const
            {
                return surfaces_;
            }

            //- Reference to feature edge mesh
            const refinementFeatures& features() const
            {
                return features_;
            }

            //- Reference to refinement shells (regions)
            const shellSurfaces& shells() const
            {
                return shells_;
            }

            //- Reference to meshcutting engine
            const hexRef8& meshCutter() const
            {
                return meshCutter_;
            }

            //- Per start-end edge the index of the surface hit
            const labelList& surfaceIndex() const
            {
                return surfaceIndex_;
            }

            labelList& surfaceIndex()
            {
                return surfaceIndex_;
            }

            //- Additional face data that is maintained across
            //  topo changes. Every entry is a list over all faces.
            //  Bit of a hack. Additional flag to say whether to maintain master
            //  only (false) or increase set to account for face-from-face.
            const List<Tuple2<mapType, labelList> >& userFaceData() const
            {
                return userFaceData_;
            }

            List<Tuple2<mapType, labelList> >& userFaceData()
            {
                return userFaceData_;
            }


        // Other

            //- Count number of intersections (local)
            label countHits() const;

            //- Redecompose according to cell count
            //  keepZoneFaces : find all faceZones from zoned surfaces and keep
            //                  owner and neighbour together
            //  keepBaffles   : find all baffles and keep them together
            autoPtr<mapDistributePolyMesh> balance
            (
                const bool keepZoneFaces,
                const bool keepBaffles,
                const scalarField& cellWeights,
                decompositionMethod& decomposer,
                fvMeshDistribute& distributor
            );

            //- Get faces with intersection.
            labelList intersectedFaces() const;

            //- Get points on surfaces with intersection and boundary faces.
            labelList intersectedPoints() const;

            //- Create patch from set of patches
            static autoPtr<indirectPrimitivePatch> makePatch
            (
                const polyMesh&,
                const labelList&
            );

            //- Helper function to make a pointVectorField with correct
            //  bcs for mesh movement:
            //  - adaptPatchIDs         : fixedValue
            //  - processor             : calculated (so free to move)
            //  - cyclic/wedge/symmetry : slip
            //  - other                 : slip
            static tmp<pointVectorField> makeDisplacementField
            (
                const pointMesh& pMesh,
                const labelList& adaptPatchIDs
            );

            //- Helper function: check that face zones are synced
            static void checkCoupledFaceZones(const polyMesh&);


        // Refinement

            //- Calculate list of cells to refine.
            labelList refineCandidates
            (
                const point& keepPoint,
                const scalar curvature,

                const bool featureRefinement,
                const bool internalRefinement,
                const bool surfaceRefinement,
                const bool curvatureRefinement,
                const label maxGlobalCells,
                const label maxLocalCells
            ) const;

            //- Refine some cells
            autoPtr<mapPolyMesh> refine(const labelList& cellsToRefine);

            //- Refine some cells and rebalance
            autoPtr<mapDistributePolyMesh> refineAndBalance
            (
                const string& msg,
                decompositionMethod& decomposer,
                fvMeshDistribute& distributor,
                const labelList& cellsToRefine,
                const scalar maxLoadUnbalance
            );

            //- Balance before refining some cells
            autoPtr<mapDistributePolyMesh> balanceAndRefine
            (
                const string& msg,
                decompositionMethod& decomposer,
                fvMeshDistribute& distributor,
                const labelList& cellsToRefine,
                const scalar maxLoadUnbalance
            );


        // Baffle handling

            //- Split off unreachable areas of mesh.
            void baffleAndSplitMesh
            (
                const bool handleSnapProblems,
                const bool removeEdgeConnectedCells,
                const scalarField& perpendicularAngle,
                const bool mergeFreeStanding,
                const dictionary& motionDict,
                Time& runTime,
                const labelList& globalToPatch,
                const point& keepPoint
            );

            //- Split off (with optional buffer layers) unreachable areas
            //  of mesh. Does not introduce baffles.
            autoPtr<mapPolyMesh> splitMesh
            (
                const label nBufferLayers,
                const labelList& globalToPatch,
                const point& keepPoint
            );

            //- Find boundary points that connect to more than one cell
            //  region and split them.
            autoPtr<mapPolyMesh> dupNonManifoldPoints(const localPointRegion&);

            //- Find boundary points that connect to more than one cell
            //  region and split them.
            autoPtr<mapPolyMesh> dupNonManifoldPoints();

            //- Create baffle for every internal face where ownPatch != -1.
            //  External faces get repatched according to ownPatch (neiPatch
            //  should be -1 for these)
            autoPtr<mapPolyMesh> createBaffles
            (
                const labelList& ownPatch,
                const labelList& neiPatch
            );

            //- Create baffles for faces straddling zoned surfaces. Return
            //  baffles.
            autoPtr<mapPolyMesh> createZoneBaffles
            (
                const labelList& globalToPatch,
                List<labelPair>&
            );

            //- Return a list of coupled face pairs, i.e. faces that
            //  use the same vertices.
            List<labelPair> getDuplicateFaces(const labelList& testFaces) const;

            //- Merge baffles. Gets pairs of faces.
            autoPtr<mapPolyMesh> mergeBaffles(const List<labelPair>&);

            //- Put faces/cells into zones according to surface specification.
            //  Returns null if no zone surfaces present. Region containing
            //  the keepPoint will not be put into a cellZone.
            autoPtr<mapPolyMesh> zonify
            (
                const point& keepPoint,
                const bool allowFreeStandingZoneFaces
            );


        // Other topo changes

            //- Helper:append patch to end of mesh.
            static label appendPatch
            (
                fvMesh&,
                const label insertPatchi,
                const word&,
                const dictionary&
            );

            //- Helper:add patch to mesh. Update all registered fields.
            //  Used by addMeshedPatch to add patches originating from surfaces.
            static label addPatch(fvMesh&, const word& name, const dictionary&);

            //- Add patch originating from meshing. Update meshedPatches_.
            label addMeshedPatch(const word& name, const dictionary&);

            //- Get patchIDs for patches added in addMeshedPatch.
            labelList meshedPatches() const;

            //- Select coupled faces that are not collocated
            void selectSeparatedCoupledFaces(boolList&) const;

            //- Split mesh. Keep part containing point.
            autoPtr<mapPolyMesh> splitMeshRegions(const point& keepPoint);

            //- Update local numbering for mesh redistribution
            void distribute(const mapDistributePolyMesh&);

            //- Update for external change to mesh. changedFaces are in new mesh
            //  face labels.
            void updateMesh
            (
                const mapPolyMesh&,
                const labelList& changedFaces
            );

            //- Helper: reorder list according to map.
            template<class T>
            static void updateList
            (
                const labelList& newToOld,
                const T& nullValue,
                List<T>& elems
            );


            // Restoring : is where other processes delete and reinsert data.

                //- Signal points/face/cells for which to store data
                void storeData
                (
                    const labelList& pointsToStore,
                    const labelList& facesToStore,
                    const labelList& cellsToStore
                );

                //- Update local numbering + undo
                //  Data to restore given as new pointlabel + stored pointlabel
                //  (i.e. what was in pointsToStore)
                void updateMesh
                (
                    const mapPolyMesh&,
                    const labelList& changedFaces,
                    const Map<label>& pointsToRestore,
                    const Map<label>& facesToRestore,
                    const Map<label>& cellsToRestore
                );

            // Merging coplanar faces and edges

                label mergePatchFacesUndo
                (
                    const scalar minCos,
                    const scalar concaveCos,
                    const labelList& patchIDs,
                    const dictionary& motionDict
                );

                autoPtr<mapPolyMesh> doRemovePoints
                (
                    removePoints& pointRemover,
                    const boolList& pointCanBeDeleted
                );

                autoPtr<mapPolyMesh> doRestorePoints
                (
                    removePoints& pointRemover,
                    const labelList& facesToRestore
                );

                labelList collectFaces
                (
                    const labelList& candidateFaces,
                    const labelHashSet& set
                ) const;

                // Pick up faces of cells of faces in set.
                labelList growFaceCellFace
                (
                    const labelHashSet& set
                ) const;

                //- Merge edges, maintain mesh quality. Return global number
                //  of edges merged
                label mergeEdgesUndo
                (
                    const scalar minCos,
                    const dictionary& motionDict
                );


        // Debug/IO

            //- Debugging: check that all faces still obey start()>end()
            void checkData();

            static void testSyncPointList
            (
                const string& msg,
                const polyMesh& mesh,
                const List<scalar>& fld
            );

            static void testSyncPointList
            (
                const string& msg,
                const polyMesh& mesh,
                const List<point>& fld
            );

            //- Compare two lists over all boundary faces
            template<class T>
            void testSyncBoundaryFaceList
            (
                const scalar mergeDistance,
                const string&,
                const UList<T>&,
                const UList<T>&
            ) const;

            //- Print some mesh stats.
            void printMeshInfo(const bool, const string&) const;

            //- Replacement for Time::timeName() : return oldInstance (if
            //  overwrite_)
            word timeName() const;

            //- Set instance of all local IOobjects
            void setInstance(const fileName&);

            //- Write mesh and all data
            bool write() const;

            //- Write refinement level as volScalarFields for postprocessing
            void dumpRefinementLevel() const;

            //- Debug: Write intersection information to OBJ format
            void dumpIntersections(const fileName& prefix) const;

            //- Do any one of above IO functions. flag is combination of
            //  writeFlag values.
            void write(const label flag, const fileName&) const;
};


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Add a T entry
template<class T> void meshRefinement::updateList
(
    const labelList& newToOld,
    const T& nullValue,
    List<T>& elems
)
{
    List<T> newElems(newToOld.size(), nullValue);

    forAll(newElems, i)
    {
        label oldI = newToOld[i];

        if (oldI >= 0)
        {
            newElems[i] = elems[oldI];
        }
    }

    elems.transfer(newElems);
}


// Compare two lists over all boundary faces
template<class T>
void meshRefinement::testSyncBoundaryFaceList
(
    const scalar tol,
    const string& msg,
    const UList<T>& faceData,
    const UList<T>& syncedFaceData
) const
{
    label nBFaces = mesh_.nFaces() - mesh_.nInternalFaces();

    if (faceData.size() != nBFaces || syncedFaceData.size() != nBFaces)
    {
        FatalErrorInFunction
            << "Boundary faces:" << nBFaces
            << " faceData:" << faceData.size()
            << " syncedFaceData:" << syncedFaceData.size()
            << abort(FatalError);
    }

    const polyBoundaryMesh& patches = mesh_.boundaryMesh();

    forAll(patches, patchi)
    {
        const polyPatch& pp = patches[patchi];

        label bFacei = pp.start() - mesh_.nInternalFaces();

        forAll(pp, i)
        {
            const T& data = faceData[bFacei];
            const T& syncData = syncedFaceData[bFacei];

            if (mag(data - syncData) > tol)
            {
                label facei = pp.start()+i;

                FatalErrorInFunction
                    << msg
                    << "patchFace:" << i
                    << " face:" << facei
                    << " fc:" << mesh_.faceCentres()[facei]
                    << " patch:" << pp.name()
                    << " faceData:" << data
                    << " syncedFaceData:" << syncData
                    << " diff:" << mag(data - syncData)
                    << abort(FatalError);
            }

            bFacei++;
        }
    }
}


//template <class T, class Mesh>
template<class GeoField>
void meshRefinement::addPatchFields(fvMesh& mesh, const word& patchFieldType)
{
    HashTable<GeoField*> flds
    (
        mesh.objectRegistry::lookupClass<GeoField>()
    );

    forAllIter(typename HashTable<GeoField*>, flds, iter)
    {
        GeoField& fld = *iter();
        typename GeoField::GeometricBoundaryField& bfld = fld.boundaryField();

        label sz = bfld.size();
        bfld.setSize(sz+1);
        bfld.set
        (
            sz,
            GeoField::PatchFieldType::New
            (
                patchFieldType,
                mesh.boundary()[sz],
                fld.dimensionedInternalField()
            )
        );
    }
}


// Reorder patch field
template<class GeoField>
void meshRefinement::reorderPatchFields(fvMesh& mesh, const labelList& oldToNew)
{
    HashTable<GeoField*> flds
    (
        mesh.objectRegistry::lookupClass<GeoField>()
    );

    forAllIter(typename HashTable<GeoField*>, flds, iter)
    {
        GeoField& fld = *iter();
        typename GeoField::GeometricBoundaryField& bfld = fld.boundaryField();

        bfld.reorder(oldToNew);
    }
}


} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
