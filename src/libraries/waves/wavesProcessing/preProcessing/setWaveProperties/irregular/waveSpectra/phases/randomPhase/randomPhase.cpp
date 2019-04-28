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

#include "randomPhase.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(randomPhase, 0);
addToRunTimeSelectionTable(phases, randomPhase, phases);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


randomPhase::randomPhase
(
    const Time& rT,
    dictionary& dict
)
:
    phases(rT, dict)
{
    Info << "\nConstructing: " << this->type() << endl;
    srand(time(nullptr));

    if (dict.found("seedForRandomPhase"))
    {
        label seed = readLabel(dict.lookup("seedForRandomPhase"));
        srand(seed);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


scalar randomPhase::phase(const scalar& freq, const vector& k)
{
    return
    (
        2.0*M_PI*static_cast<scalar>(rand())
       /static_cast<scalar>(RAND_MAX)
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
