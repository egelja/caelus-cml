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

\*---------------------------------------------------------------------------*/

#include "cellModel.hpp"
#include "pyramid.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::vector CML::cellModel::centre
(
    const labelList& pointLabels,
    const pointField& points
) const
{
    // Estimate centre of cell
    vector cEst = vector::zero;

    // Sum the points idicated by the label list
    forAll(pointLabels, i)
    {
        cEst += points[pointLabels[i]];
    }

    // Average by dividing by the number summed over.
    cEst /= scalar(pointLabels.size());


    // Calculate the centre by breaking the cell into pyramids and
    // volume-weighted averaging their centres
    scalar sumV = 0.0;
    vector sumVc = vector::zero;

    const faceList cellFaces = faces(pointLabels);

    forAll(cellFaces, i)
    {
        const face& curFace = cellFaces[i];

        scalar pyrVol =
            pyramid<point, const point&, const face&>
            (
                curFace,
                cEst
            ).mag(points);

        if (pyrVol > SMALL)
        {
            WarningInFunction
                << "zero or negative pyramid volume: " << -pyrVol
                << " for face " << i
                << endl;
        }

        sumVc -=
            pyrVol
           *pyramid<point, const point&, const face&>(curFace, cEst)
           .centre(points);

        sumV -= pyrVol;
    }

    return sumVc/(sumV + VSMALL);
}


CML::scalar CML::cellModel::mag
(
    const labelList& pointLabels,
    const pointField& points
) const
{
    // Estimate centre of cell
    vector cEst = vector::zero;

    // Sum the points idicated by the label list
    forAll(pointLabels, i)
    {
        cEst += points[pointLabels[i]];
    }

    // Average by dividing by the number summed over.
    cEst /= scalar(pointLabels.size());


    // Calculate the magnitude by summing the -mags of the pyramids
    // The sign change is because the faces point outwards
    // and a pyramid is constructed from an inward pointing face
    // and the base centre-apex vector
    scalar v = 0;

    const faceList cellFaces = faces(pointLabels);

    forAll(cellFaces, i)
    {
        const face& curFace =cellFaces[i];

        scalar pyrVol =
            pyramid<point, const point&, const face&>
            (
                curFace,
                cEst
            ).mag(points);

        if (pyrVol > SMALL)
        {
            WarningInFunction
                << "zero or negative pyramid volume: " << -pyrVol
                << " for face " << i
                << endl;
        }

        v -= pyrVol;
    }

    return v;
}

// ************************************************************************* //
