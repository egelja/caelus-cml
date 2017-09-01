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

#include "numericalBeachEmpty.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace numericalBeaches
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(numericalBeachEmpty, 0);
addToRunTimeSelectionTable(numericalBeach, numericalBeachEmpty, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

numericalBeachEmpty::numericalBeachEmpty
(
    const word& subDictName,
    const fvMesh& mesh_
)
:
    numericalBeach(subDictName, mesh_)
{
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void numericalBeachEmpty::correct
(
    const labelList& cells,
    const scalarField& sigma,
    volScalarField& artVisc
)
{
    // Nothing to be done. Dummy function.
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace numericalBeaches
} // End namespace CML

// ************************************************************************* //
