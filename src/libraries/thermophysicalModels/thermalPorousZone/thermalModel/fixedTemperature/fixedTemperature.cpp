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

#include "fixedTemperature.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "basicThermo.hpp"
#include "volFields.hpp"
#include "fvMatrices.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace porousMedia
{
    defineTypeNameAndDebug(fixedTemperature, 0);

    addToRunTimeSelectionTable
    (
        thermalModel,
        fixedTemperature,
        pZone
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::porousMedia::fixedTemperature::fixedTemperature(const porousZone& pZone)
:
    thermalModel(pZone),
    T_(readScalar(thermalCoeffs_.lookup("T")))
{}


// * * * * * * * * * * * * * * * * Destructor    * * * * * * * * * * * * * * //

CML::porousMedia::fixedTemperature::~fixedTemperature()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::porousMedia::fixedTemperature::addEnthalpySource
(
    const basicThermo& thermo,
    const volScalarField& rho,
    fvScalarMatrix& hEqn
) const
{
    const labelList& zones = pZone_.zoneIds();
    if (zones.empty() || T_ < 0.0)
    {
        return;
    }

    const fvMesh& mesh = pZone_.mesh();
    const scalarField T(hEqn.diag().size(), T_);

    forAll(zones, zoneI)
    {
        const labelList& cells = mesh.cellZones()[zones[zoneI]];
        tmp<scalarField> h = thermo.h(T, cells);
        hEqn.setValues(cells, h());
    }
}


// ************************************************************************* //
