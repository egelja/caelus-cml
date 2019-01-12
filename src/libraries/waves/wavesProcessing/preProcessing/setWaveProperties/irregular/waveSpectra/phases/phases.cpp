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

#include "phases.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(phases, 0);
defineRunTimeSelectionTable(phases, phases);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


phases::phases
(
    const Time& rT,
    dictionary& dict
)
:
    rT_(rT),
    dict_(dict)
{
}


phases::~phases()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


autoPtr<phases> phases::New
(
    const Time& rT,
    dictionary& dict
)
{
    word phaseName = dict.lookupOrDefault<word>("phaseMethod","randomPhase");

    phasesConstructorTable::iterator cstrIter =
            phasesConstructorTablePtr_->find(phaseName);

    if (cstrIter == phasesConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown phasing method '" << phaseName << "'"
            << endl << endl
            << "Valid phasing methods are:" << endl
            << phasesConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<phases>(cstrIter()(rT, dict));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
