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

#include "relaxationWeightThirdOrderPolynomial.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationWeights
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(relaxationWeightThirdOrderPolynomial, 0);
addToRunTimeSelectionTable
(
    relaxationWeight,
    relaxationWeightThirdOrderPolynomial,
    dictionary
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


relaxationWeightThirdOrderPolynomial::relaxationWeightThirdOrderPolynomial
(
    const word& subDictName,
    const fvMesh& mesh_
)
:
    relaxationWeight(subDictName, mesh_)
{

}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void relaxationWeightThirdOrderPolynomial::computeWeights
(
    const labelList& cells,
    const scalarField& sigma,
    scalarField& weight
)
{
    forAll (weight, celli)
    {
        scalar s(1.0 - sigma[celli]);
        weight[celli] = -2.0*CML::pow(s, 3.0) + 3.0*CML::pow(s, 2.0);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationWeights
} // End namespace CML

// ************************************************************************* //
