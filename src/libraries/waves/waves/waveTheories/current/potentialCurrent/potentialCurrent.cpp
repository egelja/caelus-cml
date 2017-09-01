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

#include "potentialCurrent.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(potentialCurrent, 0);
addToRunTimeSelectionTable(waveTheory, potentialCurrent, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


potentialCurrent::potentialCurrent
(
    const word& subDictName,
    const fvMesh& mesh_
)
:
    waveTheory(subDictName, mesh_),
    U_(vector(coeffDict_.lookup("U"))),
    Tsoft_(readScalar(coeffDict_.lookup("Tsoft"))),
    localSeaLevel_
    (
        coeffDict_.lookupOrDefault<scalar>("localSeaLevel", seaLevel_)
    )
{}


void potentialCurrent::printCoeffs()
{
    Info << "Loading wave theory: " << typeName << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


scalar potentialCurrent::factor(const scalar& time) const
{
    scalar factor(1);
    if (Tsoft_ > 0.0)
    {
        factor = CML::sin(PI_/2.0/Tsoft_*CML::min(Tsoft_, time));
    }

    return factor;
}


scalar potentialCurrent::eta
(
    const point& x,
    const scalar& time
) const
{
    scalar eta = localSeaLevel_;
    return eta;
}


scalar potentialCurrent::ddxPd
(
    const point& x,
    const scalar& time,
    const vector& unitVector
) const
{
    return 0.0;
}


scalar potentialCurrent::p
(
    const point& x,
    const scalar& time
) const
{
    scalar result = rhoWater_*CML::mag(g_)*localSeaLevel_;

    return result;
}


vector potentialCurrent::U
(
    const point& x,
    const scalar& time
) const
{
    return (U_*factor(time));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// ************************************************************************* //
