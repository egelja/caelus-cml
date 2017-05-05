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

#include "rotatingMotion.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "mathematicalConstants.hpp"

using namespace CML::constant::mathematical;

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace solidBodyMotionFunctions
{
    defineTypeNameAndDebug(rotatingMotion, 0);
    addToRunTimeSelectionTable
    (
        solidBodyMotionFunction,
        rotatingMotion,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidBodyMotionFunctions::rotatingMotion::rotatingMotion
(
    const dictionary& SBMFCoeffs,
    const Time& runTime
)
:
    solidBodyMotionFunction(SBMFCoeffs, runTime)
{
    read(SBMFCoeffs);
}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

CML::solidBodyMotionFunctions::rotatingMotion::~rotatingMotion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::septernion
CML::solidBodyMotionFunctions::rotatingMotion::transformation() const
{
    scalar t = time_.value();

    // Motion around a centre of gravity

    // Rotation around centre of gravity (in degrees)
    vector eulerAngles = radialVelocity_*t;

    // Convert the rotational motion from deg to rad
    eulerAngles *= pi/180.0;

    quaternion R(eulerAngles.x(), eulerAngles.y(), eulerAngles.z());
    septernion TR(septernion(CofG_)*R*septernion(-CofG_));

    Info<< "solidBodyMotionFunctions::rotatingMotion::transformation(): "
        << "Time = " << t << " transformation: " << TR << endl;

    return TR;
}


bool CML::solidBodyMotionFunctions::rotatingMotion::read
(
    const dictionary& SBMFCoeffs
)
{
    solidBodyMotionFunction::read(SBMFCoeffs);

    SBMFCoeffs_.lookup("CofG") >> CofG_;
    SBMFCoeffs_.lookup("radialVelocity") >> radialVelocity_;

    return true;
}

// ************************************************************************* //
