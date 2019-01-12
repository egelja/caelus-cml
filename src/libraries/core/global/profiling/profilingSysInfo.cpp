/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2017 OpenCFD Ltd.
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
#include "caelusVersion.hpp"
#include "clock.hpp"
#include "Ostream.hpp"
#include "OSspecific.hpp"

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
    CML::Ostream& os,
    const CML::word& key,
    const std::string& envName
)
{
    const std::string value = CML::getEnv(envName);
    if (!value.empty())
    {
        writeEntry(os, key, value);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::profilingSysInfo::profilingSysInfo()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::profilingSysInfo::~profilingSysInfo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::Ostream& CML::profilingSysInfo::write
(
    Ostream& os
) const
{
    writeEntry(os, "host", hostName(false)); // Short name
    writeEntry(os, "date", clock::dateTime());

    // Compile-time information
    writeEntry(os, "version",    std::string(CAELUSversion));
    writeEntry(os, "build",      std::string(CAELUSbuild));

    printEnv(os, "arch",           "WHICH_OS");
    printEnv(os, "Compiler",       "COMPILER");
    printEnv(os, "FloatPrecision", "PRECISION_OPTION");
    printEnv(os, "IntegerType",    "INT_TYPE");
    printEnv(os, "MPILibrary",     "MPI_LIB");

    return os;
}


// ************************************************************************* //
