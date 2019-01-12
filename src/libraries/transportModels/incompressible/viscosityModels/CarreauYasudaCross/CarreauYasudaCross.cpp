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

#include "CarreauYasudaCross.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace viscosityModels
{
    defineTypeNameAndDebug(CarreauYasudaCross, 0);
    addToRunTimeSelectionTable
    (
        viscosityModel,
        CarreauYasudaCross,
        dictionary
    );
}
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

CML::tmp<CML::volScalarField>
CML::viscosityModels::CarreauYasudaCross::calcNu() const
{
    dimensionedScalar tone("tone", dimTime, 1.0);
    dimensionedScalar rtone("rtone", dimless/dimTime, 1.0);

    tmp<volScalarField> sr(strainRate());

    return
    (
      (muInf_ + (mu0_ - muInf_)*pow(scalar(1) + pow(lambda_*
      max(sr(), dimensionedScalar("VSMALL", dimless/dimTime, VSMALL)), a_), (n_ - 1.0)/a_))
      /rhoRef_
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::viscosityModels::CarreauYasudaCross::CarreauYasudaCross
(
    const word& name,
    const dictionary& viscosityProperties,
    const volVectorField& U,
    const surfaceScalarField& phi
)
:
    viscosityModel(name, viscosityProperties, U, phi),
    CarreauYasudaCrossCoeffs_(viscosityProperties.subDict(typeName + "Coeffs")),
    mu0_(CarreauYasudaCrossCoeffs_.lookup("mu0")),
    muInf_(CarreauYasudaCrossCoeffs_.lookup("muInf")),
    lambda_(CarreauYasudaCrossCoeffs_.lookup("lambda")),
    n_(CarreauYasudaCrossCoeffs_.lookup("n")),
    a_(CarreauYasudaCrossCoeffs_.lookup("a")),
    rhoRef_(CarreauYasudaCrossCoeffs_.lookup("rhoRef")),
    nu_
    (
        IOobject
        (
            "nu",
            U_.time().timeName(),
            U_.db(),
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        calcNu()
    )
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::viscosityModels::CarreauYasudaCross::correct()
{
    viscosityModel::correct();
    nu_.storePrevIter();
    nu_ = calcNu();
    // Explicitly relax
    nu_.relax();
}


bool CML::viscosityModels::CarreauYasudaCross::read
(
    const dictionary& viscosityProperties
)
{
    viscosityModel::read(viscosityProperties);

    CarreauYasudaCrossCoeffs_ = viscosityProperties.subDict(typeName + "Coeffs");

    CarreauYasudaCrossCoeffs_.lookup("mu0") >> mu0_;
    CarreauYasudaCrossCoeffs_.lookup("muInf") >> muInf_;
    CarreauYasudaCrossCoeffs_.lookup("lambda") >> lambda_;
    CarreauYasudaCrossCoeffs_.lookup("n") >> n_;
    CarreauYasudaCrossCoeffs_.lookup("a") >> a_;
    CarreauYasudaCrossCoeffs_.lookup("rhoRef") >> rhoRef_;

    return true;
}


// ************************************************************************* //
