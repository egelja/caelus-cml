/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
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
    CML::dictionaryListEntry

Description
    Read/write List of dictionaries.

    The List entries get stored in a
    dictionary which itself is stored in the parent dictionary with
    keyword 'entryDDD' where DDD is the position in the parent dictionary.
    The printing is again in List format - the keyword is only printed as
    comment. Can be used to e.g. manipulate polyMesh/boundary files.

SourceFiles
    dictionaryListEntry.cpp
    dictionaryListEntryIO.cpp

\*---------------------------------------------------------------------------*/

#ifndef dictionaryListEntry_HPP
#define dictionaryListEntry_HPP

#include "dictionaryEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class dictionaryListEntry;

Ostream& operator<<(Ostream&, const dictionaryListEntry&);


/*---------------------------------------------------------------------------*\
                     Class dictionaryListEntry Declaration
\*---------------------------------------------------------------------------*/

class dictionaryListEntry
:
    public dictionaryEntry
{
    // Private Member Functions

        //- Returns size of dictionary without FoamFile
        static label realSize(const dictionary&);

        //- Dissallow bitwise copy
        dictionaryListEntry(const dictionary&);


public:

    // Constructors

        //- Construct from the parent dictionary and Istream
        dictionaryListEntry(const dictionary& parentDict, Istream&);

        //- Construct as copy for the given parent dictionary
        dictionaryListEntry
        (
            const dictionary& parentDict,
            const dictionaryListEntry&
        );

        autoPtr<entry> clone(const dictionary& parentDict) const
        {
            return autoPtr<entry>(new dictionaryListEntry(parentDict, *this));
        }


    // Member functions

        //- Write
        virtual void write(Ostream&) const;

        //- Return info proxy.
        //  Used to print token information to a stream
        InfoProxy<dictionaryListEntry> info() const
        {
            return *this;
        }


    // Ostream operator

        friend Ostream& operator<<(Ostream&, const dictionaryListEntry&);
};


template<>
Ostream& operator<<(Ostream&, const InfoProxy<dictionaryListEntry>&);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
