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
    CML::wordReListMatcher

Description
    A wrapper for matching a List of wordRe.

Note
    The constructor should remain non-explicit. This allows automatic
    conversion from UList\<wordRe\> to wordReListMatcher in search
    functions.

SourceFiles
    wordReListMatcherI.hpp

\*---------------------------------------------------------------------------*/

#ifndef wordReListMatcher_H
#define wordReListMatcher_H

#include "wordReList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class wordReListMatcher Declaration
\*---------------------------------------------------------------------------*/

class wordReListMatcher
{
    // Private data

        //- Reference to underlying list
        const UList<wordRe>& reList_;

public:

    // Constructors

        //- Construct from a List of wordRe
        inline wordReListMatcher(const UList<wordRe>&);


    // Member Functions

        // Access

            inline label size() const;
            inline bool  empty() const;

            //- Return underlying list of wordRe
            inline const UList<wordRe>& operator()() const;


    // Searching

            //- Return true if string matches any of the regular expressions
            //  Smart match as regular expression or as a string.
            //  Optionally specify a literal match only.
            inline bool match(const string&, bool literalMatch=false) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "wordReListMatcherI.hpp"

#endif

// ************************************************************************* //
