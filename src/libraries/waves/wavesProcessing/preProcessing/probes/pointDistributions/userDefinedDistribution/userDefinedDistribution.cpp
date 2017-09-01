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

#include "userDefinedDistribution.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(userDefinedDistribution, 0);
addToRunTimeSelectionTable
(
    pointDistributions,
    userDefinedDistribution,
    pointDistributions
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


userDefinedDistribution::userDefinedDistribution
(
    const fvMesh& mesh,
    const dictionary& dict
)
:
    pointDistributions(mesh, dict)
{
}


userDefinedDistribution::~userDefinedDistribution()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


pointField userDefinedDistribution::evaluate()
{
    // Read needed material
    scalarField x("xValues", pointDict_, readLabel( pointDict_.lookup("N")));
    scalarField y("yValues", pointDict_, readLabel( pointDict_.lookup("N")));
    scalarField z("zValues", pointDict_, readLabel( pointDict_.lookup("N")));

    // Define the return field
    pointField res(x.size(), point::zero);

    forAll (res, pointi)
    {
        res[pointi] = point(x[pointi], y[pointi], z[pointi]);
    }

    return res;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
