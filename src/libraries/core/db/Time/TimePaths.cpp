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

#include "TimePaths.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::TimePaths::TimePaths
(
    const fileName& rootPath,
    const fileName& caseName,
    const word& systemName,
    const word& constantName
)
:
    processorCase_(false),
    rootPath_(rootPath),
    case_(caseName),
    system_(systemName),
    constant_(constantName)
{
    // Find out from case name whether a processor directory
    std::string::size_type pos = caseName.find("processor");
    if (pos != string::npos)
    {
        processorCase_ = true;

        if (pos == 0)
        {
            globalCaseName_ = ".";
        }
        else
        {
            globalCaseName_ = caseName(pos-1);
        }
    }
    else
    {
        globalCaseName_ = caseName;
    }
}


CML::TimePaths::TimePaths
(
    const bool processorCase,
    const fileName& rootPath,
    const fileName& globalCaseName,
    const fileName& caseName,
    const word& systemName,
    const word& constantName
)
:
    processorCase_(processorCase),
    rootPath_(rootPath),
    globalCaseName_(globalCaseName),
    case_(caseName),
    system_(systemName),
    constant_(constantName)
{
    if (!processorCase)
    {
        // For convenience: find out from case name whether it is a
        // processor directory and set processorCase flag so file searching
        // goes up one level.
        std::string::size_type pos = caseName.find("processor");

        if (pos != string::npos)
        {
            processorCase_ = true;

            if (pos == 0)
            {
                globalCaseName_ = ".";
            }
            else
            {
                globalCaseName_ = caseName(pos-1);
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::fileName CML::TimePaths::caseSystem() const
{
    if (processorCase_)
    {
        return ".."/system();
    }
    else
    {
        return system();
    }
}


CML::fileName CML::TimePaths::caseConstant() const
{
    if (processorCase_)
    {
        return ".."/constant();
    }
    else
    {
        return constant();
    }
}



// ************************************************************************* //
