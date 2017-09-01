/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

#include "interpolateSurfaceElevation.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(interpolateSurfaceElevation, 0);
addToRunTimeSelectionTable
(
    postProcessingWaves,
    interpolateSurfaceElevation,
    postProcessingWaves
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


interpolateSurfaceElevation::interpolateSurfaceElevation
(
    const Time& rT,
    const dictionary& actionProp,
    const word& action
)
:
    postProcessingWaves(rT, actionProp, action)
{
}


interpolateSurfaceElevation::~interpolateSurfaceElevation()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void interpolateSurfaceElevation::evaluate()
{
    List<std::pair<scalar, label> > timeLabel;
    scalarField x, y, z;
    List<scalarField> etas;

    rawSurfaceElevation rse(rT_, actionProperties_, actionType_);

    rse.readSurfaceElevationData(timeLabel, x, y, z, etas);

    scalarField t = equidistantTime(timeLabel, actionProperties_);

    scalarField weights(t.size());
    labelList leftData(t.size());
    labelList rightData(t.size());

    interpolationWeights(timeLabel, t, weights, leftData, rightData);

    scalarField output(weights.size(), 0.0);

    forAll (etas, etaI)
    {
        const scalarField& eta(etas[etaI]);

        forAll (weights, ii)
        {
            output[ii] = weights[ii]*eta[leftData[ii]]
                + (1.0 - weights[ii] )*eta[rightData[ii]];
        }

        std::stringstream ss;
        ss << callName_ << "_" << etaI;

        writeIOScalarField(output, ss.str());
    }

    std::stringstream ss;
    ss << callName_ << "_time";

    writeIOScalarField(t, ss.str());

    writeXYZDict(readScalar(actionProperties_.lookup("deltaT")), x, y, z);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
