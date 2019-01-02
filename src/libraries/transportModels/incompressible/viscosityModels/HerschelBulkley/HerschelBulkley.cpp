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

#include "HerschelBulkley.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace viscosityModels
{
    defineTypeNameAndDebug(HerschelBulkley, 0);

    addToRunTimeSelectionTable
    (
        viscosityModel,
        HerschelBulkley,
        dictionary
    );
}
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

CML::tmp<CML::volScalarField>
CML::viscosityModels::HerschelBulkley::calcNu() const
{
    dimensionedScalar tone("tone", dimTime, 1.0);
    dimensionedScalar rtone("rtone", dimless/dimTime, 1.0);

 return
 (
     min
     (
         mu0_,
         (tau0_ + k_*rtone*(pow(tone*
         mag(max(strainRate(), dimensionedScalar("VSMALL", dimless/dimTime, VSMALL)) - tau0_/mu0_), n_)))
        /(max(strainRate(), dimensionedScalar("VSMALL", dimless/dimTime, VSMALL)))
     )/rhoRef_
 );

}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::viscosityModels::HerschelBulkley::HerschelBulkley
(
    const word& name,
    const dictionary& viscosityProperties,
    const volVectorField& U,
    const surfaceScalarField& phi
)
:
    viscosityModel(name, viscosityProperties, U, phi),
    HerschelBulkleyCoeffs_(viscosityProperties.subDict(typeName + "Coeffs")),
    k_(HerschelBulkleyCoeffs_.lookup("k")),
    n_(HerschelBulkleyCoeffs_.lookup("n")),
    tau0_(HerschelBulkleyCoeffs_.lookup("tau0")),
    mu0_(HerschelBulkleyCoeffs_.lookup("mu0")),
    rhoRef_(HerschelBulkleyCoeffs_.lookup("rhoRef")),
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


void CML::viscosityModels::HerschelBulkley::correct()
{
    viscosityModel::correct();
    nu_.storePrevIter();
    nu_ = calcNu();
    // Explicitly relax
    nu_.relax();
}


bool CML::viscosityModels::HerschelBulkley::read
(
    const dictionary& viscosityProperties
)
{
    viscosityModel::read(viscosityProperties);

    HerschelBulkleyCoeffs_ = viscosityProperties.subDict(typeName + "Coeffs");

    HerschelBulkleyCoeffs_.lookup("k") >> k_;
    HerschelBulkleyCoeffs_.lookup("n") >> n_;
    HerschelBulkleyCoeffs_.lookup("tau0") >> tau0_;
    HerschelBulkleyCoeffs_.lookup("mu0") >> mu0_;
    HerschelBulkleyCoeffs_.lookup("rhoRef") >> rhoRef_;

    return true;
}


// ************************************************************************* //
