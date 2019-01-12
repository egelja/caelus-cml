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

#include "externalSource.hpp"
#include "emptyExternal.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(externalSource, 0);
addToRunTimeSelectionTable(waveTheory, externalSource, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


externalSource::externalSource
(
    const word& subDictName,
    const fvMesh& mesh
)
:
    waveTheory(subDictName, mesh),

    external_
    (
        mesh.thisDb().lookupObject<externalWaveForcing>("externalWaveForcing")
    )
{
    if (external_.type() == CML::waveTheories::emptyExternal::typeName)
    {
        FatalErrorInFunction
            << "The wave theory externalSource is used together with the\n"
            << "external wave forcing 'emptyExternal'. Either specify an\n"
            << "actual external source or switch to the algebraric theories.\n"
            << endl << endl << exit(FatalError);
    }
}


void externalSource::printCoeffs()
{
    Info << "Loading wave theory: " << typeName << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


scalar externalSource::factor(const scalar& time) const
{
    scalar factor(1);

    return factor;
}


scalar externalSource::eta
(
    const point& x,
    const scalar& time
) const
{
    return external_.eta(x, time);
}


scalar externalSource::ddxPd
(
    const point& x,
    const scalar& time,
    const vector& unitVector
) const
{
    return external_.ddxPd(x, time, unitVector);
}


scalar externalSource::p
(
    const point& x,
    const scalar& time
) const
{
    return external_.p(x, time);
}


vector externalSource::U
(
    const point& x,
    const scalar& time
) const
{
    return external_.U(x, time);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// ************************************************************************* //
