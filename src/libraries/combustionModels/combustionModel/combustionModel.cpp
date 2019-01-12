/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License

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

#include "combustionModel.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(combustionModel, 0);
}

const CML::word CML::combustionModel::combustionPropertiesName
(
    "combustionProperties"
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::IOobject CML::combustionModel::createIOobject
(
    basicThermo& thermo,
    const word& combustionProperties
) const
{
    IOobject io
    (
        thermo.phasePropertyName(combustionProperties),
        thermo.db().time().constant(),
        thermo.db(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    );

    if (io.headerOk())
    {
        io.readOpt() = IOobject::MUST_READ_IF_MODIFIED;
        return io;
    }
    else
    {
        io.readOpt() = IOobject::NO_READ;
        return io;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::combustionModel::combustionModel
(
    const word& modelType,
    basicThermo& thermo,
    const compressible::turbulenceModel& turb,
    const word& combustionProperties
)
:
    IOdictionary(createIOobject(thermo, combustionProperties)),
    mesh_(thermo.p().mesh()),
    turb_(turb),
    coeffs_(subDict(modelType + "Coeffs")),
    modelType_(modelType)
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool CML::combustionModel::read()
{
    if (regIOobject::read())
    {
        coeffs_ = subDict(modelType_ + "Coeffs");
        return true;
    }
    else
    {
        return false;
    }
}
