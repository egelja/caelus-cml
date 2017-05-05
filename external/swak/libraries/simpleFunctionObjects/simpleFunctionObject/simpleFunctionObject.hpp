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
    simpleFunctionObject

Description
    Basis for the other function objects here. Evaluates the most common options

SourceFiles
    simpleFunctionObject.cpp

Contributors/Copyright:
    2008-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef simpleFunctionObject_H
#define simpleFunctionObject_H

#include "functionObject.hpp"
#include "dictionary.hpp"
#include "OFstream.hpp"
#include "HashPtrTable.hpp"
#include "boolList.hpp"
#include "Field.hpp"
#include "NamedEnum.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"
#include "switchableNotImplemented.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class simpleFunctionObject Declaration
\*---------------------------------------------------------------------------*/

    class objectRegistry;

class simpleFunctionObject
:
    public functionObject
{
public:
    enum outputControlModeType {
        ocmTimestep,
        ocmDeltaT,
        ocmOutputTime,
        ocmStartup
    };
private:
    // Private Member Functions

    //- Disallow default bitwise copy construct
    simpleFunctionObject(const simpleFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const simpleFunctionObject&);

    // Private data

    //- Do output on the terminal
    bool verbose_;

    //- Write debug-information to the terminal
    bool writeDebug_;

    //- Time after which calculation should start
    scalar after_;

    //- how many time-steps since last output
    label timeSteps_;

    //- possible outputControl-modes
    static const NamedEnum<outputControlModeType,4> outputControlModeTypeNames_;
    outputControlModeType outputControlMode_;

    //- how many time-steps between outputs
    label outputInterval_;

    //- deltaT for output
    scalar outputDeltaT_;

    const Time& time_;

    scalar lastWrite_;

protected:

    outputControlModeType outputControlMode() const
        { return outputControlMode_; }

    dictionary dict_;

    word regionName_;

    string regionString_;

    const char *regionString() const { return regionString_.c_str(); }

    const objectRegistry& obr_;

    //- Do output on the terminal
    bool verbose() const { return verbose_; }

    //- Write debug-information to tthe terminal
    bool writeDebug() const { return writeDebug_; }

    //- Do output this time?
    virtual bool outputTime(const bool forceWrite);

public:

    //- Runtime type information
    TypeName("simpleFunctionObject");


    // Constructors

    //- Construct from components
    simpleFunctionObject
    (
        const word&,
        const Time&,
        const dictionary&
    );


    // Member Functions

    //- start is called at the start of the time-loop
    virtual bool start();

    //- execute is called at each ++ or += of the time-loop
    virtual bool execute(const bool forceWrite);

    //- Read and set the function object if it's data has changed
    virtual bool read(const dictionary& dict);

    //- Sample and write
    virtual void write()=0;

    //- Optional stuff to do after write
    virtual void flush();

    //- the actual time
    const Time &time() const { return time_; }

    //- the actual time
    scalar after() const { return after_; }

    const objectRegistry &obr() const { return obr_; }

    const dictionary &dict() const { return dict_; }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
