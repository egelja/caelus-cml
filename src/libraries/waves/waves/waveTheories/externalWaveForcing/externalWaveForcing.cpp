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

#include "externalWaveForcing.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(externalWaveForcing, 0);
defineRunTimeSelectionTable(externalWaveForcing, externalWaveForcing);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


externalWaveForcing::externalWaveForcing
(
    IOobject io,
    Time& rT,
    const fvMesh& mesh
)
:
    regIOobject(io),

    rT_(rT),

    mesh_(mesh)
{

}


externalWaveForcing::~externalWaveForcing()
{}


autoPtr<externalWaveForcing> externalWaveForcing::New
(
    IOobject io,
    Time& rT,
    const fvMesh& mesh
)
{
    word externalType
    (
        (io.db().lookupObject<IOdictionary>("waveProperties"))
        .lookupOrDefault<word>("externalForcing", "emptyExternal")
    );

    externalWaveForcingConstructorTable::iterator cstrIter =
        externalWaveForcingConstructorTablePtr_->find(externalType);

    if (cstrIter == externalWaveForcingConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown type of external wave forcing: " << externalType
            << endl << endl
            << "Valid external forcings are :" << endl
            << externalWaveForcingConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<externalWaveForcing>(cstrIter()(io, rT, mesh));
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //





// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// ************************************************************************* //
