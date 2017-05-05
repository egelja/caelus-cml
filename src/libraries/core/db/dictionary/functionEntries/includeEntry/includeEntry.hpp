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
    CML::functionEntries::includeEntry

Description
    Specify an include file when reading dictionaries, expects a
    single string to follow.

    An example of the \c \#include directive:
    \verbatim
        #include "includeFile"
    \endverbatim

    The usual expansion of environment variables and other constructs
    (eg, the \c ~CAELUS/ expansion) is retained.

See Also
    fileName, string::expand()

SourceFiles
    includeEntry.cpp

\*---------------------------------------------------------------------------*/

#ifndef includeEntry_H
#define includeEntry_H

#include "functionEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace functionEntries
{

/*---------------------------------------------------------------------------*\
                        Class includeEntry Declaration
\*---------------------------------------------------------------------------*/

class includeEntry
:
    public functionEntry
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        includeEntry(const includeEntry&);

        //- Disallow default bitwise assignment
        void operator=(const includeEntry&);

protected:

    // Protected Member Functions

        //- Read the include fileName from Istream, expand and return
        static fileName includeFileName(Istream&);


public:

    // Static data members

        //- Report which file is included to stdout
        static bool report;


    //- Runtime type information
    ClassName("include");


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
