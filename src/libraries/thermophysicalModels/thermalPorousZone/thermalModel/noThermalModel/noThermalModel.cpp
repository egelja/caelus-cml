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

#include "noThermalModel.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "basicThermo.hpp"
#include "volFields.hpp"
#include "fvMatrices.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace porousMedia
{
   defineTypeNameAndDebug(noThermalModel, 0);

   addToRunTimeSelectionTable
   (
       thermalModel,
       noThermalModel,
       pZone
   );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::porousMedia::noThermalModel::noThermalModel(const porousZone& pZone)
:
    thermalModel(pZone)
{}


// * * * * * * * * * * * * * * * * Destructor    * * * * * * * * * * * * * * //

CML::porousMedia::noThermalModel::~noThermalModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::porousMedia::noThermalModel::addEnthalpySource
(
    const basicThermo&,
    const volScalarField&,
    fvScalarMatrix&
) const
{
    // do nothing
}


// ************************************************************************* //
