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

#include "regionProperties.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::regionProperties::regionProperties(const Time& runTime)
:
    IOdictionary
    (
        IOobject
        (
            "regionProperties",
            runTime.time().constant(),
            runTime.db(),
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),
    fluidRegionNames_(lookup("fluidRegionNames")),
    solidRegionNames_(lookup("solidRegionNames"))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::regionProperties::~regionProperties()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::List<CML::word>& CML::regionProperties::fluidRegionNames() const
{
    return fluidRegionNames_;
}


const CML::List<CML::word>& CML::regionProperties::solidRegionNames() const
{
    return solidRegionNames_;
}


// ************************************************************************* //
