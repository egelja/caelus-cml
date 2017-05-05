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
    CML::patchDataWave

Description
    Takes a set of patches to start MeshWave from.

    Holds after construction distance at cells and distance at patches
    (like patchWave), but also additional transported data.
    It is used, for example, in the y+ calculation.

See Also
   The patchWave class.

SourceFiles
    patchDataWave.cpp

\*---------------------------------------------------------------------------*/

#ifndef patchDataWave_H
#define patchDataWave_H

#include "cellDistFuncs.hpp"
#include "FieldField.hpp"
#include "UPtrList.hpp"
#include "MeshWave.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class polyMesh;
class wallPoint;
//template<class Type, class TrackingData> class MeshWave;

/*---------------------------------------------------------------------------*\
                           Class patchDataWave Declaration
\*---------------------------------------------------------------------------*/

template<class TransferType>
class patchDataWave
:
    public cellDistFuncs
{

private:

    typedef typename TransferType::dataType Type;


    // Private Member Data

        //- Current patch subset (stored as patchIDs)
        labelHashSet patchIDs_;

        //- Reference to initial extra data at patch faces
        const UPtrList<Field<Type> >& initialPatchValuePtrs_;

        //- Do accurate distance calculation for near-wall cells.
        bool correctWalls_;

        //
        // After construction:
        //

        //- Number of cells/faces unset after MeshWave has finished
        label nUnset_;

        //- Distance at cell centres
        scalarField distance_;

        //- Distance at patch faces
        FieldField<Field, scalar> patchDistance_;

        //- Extra data at cell centres
        Field<Type> cellData_;

        //- Extra data at patch faces
        FieldField<Field, Type> patchData_;


    // Private Member Functions

        //- Set initial set of changed faces
        void setChangedFaces
        (
            const labelHashSet& patchIDs,
            labelList&,
            List<TransferType>&
        ) const;

        //- Copy MeshWave values into *this
        label getValues(const MeshWave<TransferType>&);


public:

    // Constructors

        //- Construct from mesh, information on patches to initialize and flag
        //  whether or not to correct wall.
        //  Calculate for all cells. correctWalls : correct wall (face&point)
        //  cells for correct distance, searching neighbours.
        patchDataWave
        (
            const polyMesh& mesh,
            const labelHashSet& patchIDs,
            const UPtrList<Field<Type> >& initialPatchValuePtrs,
            bool correctWalls = true
        );


    //- Destructor
    virtual ~patchDataWave();


    // Member Functions

        //- Correct for mesh geom/topo changes
        virtual void correct();


        const scalarField& distance() const
        {
            return distance_;
        }

        //- Non const access so we can 'transfer' contents for efficiency.
        scalarField& distance()
        {
            return distance_;
        }

        const FieldField<Field, scalar>& patchDistance() const
        {
            return patchDistance_;
        }

        FieldField<Field, scalar>& patchDistance()
        {
            return patchDistance_;
        }

        const Field<Type>& cellData() const
        {
            return cellData_;
        }

        Field<Type>& cellData()
        {
            return cellData_;
        }

        const FieldField<Field, Type>& patchData() const
        {
            return patchData_;
        }

        FieldField<Field, Type>& patchData()
        {
            return patchData_;
        }

        label nUnset() const
        {
            return nUnset_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Set initial set of changed faces (= all wall faces)
template<class TransferType>
void CML::patchDataWave<TransferType>::setChangedFaces
(
    const labelHashSet& patchIDs,
    labelList& changedFaces,
    List<TransferType>& faceDist
) const
{
    const polyMesh& mesh = cellDistFuncs::mesh();

    label nChangedFaces = 0;

    forAll(mesh.boundaryMesh(), patchI)
    {
        if (patchIDs.found(patchI))
        {
            const polyPatch& patch = mesh.boundaryMesh()[patchI];

            const Field<Type>& patchField = initialPatchValuePtrs_[patchI];

            forAll(patch.faceCentres(), patchFaceI)
            {
                label meshFaceI = patch.start() + patchFaceI;

                changedFaces[nChangedFaces] = meshFaceI;

                faceDist[nChangedFaces] =
                    TransferType
                    (
                        patch.faceCentres()[patchFaceI],
                        patchField[patchFaceI],
                        0.0
                    );

                nChangedFaces++;
            }
        }
    }
}


// Copy from MeshWave data into *this (distance) and field_ (transported data)
template<class TransferType>
CML::label CML::patchDataWave<TransferType>::getValues
(
    const MeshWave<TransferType>& waveInfo
)
{
    const polyMesh& mesh = cellDistFuncs::mesh();

    const List<TransferType>& cellInfo = waveInfo.allCellInfo();
    const List<TransferType>& faceInfo = waveInfo.allFaceInfo();

    label nIllegal = 0;

    // Copy cell values
    distance_.setSize(cellInfo.size());

    forAll(cellInfo, cellI)
    {
        const TransferType & wpn = cellInfo[cellI];

        scalar dist = wpn.distSqr();

        if (cellInfo[cellI].valid(waveInfo.data()))
        {
            distance_[cellI] = CML::sqrt(dist);

            cellData_[cellI] = cellInfo[cellI].data();
        }
        else
        {
            // Illegal/unset value. What to do with data?
            // Note: mag for now. Should maybe be member of TransferType?

            distance_[cellI] = mag(dist);

            //cellData_[cellI] = point::max;
            cellData_[cellI] = cellInfo[cellI].data();

            nIllegal++;
        }
    }

    // Copy boundary values
    forAll(patchDistance_, patchI)
    {
        const polyPatch& patch = mesh.boundaryMesh()[patchI];

        // Allocate storage for patchDistance
        scalarField* patchFieldPtr = new scalarField(patch.size());

        patchDistance_.set(patchI, patchFieldPtr);

        scalarField& patchField = *patchFieldPtr;

        // Allocate storage for patchData
        Field<Type>* patchDataFieldPtr = new Field<Type>(patch.size());

        patchData_.set(patchI, patchDataFieldPtr);

        Field<Type>& patchDataField = *patchDataFieldPtr;

        // Copy distance and data
        forAll(patchField, patchFaceI)
        {
            label meshFaceI = patch.start() + patchFaceI;

            scalar dist = faceInfo[meshFaceI].distSqr();

            if (faceInfo[meshFaceI].valid(waveInfo.data()))
            {
                // Adding SMALL to avoid problems with /0 in the turbulence
                // models
                patchField[patchFaceI] = CML::sqrt(dist) + SMALL;

                patchDataField[patchFaceI] = faceInfo[meshFaceI].data();
            }
            else
            {
                // Illegal/unset value. What to do with data?

                patchField[patchFaceI] = mag(dist);

                //patchDataField[patchFaceI] = point::max;
                patchDataField[patchFaceI] = faceInfo[meshFaceI].data();

                nIllegal++;
            }
        }
    }

    return nIllegal;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
template<class TransferType>
CML::patchDataWave<TransferType>::patchDataWave
(
    const polyMesh& mesh,
    const labelHashSet& patchIDs,
    const UPtrList<Field<Type> >& initialPatchValuePtrs,
    const bool correctWalls
)
:
    cellDistFuncs(mesh),
    patchIDs_(patchIDs),
    initialPatchValuePtrs_(initialPatchValuePtrs),
    correctWalls_(correctWalls),
    nUnset_(0),
    distance_(mesh.nCells()),
    patchDistance_(mesh.boundaryMesh().size()),
    cellData_(mesh.nCells()),
    patchData_(mesh.boundaryMesh().size())
{
    patchDataWave<TransferType>::correct();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class TransferType>
CML::patchDataWave<TransferType>::~patchDataWave()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Correct for mesh geom/topo changes
template<class TransferType>
void CML::patchDataWave<TransferType>::correct()
{
    //
    // Set initial changed faces: set TransferType for wall faces
    // to wall centre.
    //

    // Count walls
    label nWalls = sumPatchSize(patchIDs_);

    List<TransferType> faceDist(nWalls);
    labelList changedFaces(nWalls);

    setChangedFaces(patchIDs_, changedFaces, faceDist);

    //
    // Do calculate wall distance by 'growing' from faces.
    //

    MeshWave<TransferType> waveInfo
    (
        mesh(),
        changedFaces,
        faceDist,
        mesh().globalData().nTotalCells()+1 // max iterations
    );


    //
    // Copy distance into return field
    //

    nUnset_ = getValues(waveInfo);

    //
    // Correct wall cells for true distance
    //

    if (correctWalls_)
    {
        Map<label> nearestFace(2 * nWalls);

        // Get distance and indices of nearest face
        correctBoundaryFaceCells
        (
            patchIDs_,
            distance_,
            nearestFace
        );

        correctBoundaryPointCells
        (
            patchIDs_,
            distance_,
            nearestFace
        );

        // Transfer data from nearest face to cell
        const List<TransferType>& faceInfo = waveInfo.allFaceInfo();

        const labelList wallCells(nearestFace.toc());

        forAll(wallCells, wallCellI)
        {
            label cellI = wallCells[wallCellI];

            label faceI = nearestFace[cellI];

            cellData_[cellI] = faceInfo[faceI].data();
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
