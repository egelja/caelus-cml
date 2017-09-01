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

#include "relaxationWeightExponential.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationWeights
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(relaxationWeightExponential, 0);
addToRunTimeSelectionTable
(
    relaxationWeight,
    relaxationWeightExponential,
    dictionary
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


relaxationWeightExponential::relaxationWeightExponential
(
    const word& subDictName,
    const fvMesh& mesh_
)
:
    relaxationWeight(subDictName, mesh_),

    exponent_(coeffDict_.lookupOrDefault<scalar>("exponent",3.5))
{

}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void relaxationWeightExponential::computeWeights
(
    const labelList& cells,
    const scalarField& sigma,
    scalarField& weight
)
{
    forAll (weight, celli)
    {
        weight[celli] = 1.0 -
            (
                CML::exp(CML::pow(sigma[celli],exponent_)) - 1.0
            )/(CML::exp(1.0) - 1.0);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationWeights
} // End namespace CML

// ************************************************************************* //
