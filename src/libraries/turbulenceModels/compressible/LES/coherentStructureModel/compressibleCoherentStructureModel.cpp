/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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

#include "compressibleCoherentStructureModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace compressible
{
namespace LESModels
{

defineTypeNameAndDebug(CoherentStructureModel, 0);
addToRunTimeSelectionTable(LESModel, CoherentStructureModel, dictionary);

void CoherentStructureModel::updateSubGridScaleFields
(
    volTensorField const& gradU
)
{
    const volScalarField S2(2*magSqr(symm(gradU)));
    muSgs_ = C(gradU)*rho()*sqrt(S2)*delta()*delta();
    muSgs_.correctBoundaryConditions();

    alphaSgs_ = muSgs_/Prt_;
    alphaSgs_.correctBoundaryConditions();
}

volScalarField CoherentStructureModel::C(volTensorField const& gradU) const
{
    tmp<volScalarField> const Q = 0.5*
    (
        (skew(gradU)&&skew(gradU)) 
        - 
        (symm(gradU)&&symm(gradU))
    );

    tmp<volScalarField> const E = 0.5*
    (
        (skew(gradU)&&skew(gradU)) 
        + 
        (symm(gradU)&&symm(gradU))
    );


    volScalarField Fcs = 
        (Q/(E+dimensionedScalar("smallE",E->dimensions(),VSMALL)))();

    dimensionedScalar FcsNegOne("FcsNegOne",Fcs.dimensions(),scalar(-1));
    dimensionedScalar FcsPosOne("FcsPosOne",Fcs.dimensions(),scalar( 1));
    dimensionedScalar FcsPosTwo("FcsPosTwo",Fcs.dimensions(),scalar( 2));
    dimensionedScalar FcsZero("FcsZero",Fcs.dimensions(),scalar(0));

    Fcs = min(max(Fcs,FcsNegOne),FcsPosOne);

    tmp<volScalarField> const FOmega = min(max(1 - Fcs,FcsZero),FcsPosTwo);

    tmp<volScalarField> tC = cPrime_*pow(min(mag(Fcs),FcsPosOne),1.5)*FOmega;
    return tC();
}

CoherentStructureModel::CoherentStructureModel
(
    const volScalarField& rho,
    volVectorField const& U,
    surfaceScalarField const& phi,
    const fluidThermo& thermophysicalModel,
    word const& turbulenceModelName,
    word const& modelName
) :
    LESModel(modelName, rho, U, phi, thermophysicalModel, turbulenceModelName),
    GenEddyVisc(rho, U, phi, thermophysicalModel),

    cPrime_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "cPrime",
            coeffDict_,
            1.0/22.0
        )
    )
{
    updateSubGridScaleFields(fvc::grad(U));
    printCoeffs();
}

void CoherentStructureModel::correct(tmp<volTensorField> const& gradU)
{
    GenEddyVisc::correct(gradU);
    updateSubGridScaleFields(gradU());
}


bool CoherentStructureModel::read()
{
    if (GenEddyVisc::read())
    {
        cPrime_.readIfPresent(coeffDict());

        return true;
    }
    else
    {
        return false;
    }
}

}
}
}

