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

#include "includeEntry.hpp"
#include "dictionary.hpp"
#include "IFstream.hpp"
#include "addToMemberFunctionSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const CML::word CML::functionEntries::includeEntry::typeName
(
    CML::functionEntries::includeEntry::typeName_()
);

// Don't lookup the debug switch here as the debug switch dictionary
// might include includeEntry
int CML::functionEntries::includeEntry::debug(0);

bool CML::functionEntries::includeEntry::report(false);


namespace CML
{
namespace functionEntries
{
    addToMemberFunctionSelectionTable
    (
        functionEntry,
        includeEntry,
        execute,
        dictionaryIstream
    );

    addToMemberFunctionSelectionTable
    (
        functionEntry,
        includeEntry,
        execute,
        primitiveEntryIstream
    );
}
}

// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //

CML::fileName CML::functionEntries::includeEntry::includeFileName
(
    Istream& is
)
{
    fileName fName(is);
    fName.expand();

    if (fName.empty() || fName.isAbsolute())
    {
        return fName;
    }
    else
    {
        // relative name
        return fileName(is.name()).path()/fName;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::functionEntries::includeEntry::execute
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
        return true;
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "Cannot open include file " << ifs.name()
            << " while reading dictionary " << parentDict.name()
            << exit(FatalIOError);

        return false;
    }
}


bool CML::functionEntries::includeEntry::execute
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
        return true;
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "Cannot open include file " << ifs.name()
            << " while reading dictionary " << parentDict.name()
            << exit(FatalIOError);

        return false;
    }
}

// ************************************************************************* //
