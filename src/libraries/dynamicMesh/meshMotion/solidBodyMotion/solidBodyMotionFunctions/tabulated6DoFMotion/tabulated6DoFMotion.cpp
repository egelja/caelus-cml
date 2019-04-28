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

#include "tabulated6DoFMotion.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "Tuple2.hpp"
#include "IFstream.hpp"
#include "interpolateSplineXY.hpp"
#include "mathematicalConstants.hpp"

using namespace CML::constant::mathematical;

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace solidBodyMotionFunctions
{
    defineTypeNameAndDebug(tabulated6DoFMotion, 0);
    addToRunTimeSelectionTable
    (
        solidBodyMotionFunction,
        tabulated6DoFMotion,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidBodyMotionFunctions::tabulated6DoFMotion::tabulated6DoFMotion
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


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::solidBodyMotionFunctions::tabulated6DoFMotion::~tabulated6DoFMotion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::septernion
CML::solidBodyMotionFunctions::tabulated6DoFMotion::transformation() const
{
    scalar oldTime = time_.value() - time_.deltaT().value();
    scalar curTime = time_.value();

    if (curTime < times_[0])
    {
        FatalErrorInFunction
            << "current time (" << curTime
            << ") is less than the minimum in the data table ("
            << times_[0] << ')'
            << exit(FatalError);
    }

    if (curTime > times_.last())
    {
        FatalErrorInFunction
            << "current time (" << curTime
            << ") is greater than the maximum in the data table ("
            << times_.last() << ')'
            << exit(FatalError);
    }

    // Current time translation and rotation
    translationRotationVectors TRV = interpolateSplineXY
    (
        curTime,
        times_,
        values_
    );

    // Convert the rotational motion from deg to rad
    TRV[1] *= pi/180.0;

    quaternion R1(quaternion::XYZ, TRV[1]);

    vector localCofG = CofG_;
    quaternion R = R1;

    if (incremental_)
    {  
        if (oldTime > times_[0])
        {
            // Previous time translation and rotation
            translationRotationVectors TRV2 = interpolateSplineXY(oldTime, times_, values_);
 
            // Convert the rotational motion from deg to rad
            TRV2[1] *= pi/180.0;

            quaternion R2(quaternion::XYZ, TRV2[1]);
            localCofG +=  TRV2[0];
            
            // Incremental quaternion
            R = R1*inv(R2);
        }
        else
        {
            quaternion R2 = R1;
            R = R1*inv(R2);
        }
    }

    septernion TR(septernion(-CofG_ + -TRV[0])*R*septernion(localCofG));

    DebugInfo<< "solidBodyMotionFunctions::tabulated6DoFMotion::transformation(): "
        << "Time = " << curTime << " transformation: " << TR << endl;

    return TR;
}


bool CML::solidBodyMotionFunctions::tabulated6DoFMotion::read
(
    const dictionary& SBMFCoeffs
)
{
    solidBodyMotionFunction::read(SBMFCoeffs);

    // If the timeDataFileName has changed read the file

    fileName newTimeDataFileName
    (
        fileName(SBMFCoeffs_.lookup("timeDataFileName")).expand()
    );

    if (newTimeDataFileName != timeDataFileName_)
    {
        timeDataFileName_ = newTimeDataFileName;

        IFstream dataStream(timeDataFileName_);

        if (dataStream.good())
        {
            List<Tuple2<scalar, translationRotationVectors> > timeValues
            (
                dataStream
            );

            times_.setSize(timeValues.size());
            values_.setSize(timeValues.size());

            forAll(timeValues, i)
            {
                times_[i] = timeValues[i].first();
                values_[i] = timeValues[i].second();
            }
        }
        else
        {
            FatalErrorInFunction
                << "Cannot open time data file " << timeDataFileName_
                << exit(FatalError);
        }
    }

    SBMFCoeffs_.lookup("CofG") >> CofG_;

    return true;
}


// ************************************************************************* //
