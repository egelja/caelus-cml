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

\*---------------------------------------------------------------------------*/

#include "includeIfPresentEntry.hpp"
#include "dictionary.hpp"
#include "IFstream.hpp"
#include "addToMemberFunctionSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const CML::word CML::functionEntries::includeIfPresentEntry::typeName
(
    CML::functionEntries::includeIfPresentEntry::typeName_()
);

// Don't lookup the debug switch here as the debug switch dictionary
// might include includeIfPresentEntry
int CML::functionEntries::includeIfPresentEntry::debug(0);

namespace CML
{
namespace functionEntries
{
    addToMemberFunctionSelectionTable
    (
        functionEntry,
        includeIfPresentEntry,
        execute,
        dictionaryIstream
    );

    addToMemberFunctionSelectionTable
    (
        functionEntry,
        includeIfPresentEntry,
        execute,
        primitiveEntryIstream
    );
}
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::functionEntries::includeIfPresentEntry::execute
(
    dictionary& parentDict,
    Istream& is
)
{
    const fileName fName(includeFileName(is));
    IFstream ifs(fName);

    if (ifs)
    {
        if (CML::functionEntries::includeEntry::report)
        {
            Info<< fName << endl;
        }
        parentDict.read(ifs);
    }

    return true;
}


bool CML::functionEntries::includeIfPresentEntry::execute
(
    const dictionary& parentDict,
    primitiveEntry& entry,
    Istream& is
)
{
    const fileName fName(includeFileName(is));
    IFstream ifs(fName);

    if (ifs)
    {
        if (CML::functionEntries::includeEntry::report)
        {
            Info<< fName << endl;
        }
        entry.read(parentDict, ifs);
    }

    return true;
}

// ************************************************************************* //
