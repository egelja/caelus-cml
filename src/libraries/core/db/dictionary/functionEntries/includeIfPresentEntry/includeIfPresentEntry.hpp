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
    CML::functionEntries::includeIfPresentEntry

Description
    Specify a file to include if it exists. Expects a single string to follow.

    The \c \#includeIfPresent directive is similar to the \c \#include
    directive, but does not generate an error if the file does not exist.

See Also
    CML::functionEntries::includeEntry

SourceFiles
    includeIfPresentEntry.cpp

\*---------------------------------------------------------------------------*/

#ifndef includeIfPresentEntry_H
#define includeIfPresentEntry_H

#include "includeEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace functionEntries
{

/*---------------------------------------------------------------------------*\
                    Class includeIfPresentEntry Declaration
\*---------------------------------------------------------------------------*/

class includeIfPresentEntry
:
    public includeEntry
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        includeIfPresentEntry(const includeIfPresentEntry&);

        //- Disallow default bitwise assignment
        void operator=(const includeIfPresentEntry&);

public:

    //- Runtime type information
    ClassName("includeIfPresent");


    // Member Functions

        //- Execute the functionEntry in a sub-dict context
        static bool execute(dictionary& parentDict, Istream&);

        //- Execute the functionEntry in a primitiveEntry context
        static bool execute
        (
            const dictionary& parentDict,
            primitiveEntry&,
            Istream&
        );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionEntries
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
