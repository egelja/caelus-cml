/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Deltares
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

#include "emptyExternal.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(emptyExternal, 0);

addToRunTimeSelectionTable
(
    externalWaveForcing,
    emptyExternal,
    externalWaveForcing
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


emptyExternal::emptyExternal
(
    IOobject io,
    Time& rT,
    const fvMesh& mesh
)
:
    externalWaveForcing(io, rT, mesh)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void emptyExternal::step()
{
    // Nothing to be done
}


scalar emptyExternal::eta
(
    const point& x,
    const scalar& time
) const
{
    return 0.0;
}


scalar emptyExternal::ddxPd
(
    const point& x,
    const scalar& time,
    const vector& unitVector
) const
{
    return 0.0;
}


scalar emptyExternal::p
(
    const point& x,
    const scalar& time
) const
{
    return 0.0;
}


vector emptyExternal::U
(
    const point& x,
    const scalar& time
) const
{
    return vector::zero;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// ************************************************************************* //
