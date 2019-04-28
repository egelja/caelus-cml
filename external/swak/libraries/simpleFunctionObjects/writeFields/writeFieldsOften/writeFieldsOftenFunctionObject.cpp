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
    2008-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "writeFieldsOftenFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(writeFieldsOftenFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        writeFieldsOftenFunctionObject,
        dictionary
    );


    // copied from Time.C because the original is protected
    // to work the order of values in writeControls must not change
template<>
const char* NamedEnum<CML::Time::writeControls, 5>::names[] =
{
    "timeStep",
    "runTime",
    "adjustableRunTime",
    "clockTime",
    "cpuTime"
};

const NamedEnum<CML::Time::writeControls, 5> writeControlNames;

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

writeFieldsOftenFunctionObject::writeFieldsOftenFunctionObject
(
    const word &name,
    const Time& t,
    const dictionary& dict
)
:
    writeFieldsGeneralFunctionObject(name,t,dict),
    writeControl_(Time::wcTimeStep),
    writeInterval_(GREAT),
    outputTimeIndex_(0)
{
}

bool writeFieldsOftenFunctionObject::start()
{
    writeFieldsGeneralFunctionObject::start();

    word wcName=dict_.lookup("writeControl");

    writeControl_ = writeControlNames.read
        (
            dict_.lookup("writeControl")
        );

    writeInterval_ = readScalar(dict_.lookup("writeIntervall"));
    if(writeControl_ == Time::wcTimeStep && label(writeInterval_) <1) {
        WarningInFunction
            << "writeInterval " << writeInterval_
                << " < 1 for writeControl timeStep. Reseting to 1 "<< endl;
        writeInterval_=1;
    }

    Info << "Additional fields " << fieldNames() << " will be written "
        << "with writeControl " << wcName << " and intervall " << writeInterval_ << endl;

    if(writeControl_ == Time::wcAdjustableRunTime) {
        WarningInFunction
            << "Cant adjust the run-time. Defaulting to runTime" << endl;

    }

    outputTimeIndex_=0;

    return true;
}

bool writeFieldsOftenFunctionObject::outputTime(const bool forceWrite)
{
    if(forceWrite) {
        return true;
    }

    if(time().time().value()<after()) {
        return false;
    }

    bool writeNow=false;

    // lifted from Time::operator++
    switch(writeControl_){
        case Time::wcTimeStep:
            writeNow = !(time().timeIndex()%label(writeInterval_));
            break;

        case Time::wcRunTime:
        case Time::wcAdjustableRunTime:
            {
                label outputTimeIndex =
                    label(((time().time().value() - time().startTime().value()) + 0.5*time().deltaT().value())/writeInterval_);

                if (outputTimeIndex > outputTimeIndex_)
                {
                    writeNow = true;
                    outputTimeIndex_ = outputTimeIndex;
                }
                else
                {
                    writeNow = false;
                }
            }
        break;

        case Time::wcCpuTime:
            {
                label outputTimeIndex =
                    label(time().elapsedCpuTime()/writeInterval_);

                if (outputTimeIndex > outputTimeIndex_)
                {
                    writeNow = true;
                    outputTimeIndex_ = outputTimeIndex;
                }
                else
                {
                    writeNow = false;
                }
            }
        break;

        case Time::wcClockTime:
            {
                label outputTimeIndex = label(time().elapsedClockTime()/writeInterval_);
                if (outputTimeIndex > outputTimeIndex_)
                {
                    writeNow = true;
                    outputTimeIndex_ = outputTimeIndex;
                }
                else
                {
                    writeNow = false;
                }
            }
        break;
        };

    return writeNow;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

} // namespace CML

// ************************************************************************* //
