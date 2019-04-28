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

#include "compressibleSpalartAllmarasIDDES.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace compressible
{
namespace LESModels
{

defineTypeNameAndDebug(SpalartAllmarasIDDES, 0);
addToRunTimeSelectionTable(LESModel, SpalartAllmarasIDDES, dictionary);

tmp<volScalarField> SpalartAllmarasIDDES::alpha() const
{
    return max
    (
        0.25 - y_/static_cast<const volScalarField&>(hmax_()),
        scalar(-5)
    );
}


tmp<volScalarField> SpalartAllmarasIDDES::ft
(
    const volScalarField& S
) const
{
    return tanh(pow3(sqr(ct_)*rd((muSgs()/rho()), S)));
}


tmp<volScalarField> SpalartAllmarasIDDES::fl
(
    const volScalarField& S
) const
{
    return tanh(pow(sqr(cl_)*rd((mu()/rho()), S), 10));
}

// Limit rd due to fl function
tmp<volScalarField> SpalartAllmarasIDDES::rd
(
    const volScalarField& visc,
    const volScalarField& S
) const
{
    return min
    (
        visc
       /(
           max
           (
               S,
               dimensionedScalar("SMALL", S.dimensions(), SMALL)
           )*sqr(kappa_*y_)
        ),
        scalar(10)
    );
}


tmp<volScalarField> SpalartAllmarasIDDES::fd(const volScalarField& S) const
{
    return 1 - tanh(pow3(8*rd(muEff(), S)));
}

tmp<volScalarField> SpalartAllmarasIDDES::S(const volTensorField& gradU) const
{
    const volScalarField contraction(gradU && gradU);
    return sqrt(contraction);
}

tmp<volScalarField> SpalartAllmarasIDDES::dTilda(const volScalarField& S) const
{
    const volScalarField alpha(this->alpha());

    tmp<volScalarField> fb = min(2*exp(-9*sqr(alpha)),scalar(1.0));

    tmp<volScalarField> fe1 = 
      2*(pos(alpha)*exp(-11.09*sqr(alpha)) + neg(alpha)*exp(-9*sqr(alpha)));

    tmp<volScalarField> fe2 = 1.0 - max(ft(S), fl(S));

    // trip terms (ft2) removed
    const volScalarField Psi
    (
        sqrt
        (
            min
            (
                scalar(100),
                (1 - Cb1_/(Cw1_*sqr(kappa_)*fwStar_)*fv2())/max(SMALL, fv1())
            )
        )
    );

    tmp<volScalarField> fe = max((fe1 - scalar(1)),scalar(0))*Psi*fe2;

    tmp<volScalarField> rdt = rd((muSgs_/rho()), S);
     
    tmp<volScalarField> fdt = 1 - tanh(pow3(8*rdt));

    const volScalarField fdTilda(max((1-fdt),fb)); 

    return max
    (
        dimensionedScalar("SMALL", dimLength, SMALL),
        fdTilda*(1 + fe)*y_
      + (1 - fdTilda)*CDES_*Psi*delta()
    );
}


SpalartAllmarasIDDES::SpalartAllmarasIDDES
(
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& phi,
    const fluidThermo& thermophysicalModel,
    const word& turbulenceModelName,
    const word& modelName
)
:
    SpalartAllmarasDES(rho, U, phi, thermophysicalModel, turbulenceModelName, modelName),
    hmax_
    (
        LESdelta::New
        (
            "hmax",
            mesh_,
            *this
        )
    ),
    IDDESDelta_
    (
        LESdelta::New
        (
            "IDDESDelta",
            mesh_,
            this->subDict(typeName + "Coeffs")
        )
    ),
    fwStar_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "fwStar",
            coeffDict_,
            0.424
        )
    ),
    cl_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "cl",
            coeffDict_,
            3.55
        )
    ),
    ct_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "ct",
            coeffDict_,
            1.63
        )
    )
{}


bool SpalartAllmarasIDDES::read()
{
    if (SpalartAllmarasDES::read())
    {
        fwStar_.readIfPresent(coeffDict());
        cl_.readIfPresent(coeffDict());
        ct_.readIfPresent(coeffDict());

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


