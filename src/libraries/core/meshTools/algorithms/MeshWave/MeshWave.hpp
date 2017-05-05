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
    CML::MeshWave

Description
    FaceCellWave plus data

SourceFiles
    MeshWave.cpp

\*---------------------------------------------------------------------------*/

#ifndef MeshWave_H
#define MeshWave_H

#include "FaceCellWave.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class MeshWaveName Declaration
\*---------------------------------------------------------------------------*/

TemplateName(MeshWave);


/*---------------------------------------------------------------------------*\
                           Class MeshWave Declaration
\*---------------------------------------------------------------------------*/

template <class Type, class TrackingData = int>
class MeshWave
:
    public MeshWaveName
{
    // Private data

        //- Wall information for all faces
        List<Type> allFaceInfo_;

        //- Wall information for all cells
        List<Type> allCellInfo_;

        //- Wave calculation engine.
        FaceCellWave<Type, TrackingData> calc_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        MeshWave(const MeshWave&);

        //- Disallow default bitwise assignment
        void operator=(const MeshWave&);

      // Private static data

            //- Used as default trackdata value to satisfy default template
            //  argument.
            static int dummyTrackData_;

public:

    // Constructors

        //- Construct from mesh and list of changed faces with the Type
        //  for these faces. Iterates until nothing changes or maxIter reached.
        //  (maxIter can be 0)
        MeshWave
        (
            const polyMesh& mesh,
            const labelList& initialChangedFaces,
            const List<Type>& changedFacesInfo,
            const label maxIter,
            TrackingData& td = dummyTrackData_
        );

        //- Construct from mesh, list of changed faces with the Type
        //  for these faces and initial field.
        //  Iterates until nothing changes or maxIter reached.
        //  (maxIter can be 0)
        MeshWave
        (
            const polyMesh& mesh,
            const labelList& initialChangedFaces,
            const List<Type>& changedFacesInfo,
            const List<Type>& allCellInfo,
            const label maxIter,
            TrackingData& td = dummyTrackData_
        );


    // Member Functions

        //- Get allFaceInfo
        const List<Type>& allFaceInfo() const
        {
            return allFaceInfo_;
        }

        //- Get allCellInfo
        const List<Type>& allCellInfo() const
        {
            return allCellInfo_;
        }

        //- Additional data to be passed into container
        const TrackingData& data() const
        {
            return calc_.data();
        }

        //- Iterate until no changes or maxIter reached. Returns actual
        //  number of iterations.
        label iterate(const label maxIter)
        {
            return calc_.iterate(maxIter);
        }

        //- Get number of unvisited cells, i.e. cells that were not (yet)
        //  reached from walking across mesh. This can happen from
        //  - not enough iterations done
        //  - a disconnected mesh
        //  - a mesh without walls in it
        label getUnsetCells() const
        {
            return calc_.getUnsetCells();
        }

        //- Get number of unvisited faces
        label getUnsetFaces() const
        {
            return calc_.getUnsetFaces();
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "polyMesh.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template <class Type, class TrackingData>
int CML::MeshWave<Type, TrackingData>::dummyTrackData_ = 12345;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Iterate, propagating changedFacesInfo across mesh, until no change (or
// maxIter reached).
template <class Type, class TrackingData>
CML::MeshWave<Type, TrackingData>::MeshWave
(
    const polyMesh& mesh,
    const labelList& changedFaces,
    const List<Type>& changedFacesInfo,
    const label maxIter,
    TrackingData& td
)
:
    allFaceInfo_(mesh.nFaces()),
    allCellInfo_(mesh.nCells()),
    calc_
    (
        mesh,
        changedFaces,
        changedFacesInfo,
        allFaceInfo_,
        allCellInfo_,
        maxIter,
        td
    )
{}


// Iterate, propagating changedFacesInfo across mesh, until no change (or
// maxIter reached). Initial cell values specified.
template <class Type, class TrackingData>
CML::MeshWave<Type, TrackingData>::MeshWave
(
    const polyMesh& mesh,
    const labelList& changedFaces,
    const List<Type>& changedFacesInfo,
    const List<Type>& allCellInfo,
    const label maxIter,
    TrackingData& td
)
:
    allFaceInfo_(mesh.nFaces()),
    allCellInfo_(allCellInfo),
    calc_
    (
        mesh,
        changedFaces,
        changedFacesInfo,
        allFaceInfo_,
        allCellInfo_,
        maxIter,
        td
    )
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
