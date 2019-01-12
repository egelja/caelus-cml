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

#include "dlLibraryTable.hpp"
#include "OSspecific.hpp"
#include "longLong.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(dlLibraryTable, 0);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::dlLibraryTable::dlLibraryTable()
{}


CML::dlLibraryTable::dlLibraryTable
(
    const dictionary& dict,
    const word& libsEntry
)
{
    open(dict, libsEntry);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::dlLibraryTable::~dlLibraryTable()
{
    forAllReverse(libPtrs_, i)
    {
        if (libPtrs_[i])
        {
            if (debug)
            {
                Info<< "dlLibraryTable::~dlLibraryTable() : closing "
                    << libNames_[i]
                    << " with handle " << reinterpret_cast<long long>(libPtrs_[i]) << endl;
            }
            dlClose(libPtrs_[i]);
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::dlLibraryTable::open
(
    const fileName& functionLibName,
    const bool verbose
)
{
    if (functionLibName.size())
    {
        void* functionLibPtr = dlOpen(functionLibName, verbose);

        if (debug)
        {
            Info<< "dlLibraryTable::open : opened " << functionLibName
                << " resulting in handle " << reinterpret_cast<long long>(functionLibPtr) << endl;
        }

        if (!functionLibPtr)
        {
            if (verbose)
            {
                WarningInFunction
                    << "could not load " << functionLibName
                    << endl;
            }

            return false;
        }
        else
        {
            libPtrs_.append(functionLibPtr);
            libNames_.append(functionLibName);
            return true;
        }
    }
    else
    {
        return false;
    }
}


bool CML::dlLibraryTable::close
(
    const fileName& functionLibName,
    const bool verbose
)
{
    label index = -1;
    forAllReverse(libNames_, i)
    {
        if (libNames_[i] == functionLibName)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        if (debug)
        {
            Info<< "dlLibraryTable::close : closing " << functionLibName
                << " with handle " << reinterpret_cast<long long>(libPtrs_[index]) << endl;
        }

        bool ok = dlClose(libPtrs_[index]);

        libPtrs_[index] = nullptr;
        libNames_[index] = fileName::null;

        if (!ok)
        {
            if (verbose)
            {
                WarningInFunction
                    << "could not close " << functionLibName
                    << endl;
            }

            return false;
        }

        return true;
    }
    return false;
}


void* CML::dlLibraryTable::findLibrary(const fileName& functionLibName)
{
    label index = -1;
    forAllReverse(libNames_, i)
    {
        if (libNames_[i] == functionLibName)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        return libPtrs_[index];
    }
    return nullptr;
}


bool CML::dlLibraryTable::open
(
    const dictionary& dict,
    const word& libsEntry
)
{
    if (dict.found(libsEntry))
    {
        fileNameList libNames(dict.lookup(libsEntry));

        bool allOpened = !libNames.empty();

        forAll(libNames, i)
        {
            allOpened = dlLibraryTable::open(libNames[i]) && allOpened;
        }

        return allOpened;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
