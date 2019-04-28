/*---------------------------------------------------------------------------*\
Copyright (C) 2009-2016 Bernhard Gschaider
Copyright (C) 2016-2017 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::profilingTrigger

Description
    Triggers for starting/stopping code profiling.

SourceFiles
    profilingTrigger.cpp

\*---------------------------------------------------------------------------*/

#ifndef profilingTrigger_HPP
#define profilingTrigger_HPP

#include "clockTime.hpp"
#include "string.hpp"

namespace CML
{

// Forward declaration of classes
class profilingInformation;

/*---------------------------------------------------------------------------*\
                      Class profilingTrigger Declaration
\*---------------------------------------------------------------------------*/

class profilingTrigger
{
    // Private Data Members

        //- The timer for the profiling information
        clockTime clock_;

        //- The profiling information
        profilingInformation *ptr_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        profilingTrigger(const profilingTrigger&) = delete;

        //- Disallow default bitwise assignment
        void operator=(const profilingTrigger&) = delete;


public:

    // Constructors

        //- Construct profiling with given description.
        //  Descriptions beginning with 'application::' are reserved for
        //  internal use.
        profilingTrigger(const char* name);

        //- Construct profiling with given description.
        //  Descriptions beginning with 'application::' are reserved for
        //  internal use.
        profilingTrigger(const string& name);


    //- Destructor
    ~profilingTrigger();


    // Member Functions

      // Access

        //- True if the triggered profiling is active
        bool running() const;


      // Edit

        //- Stop triggered profiling
        void stop();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Macros

//- Define profiling with specified name and description string
//  This is required if the description contains space, colons etc.
//  \sa addProfiling0
//  \sa endProfiling
#define addProfiling(name,descr)                                               \
    ::CML::profilingTrigger  profilingTriggerFor##name(descr)

//- Define profiling with specified name and description correspond to the name
//  \sa addProfiling
//  \sa endProfiling
#define addProfiling0(name)                                                    \
    ::CML::profilingTrigger  profilingTriggerFor##name(#name)

//- Define profiling with specified name and description correspond to the
//  compiler-defined function name string:
//  \sa addProfiling
//  \sa endProfiling
#ifdef __GNUC__
    #define addProfilingInFunction(name)                                       \
    ::CML::profilingTrigger  profilingTriggerFor##name(__PRETTY_FUNCTION__)
#else
    #define addProfilingInFunction(name)                                       \
    ::CML::profilingTrigger  profilingTriggerFor##name(__func__)
#endif

//- Remove profiling with specified name
//  \sa addProfiling
//  \sa addProfiling0
#define endProfiling(name)    profilingTriggerFor##name.stop()


#endif

// ************************************************************************* //
