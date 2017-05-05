/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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

#include "solidBodyMotionFunction.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(CML::solidBodyMotionFunction, 0);

defineRunTimeSelectionTable(CML::solidBodyMotionFunction, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidBodyMotionFunction::solidBodyMotionFunction
(
    const dictionary& SBMFCoeffs,
    const Time& runTime
)
:
    SBMFCoeffs_
    (
        SBMFCoeffs.subDict
        (
            word(SBMFCoeffs.lookup("solidBodyMotionFunction")) + "Coeffs"
        )
    ),
    time_(runTime)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::solidBodyMotionFunction::~solidBodyMotionFunction()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool CML::solidBodyMotionFunction::read(const dictionary& SBMFCoeffs)
{
    SBMFCoeffs_ = SBMFCoeffs.subDict(type() + "Coeffs");

    return true;
}


// ************************************************************************* //
