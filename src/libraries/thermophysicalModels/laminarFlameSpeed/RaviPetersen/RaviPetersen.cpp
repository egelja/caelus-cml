/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
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

#include "RaviPetersen.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace laminarFlameSpeedModels
{
    defineTypeNameAndDebug(RaviPetersen, 0);

    addToRunTimeSelectionTable
    (
        laminarFlameSpeed,
        RaviPetersen,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::laminarFlameSpeedModels::RaviPetersen::RaviPetersen
(
    const dictionary& dict,
    const psiuReactionThermo& ct
)
:
    laminarFlameSpeed(dict, ct),
    coeffsDict_(dict.subDict(typeName + "Coeffs").subDict(fuel_)),
    pPoints_(coeffsDict_.lookup("pPoints")),
    EqRPoints_(coeffsDict_.lookup("EqRPoints")),
    alpha_(coeffsDict_.lookup("alpha")),
    beta_(coeffsDict_.lookup("beta")),
    TRef_(readScalar(coeffsDict_.lookup("TRef")))
{
    checkPointsMonotonicity("equivalenceRatio", EqRPoints_);
    checkPointsMonotonicity("pressure", pPoints_);
    checkCoefficientArrayShape("alpha", alpha_);
    checkCoefficientArrayShape("beta", beta_);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::laminarFlameSpeedModels::RaviPetersen::checkPointsMonotonicity
(
    const word& name,
    const List<scalar>& x
) const
{
    for (label i = 1; i < x.size(); i ++)
    {
        if (x[i] <= x[i-1])
        {
            FatalIOErrorInFunction
            (
                coeffsDict_
            )   << "Data points for the " << name
                << " do not increase monotonically" << endl
                << exit(FatalIOError);
        }
    }
}


void CML::laminarFlameSpeedModels::RaviPetersen::checkCoefficientArrayShape
(
    const word& name,
    const List<List<List<scalar>>>& x
) const
{
    bool ok = true;

    ok &= x.size() == EqRPoints_.size() - 1;

    forAll(x, i)
    {
        ok &= x[i].size() == pPoints_.size();

        forAll(x[i], j)
        {
            ok &= x[i][j].size() == x[i][0].size();
        }
    }

    if (!ok)
    {
        FatalIOErrorInFunction
        (
            coeffsDict_
        )   << "Inconsistent size of " << name << " coefficients array" << endl
            << exit(FatalIOError);
    }
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::tmp<CML::volScalarField>
CML::laminarFlameSpeedModels::RaviPetersen::operator()() const
{
    const volScalarField& p = psiuReactionThermo_.p();
    const volScalarField& Tu = psiuReactionThermo_.Tu();

    volScalarField EqR
    (
        IOobject
        (
            "EqR",
            p.time().timeName(),
            p.db(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        p.mesh(),
        dimensionedScalar("EqR", dimless, 0.0)
    );

    if (psiuReactionThermo_.composition().contains("ft"))
    {
        const volScalarField& ft = psiuReactionThermo_.composition().Y("ft");

        EqR =
            dimensionedScalar
            (
                psiuReactionThermo_.lookup("stoichiometricAirFuelMassRatio")
            )*ft/max(1 - ft, SMALL);
    }
    else
    {
        EqR = equivalenceRatio_;
    }

    tmp<volScalarField> tSu0
    (
        new volScalarField
        (
            IOobject
            (
                "Su0",
                p.time().timeName(),
                p.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            p.mesh(),
            dimensionedScalar("Su0", dimVelocity, 0.0)
        )
    );

    volScalarField& Su0 = tSu0();

    forAll(Su0, celli)
    {
        Su0[celli] = speed(EqR[celli], p[celli], Tu[celli]);
    }

    return tSu0;
}
