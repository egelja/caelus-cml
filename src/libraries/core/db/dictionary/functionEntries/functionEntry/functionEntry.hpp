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

Namespace
    CML::functionEntries

Description
    Namespace for containing a functionEntry.


Class
    CML::functionEntry

Description
    A functionEntry causes entries to be added/manipulated on the specified
    dictionary given an input stream.

    In dictionaries, a \c '\#' sigil is typically used for a functionEntry.

SourceFiles
    functionEntry.cpp
    functionEntryIO.cpp

\*---------------------------------------------------------------------------*/

#ifndef functionEntry_H
#define functionEntry_H

#include "word.hpp"
#include "memberFunctionSelectionTables.hpp"
#include "primitiveEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class dictionary;

/*---------------------------------------------------------------------------*\
                           Class functionEntry Declaration
\*---------------------------------------------------------------------------*/

class functionEntry
:
    public primitiveEntry
{
    // Private Member Functions

        //- Read line as string token
        static token readLine(const word& key, Istream& is);

        //- Disallow default bitwise copy construct
        functionEntry(const functionEntry&);

        //- Disallow default bitwise assignment
        void operator=(const functionEntry&);


public:

    // Constructors

        //- Construct from keyword, parent dictionary and Istream
        functionEntry(const word&, const dictionary&, Istream&);


    // Member Function Selectors

        declareMemberFunctionSelectionTable
        (
            bool,
            functionEntry,
            execute,
            dictionaryIstream,
            (
                dictionary& parentDict,
                Istream& is
            ),
            (parentDict, is)
        );

        //- Execute the functionEntry in a sub-dict context
        static bool execute
        (
            const word& functionName,
            dictionary& parentDict,
            Istream&
        );

        declareMemberFunctionSelectionTable
        (
            bool,
            functionEntry,
            execute,
            primitiveEntryIstream,
            (
                const dictionary& parentDict,
                primitiveEntry& entry,
                Istream& is
            ),
            (parentDict, entry, is)
        );

        //- Execute the functionEntry in a primitiveEntry context
        static bool execute
        (
            const word& functionName,
            const dictionary& parentDict,
            primitiveEntry&,
            Istream&
        );

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
