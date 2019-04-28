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

#include "pointDistributions.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(pointDistributions, 0);
defineRunTimeSelectionTable(pointDistributions, pointDistributions);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


pointDistributions::pointDistributions
(
    const fvMesh& mesh,
    const dictionary& dict
)
:
    mesh_(mesh),

    pointDict_(dict)
{
}


autoPtr<pointDistributions> pointDistributions::New
(
    const fvMesh& mesh,
    const dictionary& dict
)
{
    word pd( dict.lookup("pointDistribution"));

    pointDistributionsConstructorTable::iterator cstrIter =
            pointDistributionsConstructorTablePtr_->find(pd);

    if (cstrIter == pointDistributionsConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown point distribution: " << pd
            << endl << endl
            << "Valid methods are :" << endl
            << pointDistributionsConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<pointDistributions>(cstrIter()(mesh, dict));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
