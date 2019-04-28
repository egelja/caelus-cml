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

#include "porousBafflePressureFvPatchFields.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

#include "compressible/turbulenceModel/compressibleTurbulenceModel.hpp"
#include "incompressible/turbulenceModel/turbulenceModel.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

makeTemplatePatchTypeField
(
    fvPatchScalarField,
    porousBafflePressureFvPatchScalarField
);

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Specialisation of the jump-condition for the pressure
template<>
void CML::porousBafflePressureFvPatchField<CML::scalar>::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const label patchI = patch().index();

    const surfaceScalarField& phi =
            db().lookupObject<surfaceScalarField>("phi");

    const fvsPatchField<scalar>& phip =
        patch().patchField<surfaceScalarField, scalar>(phi);

    scalarField Un(phip/patch().magSf());

    if (phi.dimensions() == dimensionSet(0, 3, -1, 0, 0))
    {
        const incompressible::turbulenceModel& turbModel =
            db().lookupObject<incompressible::turbulenceModel>
            (
                "turbulenceModel"
            );

        const scalarField nu = turbModel.nu()().boundaryField()[patchI];

        scalarField magUn(mag(Un));

        jump_ = -sign(Un)*(D_*nu + I_*0.5*magUn)*magUn*length_;
    }
    else
    {
        const compressible::turbulenceModel& turbModel =
            db().lookupObject<compressible::turbulenceModel>
            (
                "turbulenceModel"
            );

        const scalarField mu = turbModel.mu().boundaryField()[patchI];

        const scalarField rhow =
            patch().lookupPatchField<volScalarField, scalar>("rho");

        Un /= rhow;
        scalarField magUn(mag(Un));

        jump_ = -sign(Un)*(D_*mu + I_*0.5*rhow*magUn)*magUn*length_;
    }

    if (debug)
    {
        scalar avePressureJump = gAverage(jump_);
        scalar aveVelocity = gAverage(mag(Un));

        Info<< patch().boundaryMesh().mesh().name() << ':'
            << patch().name() << ':'
            << " Average pressure drop :" << avePressureJump
            << " Average velocity :" << aveVelocity
            << endl;
    }

    fixedJumpFvPatchField<scalar>::updateCoeffs();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
