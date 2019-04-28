/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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

#include "addToRunTimeSelectionTable.hpp"
#include "powerLawPorosity.hpp"
#include "geometricOneField.hpp"
#include "fvMatrices.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    namespace porosityModels
    {
        defineTypeNameAndDebug(powerLaw, 0);
        addToRunTimeSelectionTable(porosityModel, powerLaw, mesh);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::porosityModels::powerLaw::powerLaw
(
    const word& name,
    const word& modelType,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& cellZoneName
)
:
    porosityModel(name, modelType, mesh, dict, cellZoneName),
    C0_(readScalar(coeffs_.lookup("C0"))),
    C1_(readScalar(coeffs_.lookup("C1"))),
    rhoName_(coeffs_.lookupOrDefault<word>("rho", "rho"))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::porosityModels::powerLaw::~powerLaw()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::porosityModels::powerLaw::calcTranformModelData()
{
    // nothing to be transformed
}


void CML::porosityModels::powerLaw::calcForce
(
    const volVectorField& U,
    const volScalarField& rho,
    const volScalarField& mu,
    vectorField& force
) const
{
    scalarField Udiag(U.size(), 0.0);
    const scalarField& V = mesh_.V();

    apply(Udiag, V, rho, U);

    force = Udiag*U;
}


void CML::porosityModels::powerLaw::correct
(
    fvVectorMatrix& UEqn
) const
{
    const vectorField& U = UEqn.psi();
    const scalarField& V = mesh_.V();
    scalarField& Udiag = UEqn.diag();
 
    if (UEqn.dimensions() == dimForce)
    {
        const volScalarField& rho =
            mesh_.lookupObject<volScalarField>(rhoName_);

        apply(Udiag, V, rho, U);
    }
    else
    {
        apply(Udiag, V, geometricOneField(), U);
    }
}


void CML::porosityModels::powerLaw::correct
(
    fvVectorMatrix& UEqn,
    const volScalarField& rho,
    const volScalarField& mu
) const
{
    const vectorField& U = UEqn.psi();
    const scalarField& V = mesh_.V();
    scalarField& Udiag = UEqn.diag();
 
    apply(Udiag, V, rho, U);
}


void CML::porosityModels::powerLaw::correct
(
    const fvVectorMatrix& UEqn,
    volTensorField& AU
) const
{
    const vectorField& U = UEqn.psi();

    if (UEqn.dimensions() == dimForce)
    {
        const volScalarField& rho =
            mesh_.lookupObject<volScalarField>(rhoName_);

        apply(AU, rho, U);
    }
    else
    {
        apply(AU, geometricOneField(), U);
    }
}


bool CML::porosityModels::powerLaw::writeData(Ostream& os) const
{
    os  << indent << name_ << endl;
    dict_.write(os);

    return true;
}


// ************************************************************************* //
