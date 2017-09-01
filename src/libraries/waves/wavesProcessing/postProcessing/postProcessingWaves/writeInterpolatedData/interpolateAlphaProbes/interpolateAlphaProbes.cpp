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

#include "interpolateAlphaProbes.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(interpolateAlphaProbes, 0);
addToRunTimeSelectionTable
(
    postProcessingWaves,
    interpolateAlphaProbes,
    postProcessingWaves
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


interpolateAlphaProbes::interpolateAlphaProbes
(
    const Time& rT,
    const dictionary& actionProp,
    const word& action
)
:
    postProcessingWaves(rT, actionProp, action)
{
}


interpolateAlphaProbes::~interpolateAlphaProbes()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void interpolateAlphaProbes::evaluate()
{
    List<std::pair<scalar, label> > timeLabel;
    scalarField x, y, z;
    List<scalarField> alphas;

    rawAlphaProbes rap(rT_, actionProperties_, actionType_);

    rap.readAlphaProbesData(timeLabel, x, y, z, alphas);

    scalarField t = equidistantTime(timeLabel, actionProperties_);

    scalarField weights(t.size());
    labelList leftData(t.size());
    labelList rightData(t.size());

    interpolationWeights(timeLabel, t, weights, leftData, rightData);

    scalarField output(weights.size(), 0.0);

    forAll (alphas, alphaI)
    {
        const scalarField& alpha(alphas[alphaI]);

        forAll (weights, ii)
        {
            output[ii] = weights[ii]*alpha[leftData[ii]]
                + (1.0 - weights[ii])*alpha[rightData[ii]];
        }

        std::stringstream ss;
        ss << callName_ << "_" << alphaI;

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
