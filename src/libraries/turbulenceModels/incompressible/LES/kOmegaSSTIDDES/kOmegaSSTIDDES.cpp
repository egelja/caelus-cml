/*---------------------------------------------------------------------------*\
Copyright (C) 2018 Applied CCM
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


#include "kOmegaSSTIDDES.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
namespace incompressible
{
namespace LESModels
{

defineTypeNameAndDebug(kOmegaSSTIDDES, 0);
addToRunTimeSelectionTable(LESModel, kOmegaSSTIDDES, dictionary);

// * * * * * * * * * * * *  Private Member Functions  * * * * * * * * * * * //

tmp<volScalarField> kOmegaSSTIDDES::alpha() const
{
    return max
    (
        0.25 - this->y_/static_cast<const volScalarField&>(hmax_()),
        scalar(-5)
    );
}


tmp<volScalarField> kOmegaSSTIDDES::ft
(
    const volScalarField& S
) const
{
    return tanh(pow3(sqr(Ct_)*rd(this->nuSgs_, S)));
}


tmp<volScalarField> kOmegaSSTIDDES::fl
(
    const volScalarField& S
) const
{
    return tanh(pow(sqr(Cl_)*rd(this->nu(), S), 10));
}


tmp<volScalarField> kOmegaSSTIDDES::rd
(
    const volScalarField& visc,
    const volScalarField& S
) const
{
    tmp<volScalarField> trd
    (
        min
        (
            visc
           /(
                max
                (
                    S,
                    dimensionedScalar("SMALL", S.dimensions(), SMALL)
                )
                *sqr(this->kappa_*this->y_)
            ),
            scalar(10)
        )
    );

    return trd;
}


tmp<volScalarField> kOmegaSSTIDDES::fdt
(
    const volScalarField& S
) const
{
    return 1 - tanh(pow(Cdt1_*rd(this->nuSgs_, S), Cdt2_));
}


tmp<volScalarField> kOmegaSSTIDDES::LIDDES() const
{
    const volVectorField& U = this->U_;
    const volScalarField S(mag(fvc::grad(U)));
    const volScalarField LLES(CDES_*this->delta());

    const volScalarField alpha(this->alpha());
    const volScalarField expTerm(exp(sqr(alpha)));

    tmp<volScalarField> fB = min(2*pow(expTerm, -9.0), scalar(1));
    tmp<volScalarField> fe1 =
        2*(pos(alpha)*pow(expTerm, -11.09) + neg(alpha)*pow(expTerm, -9.0));
    tmp<volScalarField> fe2 = 1 - max(ft(S), fl(S));

    // Original formulation from Shur et al. paper (2008)
    tmp<volScalarField> fe = max(fe1 - 1, scalar(0))*fe2;

    // Simplified formulation from Gritskevich et al. paper (2011) where fe = 0
    if (simplified_)
    {
        fe() *= scalar(0);
    }

    const volScalarField fdTilda(max(1 - fdt(S), fB));

    return max
    (
        fdTilda*(1 + fe)*Lt() + (1 - fdTilda)*LLES,
        dimensionedScalar("SMALL", dimLength, SMALL)
    );
}

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

tmp<volScalarField> kOmegaSSTIDDES::FDES() const
{
    return max
    (
        Lt()/LIDDES(),
        scalar(1.0)
    );
}

// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * * //

kOmegaSSTIDDES::kOmegaSSTIDDES
(
    volVectorField const& U,
    surfaceScalarField const& phi,
    transportModel& transport,
    word const& turbulenceModelName,
    word const& modelName
) :
    kOmegaSSTDES
    (
        U,
        phi,
        transport,
        turbulenceModelName,
        modelName
    ),
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
    Cdt1_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cdt1",
            this->coeffDict_,
            20
        )
    ),
    Cdt2_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cdt2",
            this->coeffDict_,
            3
        )
    ),
    Cl_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Cl",
            this->coeffDict_,
            5
        )
    ),
    Ct_
    (
        dimensioned<scalar>::lookupOrAddToDict
        (
            "Ct",
            this->coeffDict_,
            1.87
        )
    ),
    simplified_
    (
        coeffDict_.lookupOrDefault<Switch>("simplified", false)
    )
{}


bool kOmegaSSTIDDES::read()
{
    if (kOmegaSSTDES::read())
    {
        Cdt1_.readIfPresent(coeffDict());
        Cdt2_.readIfPresent(coeffDict());
        Cl_.readIfPresent(coeffDict());
        Ct_.readIfPresent(coeffDict());
        simplified_.readIfPresent("simplified",coeffDict());

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


