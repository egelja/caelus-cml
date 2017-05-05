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

Class
    conditionDrivenWritingFunctionObject

Description
    General class for functionObjects that write timesteps if certain conditions
    are met

SourceFiles
    conditionDrivenWritingFunctionObject.cpp

Contributors/Copyright:
    2008-2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef conditionDrivenWritingFunctionObject_H
#define conditionDrivenWritingFunctionObject_H

#include "simpleFunctionObject.hpp"

#include "NamedEnum.hpp"

#include "TimeCloneList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class conditionDrivenWritingFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class conditionDrivenWritingFunctionObject
:
    public simpleFunctionObject
{
public:
    enum writeControlModeType {
        scmWriteAlways,
        scmWriteNTimesteps,
        scmWriteIntervall,
        scmWriteUntilSwitch
    };

    enum cooldownModeType {
        cdmNoCooldown,
        cdmNTimesteps,
        cdmIntervall,
        cdmRetrigger
    };

private:
    // Private Member Functions

    //- Disallow default bitwise copy construct
    conditionDrivenWritingFunctionObject(const conditionDrivenWritingFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const conditionDrivenWritingFunctionObject&);

    enum theStateType {
        stateWaiting,
        stateWriting,
        stateStartCooldown,
        stateCooldown
    };

    static const NamedEnum<writeControlModeType,4> writeControlModeTypeNames_;
    writeControlModeType writeControlMode_;

    static const NamedEnum<cooldownModeType,4> cooldownModeTypeNames_;
    cooldownModeType cooldownMode_;

    bool storeAndWritePreviousState_;

    theStateType theState_;

    label writeTimesteps_;
    scalar writeIntervall_;
    label cooldownTimesteps_;
    scalar cooldownIntervall_;

    label timestepForStateChange_;
    scalar timeForStateChange_;

    void storePreviousState();

    void writePreviousState();

    void writeNow();

    bool alreadyWritten(word timename);

    bool checkWrite();

    bool checkCooldown();

    autoPtr<TimeCloneList> lastTimes_;

protected:
    //- should writing start now?
    virtual bool checkStartWriting() =0;

    //- switch off writing? Does not have to be implemented
    virtual bool checkStopWriting();

    //- switch off cooldown? Does not have to be implemented
    virtual bool checkStopCooldown();

    cooldownModeType cooldownMode() { return cooldownMode_; }

    writeControlModeType writeControlMode() { return writeControlMode_; }

public:

    //- Runtime type information
    TypeName("conditionDrivenWriting");


    // Constructors

    //- Construct from components
    conditionDrivenWritingFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );

    bool start();

    void write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
