/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenCFD Ltd
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
#include "Pstream.hpp"
#include "simpleObjectRegistry.hpp"
#include "profiling.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::Time::readDict()
{
    word application;
    if (controlDict_.readIfPresent("application", application))
    {
        // Do not override if already set so external application can override
        setEnv("CAELUS_APPLICATION", application, false);
    }


    // Check for local switches and settings
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Debug switches
    if (controlDict_.found("DebugSwitches"))
    {
        Info<< "Overriding DebugSwitches according to " << controlDict_.name()
            << endl;

        simpleObjectRegistry& objects = debug::debugObjects();
        const dictionary& localSettings = controlDict_.subDict("DebugSwitches");
        forAllConstIter(dictionary, localSettings, iter)
        {
            const word& name = iter().keyword();

            simpleObjectRegistryEntry* objPtr = objects.lookupPtr(name);

            if (objPtr)
            {
                Info<< "    " << iter() << endl;

                const List<simpleRegIOobject*>& objects = *objPtr;

                if (iter().isDict())
                {
                    forAll(objects, i)
                    {
                        OStringStream os(IOstream::ASCII);
                        os  << iter().dict();
                        IStringStream is(os.str());
                        objects[i]->readData(is);
                    }
                }
                else
                {
                    forAll(objects, i)
                    {
                        objects[i]->readData(iter().stream());
                    }
                }
            }
        }
    }

    if (!deltaTchanged_)
    {
        deltaT_ = readScalar(controlDict_.lookup("deltaT"));
    }

    if (controlDict_.found("writeControl"))
    {
        writeControl_ = writeControlNames_.read
        (
            controlDict_.lookup("writeControl")
        );
    }

    scalar oldWriteInterval = writeInterval_;
    scalar oldSecondaryWriteInterval = secondaryWriteInterval_;

    if (controlDict_.readIfPresent("writeInterval", writeInterval_))
    {
        if (writeControl_ == wcTimeStep && label(writeInterval_) < 1)
        {
            FatalIOErrorInFunction(controlDict_)
                << "writeInterval < 1 for writeControl timeStep"
                << exit(FatalIOError);
        }
    }
    else
    {
        controlDict_.lookup("writeFrequency") >> writeInterval_;
    }


    // Additional writing
    if (controlDict_.found("secondaryWriteControl"))
    {
        secondaryWriteControl_ = writeControlNames_.read
        (
            controlDict_.lookup("secondaryWriteControl")
        );

        if
        (
            controlDict_.readIfPresent
            (
                "secondaryWriteInterval",
                secondaryWriteInterval_
            )
        )
        {
            if
            (
                secondaryWriteControl_ == wcTimeStep
             && label(secondaryWriteInterval_) < 1
            )
            {
                FatalIOErrorInFunction(controlDict_)
                    << "secondaryWriteInterval < 1"
                    << " for secondaryWriteControl timeStep"
                    << exit(FatalIOError);
            }
        }
        else
        {
            controlDict_.lookup("secondaryWriteFrequency")
                >> secondaryWriteInterval_;
        }
    }



    if (oldWriteInterval != writeInterval_)
    {
        switch (writeControl_)
        {
            case wcRunTime:
            case wcAdjustableRunTime:
                // Recalculate outputTimeIndex_ to be in units of current
                // writeInterval.
                outputTimeIndex_ = label
                (
                    outputTimeIndex_
                  * oldWriteInterval
                  / writeInterval_
                );
            break;

            default:
            break;
        }
    }
    if (oldSecondaryWriteInterval != secondaryWriteInterval_)
    {
        switch (secondaryWriteControl_)
        {
            case wcRunTime:
            case wcAdjustableRunTime:
                // Recalculate secondaryOutputTimeIndex_ to be in units of
                // current writeInterval.
                secondaryOutputTimeIndex_ = label
                (
                    secondaryOutputTimeIndex_
                  * oldSecondaryWriteInterval
                  / secondaryWriteInterval_
                );
            break;

            default:
            break;
        }
    }

    if (controlDict_.readIfPresent("purgeWrite", purgeWrite_))
    {
        if (purgeWrite_ < 0)
        {
            WarningInFunction
                << "invalid value for purgeWrite " << purgeWrite_
                << ", should be >= 0, setting to 0"
                << endl;

            purgeWrite_ = 0;
        }
    }

    if (controlDict_.found("timeFormat"))
    {
        const word formatName(controlDict_.lookup("timeFormat"));

        if (formatName == "general")
        {
            format_ = general;
        }
        else if (formatName == "fixed")
        {
            format_ = fixed;
        }
        else if (formatName == "scientific")
        {
            format_ = scientific;
        }
        else
        {
            WarningInFunction
                << "unsupported time format " << formatName
                << endl;
        }
    }

    controlDict_.readIfPresent("timePrecision", precision_);

    // stopAt at 'endTime' or a specified value
    // if nothing is specified, the endTime is zero
    if (controlDict_.found("stopAt"))
    {
        stopAt_ = stopAtControlNames_.read(controlDict_.lookup("stopAt"));

        if (stopAt_ == saEndTime)
        {
            controlDict_.lookup("endTime") >> endTime_;
        }
        else
        {
            endTime_ = GREAT;
        }
    }
    else if (!controlDict_.readIfPresent("endTime", endTime_))
    {
        endTime_ = 0;
    }

    dimensionedScalar::name() = timeName(value());

    if (controlDict_.found("writeVersion"))
    {
        writeVersion_ = IOstream::versionNumber
        (
            controlDict_.lookup("writeVersion")
        );
    }

    if (controlDict_.found("writeFormat"))
    {
        writeFormat_ = IOstream::formatEnum
        (
            controlDict_.lookup("writeFormat")
        );
    }

    if (controlDict_.found("writePrecision"))
    {
        IOstream::defaultPrecision
        (
            readUint(controlDict_.lookup("writePrecision"))
        );

        Sout.precision(IOstream::defaultPrecision());
        Serr.precision(IOstream::defaultPrecision());

        Pout.precision(IOstream::defaultPrecision());
        Perr.precision(IOstream::defaultPrecision());

        FatalError().precision(IOstream::defaultPrecision());
        FatalIOError.error::operator()().precision
        (
            IOstream::defaultPrecision()
        );
    }

    if (controlDict_.found("writeCompression"))
    {
        writeCompression_ = IOstream::compressionEnum
        (
            controlDict_.lookup("writeCompression")
        );

        if
        (
            writeFormat_ == IOstream::BINARY
         && writeCompression_ == IOstream::COMPRESSED
        )
        {
            IOWarningInFunction(controlDict_)
                << "Selecting compressed binary is inefficient and ineffective"
                << ", resetting to uncompressed binary"
                << endl;

            writeCompression_ = IOstream::UNCOMPRESSED;
        }
    }

    controlDict_.readIfPresent("graphFormat", graphFormat_);
    controlDict_.readIfPresent("runTimeModifiable", runTimeModifiable_);

    if (!runTimeModifiable_ && controlDict_.watchIndex() != -1)
    {
        removeWatch(controlDict_.watchIndex());
        controlDict_.watchIndex() = -1;
    }
}


bool CML::Time::read()
{
    if (controlDict_.regIOobject::read())
    {
        readDict();
        return true;
    }
    else
    {
        return false;
    }
}


void CML::Time::readModifiedObjects()
{
    if (runTimeModifiable_)
    {
        // Get state of all monitored objects (=registered objects with a
        // valid filePath).
        // Note: requires same ordering in objectRegistries on different
        // processors!
        monitorPtr_().updateStates
        (
            (
                regIOobject::fileModificationChecking == inotifyMaster
             || regIOobject::fileModificationChecking == timeStampMaster
            ),
            Pstream::parRun()
        );

        // Time handling is special since controlDict_ is the one dictionary
        // that is not registered to any database.

        if (controlDict_.readIfModified())
        {
            readDict();
            functionObjects_.read();
        }

        bool registryModified = objectRegistry::modified();

        if (registryModified)
        {
            objectRegistry::readModifiedObjects();
        }
    }
}


bool CML::Time::writeObject
(
    IOstream::streamFormat fmt,
    IOstream::versionNumber ver,
    IOstream::compressionType cmp
) const
{
    if (outputTime())
    {
        addProfiling(writing, "objectRegistry::writeObject");

        const word tmName(timeName());

        IOdictionary timeDict
        (
            IOobject
            (
                "time",
                tmName,
                "uniform",
                *this,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            )
        );

        timeDict.add("value", timeToUserTime(value()));
        timeDict.add("name", string(tmName));
        timeDict.add("index", timeIndex_);
        timeDict.add("deltaT", timeToUserTime(deltaT_));
        timeDict.add("deltaT0", timeToUserTime(deltaT0_));

        timeDict.regIOobject::writeObject(fmt, ver, cmp);
        bool writeOK = objectRegistry::writeObject(fmt, ver, cmp);

        if (writeOK && purgeWrite_)
        {
            previousOutputTimes_.push(tmName);

            while (previousOutputTimes_.size() > purgeWrite_)
            {
                rmDir(objectRegistry::path(previousOutputTimes_.pop()));
            }
        }

        return writeOK;
    }
    else
    {
        return false;
    }
}


bool CML::Time::writeNow()
{
    outputTime_ = true;
    return write();
}


bool CML::Time::writeAndEnd()
{
    stopAt_  = saWriteNow;
    endTime_ = value();

    return writeNow();
}


void CML::Time::writeOnce()
{
    writeOnce_ = true;
}


// ************************************************************************* //
