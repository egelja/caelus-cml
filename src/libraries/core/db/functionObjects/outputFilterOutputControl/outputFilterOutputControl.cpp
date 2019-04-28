/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "outputFilterOutputControl.hpp"
#include "PstreamReduceOps.hpp"

// * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * * //

namespace CML
{
    template<>
    const char* CML::NamedEnum
    <
        CML::outputFilterOutputControl::outputControls,
        7
    >::names[] =
    {
        "timeStep",
        "outputTime",
        "adjustableTime",
        "runTime",
        "clockTime",
        "cpuTime",
        "none"
    };
}

const CML::NamedEnum<CML::outputFilterOutputControl::outputControls, 7>
    CML::outputFilterOutputControl::outputControlNames_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::outputFilterOutputControl::outputFilterOutputControl
(
    const Time& t,
    const dictionary& dict,
    const word prefix
)
:
    time_(t),
    prefix_(prefix),
    outputControl_(ocTimeStep),
    outputInterval_(0),
    outputTimeLastDump_(0),
    writeInterval_(-1)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::outputFilterOutputControl::~outputFilterOutputControl()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::outputFilterOutputControl::read(const dictionary& dict)
{
    word controlName(prefix_ + "Control");
    word intervalName(prefix_ + "Interval");

    // For backward compatibility support the deprecated 'outputControl' option
    // now superseded by 'writeControl'
    if (prefix_ == "write" && dict.found("outputControl"))
    {
        IOWarningIn("outputFilterOutputControl::read(const dictionary& dict)",dict)
            << "Using deprecated 'outputControl'" << nl
            << "    Please use 'writeControl' with 'writeInterval'"
            << endl;
        
        // Change the old names for this option
        controlName = "outputControl";
        intervalName = "outputInterval";
    }

    if (dict.found(controlName))
    {
        outputControl_ = outputControlNames_.read(dict.lookup(controlName));
    }
    else
    {
        outputControl_ = ocTimeStep;
    }

    switch (outputControl_)
    {
        case ocTimeStep:
        {
            outputInterval_ = dict.lookupOrDefault<label>(intervalName, 0);
            break;
        }

        case ocOutputTime:
        {
            outputInterval_ = dict.lookupOrDefault<label>(intervalName, 1);
            break;
        }

        case ocClockTime:
        case ocRunTime:
        case ocCpuTime:
        case ocAdjustableTime:
        {
            writeInterval_ = readScalar(dict.lookup(intervalName));
            break;
        }

        default:
        {
            // do nothing
            break;
        }
    }
}


bool CML::outputFilterOutputControl::output()
{
    switch (outputControl_)
    {
        case ocTimeStep:
        {
            return
            (
                (outputInterval_ <= 1)
             || !(time_.timeIndex() % outputInterval_)
            );
            break;
        }

        case ocOutputTime:
        {
            if (time_.outputTime())
            {
                outputTimeLastDump_ ++;
                return !(outputTimeLastDump_ % outputInterval_);
            }
            break;
        }

        case ocRunTime:
        case ocAdjustableTime:
        {
            label outputIndex = label
            (
                (
                    (time_.value() - time_.startTime().value())
                  + 0.5*time_.deltaTValue()
                )
                / writeInterval_
            );

            if (outputIndex > outputTimeLastDump_)
            {
                outputTimeLastDump_ = outputIndex;
                return true;
            }
            break;
        }

        case ocCpuTime:
        {
            label outputIndex = label
            (
                returnReduce(time_.elapsedCpuTime(), maxOp<double>())
                / writeInterval_
            );
            if (outputIndex > outputTimeLastDump_)
            {
                outputTimeLastDump_ = outputIndex;
                return true;
            }
            break;
        }

        case ocClockTime:
        {
            label outputIndex = label
            (
                returnReduce(label(time_.elapsedClockTime()), maxOp<label>())
                / writeInterval_
            );
            if (outputIndex > outputTimeLastDump_)
            {
                outputTimeLastDump_ = outputIndex;
                return true;
            }
            break;
        }

        case ocNone:
        {
            return false;
        }

        default:
        {
            // this error should not actually be possible
            FatalErrorInFunction
                << "Undefined output control: "
                << outputControlNames_[outputControl_] << nl
                << abort(FatalError);
            break;
        }
    }

    return false;
}


// ************************************************************************* //
