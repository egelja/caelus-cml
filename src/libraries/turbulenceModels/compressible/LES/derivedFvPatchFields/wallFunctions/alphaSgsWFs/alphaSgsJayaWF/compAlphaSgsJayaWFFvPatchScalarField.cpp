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

#include "compAlphaSgsJayaWFFvPatchScalarField.hpp"
#include "compressibleLESModel.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "wallFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{
namespace LESModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

scalar alphaSgsJayatillekeWallFunctionFvPatchScalarField::maxExp_ = 50.0;
scalar alphaSgsJayatillekeWallFunctionFvPatchScalarField::tolerance_ = 0.01;
label alphaSgsJayatillekeWallFunctionFvPatchScalarField::maxIters_ = 10;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void alphaSgsJayatillekeWallFunctionFvPatchScalarField::checkType()
{
    if (!isA<wallFvPatch>(patch()))
    {
        FatalErrorInFunction
            << "Patch type for patch " << patch().name() << " must be wall\n"
            << "Current patch type is " << patch().type() << nl
            << exit(FatalError);
    }
}


scalar alphaSgsJayatillekeWallFunctionFvPatchScalarField::Psmooth
(
    const scalar Prat
) const
{
    return 9.24*(pow(Prat, 0.75) - 1.0)*(1.0 + 0.28*exp(-0.007*Prat));
}


scalar alphaSgsJayatillekeWallFunctionFvPatchScalarField::yPlusTherm
(
    const scalar P,
    const scalar Prat
) const
{
    scalar ypt = 11.0;

    for (int i=0; i<maxIters_; i++)
    {
        scalar f = ypt - (log(E_*ypt)/kappa_ + P)/Prat;
        scalar df = 1.0 - 1.0/(ypt*kappa_*Prat);
        scalar yptNew = ypt - f/df;

        if (yptNew < VSMALL)
        {
            return 0;
        }
        else if (mag(yptNew - ypt) < tolerance_)
        {
            return yptNew;
        }
        else
        {
            ypt = yptNew;
        }
     }

    return ypt;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

alphaSgsJayatillekeWallFunctionFvPatchScalarField::
alphaSgsJayatillekeWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(p, iF),
    Prt_(0.85),
    Cmu_(0.09),
    kappa_(0.41),
    E_(9.8)
{
    checkType();
}


alphaSgsJayatillekeWallFunctionFvPatchScalarField::
alphaSgsJayatillekeWallFunctionFvPatchScalarField
(
    const alphaSgsJayatillekeWallFunctionFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchScalarField(ptf, p, iF, mapper),
    Prt_(ptf.Prt_),
    Cmu_(ptf.Cmu_),
    kappa_(ptf.kappa_),
    E_(ptf.E_)
{}


alphaSgsJayatillekeWallFunctionFvPatchScalarField::
alphaSgsJayatillekeWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchScalarField(p, iF, dict),
    Prt_(dict.lookupOrDefault<scalar>("Prt", 0.85)),
    Cmu_(dict.lookupOrDefault<scalar>("Cmu", 0.09)),
    kappa_(dict.lookupOrDefault<scalar>("kappa", 0.41)),
    E_(dict.lookupOrDefault<scalar>("E", 9.8))
{
    checkType();
}


alphaSgsJayatillekeWallFunctionFvPatchScalarField::
alphaSgsJayatillekeWallFunctionFvPatchScalarField
(
    const alphaSgsJayatillekeWallFunctionFvPatchScalarField& awfpsf
)
:
    fixedValueFvPatchScalarField(awfpsf),
    Prt_(awfpsf.Prt_),
    Cmu_(awfpsf.Cmu_),
    kappa_(awfpsf.kappa_),
    E_(awfpsf.E_)
{
    checkType();
}


alphaSgsJayatillekeWallFunctionFvPatchScalarField::
alphaSgsJayatillekeWallFunctionFvPatchScalarField
(
    const alphaSgsJayatillekeWallFunctionFvPatchScalarField& awfpsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(awfpsf, iF),
    Prt_(awfpsf.Prt_),
    Cmu_(awfpsf.Cmu_),
    kappa_(awfpsf.kappa_),
    E_(awfpsf.E_)
{
    checkType();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void alphaSgsJayatillekeWallFunctionFvPatchScalarField::evaluate
(
    const Pstream::commsTypes
)
{
    const LESModel& turbModel = db().lookupObject<LESModel>("LESProperties");

    // Field data
    const label patchi = patch().index();

    const scalarField& muw = turbModel.mu().boundaryField()[patchi];
    const scalarField muSgsw(turbModel.muSgs()().boundaryField()[patchi]);

    const scalarField& alphaw = turbModel.alpha().boundaryField()[patchi];
    scalarField& alphaSgsw = *this;

    const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
    const scalarField magUp(mag(Uw.patchInternalField() - Uw));
    const scalarField magGradUw(mag(Uw.snGrad()));

    const scalarField& rhow = turbModel.rho().boundaryField()[patchi];
    const fvPatchScalarField& hew =
        turbModel.thermo().he().boundaryField()[patchi];

    const scalarField& ry = patch().deltaCoeffs();

    // Heat flux [W/m2] - lagging alphaSgsw
    const scalarField qDot((alphaw + alphaSgsw)*hew.snGrad());

    // Populate boundary values
    forAll(alphaSgsw, facei)
    {
        // Calculate uTau using Newton-Raphson iteration
        scalar uTau =
            sqrt((muSgsw[facei] + muw[facei])/rhow[facei]*magGradUw[facei]);

        if (uTau > ROOTVSMALL)
        {
            label iter = 0;
            scalar err = GREAT;

            do
            {
                scalar kUu = min(kappa_*magUp[facei]/uTau, maxExp_);
                scalar fkUu = exp(kUu) - 1.0 - kUu*(1.0 + 0.5*kUu);

                scalar f =
                    - uTau/(ry[facei]*muw[facei]/rhow[facei])
                    + magUp[facei]/uTau
                    + 1.0/E_*(fkUu - 1.0/6.0*kUu*sqr(kUu));

                scalar df =
                    - 1.0/(ry[facei]*muw[facei]/rhow[facei])
                    - magUp[facei]/sqr(uTau)
                    - 1.0/E_*kUu*fkUu/uTau;

                scalar uTauNew = uTau - f/df;
                err = mag((uTau - uTauNew)/uTau);
                uTau = uTauNew;

            } while (uTau>VSMALL && err>tolerance_ && ++iter<maxIters_);

            scalar yPlus = uTau/ry[facei]/(muw[facei]/rhow[facei]);

            // Molecular Prandtl number
            scalar Pr = muw[facei]/alphaw[facei];

            // Molecular-to-turbulenbt Prandtl number ratio
            scalar Prat = Pr/Prt_;

            // Thermal sublayer thickness
            scalar P = Psmooth(Prat);
            scalar yPlusTherm = this->yPlusTherm(P, Prat);

            // Evaluate new effective thermal diffusivity
            scalar alphaEff = 0.0;
            if (yPlus < yPlusTherm)
            {
                scalar A = qDot[facei]*rhow[facei]*uTau/ry[facei];
                scalar B = qDot[facei]*Pr*yPlus;
                scalar C = Pr*0.5*rhow[facei]*uTau*sqr(magUp[facei]);
                alphaEff = A/(B + C + VSMALL);
            }
            else
            {
                scalar A = qDot[facei]*rhow[facei]*uTau/ry[facei];
                scalar B = qDot[facei]*Prt_*(1.0/kappa_*log(E_*yPlus) + P);
                scalar magUc = uTau/kappa_*log(E_*yPlusTherm) - mag(Uw[facei]);
                scalar C =
                    0.5*rhow[facei]*uTau
                   *(Prt_*sqr(magUp[facei]) + (Pr - Prt_)*sqr(magUc));
                alphaEff = A/(B + C + VSMALL);
            }

            // Update turbulent thermal diffusivity
            alphaSgsw[facei] = max(0.0, alphaEff - alphaw[facei]);

            if (debug)
            {
                Info<< "    uTau           = " << uTau << nl
                    << "    Pr             = " << Pr << nl
                    << "    Prt            = " << Prt_ << nl
                    << "    qDot           = " << qDot[facei] << nl
                    << "    yPlus          = " << yPlus << nl
                    << "    yPlusTherm     = " << yPlusTherm << nl
                    << "    alphaEff       = " << alphaEff << nl
                    << "    alphaw         = " << alphaw[facei] << nl
                    << "    alphaSgsw      = " << alphaSgsw[facei] << nl
                    << endl;
            }
        }
        else
        {
            alphaSgsw[facei] = 0.0;
        }
    }
}


void alphaSgsJayatillekeWallFunctionFvPatchScalarField::write(Ostream& os) const
{
    fvPatchField<scalar>::write(os);
    os.writeKeyword("Prt") << Prt_ << token::END_STATEMENT << nl;
    os.writeKeyword("Cmu") << Cmu_ << token::END_STATEMENT << nl;
    os.writeKeyword("kappa") << kappa_ << token::END_STATEMENT << nl;
    os.writeKeyword("E") << E_ << token::END_STATEMENT << nl;
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchScalarField,
    alphaSgsJayatillekeWallFunctionFvPatchScalarField
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace LESModels
} // End namespace compressible
} // End namespace CML

// ************************************************************************* //
