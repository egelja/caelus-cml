/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

#include "Gulders.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace laminarFlameSpeedModels
{
    defineTypeNameAndDebug(Gulders, 0);

    addToRunTimeSelectionTable
    (
        laminarFlameSpeed,
        Gulders,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::laminarFlameSpeedModels::Gulders::Gulders
(
    const dictionary& dict,
    const psiuReactionThermo& ct
)
:
    laminarFlameSpeed(dict, ct),

    coeffsDict_(dict.subDict(typeName + "Coeffs").subDict(fuel_)),
    W_(readScalar(coeffsDict_.lookup("W"))),
    eta_(readScalar(coeffsDict_.lookup("eta"))),
    xi_(readScalar(coeffsDict_.lookup("xi"))),
    f_(readScalar(coeffsDict_.lookup("f"))),
    alpha_(readScalar(coeffsDict_.lookup("alpha"))),
    beta_(readScalar(coeffsDict_.lookup("beta")))
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::tmp<CML::volScalarField> CML::laminarFlameSpeedModels::Gulders::Su0pTphi
(
    const volScalarField& p,
    const volScalarField& Tu,
    scalar phi
) const
{
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
        Su0[celli] = Su0pTphi(p[celli], Tu[celli], phi, 0.0);
    }

    forAll(Su0.boundaryField(), patchi)
    {
        forAll(Su0.boundaryField()[patchi], facei)
        {
            Su0.boundaryField()[patchi][facei] =
                Su0pTphi
                (
                    p.boundaryField()[patchi][facei],
                    Tu.boundaryField()[patchi][facei],
                    phi,
                    0.0
                );
        }
    }

    return tSu0;
}


CML::tmp<CML::volScalarField> CML::laminarFlameSpeedModels::Gulders::Su0pTphi
(
    const volScalarField& p,
    const volScalarField& Tu,
    const volScalarField& phi
) const
{
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
        Su0[celli] = Su0pTphi(p[celli], Tu[celli], phi[celli], 0.0);
    }

    forAll(Su0.boundaryField(), patchi)
    {
        forAll(Su0.boundaryField()[patchi], facei)
        {
            Su0.boundaryField()[patchi][facei] =
                Su0pTphi
                (
                    p.boundaryField()[patchi][facei],
                    Tu.boundaryField()[patchi][facei],
                    phi.boundaryField()[patchi][facei],
                    0.0
                );
        }
    }

    return tSu0;
}


CML::tmp<CML::volScalarField>
CML::laminarFlameSpeedModels::Gulders::operator()() const
{
    if (psiuReactionThermo_.composition().contains("ft"))
    {
        const volScalarField& ft = psiuReactionThermo_.composition().Y("ft");

        return Su0pTphi
        (
            psiuReactionThermo_.p(),
            psiuReactionThermo_.Tu(),
            dimensionedScalar
            (
                psiuReactionThermo_.lookup("stoichiometricAirFuelMassRatio")
            )*ft/max(1 - ft, SMALL)
        );
    }
    else
    {
        return Su0pTphi
        (
            psiuReactionThermo_.p(),
            psiuReactionThermo_.Tu(),
            equivalenceRatio_
        );
    }
}
