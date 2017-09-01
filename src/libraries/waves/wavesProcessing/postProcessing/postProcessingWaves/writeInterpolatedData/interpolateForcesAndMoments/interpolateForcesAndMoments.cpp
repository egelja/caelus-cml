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

#include "interpolateForcesAndMoments.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(interpolateForcesAndMoments, 0);
addToRunTimeSelectionTable
(
    postProcessingWaves,
    interpolateForcesAndMoments,
    postProcessingWaves
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


interpolateForcesAndMoments::interpolateForcesAndMoments
(
    const Time& rT,
    const dictionary& actionProp,
    const word& action
)
:
    postProcessingWaves(rT, actionProp, action)
{
}


interpolateForcesAndMoments::~interpolateForcesAndMoments()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void interpolateForcesAndMoments::evaluate()
{
    List<std::pair<scalar, label> > timeLabel;
    vectorField forces, moments;

    rawForcesAndMoments rfm(rT_, actionProperties_, actionType_);

    rfm.readForceAndMomentData(timeLabel, forces, moments);

    scalarField t = equidistantTime(timeLabel, actionProperties_);

    scalarField weights(t.size());
    labelList leftData(t.size());
    labelList rightData(t.size());

    interpolationWeights(timeLabel, t, weights, leftData, rightData);

    vectorField output(weights.size(), vector::zero);

    // Outputs the forces as index 0
    {
        forAll (weights, ii)
        {
            output[ii] = weights[ii]*forces[leftData[ii]]
                + (1.0 - weights[ii])*forces[rightData[ii]];
        }

        std::stringstream ss;
        ss << callName_ << "_0";

        writeIOVectorField( output, ss.str() );
    }

    // Outputs the moments as index 1
    {
        forAll (weights, ii)
        {
            output[ii] = weights[ii]*moments[leftData[ii]]
                + (1.0 - weights[ii])*moments[rightData[ii]];
        }

        std::stringstream ss;
        ss << callName_ << "_1";

        writeIOVectorField(output, ss.str());
    }

    // Outputs the time instances
    std::stringstream ss;
    ss << callName_ << "_time";

    writeIOScalarField(t, ss.str());

    // Outputs the names and deltaT
    wordList wl(2);
    wl[0] = "forces";
    wl[1] = "moments";

    writeNameDict(readScalar(actionProperties_.lookup("deltaT")), wl);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
