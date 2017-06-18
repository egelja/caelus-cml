/*---------------------------------------------------------------------------*\
Copyright (C) 2009-2016 Bernhard Gschaider
Copyright (C) 2016 OpenCFD Ltd.
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
    CML::profiling

Description
    Code profiling.

    This is typically activated from within the system/controlDict as follows:
    \code
        profiling
        {
            active      true;  // default: true
            sysInfo     true;  // default: true
        }
    \endcode
    or simply using all defaults:
    \code
        profiling
        {}
    \endcode

SourceFiles
    profiling.cpp

\*---------------------------------------------------------------------------*/

#ifndef profiling_HPP
#define profiling_HPP

#include "HashPtrTable.hpp"
#ifdef windows
#undef DebugInfo
#endif
#include "LIFOStack.hpp"
#include "Map.hpp"
#include "Time.hpp"
#include "clockTime.hpp"

namespace CML
{

// Forward declaration of classes
class Ostream;
class dictionary;

/*---------------------------------------------------------------------------*\
                          Class profiling Declaration
\*---------------------------------------------------------------------------*/

class profiling
:
    public regIOobject
{
public:

    // Forward declarations of components

        class Information;
        class Trigger;
        class sysInfo;

private:

    // Private Static Data Members

        //- Only one global pool object is possible
        static profiling *pool_;


    // Private Data Members

        typedef HashPtrTable<Information, string> StorageContainer;
        typedef LIFOStack<Information*> StackContainer;


        //- The owner of the profiling
        const Time& owner_;

        //- A global timer for the profiling
        clockTime clockTime_;

        //- Storage of profiling information
        StorageContainer hash_;

        //- Local stack of profiling information
        StackContainer stack_;

        //- Note the timers (by Id) for the correct stack-output
        Map<clockTime*> timers_;

        //- General system information (optional)
        sysInfo* sysInfo_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        profiling(const profiling&);

        //- Disallow default bitwise assignment
        void operator=(const profiling&);


protected:

    // Friendship

        friend class Time;


    // Constructors

        //- Construct IO object, everything enabled
        profiling(const IOobject&, const Time&);

        //- Construct IO object with finer control over behaviour
        profiling(const dictionary&, const IOobject&, const Time&);


    //- Destructor
    ~profiling();


    // Protected Member Functions

        //- Find profiling information element or null on failure
        Information* find(const string& name);

        //- Add to hashed storage,
        //  returns pointer to newly stored element for chaining
        Information* store(Information*);

        //- Add to stack and set timer lookup (based on Id)
        void push(Information*, clockTime& timer);

        //- Remove from stack and remove timer lookup (based on Id).
        //  Returns pointer to profiling information element
        Information* pop();


    // Static control elements

        //- Singleton to initialize profiling pool, everything enabled
        static void initialize(const IOobject&, const Time&);

        //- Singleton to initialize profiling pool with finer control
        static void initialize(const dictionary&, const IOobject&, const Time&);

        //- Stop profiling, cleanup pool if possible
        static void stop(const Time&);

        //- Existing or new element on pool, add to stack.
        //  Returns null if profiling has not been initialized
        static Information* New(const string& name, clockTime& timer);

        //- Remove the information from the top of the stack
        static void unstack(const Information*);

public:

    // Static Member Functions

        //- True if profiling is active
        static bool active();

        //- Print profiling information to specified output
        //  Forwards to writeData member of top-level object
        static bool print(Ostream&);

        //- Write profiling information now
        static bool writeNow();

        //- The owner of the profiling
        const Time& owner() const;

        //- The size of the current stack
        CML::label size() const;

        //- writeData member function required by regIOobject
        virtual bool writeData(Ostream&) const;

        //- Write as uncompressed ASCII, using given format
        virtual bool writeObject
        (
            IOstream::streamFormat ignoreAlwaysASCII,
            IOstream::versionNumber ver,
            IOstream::compressionType ignoreAlwaysUncompressed
        ) const;

};


/*---------------------------------------------------------------------------*\
                   Class profiling::Information Declaration
\*---------------------------------------------------------------------------*/

class profiling::Information
{
    // Private Static Data Members

        //- Counter to generate the ids
        static label nextId_;

        //- get a new ID and update the counter
        static label getNextId();

        //- raise the next possible ID (to avoid ID-clashes during reading)
        static void raiseID(label maxVal);


    // Private Data Members

        //- Unique id to identify it
        const label id_;

        //- What this timer does
        const string description_;

        //- Pointer to the parent object (or self for top-level)
        Information* parent_;

        //- Nr of times this was called
        long calls_;

        //- Total time spent
        scalar totalTime_;

        //- Time spent in children
        scalar childTime_;

        //- Is this information currently on the stack?
        mutable bool onStack_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        Information(const Information&);

        //- Disallow default bitwise assignment
        void operator=(const Information&);


protected:

    // Friendship

        friend class profiling;


    // Constructors

        //- Construct null - only the master-element
        Information();


    // Member Functions

        //- Mark as being on the stack
        void push() const;

        //- Mark as being off the stack
        void pop() const;


        //- Write the profiling times, optionally with additional values
        //  Use dictionary format.
        Ostream& write
        (
            Ostream& os,
            const bool offset = false,
            const scalar& elapsedTime = 0,
            const scalar& childTime = 0
        ) const;

public:


    // Constructors

        //- Construct from components
        Information(Information* parent, const string& descr);


    //- Destructor
    ~Information();


    // Member Functions

    // Access

        inline label id() const
        {
            return id_;
        }


        inline const string& description() const
        {
            return description_;
        }


        inline Information& parent() const
        {
            return *parent_;
        }


        inline label calls() const
        {
            return calls_;
        }


        inline const scalar& totalTime() const
        {
            return totalTime_;
        }


        inline const scalar& childTime() const
        {
            return childTime_;
        }


        inline bool onStack() const
        {
            return onStack_;
        }


    // Edit

        //- Update it with a new timing information
        void update(const scalar& elapsedTime);


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const Information&);

};


/*---------------------------------------------------------------------------*\
                      Class profiling::Trigger Declaration
\*---------------------------------------------------------------------------*/

class profiling::Trigger
{
    // Private Data Members

        //- The timer for the profiling information
        clockTime clock_;

        //- The profiling information
        Information *ptr_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        Trigger(const Trigger&);

        //- Disallow default bitwise assignment
        void operator=(const Trigger&);


public:

    // Constructors

        //- Construct profiling with given description.
        //  Descriptions beginning with 'application::' are reserved for
        //  internal use.
        Trigger(const char* name);

        //- Construct profiling with given description.
        //  Descriptions beginning with 'application::' are reserved for
        //  internal use.
        Trigger(const string& name);


    //- Destructor
    ~Trigger();


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
#define addProfiling(name,descr)                                              \
    ::CML::profiling::Trigger  profilingTriggerFor##name(descr)

//- Define profiling with specified name and description correspond to the name
//  \sa addProfiling
//  \sa endProfiling
#define addProfiling0(name)                                                    \
    ::CML::profiling::Trigger  profilingTriggerFor##name(#name)

//- Define profiling with specified name and description correspond to the
//  compiler-defined function name string:
//  \sa addProfiling
//  \sa endProfiling
#ifdef __GNUC__
    #define addProfilingInFunction(name)                                       \
    ::CML::profiling::Trigger  profilingTriggerFor##name(__PRETTY_FUNCTION__)
#else
    #define addProfilingInFunction(name)                                       \
    ::CML::profiling::Trigger  profilingTriggerFor##name(__func__)
#endif

//- Remove profiling with specified name
//  \sa addProfiling
//  \sa addProfiling0
#define endProfiling(name)      profilingTriggerFor##name.stop()


#endif

// ************************************************************************* //
