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

#include "powerLaw.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace viscosityModels
{
    defineTypeNameAndDebug(powerLaw, 0);

    addToRunTimeSelectionTable
    (
        viscosityModel,
        powerLaw,
        dictionary
    );
}
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

CML::tmp<CML::volScalarField>
CML::viscosityModels::powerLaw::calcNu() const
{
    dimensionedScalar tone("tone", dimTime, 1.0);

    return max
    (
        muMin_,
        min
        (
            muMax_,
            k_*pow
            (
                max
                (
                    tone*strainRate(),
                    dimensionedScalar("VSMALL", dimless, VSMALL)
                ),
                n_.value() - scalar(1.0)
            )
        )
    )/rhoRef_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::viscosityModels::powerLaw::powerLaw
(
    const word& name,
    const dictionary& viscosityProperties,
    const volVectorField& U,
    const surfaceScalarField& phi
)
:
    viscosityModel(name, viscosityProperties, U, phi),
    powerLawCoeffs_(viscosityProperties.subDict(typeName + "Coeffs")),
    k_(powerLawCoeffs_.lookup("k")),
    n_(powerLawCoeffs_.lookup("n")),
    muMin_(powerLawCoeffs_.lookup("muMin")),
    muMax_(powerLawCoeffs_.lookup("muMax")),
    rhoRef_(powerLawCoeffs_.lookup("rhoRef")),
    nu_
    (
        IOobject
        (
            name,
            U_.time().timeName(),
            U_.db(),
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        calcNu()
    )
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


void CML::viscosityModels::powerLaw::correct()
{
    viscosityModel::correct();
    nu_.storePrevIter();
    nu_ = calcNu();
    nu_.relax();
}


bool CML::viscosityModels::powerLaw::read
(
    const dictionary& viscosityProperties
)
{
    viscosityModel::read(viscosityProperties);

    powerLawCoeffs_ = viscosityProperties.subDict(typeName + "Coeffs");

    powerLawCoeffs_.lookup("k") >> k_;
    powerLawCoeffs_.lookup("n") >> n_;
    powerLawCoeffs_.lookup("muMin") >> muMin_;
    powerLawCoeffs_.lookup("muMax") >> muMax_;
    powerLawCoeffs_.lookup("rhoRef") >> rhoRef_;

    return true;
}


// ************************************************************************* //
