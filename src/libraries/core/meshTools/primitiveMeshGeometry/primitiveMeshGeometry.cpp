/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "primitiveMeshGeometry.hpp"
#include "pyramidPointFaceRef.hpp"
#include "unitConversion.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(primitiveMeshGeometry, 0);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::primitiveMeshGeometry::updateFaceCentresAndAreas
(
    const pointField& p,
    const labelList& changedFaces
)
{
    const faceList& fs = mesh_.faces();

    forAll(changedFaces, i)
    {
        label facei = changedFaces[i];

        const labelList& f = fs[facei];
        label nPoints = f.size();

        // If the face is a triangle, do a direct calculation for efficiency
        // and to avoid round-off error-related problems
        if (nPoints == 3)
        {
            faceCentres_[facei] = (1.0/3.0)*(p[f[0]] + p[f[1]] + p[f[2]]);
            faceAreas_[facei] = 0.5*((p[f[1]] - p[f[0]])^(p[f[2]] - p[f[0]]));
        }
        else
        {
            vector sumN = vector::zero;
            scalar sumA = 0.0;
            vector sumAc = vector::zero;

            point fCentre = p[f[0]];
            for (label pi = 1; pi < nPoints; pi++)
            {
                fCentre += p[f[pi]];
            }

            fCentre /= nPoints;

            for (label pi = 0; pi < nPoints; pi++)
            {
                const point& nextPoint = p[f[(pi + 1) % nPoints]];

                vector c = p[f[pi]] + nextPoint + fCentre;
                vector n = (nextPoint - p[f[pi]])^(fCentre - p[f[pi]]);
                scalar a = mag(n);

                sumN += n;
                sumA += a;
                sumAc += a*c;
            }

            faceCentres_[facei] = (1.0/3.0)*sumAc/(sumA + VSMALL);
            faceAreas_[facei] = 0.5*sumN;
        }
    }
}


void CML::primitiveMeshGeometry::updateCellCentresAndVols
(
    const labelList& changedCells,
    const labelList& changedFaces
)
{
    // Clear the fields for accumulation
    UIndirectList<vector>(cellCentres_, changedCells) = vector::zero;
    UIndirectList<scalar>(cellVolumes_, changedCells) = 0.0;

    const labelList& own = mesh_.faceOwner();
    const labelList& nei = mesh_.faceNeighbour();

    // first estimate the approximate cell centre as the average of face centres

    vectorField cEst(mesh_.nCells());
    UIndirectList<vector>(cEst, changedCells) = vector::zero;
    scalarField nCellFaces(mesh_.nCells());
    UIndirectList<scalar>(nCellFaces, changedCells) = 0.0;

    forAll(changedFaces, i)
    {
        label facei = changedFaces[i];
        cEst[own[facei]] += faceCentres_[facei];
        nCellFaces[own[facei]] += 1;

        if (mesh_.isInternalFace(facei))
        {
            cEst[nei[facei]] += faceCentres_[facei];
            nCellFaces[nei[facei]] += 1;
        }
    }

    forAll(changedCells, i)
    {
        label celli = changedCells[i];
        cEst[celli] /= nCellFaces[celli];
    }

    forAll(changedFaces, i)
    {
        label facei = changedFaces[i];

        // Calculate 3*face-pyramid volume
        scalar pyr3Vol = max
        (
            faceAreas_[facei] & (faceCentres_[facei] - cEst[own[facei]]),
            VSMALL
        );

        // Calculate face-pyramid centre
        vector pc = (3.0/4.0)*faceCentres_[facei] + (1.0/4.0)*cEst[own[facei]];

        // Accumulate volume-weighted face-pyramid centre
        cellCentres_[own[facei]] += pyr3Vol*pc;

        // Accumulate face-pyramid volume
        cellVolumes_[own[facei]] += pyr3Vol;

        if (mesh_.isInternalFace(facei))
        {
            // Calculate 3*face-pyramid volume
            scalar pyr3Vol = max
            (
                faceAreas_[facei] & (cEst[nei[facei]] - faceCentres_[facei]),
                VSMALL
            );

            // Calculate face-pyramid centre
            vector pc =
                (3.0/4.0)*faceCentres_[facei]
              + (1.0/4.0)*cEst[nei[facei]];

            // Accumulate volume-weighted face-pyramid centre
            cellCentres_[nei[facei]] += pyr3Vol*pc;

            // Accumulate face-pyramid volume
            cellVolumes_[nei[facei]] += pyr3Vol;
        }
    }

    forAll(changedCells, i)
    {
        label celli = changedCells[i];

        cellCentres_[celli] /= cellVolumes_[celli];
        cellVolumes_[celli] *= (1.0/3.0);
    }
}


CML::labelList CML::primitiveMeshGeometry::affectedCells
(
    const labelList& changedFaces
) const
{
    const labelList& own = mesh_.faceOwner();
    const labelList& nei = mesh_.faceNeighbour();

    labelHashSet affectedCells(2*changedFaces.size());

    forAll(changedFaces, i)
    {
        label facei = changedFaces[i];

        affectedCells.insert(own[facei]);

        if (mesh_.isInternalFace(facei))
        {
            affectedCells.insert(nei[facei]);
        }
    }
    return affectedCells.toc();
}



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::primitiveMeshGeometry::primitiveMeshGeometry
(
    const primitiveMesh& mesh
)
:
    mesh_(mesh)
{
    correct();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Take over properties from mesh
void CML::primitiveMeshGeometry::correct()
{
    faceAreas_ = mesh_.faceAreas();
    faceCentres_ = mesh_.faceCentres();
    cellCentres_ = mesh_.cellCentres();
    cellVolumes_ = mesh_.cellVolumes();
}


//- Recalculate on selected faces
void CML::primitiveMeshGeometry::correct
(
    const pointField& p,
    const labelList& changedFaces
)
{
    // Update face quantities
    updateFaceCentresAndAreas(p, changedFaces);
    // Update cell quantities from face quantities
    updateCellCentresAndVols(affectedCells(changedFaces), changedFaces);
}


bool CML::primitiveMeshGeometry::checkFaceDotProduct
(
    const bool report,
    const scalar orthWarn,
    const primitiveMesh& mesh,
    const vectorField& cellCentres,
    const vectorField& faceAreas,
    const labelList& checkFaces,
    labelHashSet* setPtr
)
{
    // for all internal faces check theat the d dot S product is positive

    const labelList& own = mesh.faceOwner();
    const labelList& nei = mesh.faceNeighbour();

    // Severe nonorthogonality threshold
    const scalar severeNonorthogonalityThreshold = ::cos(degToRad(orthWarn));

    scalar minDDotS = GREAT;

    scalar sumDDotS = 0;

    label severeNonOrth = 0;

    label errorNonOrth = 0;

    forAll(checkFaces, i)
    {
        label facei = checkFaces[i];

        if (mesh.isInternalFace(facei))
        {
            vector d = cellCentres[nei[facei]] - cellCentres[own[facei]];
            const vector& s = faceAreas[facei];

            scalar dDotS = (d & s)/(mag(d)*mag(s) + VSMALL);

            if (dDotS < severeNonorthogonalityThreshold)
            {
                if (dDotS > SMALL)
                {
                    if (report)
                    {
                        // Severe non-orthogonality but mesh still OK
                        Pout<< "Severe non-orthogonality for face " << facei
                            << " between cells " << own[facei]
                            << " and " << nei[facei]
                            << ": Angle = " << radToDeg(::acos(dDotS))
                            << " deg." << endl;
                    }

                    if (setPtr)
                    {
                        setPtr->insert(facei);
                    }

                    severeNonOrth++;
                }
                else
                {
                    // Non-orthogonality greater than 90 deg
                    if (report)
                    {
                        WarningInFunction
                            << "Severe non-orthogonality detected for face "
                            << facei
                            << " between cells " << own[facei] << " and "
                            << nei[facei]
                            << ": Angle = " << radToDeg(::acos(dDotS))
                            << " deg." << endl;
                    }

                    errorNonOrth++;

                    if (setPtr)
                    {
                        setPtr->insert(facei);
                    }
                }
            }

            if (dDotS < minDDotS)
            {
                minDDotS = dDotS;
            }

            sumDDotS += dDotS;
        }
    }

    reduce(minDDotS, minOp<scalar>());
    reduce(sumDDotS, sumOp<scalar>());
    reduce(severeNonOrth, sumOp<label>());
    reduce(errorNonOrth, sumOp<label>());

    label neiSize = nei.size();
    reduce(neiSize, sumOp<label>());

    // Only report if there are some internal faces
    if (neiSize > 0)
    {
        if (report && minDDotS < severeNonorthogonalityThreshold)
        {
            Info<< "Number of non-orthogonality errors: " << errorNonOrth
                << ". Number of severely non-orthogonal faces: "
                << severeNonOrth  << "." << endl;
        }
    }

    if (report)
    {
        if (neiSize > 0)
        {
            Info<< "Mesh non-orthogonality Max: "
                << radToDeg(::acos(minDDotS))
                << " average: " << radToDeg(::acos(sumDDotS/neiSize))
                << endl;
        }
    }

    if (errorNonOrth > 0)
    {
        if (report)
        {
            SeriousErrorInFunction
                << "Error in non-orthogonality detected" << endl;
        }

        return true;
    }
    else
    {
        if (report)
        {
            Info<< "Non-orthogonality check OK.\n" << endl;
        }

        return false;
    }
}


bool CML::primitiveMeshGeometry::checkFacePyramids
(
    const bool report,
    const scalar minPyrVol,
    const primitiveMesh& mesh,
    const vectorField& cellCentres,
    const pointField& p,
    const labelList& checkFaces,
    labelHashSet* setPtr
)
{
    // check whether face area vector points to the cell with higher label
    const labelList& own = mesh.faceOwner();
    const labelList& nei = mesh.faceNeighbour();

    const faceList& f = mesh.faces();

    label nErrorPyrs = 0;

    forAll(checkFaces, i)
    {
        label facei = checkFaces[i];

        // Create the owner pyramid - it will have negative volume
        scalar pyrVol = pyramidPointFaceRef
        (
            f[facei],
            cellCentres[own[facei]]
        ).mag(p);

        if (pyrVol > -minPyrVol)
        {
            if (report)
            {
                Pout<< "bool primitiveMeshGeometry::checkFacePyramids("
                    << "const bool, const scalar, const pointField&"
                    << ", const labelList&, labelHashSet*): "
                    << "face " << facei << " points the wrong way. " << endl
                    << "Pyramid volume: " << -pyrVol
                    << " Face " << f[facei] << " area: " << f[facei].mag(p)
                    << " Owner cell: " << own[facei] << endl
                    << "Owner cell vertex labels: "
                    << mesh.cells()[own[facei]].labels(f)
                    << endl;
            }


            if (setPtr)
            {
                setPtr->insert(facei);
            }

            nErrorPyrs++;
        }

        if (mesh.isInternalFace(facei))
        {
            // Create the neighbour pyramid - it will have positive volume
            scalar pyrVol =
                pyramidPointFaceRef(f[facei], cellCentres[nei[facei]]).mag(p);

            if (pyrVol < minPyrVol)
            {
                if (report)
                {
                    Pout<< "bool primitiveMeshGeometry::checkFacePyramids("
                        << "const bool, const scalar, const pointField&"
                        << ", const labelList&, labelHashSet*): "
                        << "face " << facei << " points the wrong way. " << endl
                        << "Pyramid volume: " << -pyrVol
                        << " Face " << f[facei] << " area: " << f[facei].mag(p)
                        << " Neighbour cell: " << nei[facei] << endl
                        << "Neighbour cell vertex labels: "
                        << mesh.cells()[nei[facei]].labels(f)
                        << endl;
                }

                if (setPtr)
                {
                    setPtr->insert(facei);
                }

                nErrorPyrs++;
            }
        }
    }

    reduce(nErrorPyrs, sumOp<label>());

    if (nErrorPyrs > 0)
    {
        if (report)
        {
            SeriousErrorInFunction
                << "Error in face pyramids: faces pointing the wrong way!"
                << endl;
        }

        return true;
    }
    else
    {
        if (report)
        {
            Info<< "Face pyramids OK.\n" << endl;
        }

        return false;
    }
}


bool CML::primitiveMeshGeometry::checkFaceSkewness
(
    const bool report,
    const scalar internalSkew,
    const scalar boundarySkew,
    const primitiveMesh& mesh,
    const vectorField& cellCentres,
    const vectorField& faceCentres,
    const vectorField& faceAreas,
    const labelList& checkFaces,
    labelHashSet* setPtr
)
{
    // Warn if the skew correction vector is more than skew times
    // larger than the face area vector

    const labelList& own = mesh.faceOwner();
    const labelList& nei = mesh.faceNeighbour();

    scalar maxSkew = 0;

    label nWarnSkew = 0;

    forAll(checkFaces, i)
    {
        label facei = checkFaces[i];

        if (mesh.isInternalFace(facei))
        {
            scalar dOwn = mag(faceCentres[facei] - cellCentres[own[facei]]);
            scalar dNei = mag(faceCentres[facei] - cellCentres[nei[facei]]);

            point faceIntersection =
                cellCentres[own[facei]]*dNei/(dOwn+dNei)
              + cellCentres[nei[facei]]*dOwn/(dOwn+dNei);

            scalar skewness =
                mag(faceCentres[facei] - faceIntersection)
              / (
                    mag(cellCentres[nei[facei]]-cellCentres[own[facei]])
                  + VSMALL
                );

            // Check if the skewness vector is greater than the PN vector.
            // This does not cause trouble but is a good indication of a poor
            // mesh.
            if (skewness > internalSkew)
            {
                if (report)
                {
                    Pout<< "Severe skewness for face " << facei
                        << " skewness = " << skewness << endl;
                }

                if (setPtr)
                {
                    setPtr->insert(facei);
                }

                nWarnSkew++;
            }

            if (skewness > maxSkew)
            {
                maxSkew = skewness;
            }
        }
        else
        {
            // Boundary faces: consider them to have only skewness error.
            // (i.e. treat as if mirror cell on other side)

            vector faceNormal = faceAreas[facei];
            faceNormal /= mag(faceNormal) + VSMALL;

            vector dOwn = faceCentres[facei] - cellCentres[own[facei]];

            vector dWall = faceNormal*(faceNormal & dOwn);

            point faceIntersection = cellCentres[own[facei]] + dWall;

            scalar skewness =
                mag(faceCentres[facei] - faceIntersection)
                /(2*mag(dWall) + VSMALL);

            // Check if the skewness vector is greater than the PN vector.
            // This does not cause trouble but is a good indication of a poor
            // mesh.
            if (skewness > boundarySkew)
            {
                if (report)
                {
                    Pout<< "Severe skewness for boundary face " << facei
                        << " skewness = " << skewness << endl;
                }

                if (setPtr)
                {
                    setPtr->insert(facei);
                }

                nWarnSkew++;
            }

            if (skewness > maxSkew)
            {
                maxSkew = skewness;
            }
        }
    }

    reduce(maxSkew, maxOp<scalar>());
    reduce(nWarnSkew, sumOp<label>());

    if (nWarnSkew > 0)
    {
        if (report)
        {
            WarningInFunction
                << "Large face skewness detected.  Max skewness = "
                << 100*maxSkew
                << " percent.\nThis may impair the quality of the result." << nl
                << nWarnSkew << " highly skew faces detected."
                << endl;
        }

        return true;
    }
    else
    {
        if (report)
        {
            Info<< "Max skewness = " << 100*maxSkew
                << " percent.  Face skewness OK.\n" << endl;
        }

        return false;
    }
}


bool CML::primitiveMeshGeometry::checkFaceWeights
(
    const bool report,
    const scalar warnWeight,
    const primitiveMesh& mesh,
    const vectorField& cellCentres,
    const vectorField& faceCentres,
    const vectorField& faceAreas,
    const labelList& checkFaces,
    labelHashSet* setPtr
)
{
    // Warn if the delta factor (0..1) is too large.

    const labelList& own = mesh.faceOwner();
    const labelList& nei = mesh.faceNeighbour();

    scalar minWeight = GREAT;

    label nWarnWeight = 0;

    forAll(checkFaces, i)
    {
        label facei = checkFaces[i];

        if (mesh.isInternalFace(facei))
        {
            const point& fc = faceCentres[facei];

            scalar dOwn = mag(faceAreas[facei] & (fc-cellCentres[own[facei]]));
            scalar dNei = mag(faceAreas[facei] & (cellCentres[nei[facei]]-fc));

            scalar weight = min(dNei,dOwn)/(dNei+dOwn);

            if (weight < warnWeight)
            {
                if (report)
                {
                    Pout<< "Small weighting factor for face " << facei
                        << " weight = " << weight << endl;
                }

                if (setPtr)
                {
                    setPtr->insert(facei);
                }

                nWarnWeight++;
            }

            minWeight = min(minWeight, weight);
        }
    }

    reduce(minWeight, minOp<scalar>());
    reduce(nWarnWeight, sumOp<label>());

    if (minWeight < warnWeight)
    {
        if (report)
        {
            WarningInFunction
                << "Small interpolation weight detected.  Min weight = "
                << minWeight << '.' << nl
                << nWarnWeight << " faces with small weights detected."
                << endl;
        }

        return true;
    }
    else
    {
        if (report)
        {
            Info<< "Min weight = " << minWeight
                << " percent.  Weights OK.\n" << endl;
        }

        return false;
    }
}


// Check convexity of angles in a face. Allow a slight non-convexity.
// E.g. maxDeg = 10 allows for angles < 190 (or 10 degrees concavity)
// (if truly concave and points not visible from face centre the face-pyramid
//  check in checkMesh will fail)
bool CML::primitiveMeshGeometry::checkFaceAngles
(
    const bool report,
    const scalar maxDeg,
    const primitiveMesh& mesh,
    const vectorField& faceAreas,
    const pointField& p,
    const labelList& checkFaces,
    labelHashSet* setPtr
)
{
    if (maxDeg < -SMALL || maxDeg > 180+SMALL)
    {
        FatalErrorInFunction
            << "maxDeg should be [0..180] but is now " << maxDeg
            << abort(FatalError);
    }

    const scalar maxSin = CML::sin(degToRad(maxDeg));

    const faceList& fcs = mesh.faces();

    scalar maxEdgeSin = 0.0;

    label nConcave = 0;

    label errorFacei = -1;

    forAll(checkFaces, i)
    {
        label facei = checkFaces[i];

        const face& f = fcs[facei];

        vector faceNormal = faceAreas[facei];
        faceNormal /= mag(faceNormal) + VSMALL;

        // Get edge from f[0] to f[size-1];
        vector ePrev(p[f.first()] - p[f.last()]);
        scalar magEPrev = mag(ePrev);
        ePrev /= magEPrev + VSMALL;

        forAll(f, fp0)
        {
            // Get vertex after fp
            label fp1 = f.fcIndex(fp0);

            // Normalized vector between two consecutive points
            vector e10(p[f[fp1]] - p[f[fp0]]);
            scalar magE10 = mag(e10);
            e10 /= magE10 + VSMALL;

            if (magEPrev > SMALL && magE10 > SMALL)
            {
                vector edgeNormal = ePrev ^ e10;
                scalar magEdgeNormal = mag(edgeNormal);

                if (magEdgeNormal < maxSin)
                {
                    // Edges (almost) aligned -> face is ok.
                }
                else
                {
                    // Check normal
                    edgeNormal /= magEdgeNormal;

                    if ((edgeNormal & faceNormal) < SMALL)
                    {
                        if (facei != errorFacei)
                        {
                            // Count only one error per face.
                            errorFacei = facei;
                            nConcave++;
                        }

                        if (setPtr)
                        {
                            setPtr->insert(facei);
                        }

                        maxEdgeSin = max(maxEdgeSin, magEdgeNormal);
                    }
                }
            }

            ePrev = e10;
            magEPrev = magE10;
        }
    }

    reduce(nConcave, sumOp<label>());
    reduce(maxEdgeSin, maxOp<scalar>());

    if (report)
    {
        if (maxEdgeSin > SMALL)
        {
            scalar maxConcaveDegr =
                radToDeg(CML::asin(CML::min(1.0, maxEdgeSin)));

            Info<< "There are " << nConcave
                << " faces with concave angles between consecutive"
                << " edges. Max concave angle = " << maxConcaveDegr
                << " degrees.\n" << endl;
        }
        else
        {
            Info<< "All angles in faces are convex or less than "  << maxDeg
                << " degrees concave.\n" << endl;
        }
    }

    if (nConcave > 0)
    {
        if (report)
        {
            WarningInFunction
                << nConcave  << " face points with severe concave angle (> "
                << maxDeg << " deg) found.\n"
                << endl;
        }

        return true;
    }
    else
    {
        return false;
    }
}


//// Check warpage of faces. Is calculated as the difference between areas of
//// individual triangles and the overall area of the face (which ifself is
//// is the average of the areas of the individual triangles).
//bool CML::primitiveMeshGeometry::checkFaceFlatness
//(
//    const bool report,
//    const scalar warnFlatness,
//    const primitiveMesh& mesh,
//    const vectorField& faceAreas,
//    const vectorField& faceCentres,
//    const pointField& p,
//    const labelList& checkFaces,
//    labelHashSet* setPtr
//)
//{
//    if (warnFlatness < 0 || warnFlatness > 1)
//    {
//        FatalErrorInFunction
//            << "warnFlatness should be [0..1] but is now " << warnFlatness
//            << abort(FatalError);
//    }
//
//
//    const faceList& fcs = mesh.faces();
//
//    // Areas are calculated as the sum of areas. (see
//    // primitiveMeshFaceCentresAndAreas.C)
//
//    label nWarped = 0;
//
//    scalar minFlatness = GREAT;
//    scalar sumFlatness = 0;
//    label nSummed = 0;
//
//    forAll(checkFaces, i)
//    {
//        label facei = checkFaces[i];
//
//        const face& f = fcs[facei];
//
//        scalar magArea = mag(faceAreas[facei]);
//
//        if (f.size() > 3 && magArea > VSMALL)
//        {
//            const point& fc = faceCentres[facei];
//
//            // Calculate the sum of magnitude of areas and compare to
//            // magnitude of sum of areas.
//
//            scalar sumA = 0.0;
//
//            forAll(f, fp)
//            {
//                const point& thisPoint = p[f[fp]];
//                const point& nextPoint = p[f.nextLabel(fp)];
//
//                // Triangle around fc.
//                vector n = 0.5*((nextPoint - thisPoint)^(fc - thisPoint));
//                sumA += mag(n);
//            }
//
//            scalar flatness = magArea / (sumA+VSMALL);
//
//            sumFlatness += flatness;
//            nSummed++;
//
//            minFlatness = min(minFlatness, flatness);
//
//            if (flatness < warnFlatness)
//            {
//                nWarped++;
//
//                if (setPtr)
//                {
//                    setPtr->insert(facei);
//                }
//            }
//        }
//    }
//
//
//    reduce(nWarped, sumOp<label>());
//    reduce(minFlatness, minOp<scalar>());
//
//    reduce(nSummed, sumOp<label>());
//    reduce(sumFlatness, sumOp<scalar>());
//
//    if (report)
//    {
//        if (nSummed > 0)
//        {
//            Info<< "Face flatness (1 = flat, 0 = butterfly) : average = "
//                << sumFlatness / nSummed << "  min = " << minFlatness << endl;
//        }
//
//        if (nWarped> 0)
//        {
//            Info<< "There are " << nWarped
//                << " faces with ratio between projected and actual area < "
//                << warnFlatness
//                << ".\nMinimum ratio (minimum flatness, maximum warpage) = "
//                << minFlatness << nl << endl;
//        }
//        else
//        {
//            Info<< "All faces are flat in that the ratio between projected"
//                << " and actual area is > " << warnFlatness << nl << endl;
//        }
//    }
//
//    if (nWarped > 0)
//    {
//        if (report)
//        {
//            WarningInFunction
//                << nWarped  << " faces with severe warpage (flatness < "
//                << warnFlatness << ") found.\n"
//                << endl;
//        }
//
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}


// Check twist of faces. Is calculated as the difference between areas of
// individual triangles and the overall area of the face (which ifself is
// is the average of the areas of the individual triangles).
bool CML::primitiveMeshGeometry::checkFaceTwist
(
    const bool report,
    const scalar minTwist,
    const primitiveMesh& mesh,
    const vectorField& faceAreas,
    const vectorField& faceCentres,
    const pointField& p,
    const labelList& checkFaces,
    labelHashSet* setPtr
)
{
    if (minTwist < -1-SMALL || minTwist > 1+SMALL)
    {
        FatalErrorInFunction
            << "minTwist should be [-1..1] but is now " << minTwist
            << abort(FatalError);
    }


    const faceList& fcs = mesh.faces();

    // Areas are calculated as the sum of areas. (see
    // primitiveMeshFaceCentresAndAreas.C)

    label nWarped = 0;

    forAll(checkFaces, i)
    {
        label facei = checkFaces[i];

        const face& f = fcs[facei];

        scalar magArea = mag(faceAreas[facei]);

        if (f.size() > 3 && magArea > VSMALL)
        {
            const vector nf = faceAreas[facei] / magArea;

            const point& fc = faceCentres[facei];

            forAll(f, fpI)
            {
                vector triArea
                (
                    triPointRef
                    (
                        p[f[fpI]],
                        p[f.nextLabel(fpI)],
                        fc
                    ).area()
                );

                scalar magTri = mag(triArea);

                if (magTri > VSMALL && ((nf & triArea/magTri) < minTwist))
                {
                    nWarped++;

                    if (setPtr)
                    {
                        setPtr->insert(facei);
                    }
                }
            }
        }
    }


    reduce(nWarped, sumOp<label>());

    if (report)
    {
        if (nWarped> 0)
        {
            Info<< "There are " << nWarped
                << " faces with cosine of the angle"
                << " between triangle normal and face normal less than "
                << minTwist << nl << endl;
        }
        else
        {
            Info<< "All faces are flat in that the cosine of the angle"
                << " between triangle normal and face normal less than "
                << minTwist << nl << endl;
        }
    }

    if (nWarped > 0)
    {
        if (report)
        {
            WarningInFunction
                << nWarped  << " faces with severe warpage "
                << "(cosine of the angle between triangle normal and "
                << "face normal < " << minTwist << ") found.\n"
                << endl;
        }

        return true;
    }
    else
    {
        return false;
    }
}


bool CML::primitiveMeshGeometry::checkFaceArea
(
    const bool report,
    const scalar minArea,
    const primitiveMesh& mesh,
    const vectorField& faceAreas,
    const labelList& checkFaces,
    labelHashSet* setPtr
)
{
    label nZeroArea = 0;

    forAll(checkFaces, i)
    {
        label facei = checkFaces[i];

        if (mag(faceAreas[facei]) < minArea)
        {
            if (setPtr)
            {
                setPtr->insert(facei);
            }
            nZeroArea++;
        }
    }


    reduce(nZeroArea, sumOp<label>());

    if (report)
    {
        if (nZeroArea > 0)
        {
            Info<< "There are " << nZeroArea
                << " faces with area < " << minArea << '.' << nl << endl;
        }
        else
        {
            Info<< "All faces have area > " << minArea << '.' << nl << endl;
        }
    }

    if (nZeroArea > 0)
    {
        if (report)
        {
            WarningInFunction
                << nZeroArea  << " faces with area < " << minArea
                << " found.\n"
                << endl;
        }

        return true;
    }
    else
    {
        return false;
    }
}


bool CML::primitiveMeshGeometry::checkCellDeterminant
(
    const bool report,
    const scalar warnDet,
    const primitiveMesh& mesh,
    const vectorField& faceAreas,
    const labelList& checkFaces,
    const labelList& affectedCells,
    labelHashSet* setPtr
)
{
    const cellList& cells = mesh.cells();

    scalar minDet = GREAT;
    scalar sumDet = 0.0;
    label nSumDet = 0;
    label nWarnDet = 0;

    forAll(affectedCells, i)
    {
        const cell& cFaces = cells[affectedCells[i]];

        tensor areaSum(tensor::zero);
        scalar magAreaSum = 0;

        forAll(cFaces, cFacei)
        {
            label facei = cFaces[cFacei];

            scalar magArea = mag(faceAreas[facei]);

            magAreaSum += magArea;
            areaSum += faceAreas[facei]*(faceAreas[facei]/magArea);
        }

        scalar scaledDet = det(areaSum/magAreaSum)/0.037037037037037;

        minDet = min(minDet, scaledDet);
        sumDet += scaledDet;
        nSumDet++;

        if (scaledDet < warnDet)
        {
            if (setPtr)
            {
                // Insert all faces of the cell.
                forAll(cFaces, cFacei)
                {
                    label facei = cFaces[cFacei];
                    setPtr->insert(facei);
                }
            }
            nWarnDet++;
        }
    }

    reduce(minDet, minOp<scalar>());
    reduce(sumDet, sumOp<scalar>());
    reduce(nSumDet, sumOp<label>());
    reduce(nWarnDet, sumOp<label>());

    if (report)
    {
        if (nSumDet > 0)
        {
            Info<< "Cell determinant (1 = uniform cube) : average = "
                << sumDet / nSumDet << "  min = " << minDet << endl;
        }

        if (nWarnDet > 0)
        {
            Info<< "There are " << nWarnDet
                << " cells with determinant < " << warnDet << '.' << nl
                << endl;
        }
        else
        {
            Info<< "All faces have determinant > " << warnDet << '.' << nl
                << endl;
        }
    }

    if (nWarnDet > 0)
    {
        if (report)
        {
            WarningInFunction
                << nWarnDet << " cells with determinant < " << warnDet
                << " found.\n"
                << endl;
        }

        return true;
    }
    else
    {
        return false;
    }
}


bool CML::primitiveMeshGeometry::checkFaceDotProduct
(
    const bool report,
    const scalar orthWarn,
    const labelList& checkFaces,
    labelHashSet* setPtr
) const
{
    return checkFaceDotProduct
    (
        report,
        orthWarn,
        mesh_,
        cellCentres_,
        faceAreas_,
        checkFaces,
        setPtr
    );
}


bool CML::primitiveMeshGeometry::checkFacePyramids
(
    const bool report,
    const scalar minPyrVol,
    const pointField& p,
    const labelList& checkFaces,
    labelHashSet* setPtr
) const
{
    return checkFacePyramids
    (
        report,
        minPyrVol,
        mesh_,
        cellCentres_,
        p,
        checkFaces,
        setPtr
    );
}


bool CML::primitiveMeshGeometry::checkFaceSkewness
(
    const bool report,
    const scalar internalSkew,
    const scalar boundarySkew,
    const labelList& checkFaces,
    labelHashSet* setPtr
) const
{
    return checkFaceSkewness
    (
        report,
        internalSkew,
        boundarySkew,
        mesh_,
        cellCentres_,
        faceCentres_,
        faceAreas_,
        checkFaces,
        setPtr
    );
}


bool CML::primitiveMeshGeometry::checkFaceWeights
(
    const bool report,
    const scalar warnWeight,
    const labelList& checkFaces,
    labelHashSet* setPtr
) const
{
    return checkFaceWeights
    (
        report,
        warnWeight,
        mesh_,
        cellCentres_,
        faceCentres_,
        faceAreas_,
        checkFaces,
        setPtr
    );
}


bool CML::primitiveMeshGeometry::checkFaceAngles
(
    const bool report,
    const scalar maxDeg,
    const pointField& p,
    const labelList& checkFaces,
    labelHashSet* setPtr
) const
{
    return checkFaceAngles
    (
        report,
        maxDeg,
        mesh_,
        faceAreas_,
        p,
        checkFaces,
        setPtr
    );
}


//bool CML::primitiveMeshGeometry::checkFaceFlatness
//(
//    const bool report,
//    const scalar warnFlatness,
//    const pointField& p,
//    const labelList& checkFaces,
//    labelHashSet* setPtr
//) const
//{
//    return checkFaceFlatness
//    (
//        report,
//        warnFlatness,
//        mesh_,
//        faceAreas_,
//        faceCentres_,
//        p,
//        checkFaces,
//        setPtr
//    );
//}


bool CML::primitiveMeshGeometry::checkFaceTwist
(
    const bool report,
    const scalar minTwist,
    const pointField& p,
    const labelList& checkFaces,
    labelHashSet* setPtr
) const
{
    return checkFaceTwist
    (
        report,
        minTwist,
        mesh_,
        faceAreas_,
        faceCentres_,
        p,
        checkFaces,
        setPtr
    );
}


bool CML::primitiveMeshGeometry::checkFaceArea
(
    const bool report,
    const scalar minArea,
    const labelList& checkFaces,
    labelHashSet* setPtr
) const
{
    return checkFaceArea
    (
        report,
        minArea,
        mesh_,
        faceAreas_,
        checkFaces,
        setPtr
    );
}


bool CML::primitiveMeshGeometry::checkCellDeterminant
(
    const bool report,
    const scalar warnDet,
    const labelList& checkFaces,
    const labelList& affectedCells,
    labelHashSet* setPtr
) const
{
    return checkCellDeterminant
    (
        report,
        warnDet,
        mesh_,
        faceAreas_,
        checkFaces,
        affectedCells,
        setPtr
    );
}


// ************************************************************************* //
