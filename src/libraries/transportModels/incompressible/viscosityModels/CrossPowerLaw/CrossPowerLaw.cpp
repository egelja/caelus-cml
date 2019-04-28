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

#include "CrossPowerLaw.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace viscosityModels
{
    defineTypeNameAndDebug(CrossPowerLaw, 0);

    addToRunTimeSelectionTable
    (
        viscosityModel,
        CrossPowerLaw,
        dictionary
    );
}
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

CML::tmp<CML::volScalarField>
CML::viscosityModels::CrossPowerLaw::calcNu() const
{

    return 
    (
        ((mu0_ - muInf_)/(scalar(1) + pow(lambda_*
        max(strainRate(), dimensionedScalar("VSMALL", dimless/dimTime, VSMALL)), n_)) 
        + muInf_)/rhoRef_
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::viscosityModels::CrossPowerLaw::CrossPowerLaw
(
    const word& name,
    const dictionary& viscosityProperties,
    const volVectorField& U,
    const surfaceScalarField& phi
)
:
    viscosityModel(name, viscosityProperties, U, phi),
    CrossPowerLawCoeffs_(viscosityProperties.subDict(typeName + "Coeffs")),
    mu0_(CrossPowerLawCoeffs_.lookup("mu0")),
    muInf_(CrossPowerLawCoeffs_.lookup("muInf")),
    lambda_(CrossPowerLawCoeffs_.lookup("lambda")),
    n_(CrossPowerLawCoeffs_.lookup("n")),
    rhoRef_(CrossPowerLawCoeffs_.lookup("rhoRef")),
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


void CML::viscosityModels::CrossPowerLaw::correct()
{
    viscosityModel::correct();
    nu_.storePrevIter();
    nu_ = calcNu();
    // Explicitly relax
    nu_.relax();
}


bool CML::viscosityModels::CrossPowerLaw::read
(
    const dictionary& viscosityProperties
)
{
    viscosityModel::read(viscosityProperties);

    CrossPowerLawCoeffs_ = viscosityProperties.subDict(typeName + "Coeffs");

    CrossPowerLawCoeffs_.lookup("mu0") >> mu0_;
    CrossPowerLawCoeffs_.lookup("muInf") >> muInf_;
    CrossPowerLawCoeffs_.lookup("lambda") >> lambda_;
    CrossPowerLawCoeffs_.lookup("n") >> n_;
    CrossPowerLawCoeffs_.lookup("rhoRef") >> rhoRef_;

    return true;
}


// ************************************************************************* //
