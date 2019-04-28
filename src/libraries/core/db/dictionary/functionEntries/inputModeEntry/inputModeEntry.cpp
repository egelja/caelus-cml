/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "inputModeEntry.hpp"
#include "dictionary.hpp"
#include "addToMemberFunctionSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const CML::word CML::functionEntries::inputModeEntry::typeName
(
    CML::functionEntries::inputModeEntry::typeName_()
);

// Don't lookup the debug switch here as the debug switch dictionary
// might include inputModeEntries
int CML::functionEntries::inputModeEntry::debug(0);

CML::functionEntries::inputModeEntry::inputMode
    CML::functionEntries::inputModeEntry::mode_(MERGE);

namespace CML
{
namespace functionEntries
{
    addToMemberFunctionSelectionTable
    (
        functionEntry,
        inputModeEntry,
        execute,
        dictionaryIstream
    );
}
}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// we could combine this into execute() directly, but leave it here for now
void CML::functionEntries::inputModeEntry::setMode(Istream& is)
{
    clear();

    word mode(is);
    if (mode == "merge" || mode == "default")
    {
        mode_ = MERGE;
    }
    else if (mode == "overwrite")
    {
        mode_ = OVERWRITE;
    }
    else if (mode == "protect")
    {
        mode_ = PROTECT;
    }
    else if (mode == "warn")
    {
        mode_ = WARN;
    }
    else if (mode == "error")
    {
        mode_ = ERROR;
    }
    else
    {
        WarningInFunction
            << "unsupported input mode '" << mode
            << "' ... defaulting to 'merge'"
            << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::functionEntries::inputModeEntry::execute
(
    dictionary& parentDict,
    Istream& is
)
{
    setMode(is);
    return true;
}


void CML::functionEntries::inputModeEntry::clear()
{
    mode_ = MERGE;
}


bool CML::functionEntries::inputModeEntry::merge()
{
    return mode_ == MERGE;
}


bool CML::functionEntries::inputModeEntry::overwrite()
{
    return mode_ == OVERWRITE;
}


bool CML::functionEntries::inputModeEntry::protect()
{
    return mode_ == PROTECT;
}

bool CML::functionEntries::inputModeEntry::error()
{
    return mode_ == ERROR;
}


// ************************************************************************* //
