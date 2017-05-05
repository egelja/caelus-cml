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

#include "oscillatingLinearMotion.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace solidBodyMotionFunctions
{
    defineTypeNameAndDebug(oscillatingLinearMotion, 0);
    addToRunTimeSelectionTable
    (
        solidBodyMotionFunction,
        oscillatingLinearMotion,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidBodyMotionFunctions::oscillatingLinearMotion::oscillatingLinearMotion
(
    const dictionary& SBMFCoeffs,
    const Time& runTime,
    const bool incremental
)
:
    solidBodyMotionFunction(SBMFCoeffs, runTime, incremental)
{
    read(SBMFCoeffs);
}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

CML::solidBodyMotionFunctions::oscillatingLinearMotion::
~oscillatingLinearMotion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::septernion
CML::solidBodyMotionFunctions::oscillatingLinearMotion::transformation() const
{
    scalar oldTime = time_.value() - time_.deltaT().value();
    scalar curTime = time_.value();

    vector displacement = amplitude_*sin(omega_*curTime);

    if (incremental_)
    {  
        displacement -= amplitude_*sin(omega_*oldTime);
    }
    
    quaternion R(1);
    septernion TR(septernion(-displacement)*R);

    DebugInfo<< "solidBodyMotionFunctions::oscillatingLinearMotion::"
        << "transformation(): "
        << "Time = " << curTime << " transformation: " << TR << endl;

    return TR;
}


bool CML::solidBodyMotionFunctions::oscillatingLinearMotion::read
(
    const dictionary& SBMFCoeffs
)
{
    solidBodyMotionFunction::read(SBMFCoeffs);

    SBMFCoeffs_.lookup("amplitude") >> amplitude_;
    SBMFCoeffs_.lookup("omega") >> omega_;

    return true;
}


// ************************************************************************* //
