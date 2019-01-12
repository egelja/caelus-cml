/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2016 Applied CCM
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

Description
    Efficient cell-centre calculation using face-addressing, face-centres and
    face-areas.

\*---------------------------------------------------------------------------*/

#include "primitiveMesh.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::primitiveMesh::calcCellCentres() const
{
    if (debug)
    {
        Pout<< "primitiveMesh::calcCellCentres() : "
            << "Calculating cell centres"
            << endl;
    }

    // It is an error to attempt to recalculate cellCentres
    // if the pointer is already set
    if (cellCentresPtr_)
    {
        FatalErrorInFunction
            << "Cell centres already calculated"
            << abort(FatalError);
    }

    // Set the accumulated cell centre to zero vector
    cellCentresPtr_ = new vectorField(nCells());
    vectorField& cellCtrs = *cellCentresPtr_;
    
    if (defectCorr_)
    {
        makeCellCentres(faceCentres(), faceAreas(), cellCtrs);
    }
    else
    {
        cellCtrs = cellCentresGeometric();
    }

    if (debug)
    {
        Pout<< "primitiveMesh::calcCellCentres() : "
            << "Finished calculating cell centres"
            << endl;
    }
}


void CML::primitiveMesh::makeCellCentres
(
    const vectorField& fCtrs,
    const vectorField& fAreas,
    vectorField& cellCtrs
) const
{
    // Clear the fields for accumulation
    cellCtrs = vector::zero;

    const labelList& own = faceOwner();
    const labelList& nei = faceNeighbour();

    // Face weighted average
    scalarField sumA(nCells(), 0);
    vectorField sumPA(nCells(), vector::zero);

    forAll(own, facei)
    {
        // Face weighted average
        sumA[own[facei]] += mag(fAreas[facei]);
        sumPA[own[facei]] += fCtrs[facei]*mag(fAreas[facei]);
    }

    forAll(nei, facei)
    {
        // Face weighted average
        sumA[nei[facei]] += mag(fAreas[facei]);
        sumPA[nei[facei]] += fCtrs[facei]*mag(fAreas[facei]);
    }

    forAll(cellCtrs, celli)
    {
        // Face weighted average
        cellCtrs[celli] = sumPA[celli]/sumA[celli];
    }
}


void CML::primitiveMesh::calcCellCentresAndVolsGeometric() const
{
    if (debug)
    {
        Pout<< "primitiveMesh::calcCellCentresAndVolsGeometric() : "
            << "Calculating geometric cell centres and cell volumes"
            << endl;
    }

    // It is an error to attempt to recalculate cellCentres
    // if the pointer is already set
    if (cellCentresGeometricPtr_ || cellVolumesPtr_)
    {
        FatalErrorInFunction
            << "Geometric cell centres or cell volumes already calculated"
            << abort(FatalError);
    }

    // Set the accumulated cell centre to zero vector
    cellCentresGeometricPtr_ = new vectorField(nCells());
    vectorField& cellCtrs = *cellCentresGeometricPtr_;

    // Initialise cell volumes to 0
    cellVolumesPtr_ = new scalarField(nCells());
    scalarField& cellVols = *cellVolumesPtr_;

    // Make centres and volumes
    makeCellCentresAndVolsGeometric(faceCentres(), faceAreas(), cellCtrs, cellVols);

    if (debug)
    {
        Pout<< "primitiveMesh::calcCellCentresAndVolsGeometric() : "
            << "Finished calculating geometric cell centres and cell volumes"
            << endl;
    }
}


void CML::primitiveMesh::makeCellCentresAndVolsGeometric
(
    const vectorField& fCtrs,
    const vectorField& fAreas,
    vectorField& cellCtrs,
    scalarField& cellVols
) const
{
    // Clear the fields for accumulation
    cellCtrs = vector::zero;
    cellVols = 0.0;

    const labelList& own = faceOwner();
    const labelList& nei = faceNeighbour();

    // First estimate the approximate cell centre as the average of
    // face centres

    vectorField cEst(nCells(), vector::zero);
    vectorField cEstAvg(nCells(), vector::zero);
    labelField estCentre(nCells(), 0);
    labelField nCellFaces(nCells(), 0);

    forAll(own, facei)
    {
        cEst[own[facei]] += fCtrs[facei];
        nCellFaces[own[facei]] += 1;
    }

    forAll(nei, facei)
    {
        cEst[nei[facei]] += fCtrs[facei];
        nCellFaces[nei[facei]] += 1;
    }

    forAll(cEst, celli)
    {
        cEst[celli] /= nCellFaces[celli];

        // Simple node averaging
        const labelList& cPts = cellPoints()[celli];
        const label nPoints = cPts.size();

        for (label pi = 0; pi < nPoints; pi++)
        {
            cEstAvg[celli] += points()[cPts[pi]];
        }
        cEstAvg[celli] /= nPoints;
    }

    forAll(own, facei)
    {
        // Calculate 3*face-pyramid volume
        scalar pyr3Vol =
            fAreas[facei] & (fCtrs[facei] - cEst[own[facei]]);

        // Calculate face-pyramid centre
        vector pc = (3.0/4.0)*fCtrs[facei] + (1.0/4.0)*cEst[own[facei]];

        // Accumulate volume-weighted face-pyramid centre
        cellCtrs[own[facei]] += pyr3Vol*pc;

        // Accumulate face-pyramid volume
        cellVols[own[facei]] += pyr3Vol;

        // Flag this cell as needing the centroid to be estimated by simple nodal averaging
        if (mag(fAreas[facei])<areaSwitch_)
        {
            estCentre[own[facei]] = 1; 
        }
    }

    forAll(nei, facei)
    {
        // Calculate 3*face-pyramid volume
        scalar pyr3Vol =
            fAreas[facei] & (cEst[nei[facei]] - fCtrs[facei]);

        // Calculate face-pyramid centre
        vector pc = (3.0/4.0)*fCtrs[facei] + (1.0/4.0)*cEst[nei[facei]];

        // Accumulate volume-weighted face-pyramid centre
        cellCtrs[nei[facei]] += pyr3Vol*pc;

        // Accumulate face-pyramid volume
        cellVols[nei[facei]] += pyr3Vol;

        // Flag this cell as needing the centroid to be estimated by simple nodal averaging
        if (mag(fAreas[facei])<areaSwitch_)
        {
            estCentre[nei[facei]] = 1; 
        }
    }

    forAll(cellCtrs, celli)
    {
        // If cell has bene flagg for centroid estimation
        if(estCentre[celli])
        {
            cellCtrs[celli] = cEstAvg[celli];
        }
        else
        {
            if (mag(cellVols[celli]) > VSMALL)
            {
                cellCtrs[celli] /= cellVols[celli];
            }
            else
            {
                cellCtrs[celli] = cEst[celli];
            }
        }
    }

    cellVols *= (1.0/3.0);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::vectorField& CML::primitiveMesh::cellCentres() const
{
    if (!cellCentresPtr_)
    {
        calcCellCentres();
    }

    return *cellCentresPtr_;
}


const CML::vectorField& CML::primitiveMesh::cellCentresGeometric() const
{
    if (!cellCentresGeometricPtr_)
    {
        calcCellCentresAndVolsGeometric();
    }

    return *cellCentresGeometricPtr_;
}


const CML::scalarField& CML::primitiveMesh::cellVolumes() const
{
    if (!cellVolumesPtr_)
    {
        calcCellCentresAndVolsGeometric();
    }

    return *cellVolumesPtr_;
}


// ************************************************************************* //
