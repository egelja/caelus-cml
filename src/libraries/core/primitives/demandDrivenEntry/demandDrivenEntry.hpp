/*---------------------------------------------------------------------------*\
Copyright (C) 2013 OpenFOAM Foundation
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
    CML::demandDrivenEntry

Description
    Class for demand-driven dictionary entries

    Holds a reference to a dictionary, which is then queried if the value
    is requested and has not already been cached

\*---------------------------------------------------------------------------*/

#ifndef demandDrivenEntry_HPP
#define demandDrivenEntry_HPP

#include "dictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class demandDrivenEntry Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class demandDrivenEntry
{
private:

    // Private data

        //- Reference to the dictionary
        const dictionary& dict_;

        //- Keyword to look up
        const word keyword_;

        //- Value
        mutable Type value_;

        //- Flag to say that the value has been stored
        mutable bool stored_;


public:

    //- Constructors

        //- Construct from dictionary and value - cannot be re-read
        demandDrivenEntry
        (
            const dictionary& dict,
            const Type& value
        );


        //- Construct from dictionary and keyword
        demandDrivenEntry
        (
            const dictionary& dict,
            const word& keyword
        );


        //- Construct from dictionary, keyword and default value
        demandDrivenEntry
        (
            const dictionary& dict,
            const word& keyword,
            const Type& defaultValue,
            const bool readIfPresent = true
        );

        //- Copy constructor
        demandDrivenEntry(const demandDrivenEntry& dde);


    // Public Member Functions

        //- Initialise
        inline void initialise() const;

        //- Return the value
        inline const Type& value() const;

        //- Set the value
        inline void setValue(const Type& value);

        //- Reset the demand-driven entry
        inline void reset();
};


} // End namespace CML


template<class Type>
inline void CML::demandDrivenEntry<Type>::initialise() const
{
    if (!stored_)
    {
        dict_.lookup(keyword_) >> value_;
        stored_ = true;
    }
}


template<class Type>
inline const Type& CML::demandDrivenEntry<Type>::value() const
{
    initialise();

    return value_;
}


template<class Type>
inline void CML::demandDrivenEntry<Type>::setValue(const Type& value)
{
//    dict_.set<Type>(keyword_, value);
    value_ = value;
    stored_ = true;
}


template<class Type>
inline void CML::demandDrivenEntry<Type>::reset()
{
    stored_ = false;
}


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
CML::demandDrivenEntry<Type>::demandDrivenEntry
(
    const dictionary& dict,
    const Type& value
)
:
    dict_(dict),
    keyword_("unknown-keyword"),
    value_(value),
    stored_(true)
{}


template<class Type>
CML::demandDrivenEntry<Type>::demandDrivenEntry
(
    const dictionary& dict,
    const word& keyword
)
:
    dict_(dict),
    keyword_(keyword),
    value_(Zero),
    stored_(false)
{}


template<class Type>
CML::demandDrivenEntry<Type>::demandDrivenEntry
(
    const dictionary& dict,
    const word& keyword,
    const Type& defaultValue,
    const bool readIfPresent
)
:
    dict_(dict),
    keyword_(keyword),
    value_(defaultValue),
    stored_(true)
{
    if (readIfPresent)
    {
        dict_.readIfPresent<Type>(keyword, value_);
    }
}


template<class Type>
CML::demandDrivenEntry<Type>::demandDrivenEntry(const demandDrivenEntry& dde)
:
    dict_(dde.dict_),
    keyword_(dde.keyword_),
    value_(dde.value_),
    stored_(dde.stored_)
{}


#endif
