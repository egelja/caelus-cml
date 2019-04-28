/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
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

#include "compressibleSpalartAllmarasDDES.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace compressible
{
namespace LESModels
{

defineTypeNameAndDebug(SpalartAllmarasDDES, 0);
addToRunTimeSelectionTable(LESModel, SpalartAllmarasDDES, dictionary);

tmp<volScalarField> SpalartAllmarasDDES::rd
(
    volScalarField const& visc,
    volScalarField const& S
) const
{
    return
    (
        visc
        /(
            max
            (
                S,
                dimensionedScalar("SMALL", S.dimensions(), SMALL)
            )*sqr(kappa_*y_)
         )
    );
}


tmp<volScalarField> SpalartAllmarasDDES::fd(volScalarField const& S) const
{
    return 1 - tanh(pow3(8*rd((muEff()/rho()), S)));
}


tmp<volScalarField> SpalartAllmarasDDES::S(volTensorField const& gradU) const
{
    volScalarField const contraction(gradU && gradU);
    return sqrt(contraction);
}


tmp<volScalarField> SpalartAllmarasDDES::dTilda(volScalarField const& S) const
{
    return max
    (
        y_
      - fd(S)
       *max(y_ - CDES_*delta(), dimensionedScalar("zero", dimLength, 0)),
        dimensionedScalar("small", dimLength, SMALL)
    );
}


SpalartAllmarasDDES::SpalartAllmarasDDES
(
    volScalarField const& rho,
    volVectorField const& U,
    surfaceScalarField const& phi,
    fluidThermo const& thermophysicalModel,
    word const& turbulenceModelName,
    word const& modelName
) :
    SpalartAllmarasDES
    (
        rho,
        U,
        phi,
        thermophysicalModel,
        turbulenceModelName, modelName
    )
{}


}
}
}


