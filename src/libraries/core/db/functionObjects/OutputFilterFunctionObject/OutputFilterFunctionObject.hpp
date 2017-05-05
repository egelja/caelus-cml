/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

Class
    CML::OutputFilterFunctionObject

Description
    A functionObject wrapper around OutputFilter to allow them to be
    created via the functions entry within controlDict.

Note
    Since the timeIndex is used directly from CML::Time, it is unaffected
    by user-time conversions. For example, CML::engineTime might cause \a
    writeInterval to be degrees crank angle, but the functionObject
    execution \a interval would still be in timestep.


\*---------------------------------------------------------------------------*/

#ifndef OutputFilterFunctionObject_H
#define OutputFilterFunctionObject_H

#include "functionObject.hpp"
#include "dictionary.hpp"
#include "outputFilterOutputControl.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class OutputFilterFunctionObject Declaration
\*---------------------------------------------------------------------------*/

template<class OutputFilter>
class OutputFilterFunctionObject
:
    public functionObject
{
    // Private data

        //- Reference to the time database
        const Time& time_;

        //- Input dictionary
        dictionary dict_;


        // Optional user inputs

            //- Name of region - defaults to name of polyMesh::defaultRegion
            word regionName_;

            //- Dictionary name to supply required inputs
            word dictName_;

            //- Switch for the execution - defaults to 'yes/on'
            bool enabled_;

            //- Switch to store filter in between writes or use on-the-fly
            //  construction - defaults to true
            bool storeFilter_;

            //- Activation time - defaults to -VGREAT
            scalar timeStart_;

            //- De-activation time - defaults to VGREAT
            scalar timeEnd_;


        //- Output controls
        outputFilterOutputControl outputControl_;

        //- Pointer to the output filter
        autoPtr<OutputFilter> ptr_;


    // Private Member Functions

        //- Read relevant dictionary entries
        void readDict();

        //- Creates most of the data associated with this object.
        void allocateFilter();

        //- Destroys most of the data associated with this object.
        void destroyFilter();

        //- Returns true if active (enabled and within time bounds)
        bool active() const;

        //- Disallow default bitwise copy construct
        OutputFilterFunctionObject(const OutputFilterFunctionObject&);

        //- Disallow default bitwise assignment
        void operator=(const OutputFilterFunctionObject&);


public:

    //- Runtime type information
    TypeName(OutputFilter::typeName_());


    // Constructors

        //- Construct from components
        OutputFilterFunctionObject
        (
            const word& name,
            const Time&,
            const dictionary&
        );


    // Member Functions

        // Access

            //- Return time database
            virtual const Time& time() const
            {
                return time_;
            }

            //- Return the input dictionary
            virtual const dictionary& dict() const
            {
                return dict_;
            }

            //- Return the region name
            virtual const word& regionName() const
            {
                return regionName_;
            }

            //- Return the optional dictionary name
            virtual const word& dictName() const
            {
                return dictName_;
            }

            //- Return the enabled flag
            virtual bool enabled() const
            {
                return enabled_;
            }

            //- Return the output control object
            virtual const outputFilterOutputControl& outputControl() const
            {
                return outputControl_;
            }

            //- Return the output filter
            virtual const OutputFilter& outputFilter() const
            {
                return ptr_();
            }


        // Function object control

            //- Switch the function object on
            virtual void on();

            //- Switch the function object off
            virtual void off();


            //- Called at the start of the time-loop
            virtual bool start();

            //- Called at each ++ or += of the time-loop
            virtual bool execute(const bool forceWrite);

            //- Called when Time::run() determines that the time-loop exits
            virtual bool end();


            //- Read and set the function object if its data have changed
            virtual bool read(const dictionary&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IOOutputFilter.hpp"
#include "polyMesh.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * Private Members * * * * * * * * * * * * * * //

template<class OutputFilter>
void CML::OutputFilterFunctionObject<OutputFilter>::readDict()
{
    dict_.readIfPresent("region", regionName_);
    dict_.readIfPresent("dictionary", dictName_);
    dict_.readIfPresent("enabled", enabled_);
    dict_.readIfPresent("storeFilter", storeFilter_);
    dict_.readIfPresent("timeStart", timeStart_);
    dict_.readIfPresent("timeEnd", timeEnd_);
}


template<class OutputFilter>
bool CML::OutputFilterFunctionObject<OutputFilter>::active() const
{
    return
        enabled_
     && time_.value() >= timeStart_
     && time_.value() <= timeEnd_;
}


template<class OutputFilter>
void CML::OutputFilterFunctionObject<OutputFilter>::allocateFilter()
{
    if (dictName_.size())
    {
        ptr_.reset
        (
            new IOOutputFilter<OutputFilter>
            (
                name(),
                time_.lookupObject<objectRegistry>(regionName_),
                dictName_
            )
        );
    }
    else
    {
        ptr_.reset
        (
            new OutputFilter
            (
                name(),
                time_.lookupObject<objectRegistry>(regionName_),
                dict_
            )
        );
    }
}


template<class OutputFilter>
void CML::OutputFilterFunctionObject<OutputFilter>::destroyFilter()
{
    ptr_.reset();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class OutputFilter>
CML::OutputFilterFunctionObject<OutputFilter>::OutputFilterFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    functionObject(name),
    time_(t),
    dict_(dict),
    regionName_(polyMesh::defaultRegion),
    dictName_(),
    enabled_(true),
    storeFilter_(true),
    timeStart_(-VGREAT),
    timeEnd_(VGREAT),
    outputControl_(t, dict)
{
    readDict();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class OutputFilter>
void CML::OutputFilterFunctionObject<OutputFilter>::on()
{
    enabled_ = true;
}


template<class OutputFilter>
void CML::OutputFilterFunctionObject<OutputFilter>::off()
{
    enabled_ = false;
}


template<class OutputFilter>
bool CML::OutputFilterFunctionObject<OutputFilter>::start()
{
    readDict();

    if (enabled_ && storeFilter_)
    {
        allocateFilter();
    }

    return true;
}


template<class OutputFilter>
bool CML::OutputFilterFunctionObject<OutputFilter>::execute
(
    const bool forceWrite
)
{
    if (active())
    {
        if (!storeFilter_)
        {
            allocateFilter();
        }

        ptr_->execute();

        if (forceWrite || outputControl_.output())
        {
            ptr_->write();
        }

        if (!storeFilter_)
        {
            destroyFilter();
        }
    }

    return true;
}


template<class OutputFilter>
bool CML::OutputFilterFunctionObject<OutputFilter>::end()
{
    // note: use enabled_ here instead of active() since end should be called
    // even if out of time bounds
    if (enabled_)
    {
        if (!storeFilter_)
        {
            allocateFilter();
        }

        ptr_->end();

        if (outputControl_.output())
        {
            ptr_->write();
        }

        if (!storeFilter_)
        {
            destroyFilter();
        }
    }

    return true;
}


template<class OutputFilter>
bool CML::OutputFilterFunctionObject<OutputFilter>::read
(
    const dictionary& dict
)
{
    if (dict != dict_)
    {
        dict_ = dict;
        outputControl_.read(dict);

        return start();
    }
    else
    {
        return false;
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
