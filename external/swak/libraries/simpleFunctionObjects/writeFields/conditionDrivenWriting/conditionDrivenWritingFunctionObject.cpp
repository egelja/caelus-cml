/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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

Contributors/Copyright:
    2008-2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "conditionDrivenWritingFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

#include "DebugOStream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(conditionDrivenWritingFunctionObject, 0);

    template<>
    const char* NamedEnum<CML::conditionDrivenWritingFunctionObject::writeControlModeType,4>::names[]=
    {
        "always",
        "timesteps",
        "intervall",
        "untilSwitch"
    };
    const NamedEnum<conditionDrivenWritingFunctionObject::writeControlModeType,4> conditionDrivenWritingFunctionObject::writeControlModeTypeNames_;

    template<>
    const char* NamedEnum<CML::conditionDrivenWritingFunctionObject::cooldownModeType,4>::names[]=
    {
        "no",
        "timesteps",
        "intervall",
        "retrigger"
    };
    const NamedEnum<conditionDrivenWritingFunctionObject::cooldownModeType,4> conditionDrivenWritingFunctionObject::cooldownModeTypeNames_;



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

conditionDrivenWritingFunctionObject::conditionDrivenWritingFunctionObject
(
    const word &name,
    const Time& t,
    const dictionary& dict
)
:
    simpleFunctionObject(name,t,dict),
    writeControlMode_(
        writeControlModeTypeNames_[
            word(dict.lookup("writeControlMode"))
        ]
    ),
    cooldownMode_(
        cooldownModeTypeNames_[
            word(dict.lookup("cooldownMode"))
        ]
    ),
    storeAndWritePreviousState_(
        readBool(
            dict.lookup("storeAndWritePreviousState")
        )
    ),
    theState_(stateWaiting),
    writeTimesteps_(-1),
    writeIntervall_(-1),
    cooldownTimesteps_(-1),
    cooldownIntervall_(-1),
    timestepForStateChange_(t.timeIndex()),
    timeForStateChange_(t.value())
{
    switch(writeControlMode_) {
        case scmWriteNTimesteps:
            writeTimesteps_=readLabel(dict.lookup("writeTimesteps"));
            break;
        case scmWriteIntervall:
            writeIntervall_=readScalar(dict.lookup("writeIntervall"));
            break;
        case scmWriteUntilSwitch:
            // to be implemented by subclass
            break;
        default:
            // do nothing
            break;
    }
    switch(cooldownMode_) {
        case cdmNTimesteps:
            cooldownTimesteps_=readLabel(dict.lookup("cooldownTimesteps"));
            break;
        case cdmIntervall:
            cooldownIntervall_=readScalar(dict.lookup("cooldownIntervall"));
            break;
        case cdmRetrigger:
            // to be implemented by subclass
            break;
        default:
            // do nothing
            break;
    }
    if(storeAndWritePreviousState_) {
        lastTimes_.set(
            new TimeCloneList(
                dict
            )
        );
    }
}

bool conditionDrivenWritingFunctionObject::checkStopWriting()
{
    FatalErrorInFunction
        << name() << " does not implement 'untilSwitch' for 'writeControlMode'"
            << endl
            << exit(FatalError);

    return false;
}

bool conditionDrivenWritingFunctionObject::checkStopCooldown()
{
    FatalErrorInFunction
        << name() << " does not implement 'retrigger' for 'cooldownMode'"
            << endl
            << exit(FatalError);

    return false;
}

bool conditionDrivenWritingFunctionObject::alreadyWritten(word timename)
{
    // TODO: implement properly
    return false;
}

void conditionDrivenWritingFunctionObject::storePreviousState()
{
    if(lastTimes_.valid()) {
        lastTimes_->copy(time());
    } else {
        WarningInFunction
            << "Logic error"
                << endl;
    }
}

void conditionDrivenWritingFunctionObject::writePreviousState()
{
    if(lastTimes_.valid()) {
        lastTimes_->write();
    } else {
        WarningInFunction
            << "Logic error"
                << endl;
    }
}

void conditionDrivenWritingFunctionObject::writeNow()
{
    Info << name() << " triggered writing of t=" << time().timeName() << endl;
    if(!alreadyWritten(time().timeName())) {
        Info << name() << ": Writing ...." << endl;
        bool result=const_cast<Time&>(time()).writeNow();
        Dbug << "Write now " << result << endl;
    } else {
        Info << name() << ": Already written. Skipping" << endl;
    }

    if(storeAndWritePreviousState_) {
        Info << name() << ": Attempt to write previous timestep" << endl;
        writePreviousState();
    }
}

bool conditionDrivenWritingFunctionObject::start()
{
    simpleFunctionObject::start();

    if(cooldownMode_==cdmRetrigger) {
        // provoke 'not implemented'
        bool result=checkStopCooldown();
        Dbug << "Stop cooldown: " << result << endl;
    }

    if(writeControlMode_==scmWriteUntilSwitch) {
        // provoke 'not implemented'
        bool result=checkStopWriting();
        Dbug << "Stop writing: " << result << endl;
    }

    if(storeAndWritePreviousState_) {
        storePreviousState();
    }

    return true;
}

bool conditionDrivenWritingFunctionObject::checkCooldown()
{
    switch(cooldownMode_) {
        case cdmNoCooldown:
            return true;
        case cdmNTimesteps:
            return time().timeIndex()>=timestepForStateChange_;
            break;
        case cdmIntervall:
            return time().value()>timeForStateChange_;
            break;
        case cdmRetrigger:
            return checkStopCooldown();
            break;
        default:
            FatalErrorInFunction
                << "Unsupported cooldown mode"
                    << endl
                    << exit(FatalError);
    }
    return false;
}

bool conditionDrivenWritingFunctionObject::checkWrite()
{
    if(checkStartWriting()) {
        Info << name() << " starts writing" << endl;
        theState_=stateWriting;
        switch(writeControlMode_) {
            case scmWriteAlways:
                break;
            case scmWriteNTimesteps:
                Info << name() << ": Writing for " << writeTimesteps_
                    << endl;
                timestepForStateChange_=time().timeIndex()+writeTimesteps_-1;
                break;
            case scmWriteIntervall:
                timeForStateChange_=time().value()+writeIntervall_;
                Info << name() << ": Writing till " << timeForStateChange_
                    << endl;
                break;
            case scmWriteUntilSwitch:
                Info << name() << ": writing until switched off"
                    << endl;
                break;
            default:
                FatalErrorInFunction
                    << "Unimplemented 'writeControlMode'"
                        << endl
                        << exit(FatalError);
        }
        return true;
    } else {
        return false;
    }
}

void conditionDrivenWritingFunctionObject::write()
{
    bool doWrite=false;

    switch(theState_) {
        case stateWaiting:
            doWrite=checkWrite();
            break;
        case stateWriting:
            switch(writeControlMode_) {
                case scmWriteAlways:
                    doWrite=checkStartWriting();
                    break;
                case scmWriteNTimesteps:
                    doWrite=(time().timeIndex()<=timestepForStateChange_);
                    break;
                case scmWriteIntervall:
                    doWrite=(time().value()<=timeForStateChange_);
                    break;
                case scmWriteUntilSwitch:
                    doWrite=!checkStopWriting();
                    break;
                default:
                    FatalErrorInFunction
                        << "Unimplemented 'writeControlMode'"
                            << endl
                            << exit(FatalError);
            }
            if(!doWrite) {
                Info << name() << ": Stopped writting. Starting cooldown" << endl;
                theState_=stateStartCooldown;
            }
            break;
        case stateCooldown:
            {
                bool stopCooldown=checkCooldown();
                if(stopCooldown) {
                    Info << name() << " cooldown ended. Writing possible" << endl;
                    theState_=stateWaiting;
                    doWrite=checkWrite();
                }
            }
            break;
        case stateStartCooldown:
            FatalErrorInFunction
                << "State 'startCooldown' should not be reached here"
                    << endl
                    << exit(FatalError);
            break;
        default:
            FatalErrorInFunction
                << "Unsupported state"
                    << endl
                    << exit(FatalError);
            break;
    }

    if(theState_==stateStartCooldown) {
        theState_=stateCooldown;
        switch(cooldownMode_) {
            case cdmNoCooldown:
                theState_=stateWaiting;
                break;
            case cdmNTimesteps:
                Info << name() << ": Waiting for " << cooldownTimesteps_
                    << " till next output" << endl;
                timestepForStateChange_=time().timeIndex()+cooldownTimesteps_;
                break;
            case cdmIntervall:
                timeForStateChange_=time().value()+cooldownIntervall_;
                Info << name() << ": Waiting till " << timeForStateChange_
                    << " till next output" << endl;
                break;
            case cdmRetrigger:
                Info << name() << ": waiting until triggered for next output"
                    << endl;
                break;
            default:
                FatalErrorInFunction
                    << "Unsupported cooldown mode"
                        << endl
                        << exit(FatalError);
        }
        bool stopCooldown=checkCooldown();
        if(stopCooldown) {
            Info << name() << " cooldown ended. Writing possible" << endl;
            theState_=stateWaiting;
            doWrite=checkWrite();
        }
    }

    if(doWrite) {
        writeNow();
    } else {
        // don't need to store the state if it was already written
        if(storeAndWritePreviousState_) {
            storePreviousState();
        }
    }
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

} // namespace CML

// ************************************************************************* //
