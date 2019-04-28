/*---------------------------------------------------------------------------*\
Copyright (C) 2016 Applied CCM
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


#include "compressiblekOmegaSSTDDES.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace compressible
{
namespace LESModels
{

defineTypeNameAndDebug(kOmegaSSTDDES, 0);
addToRunTimeSelectionTable(LESModel, kOmegaSSTDDES, dictionary);

tmp<volScalarField> kOmegaSSTDDES::FDES() const
{

    volScalarField const CDkOmega
    (
        (2*rho_*alphaOmega2_)*(fvc::grad(k_) & fvc::grad(omega_))/omega_
    );
    
    return max
    (
        Lt()/(CDES_*delta())*(scalar(1.0) - F1(CDkOmega)),
        scalar(1.0)
    );
}


kOmegaSSTDDES::kOmegaSSTDDES
(
    volScalarField const& rho,
    volVectorField const& U,
    surfaceScalarField const& phi,
    fluidThermo const& thermophysicalModel,
    word const& turbulenceModelName,
    word const& modelName
) :
    kOmegaSSTDES
    (
        rho,
        U,
        phi,
        thermophysicalModel,
        turbulenceModelName,
        modelName
    )
{}


}
}
}


