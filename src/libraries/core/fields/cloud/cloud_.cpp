/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
Copyright (C) 2016-2018 OpenCFD Ltd.
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

#include "cloud_.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(cloud, 0);
}

const CML::word CML::cloud::prefix("lagrangian");
CML::word CML::cloud::defaultName("defaultCloud");

const CML::Enum<CML::cloud::geometryType>
CML::cloud::geometryTypeNames
{
    { geometryType::COORDINATES, "coordinates" },
    { geometryType::POSITIONS, "positions" }
};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::cloud::cloud(const objectRegistry& obr, const word& cloudName)
:
    objectRegistry
    (
        IOobject
        (
            cloudName,
            obr.time().timeName(),
            prefix,
            obr,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        )
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::cloud::autoMap(const mapPolyMesh&)
{
    NotImplemented;
}


// ************************************************************************* //
