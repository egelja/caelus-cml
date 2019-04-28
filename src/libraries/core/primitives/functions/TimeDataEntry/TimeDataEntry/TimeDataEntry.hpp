/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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
    CML::TimeDataEntry

Description
    Light wrapper around DataEntry to provide a mechanism to update time-based
    entries.

SourceFiles
    TimeDataEntry.cpp

\*---------------------------------------------------------------------------*/

#ifndef TimeDataEntry_H
#define TimeDataEntry_H

#include "DataEntry.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
template<class Type>
class TimeDataEntry;

template<class Type>
Ostream& operator<<
(
    Ostream&,
    const TimeDataEntry<Type>&
);

/*---------------------------------------------------------------------------*\
                       Class TimeDataEntry Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class TimeDataEntry
{

protected:

    // Protected data

        //- Reference to the time database
        const Time& time_;

        //- Name of the data entry
        const word name_;

        //- The underlying DataEntry
        autoPtr<DataEntry<Type> > entry_;


public:

    // Constructor

        //- Construct from entry name
        TimeDataEntry
        (
            const Time& t,
            const word& name,
            const dictionary& dict
        );

        //- Construct null from entry name
        TimeDataEntry
        (
            const Time& t,
            const word& entryName
        );

        //- Copy constructor
        TimeDataEntry(const TimeDataEntry<Type>& tde);


    //- Destructor
    virtual ~TimeDataEntry();


    // Member Functions

        // Access

            //- Reset entry by re-reading from dictionary
            void reset(const dictionary& dict);

            //- Return the name of the entry
            const word& name() const;


        // Evaluation

            //- Return value as a function of (scalar) independent variable
            virtual Type value(const scalar x) const;

            //- Integrate between two (scalar) values
            virtual Type integrate(const scalar x1, const scalar x2) const;


    // I/O

        //- Ostream Operator
        friend Ostream& operator<< <Type>
        (
            Ostream& os,
            const TimeDataEntry<Type>& de
        );

        //- Write in dictionary format
        virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * //

template<class Type>
CML::TimeDataEntry<Type>::TimeDataEntry
(
    const Time& t,
    const word& name,
    const dictionary& dict
)
:
    time_(t),
    name_(name),
    entry_(DataEntry<Type>::New(name, dict))
{
    entry_->convertTimeBase(t);
}


template<class Type>
CML::TimeDataEntry<Type>::TimeDataEntry(const Time& t, const word& name)
:
    time_(t),
    name_(name),
    entry_(nullptr)
{}


template<class Type>
CML::TimeDataEntry<Type>::TimeDataEntry
(
    const TimeDataEntry<Type>& tde
)
:
    time_(tde.time_),
    name_(tde.name_),
    entry_()
{
    if (tde.entry_.valid())
    {
        entry_.reset(tde.entry_->clone().ptr());
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::TimeDataEntry<Type>::~TimeDataEntry()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::TimeDataEntry<Type>::reset(const dictionary& dict)
{
    entry_.reset
    (
        DataEntry<Type>::New
        (
            name_,
            dict
        ).ptr()
    );

    entry_->convertTimeBase(time_);
}


template<class Type>
const CML::word& CML::TimeDataEntry<Type>::name() const
{
    return entry_->name();
}


template<class Type>
Type CML::TimeDataEntry<Type>::value(const scalar x) const
{
    return entry_->value(x);
}


template<class Type>
Type CML::TimeDataEntry<Type>::integrate
(
    const scalar x1,
    const scalar x2
) const
{
    return entry_->integrate(x1, x2);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const TimeDataEntry<Type>& de
)
{
    return de.entry_->operator<<(os, de);
}


template<class Type>
void CML::TimeDataEntry<Type>::writeData(Ostream& os) const
{
    entry_->writeData(os);
}


#endif

// ************************************************************************* //
