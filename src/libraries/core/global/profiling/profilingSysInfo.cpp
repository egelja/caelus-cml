/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenCFD Ltd
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

\*---------------------------------------------------------------------------*/

#include "profilingSysInfo.hpp"
#include "demandDrivenData.hpp"
#include "caelusVersion.hpp"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

// File-scope function
template<class T>
inline static void writeEntry
(
    CML::Ostream& os, const CML::word& key, const T& value
)
{
    os.writeKeyword(key) << value << CML::token::END_STATEMENT << '\n';
}


// File-scope function
inline static void printEnv
(
    CML::Ostream& os, const CML::word& key, const CML::word& envName
)
{
    const std::string value = getEnv(envName);
    if (!value.empty())
    {
        writeEntry(os, key, value);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::profiling::sysInfo::sysInfo()
{}



// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::profiling::sysInfo::~sysInfo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::Ostream& CML::profiling::sysInfo::write
(
    Ostream& os
) const
{
    writeEntry(os, "host", hostName(false)); // Short name
    writeEntry(os, "date", clock::dateTime());

    // Compile-time information
    writeEntry(os, "version",     std::string(CAELUSversion));
    writeEntry(os, "build",       std::string(CAELUSbuild));

    printEnv(os, "arch",            "WHICH_OS");
    printEnv(os, "Compiler",        "COMPILER");
    printEnv(os, "FloatPrecision", "PRECISION_OPTION");
    printEnv(os, "IntegerType",    "INT_TYPE");
    printEnv(os, "MPILibrary",     "MPI_LIB");

    return os;
}


// ************************************************************************* //
