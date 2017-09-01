/*---------------------------------------------------------------------------*\
Copyright  Niels Gjøl Jacobsen, Technical University of Denmark.
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

#include "combinedWavesProperties.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(combinedWavesProperties, 0);
addToRunTimeSelectionTable
(
    setWaveProperties,
    combinedWavesProperties,
    setWaveProperties
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


combinedWavesProperties::combinedWavesProperties
(
    const Time& rT,
    dictionary& dict,
    bool write
)
:
    setWaveProperties(rT, dict, write)
{
    Info << "\nConstructing: " << this->type() << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void combinedWavesProperties::set(Ostream& os)
{
    // Write the beginning of the sub-dictionary
    writeBeginning(os);

    // Write the already given parameters
    writeGiven(os, "waveType");

    writeGiven(os, "combinedWaveNames");

    // Write the relaxation zone
    writeRelaxationZone(os);

    // Write the closing bracket
    writeEnding(os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
