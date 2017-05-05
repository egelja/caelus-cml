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
    writeOldTimesOnSignalFunctionObject

Description
    This function object tries to record old fields and writes them in
    case of a Signal

SourceFiles
    writeOldTimesOnSignalFunctionObject.cpp

Contributors/Copyright:
    2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef writeOldTimesOnSignalFunctionObject_H
#define writeOldTimesOnSignalFunctionObject_H

#include "functionObject.hpp"
#include "dictionary.hpp"
#include "TimeCloneList.hpp"
#include "DynamicList.hpp"

#include <signal.h>

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class writeOldTimesOnSignalFunctionObject Declaration
\*---------------------------------------------------------------------------*/

    class objectRegistry;

class writeOldTimesOnSignalFunctionObject
:
    public functionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    writeOldTimesOnSignalFunctionObject(const writeOldTimesOnSignalFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const writeOldTimesOnSignalFunctionObject&);

    TimeCloneList times_;

    const Time &theTime_;

    class SignalHandlerInfo {
    protected:
        struct sigaction oldAction_;
        word name_;
        int sig_;
        bool set_;
    public:
        SignalHandlerInfo(
            word name,
            int sig
        );

        SignalHandlerInfo()
            :
            name_("nix"),
            sig_(-1),
            set_(false)
            {}

        void resetHandler();

        ~SignalHandlerInfo() {};
    };

    static void sigHandler(int);

    static writeOldTimesOnSignalFunctionObject *singleton_;

    DynamicList<SignalHandlerInfo> handlers_;

    bool writeCurrent_;

    bool sigFPE_;
    bool sigSEGV_;
    bool sigINT_;
    bool sigQUIT_;

public:

    //- Runtime type information
    TypeName("writeOldTimesOnSignal");


    // Constructors

    //- Construct from components
    writeOldTimesOnSignalFunctionObject
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

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
