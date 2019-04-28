/*---------------------------------------------------------------------------*\
Copyright (C) 2015-2016 OpenCFD Ltd
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

#include "Time.hpp"
#include "PstreamReduceOps.hpp"
#include "argList.hpp"
#include "profiling.hpp"

#include <sstream>

// * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(Time, 0);

    template<>
    const char* CML::NamedEnum
    <
        CML::Time::stopAtControls,
        4
    >::names[] =
    {
        "endTime",
        "noWriteNow",
        "writeNow",
        "nextWrite"
    };

    template<>
    const char* CML::NamedEnum
    <
        CML::Time::writeControls,
        5
    >::names[] =
    {
        "timeStep",
        "runTime",
        "adjustableRunTime",
        "clockTime",
        "cpuTime"
    };
}

const CML::NamedEnum<CML::Time::stopAtControls, 4>
    CML::Time::stopAtControlNames_;

const CML::NamedEnum<CML::Time::writeControls, 5>
    CML::Time::writeControlNames_;

CML::Time::fmtflags CML::Time::format_(CML::Time::general);
int CML::Time::precision_(6);

CML::word CML::Time::controlDictName("controlDict");


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::Time::adjustDeltaT()
{
    bool adjustTime = false;
    scalar timeToNextWrite = VGREAT;

    if (writeControl_ == wcAdjustableRunTime)
    {
        adjustTime = true;
        timeToNextWrite = max
        (
            0.0,
            (outputTimeIndex_ + 1)*writeInterval_ - (value() - startTime_)
        );
    }
    if (secondaryWriteControl_ == wcAdjustableRunTime)
    {
        adjustTime = true;
        timeToNextWrite = max
        (
            0.0,
            min
            (
                timeToNextWrite,
                (secondaryOutputTimeIndex_ + 1)*secondaryWriteInterval_
              - (value() - startTime_)
            )
        );
    }

    if (adjustTime)
    {
        scalar nSteps = timeToNextWrite/deltaT_ - SMALL;

        // For tiny deltaT the label can overflow!
        if (nSteps < labelMax)
        {
            label nStepsToNextWrite = label(nSteps) + 1;

            scalar newDeltaT = timeToNextWrite/nStepsToNextWrite;

            // Control the increase of the time step to within a factor of 2
            // and the decrease within a factor of 5.
            if (newDeltaT >= deltaT_)
            {
                deltaT_ = min(newDeltaT, 2.0*deltaT_);
            }
            else
            {
                deltaT_ = max(newDeltaT, 0.2*deltaT_);
            }
        }
    }

    functionObjects_.adjustTimeStep();
}


void CML::Time::setControls()
{
    // default is to resume calculation from "latestTime"
    const word startFrom = controlDict_.lookupOrDefault<word>
    (
        "startFrom",
        "latestTime"
    );

    if (startFrom == "startTime")
    {
        controlDict_.lookup("startTime") >> startTime_;
    }
    else
    {
        // Search directory for valid time directories
        instantList timeDirs = findTimes(path(), constant());

        if (startFrom == "firstTime")
        {
            if (timeDirs.size())
            {
                if (timeDirs[0].name() == constant() && timeDirs.size() >= 2)
                {
                    startTime_ = timeDirs[1].value();
                }
                else
                {
                    startTime_ = timeDirs[0].value();
                }
            }
        }
        else if (startFrom == "latestTime")
        {
            if (timeDirs.size())
            {
                startTime_ = timeDirs.last().value();
            }
        }
        else
        {
            FatalIOErrorInFunction(controlDict_)
                << "expected startTime, firstTime or latestTime"
                << " found '" << startFrom << "'"
                << exit(FatalIOError);
        }
    }

    setTime(startTime_, 0);

    readDict();
    deltaTSave_ = deltaT_;
    deltaT0_ = deltaT_;

    if (Pstream::parRun())
    {
        scalar sumStartTime = startTime_;
        reduce(sumStartTime, sumOp<scalar>());
        if
        (
            mag(Pstream::nProcs()*startTime_ - sumStartTime)
          > Pstream::nProcs()*deltaT_/10.0
        )
        {
            FatalIOErrorInFunction(controlDict_)
                << "Start time is not the same for all processors" << nl
                << "processor " << Pstream::myProcNo() << " has startTime "
                << startTime_ << exit(FatalIOError);
        }
    }

    IOdictionary timeDict
    (
        IOobject
        (
            "time",
            timeName(),
            "uniform",
            *this,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE,
            false
        )
    );

    // Read and set the deltaT only if time-step adjustment is active
    // otherwise use the deltaT from the controlDict
    if (controlDict_.lookupOrDefault<Switch>("adjustTimeStep", false))
    {
        if (timeDict.readIfPresent("deltaT", deltaT_))
        {
            deltaTSave_ = deltaT_;
            deltaT0_ = deltaT_;
        }
    }

    timeDict.readIfPresent("deltaT0", deltaT0_);

    if (timeDict.readIfPresent("index", startTimeIndex_))
    {
        timeIndex_ = startTimeIndex_;
    }


    // Check if values stored in time dictionary are consistent

    // 1. Based on time name
    bool checkValue = true;

    string storedTimeName;
    if (timeDict.readIfPresent("name", storedTimeName))
    {
        if (storedTimeName == timeName())
        {
            // Same time. No need to check stored value
            checkValue = false;
        }
    }

    // 2. Based on time value
    //    (consistent up to the current time writing precision so it won't
    //     trigger if we just change the write precision)
    if (checkValue)
    {
        scalar storedTimeValue;
        if (timeDict.readIfPresent("value", storedTimeValue))
        {
            word storedTimeName(timeName(storedTimeValue));

            if (storedTimeName != timeName())
            {
                IOWarningInFunction(timeDict)
                    << "Time read from time dictionary " << storedTimeName
                    << " differs from actual time " << timeName() << '.' << nl
                    << "    This may cause unexpected database behaviour."
                    << " If you are not interested" << nl
                    << "    in preserving time state delete"
                    << " the time dictionary."
                    << endl;
            }
        }
    }
}


void CML::Time::setMonitoring(bool forceProfiling)
{
    const dictionary* profilingDict = controlDict_.subDictPtr("profiling");

    // initialize profiling on request
    // otherwise rely on profiling entry within controlDict
    // and skip if 'active' keyword is explicitly set to false
    if (forceProfiling)
    {
        profiling::initialize
        (
            IOobject
            (
                "profiling",
                timeName(),
                "uniform",
                *this,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            *this
        );
    }
    else if
    (
        profilingDict
     && profilingDict->lookupOrDefault<Switch>("active", true)
    )
    {
        profiling::initialize
        (
            *profilingDict,
            IOobject
            (
                "profiling",
                timeName(),
                "uniform",
                *this,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            *this
        );
    }

    // Time objects not registered so do like objectRegistry::checkIn ourselves.
    if (runTimeModifiable_)
    {
        monitorPtr_.reset
        (
            new fileMonitor
            (
                regIOobject::fileModificationChecking == inotify
             || regIOobject::fileModificationChecking == inotifyMaster
            )
        );

        // File might not exist yet.
        fileName f(controlDict_.filePath());

        if (!f.size())
        {
            // We don't have this file but would like to re-read it.
            // Possibly if in master-only reading mode. Use a non-existing
            // file to keep fileMonitor synced.
            f = controlDict_.objectPath();
        }

        controlDict_.watchIndex() = addWatch(f);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::Time::Time
(
    const word& controlDictName,
    const fileName& rootPath,
    const fileName& caseName,
    const word& systemName,
    const word& constantName,
    const bool enableFunctionObjects
)
:
    TimePaths
    (
        rootPath,
        caseName,
        systemName,
        constantName
    ),

    objectRegistry(*this),

    libs_(),

    controlDict_
    (
        IOobject
        (
            controlDictName,
            system(),
            *this,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE,
            false
        )
    ),

    startTimeIndex_(0),
    startTime_(0),
    endTime_(0),

    stopAt_(saEndTime),
    writeControl_(wcTimeStep),
    writeInterval_(GREAT),
    secondaryWriteControl_(wcTimeStep),
    secondaryWriteInterval_(labelMax/10.0), // bit less to allow calculations
    purgeWrite_(0),
    writeOnce_(false),
    subCycling_(false),
    sigWriteNow_(true, *this),
    sigStopAtWriteNow_(true, *this),

    writeFormat_(IOstream::ASCII),
    writeVersion_(IOstream::currentVersion),
    writeCompression_(IOstream::UNCOMPRESSED),
    graphFormat_("raw"),
    runTimeModifiable_(true),

    functionObjects_(*this, enableFunctionObjects)
{
    libs_.open(controlDict_, "libs");

    // Explicitly set read flags on objectRegistry so anything constructed
    // from it reads as well (e.g. fvSolution).
    readOpt() = IOobject::MUST_READ_IF_MODIFIED;

    setControls();
    setMonitoring();
}


CML::Time::Time
(
    const word& controlDictName,
    const argList& args,
    const word& systemName,
    const word& constantName
)
:
    TimePaths
    (
        args.parRunControl().parRun(),
        args.rootPath(),
        args.globalCaseName(),
        args.caseName(),
        systemName,
        constantName
    ),

    objectRegistry(*this),

    libs_(),

    controlDict_
    (
        IOobject
        (
            controlDictName,
            system(),
            *this,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE,
            false
        )
    ),

    startTimeIndex_(0),
    startTime_(0),
    endTime_(0),

    stopAt_(saEndTime),
    writeControl_(wcTimeStep),
    writeInterval_(GREAT),
    secondaryWriteControl_(wcTimeStep),
    secondaryWriteInterval_(labelMax/10.0),
    purgeWrite_(0),
    writeOnce_(false),
    subCycling_(false),
    sigWriteNow_(true, *this),
    sigStopAtWriteNow_(true, *this),

    writeFormat_(IOstream::ASCII),
    writeVersion_(IOstream::currentVersion),
    writeCompression_(IOstream::UNCOMPRESSED),
    graphFormat_("raw"),
    runTimeModifiable_(true),

    functionObjects_(*this, !args.optionFound("noFunctionObjects"))
{
    libs_.open(controlDict_, "libs");

    // Explicitly set read flags on objectRegistry so anything constructed
    // from it reads as well (e.g. fvSolution).
    readOpt() = IOobject::MUST_READ_IF_MODIFIED;

    setControls();

    // '-profiling' = force profiling, ignore controlDict entry
    setMonitoring(args.optionFound("profiling"));
}


CML::Time::Time
(
    const dictionary& dict,
    const fileName& rootPath,
    const fileName& caseName,
    const word& systemName,
    const word& constantName,
    const bool enableFunctionObjects
)
:
    TimePaths
    (
        rootPath,
        caseName,
        systemName,
        constantName
    ),

    objectRegistry(*this),

    libs_(),

    controlDict_
    (
        IOobject
        (
            controlDictName,
            system(),
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        dict
    ),

    startTimeIndex_(0),
    startTime_(0),
    endTime_(0),

    stopAt_(saEndTime),
    writeControl_(wcTimeStep),
    writeInterval_(GREAT),
    secondaryWriteControl_(wcTimeStep),
    secondaryWriteInterval_(labelMax/10.0),
    purgeWrite_(0),
    writeOnce_(false),
    subCycling_(false),
    sigWriteNow_(true, *this),
    sigStopAtWriteNow_(true, *this),

    writeFormat_(IOstream::ASCII),
    writeVersion_(IOstream::currentVersion),
    writeCompression_(IOstream::UNCOMPRESSED),
    graphFormat_("raw"),
    runTimeModifiable_(true),

    functionObjects_(*this, enableFunctionObjects)
{
    libs_.open(controlDict_, "libs");


    // Explicitly set read flags on objectRegistry so anything constructed
    // from it reads as well (e.g. fvSolution).
    readOpt() = IOobject::MUST_READ_IF_MODIFIED;

    // Since could not construct regIOobject with setting:
    controlDict_.readOpt() = IOobject::MUST_READ_IF_MODIFIED;

    setControls();
    setMonitoring();
}


CML::Time::Time
(
    const fileName& rootPath,
    const fileName& caseName,
    const word& systemName,
    const word& constantName,
    const bool enableFunctionObjects
)
:
    TimePaths
    (
        rootPath,
        caseName,
        systemName,
        constantName
    ),

    objectRegistry(*this),

    libs_(),

    controlDict_
    (
        IOobject
        (
            controlDictName,
            system(),
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        )
    ),

    startTimeIndex_(0),
    startTime_(0),
    endTime_(0),

    stopAt_(saEndTime),
    writeControl_(wcTimeStep),
    writeInterval_(GREAT),
    secondaryWriteControl_(wcTimeStep),
    secondaryWriteInterval_(labelMax/10.0),
    purgeWrite_(0),
    writeOnce_(false),
    subCycling_(false),

    writeFormat_(IOstream::ASCII),
    writeVersion_(IOstream::currentVersion),
    writeCompression_(IOstream::UNCOMPRESSED),
    graphFormat_("raw"),
    runTimeModifiable_(true),

    functionObjects_(*this, enableFunctionObjects)
{
    libs_.open(controlDict_, "libs");
    setMonitoring(); // for profiling etc
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::Time::~Time()
{
    if (controlDict_.watchIndex() != -1)
    {
        removeWatch(controlDict_.watchIndex());
    }

    // destroy function objects first
    functionObjects_.clear();

    // cleanup profiling
    profiling::stop(*this);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::label CML::Time::addWatch(const fileName& fName) const
{
    return monitorPtr_().addWatch(fName);
}


bool CML::Time::removeWatch(const label watchIndex) const
{
    return monitorPtr_().removeWatch(watchIndex);
}

const CML::fileName& CML::Time::getFile(const label watchIndex) const
{
    return monitorPtr_().getFile(watchIndex);
}


CML::fileMonitor::fileState CML::Time::getState
(
    const label watchFd
) const
{
    return monitorPtr_().getState(watchFd);
}


void CML::Time::setUnmodified(const label watchFd) const
{
    monitorPtr_().setUnmodified(watchFd);
}


CML::word CML::Time::timeName(const scalar t)
{
    std::ostringstream buf;
    buf.setf(ios_base::fmtflags(format_), ios_base::floatfield);
    buf.precision(precision_);
    buf << t;
    return buf.str();
}


CML::word CML::Time::timeName() const
{
    return dimensionedScalar::name();
}


// Search the construction path for times
CML::instantList CML::Time::times() const
{
    return findTimes(path(), constant());
}


CML::word CML::Time::findInstancePath
(
    const fileName& directory,
    const instant& t
) const
{
    // Read directory entries into a list
    fileNameList dirEntries(readDir(directory, fileName::DIRECTORY));

    forAll(dirEntries, i)
    {
        scalar timeValue;
        if (readScalar(dirEntries[i].c_str(), timeValue) && t.equal(timeValue))
        {
            return dirEntries[i];
        }
    }

    if (t.equal(0.0))
    {
        const word& constantName = constant();

        // Looking for 0 or constant. 0 already checked above.
        if (isDir(directory/constantName))
        {
            return constantName;
        }
    }

    return word::null;
}


CML::word CML::Time::findInstancePath(const instant& t) const
{
    return findInstancePath(path(), t);
}


CML::instant CML::Time::findClosestTime(const scalar t) const
{
    instantList timeDirs = findTimes(path(), constant());

    // there is only one time (likely "constant") so return it
    if (timeDirs.size() == 1)
    {
        return timeDirs[0];
    }

    if (t < timeDirs[1].value())
    {
        return timeDirs[1];
    }
    else if (t > timeDirs.last().value())
    {
        return timeDirs.last();
    }

    label nearestIndex = -1;
    scalar deltaT = GREAT;

    for (label timeI=1; timeI < timeDirs.size(); ++timeI)
    {
        scalar diff = mag(timeDirs[timeI].value() - t);
        if (diff < deltaT)
        {
            deltaT = diff;
            nearestIndex = timeI;
        }
    }

    return timeDirs[nearestIndex];
}


// This should work too,
// if we don't worry about checking "constant" explicitly
//
// CML::instant CML::Time::findClosestTime(const scalar t) const
// {
//     instantList timeDirs = findTimes(path(), constant());
//     label timeIndex = min(findClosestTimeIndex(timeDirs, t), 0, constant());
//     return timeDirs[timeIndex];
// }

CML::label CML::Time::findClosestTimeIndex
(
    const instantList& timeDirs,
    const scalar t,
    const word& constantName
)
{
    label nearestIndex = -1;
    scalar deltaT = GREAT;

    forAll(timeDirs, timeI)
    {
        if (timeDirs[timeI].name() == constantName) continue;

        scalar diff = mag(timeDirs[timeI].value() - t);
        if (diff < deltaT)
        {
            deltaT = diff;
            nearestIndex = timeI;
        }
    }

    return nearestIndex;
}


CML::label CML::Time::startTimeIndex() const
{
    return startTimeIndex_;
}


CML::dimensionedScalar CML::Time::startTime() const
{
    return dimensionedScalar("startTime", dimTime, startTime_);
}


CML::dimensionedScalar CML::Time::endTime() const
{
    return dimensionedScalar("endTime", dimTime, endTime_);
}


bool CML::Time::run() const
{
    bool running = value() < (endTime_ - 0.5*deltaT_);

    if (!subCycling_)
    {
        // only execute when the condition is no longer true
        // ie, when exiting the control loop
        if (!running && timeIndex_ != startTimeIndex_)
        {
            // Ensure functionObjects execute on last time step
            // (and hence write uptodate functionObjectProperties)
            addProfiling(foExec, "functionObjects.execute()");
            functionObjects_.execute();
            endProfiling(foExec);

            addProfiling(foEnd, "functionObjects.end()");
            functionObjects_.end();
            endProfiling(foEnd);
        }
    }

    if (running)
    {
        if (!subCycling_)
        {
            const_cast<Time&>(*this).readModifiedObjects();

            if (timeIndex_ == startTimeIndex_)
            {
                addProfiling(functionObjects, "functionObjects.start()");
                functionObjects_.start();
            }
            else
            {
                addProfiling(functionObjects, "functionObjects.execute()");
                functionObjects_.execute();
            }
        }

        // Update the "running" status following the
        // possible side-effects from functionObjects
        running = value() < (endTime_ - 0.5*deltaT_);
    }

    return running;
}


bool CML::Time::loop()
{
    bool running = run();

    if (running)
    {
        operator++();
    }

    return running;
}


bool CML::Time::end() const
{
    return value() > (endTime_ + 0.5*deltaT_);
}


bool CML::Time::stopAt(const stopAtControls sa) const
{
    const bool changed = (stopAt_ != sa);
    stopAt_ = sa;

    // adjust endTime
    if (sa == saEndTime)
    {
        controlDict_.lookup("endTime") >> endTime_;
    }
    else
    {
        endTime_ = GREAT;
    }
    return changed;
}


void CML::Time::setTime(const Time& t)
{
    value() = t.value();
    dimensionedScalar::name() = t.dimensionedScalar::name();
    timeIndex_ = t.timeIndex_;
}


void CML::Time::setTime(const instant& inst, const label newIndex)
{
    value() = inst.value();
    dimensionedScalar::name() = inst.name();
    timeIndex_ = newIndex;

    IOdictionary timeDict
    (
        IOobject
        (
            "time",
            timeName(),
            "uniform",
            *this,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE,
            false
        )
    );

    timeDict.readIfPresent("deltaT", deltaT_);
    timeDict.readIfPresent("deltaT0", deltaT0_);
    timeDict.readIfPresent("index", timeIndex_);
}


void CML::Time::setTime(const dimensionedScalar& newTime, const label newIndex)
{
    setTime(newTime.value(), newIndex);
}


void CML::Time::setTime(const scalar newTime, const label newIndex)
{
    value() = newTime;
    dimensionedScalar::name() = timeName(timeToUserTime(newTime));
    timeIndex_ = newIndex;
}


void CML::Time::setEndTime(const dimensionedScalar& endTime)
{
    setEndTime(endTime.value());
}


void CML::Time::setEndTime(const scalar endTime)
{
    endTime_ = endTime;
}


void CML::Time::setDeltaT
(
    const dimensionedScalar& deltaT,
    const bool bAdjustDeltaT
)
{
    setDeltaT(deltaT.value(), bAdjustDeltaT);
}


void CML::Time::setDeltaT(const scalar deltaT, const bool bAdjustDeltaT)
{
    deltaT_ = deltaT;
    deltaTchanged_ = true;

    if (bAdjustDeltaT)
    {
        adjustDeltaT();
    }
}


CML::TimeState CML::Time::subCycle(const label nSubCycles)
{
    subCycling_ = true;
    prevTimeState_.set(new TimeState(*this));

    setTime(*this - deltaT(), (timeIndex() - 1)*nSubCycles);
    deltaT_ /= nSubCycles;
    deltaT0_ /= nSubCycles;
    deltaTSave_ = deltaT0_;

    return prevTimeState();
}


void CML::Time::endSubCycle()
{
    if (subCycling_)
    {
        subCycling_ = false;
        TimeState::operator=(prevTimeState());
        prevTimeState_.clear();
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

CML::Time& CML::Time::operator+=(const dimensionedScalar& deltaT)
{
    return operator+=(deltaT.value());
}


CML::Time& CML::Time::operator+=(const scalar deltaT)
{
    setDeltaT(deltaT);
    return operator++();
}


CML::Time& CML::Time::operator++()
{
    deltaT0_ = deltaTSave_;
    deltaTSave_ = deltaT_;

    // Save old time name
    const word oldTimeName = dimensionedScalar::name();

    setTime(value() + deltaT_, timeIndex_ + 1);

    if (!subCycling_)
    {
        // If the time is very close to zero reset to zero
        if (mag(value()) < 10*SMALL*deltaT_)
        {
            setTime(0.0, timeIndex_);
        }
    }


    // Check that new time representation differs from old one
    if (dimensionedScalar::name() == oldTimeName)
    {
        int oldPrecision = precision_;
        do
        {
            precision_++;
            setTime(value(), timeIndex());
        }
        while (precision_ < 100 && dimensionedScalar::name() == oldTimeName);

        WarningInFunction
            << "Increased the timePrecision from " << oldPrecision
            << " to " << precision_
            << " to distinguish between timeNames at time " << value()
            << endl;

        if (precision_ == 100 && precision_ != oldPrecision)
        {
            // Reached limit.
            WarningInFunction
                << "Current time name " << dimensionedScalar::name()
                << " is the old as the previous one " << oldTimeName
                << endl
                << "    This might result in overwriting old results."
                << endl;
        }
    }


    if (!subCycling_)
    {
        if (sigStopAtWriteNow_.active() || sigWriteNow_.active())
        {
            // A signal might have been sent on one processor only
            // Reduce so all decide the same.

            label flag = 0;
            if (sigStopAtWriteNow_.active() && stopAt_ == saWriteNow)
            {
                flag += 1;
            }
            if (sigWriteNow_.active() && writeOnce_)
            {
                flag += 2;
            }
            reduce(flag, maxOp<label>());

            if (flag & 1)
            {
                stopAt_ = saWriteNow;
            }
            if (flag & 2)
            {
                writeOnce_ = true;
            }
        }


        outputTime_ = false;

        switch (writeControl_)
        {
            case wcTimeStep:
                outputTime_ = !(timeIndex_ % label(writeInterval_));
            break;

            case wcRunTime:
            case wcAdjustableRunTime:
            {
                label outputIndex = label
                (
                    ((value() - startTime_) + 0.5*deltaT_)
                  / writeInterval_
                );

                if (outputIndex > outputTimeIndex_)
                {
                    outputTime_ = true;
                    outputTimeIndex_ = outputIndex;
                }
            }
            break;

            case wcCpuTime:
            {
                label outputIndex = label
                (
                    returnReduce(elapsedCpuTime(), maxOp<double>())
                  / writeInterval_
                );
                if (outputIndex > outputTimeIndex_)
                {
                    outputTime_ = true;
                    outputTimeIndex_ = outputIndex;
                }
            }
            break;

            case wcClockTime:
            {
                label outputIndex = label
                (
                    returnReduce(label(elapsedClockTime()), maxOp<label>())
                  / writeInterval_
                );
                if (outputIndex > outputTimeIndex_)
                {
                    outputTime_ = true;
                    outputTimeIndex_ = outputIndex;
                }
            }
            break;
        }


        // Adapt for secondaryWrite controls
        switch (secondaryWriteControl_)
        {
            case wcTimeStep:
                outputTime_ =
                    outputTime_
                || !(timeIndex_ % label(secondaryWriteInterval_));
            break;

            case wcRunTime:
            case wcAdjustableRunTime:
            {
                label outputIndex = label
                (
                    ((value() - startTime_) + 0.5*deltaT_)
                  / secondaryWriteInterval_
                );

                if (outputIndex > secondaryOutputTimeIndex_)
                {
                    outputTime_ = true;
                    secondaryOutputTimeIndex_ = outputIndex;
                }
            }
            break;

            case wcCpuTime:
            {
                label outputIndex = label
                (
                    returnReduce(elapsedCpuTime(), maxOp<double>())
                  / secondaryWriteInterval_
                );
                if (outputIndex > secondaryOutputTimeIndex_)
                {
                    outputTime_ = true;
                    secondaryOutputTimeIndex_ = outputIndex;
                }
            }
            break;

            case wcClockTime:
            {
                label outputIndex = label
                (
                    returnReduce(label(elapsedClockTime()), maxOp<label>())
                  / secondaryWriteInterval_
                );
                if (outputIndex > secondaryOutputTimeIndex_)
                {
                    outputTime_ = true;
                    secondaryOutputTimeIndex_ = outputIndex;
                }
            }
            break;
        }


        // see if endTime needs adjustment to stop at the next run()/end() check
        if (!end())
        {
            if (stopAt_ == saNoWriteNow)
            {
                endTime_ = value();
            }
            else if (stopAt_ == saWriteNow)
            {
                endTime_ = value();
                outputTime_ = true;
            }
            else if (stopAt_ == saNextWrite && outputTime_ == true)
            {
                endTime_ = value();
            }
        }

        // Override outputTime if one-shot writing
        if (writeOnce_)
        {
            outputTime_ = true;
            writeOnce_ = false;
        }

        functionObjects_.timeSet();
    }

    return *this;
}


CML::Time& CML::Time::operator++(int)
{
    return operator++();
}


// ************************************************************************* //
