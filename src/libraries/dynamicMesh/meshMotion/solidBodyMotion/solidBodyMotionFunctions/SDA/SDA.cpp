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

#include "SDA.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "mathematicalConstants.hpp"

using namespace CML::constant::mathematical;

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace solidBodyMotionFunctions
{
    defineTypeNameAndDebug(SDA, 0);
    addToRunTimeSelectionTable(solidBodyMotionFunction, SDA, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidBodyMotionFunctions::SDA::SDA
(
    const dictionary& SBMFCoeffs,
    const Time& runTime,
    const bool incremental
)
:
    solidBodyMotionFunction(SBMFCoeffs, runTime, incremental),
    CofG_(SBMFCoeffs_.lookup("CofG"))
{
    read(SBMFCoeffs);
}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

CML::solidBodyMotionFunctions::SDA::~SDA()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::septernion CML::solidBodyMotionFunctions::SDA::transformation() const
{
    scalar oldTime = time_.value() - time_.deltaT().value();
    scalar curTime = time_.value();

    scalar Tpi = Tp_ + dTp_*(curTime/dTi_);   // Current roll period [sec]
    scalar wr = twoPi/Tpi; // Current Freq [/sec]

    // Current Phase for roll [rad]
    scalar r = dTp_/dTi_;
    scalar u = Tp_ + r*curTime;
    scalar phr = twoPi*((Tp_/u - 1) + log(mag(u)) - log(Tp_))/r;

    // Current Phase for Sway [rad]
    scalar phs = phr + pi;

    // Current Phase for Heave [rad]
    scalar phh = phr + piByTwo;

    scalar rollA = max(rollAmax_*exp(-sqr(Tpi - Tpn_)/(2*Q_)), rollAmin_);

    vector T
    (
        0,
        swayA_*(sin(wr*curTime + phs) - sin(phs)),
        heaveA_*(sin(wr*curTime + phh) - sin(phh))
    );

    quaternion R1(quaternion::XYZ, vector(rollA*sin(wr*curTime + phr), 0, 0));

    vector localCofG = CofG_;
    quaternion R = R1;

    if (incremental_)
    {  
        Tpi = Tp_ + dTp_*(oldTime/dTi_);   // Previous roll period [sec]
        wr = twoPi/Tpi; // Previous Freq [/sec]

        // Previous Phase for roll [rad]
        r = dTp_/dTi_;
        u = Tp_ + r*oldTime;
        phr = twoPi*((Tp_/u - 1) + log(mag(u)) - log(Tp_))/r;

        // Previous Phase for Sway [rad]
        phs = phr + pi;

        // Previous Phase for Heave [rad]
        phh = phr + piByTwo;

        rollA = max(rollAmax_*exp(-sqr(Tpi - Tpn_)/(2*Q_)), rollAmin_);

        vector T2
        (
            0,
            swayA_*(sin(wr*oldTime + phs) - sin(phs)),
            heaveA_*(sin(wr*oldTime + phh) - sin(phh))
        );

        localCofG +=  T2;
        quaternion R2(quaternion::XYZ, vector(rollA*sin(wr*oldTime + phr), 0, 0));

        // Incremental quaternion
        R = R1*inv(R2);
    }

    septernion TR(septernion(-CofG_ + T)*R*septernion(localCofG));

    DebugInfo<< "solidBodyMotionFunctions::SDA::transformation(): "
        << "Time = " << curTime << " transformation: " << TR << endl;

    return TR;
}


bool CML::solidBodyMotionFunctions::SDA::read(const dictionary& SBMFCoeffs)
{
    solidBodyMotionFunction::read(SBMFCoeffs);

    SBMFCoeffs_.lookup("CofG") >> CofG_;
    SBMFCoeffs_.lookup("lamda") >> lamda_;
    SBMFCoeffs_.lookup("rollAmax") >> rollAmax_;
    SBMFCoeffs_.lookup("rollAmin") >> rollAmin_;
    SBMFCoeffs_.lookup("heaveA") >> heaveA_;
    SBMFCoeffs_.lookup("swayA") >> swayA_;
    SBMFCoeffs_.lookup("Q") >> Q_;
    SBMFCoeffs_.lookup("Tp") >> Tp_;
    SBMFCoeffs_.lookup("Tpn") >> Tpn_;
    SBMFCoeffs_.lookup("dTi") >> dTi_;
    SBMFCoeffs_.lookup("dTp") >> dTp_;

    // Rescale parameters according to the given scale parameter
    if (lamda_ > 1 + SMALL)
    {
        heaveA_ /= lamda_;
        swayA_ /= lamda_;
        Tp_ /= sqrt(lamda_);
        Tpn_ /= sqrt(lamda_);
        dTi_ /= sqrt(lamda_);
        dTp_ /= sqrt(lamda_);
    }

    return true;
}


// ************************************************************************* //
