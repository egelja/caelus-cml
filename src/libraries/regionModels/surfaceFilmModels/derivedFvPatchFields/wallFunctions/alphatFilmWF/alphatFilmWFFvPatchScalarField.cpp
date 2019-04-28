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

#include "alphatFilmWFFvPatchScalarField.hpp"
#include "compressibleRASModel.hpp"
#include "surfaceFilmRegionModel.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "mappedWallPolyPatch.hpp"
#include "mapDistribute.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{
namespace RASModels
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

alphatFilmWallFunctionFvPatchScalarField::
alphatFilmWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(p, iF),
    B_(5.5),
    yPlusCrit_(11.05),
    Cmu_(0.09),
    kappa_(0.41),
    Prt_(0.85)
{}


alphatFilmWallFunctionFvPatchScalarField::
alphatFilmWallFunctionFvPatchScalarField
(
    const alphatFilmWallFunctionFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchScalarField(ptf, p, iF, mapper),
    B_(ptf.B_),
    yPlusCrit_(ptf.yPlusCrit_),
    Cmu_(ptf.Cmu_),
    kappa_(ptf.kappa_),
    Prt_(ptf.Prt_)
{}


alphatFilmWallFunctionFvPatchScalarField::
alphatFilmWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchScalarField(p, iF, dict),
    B_(dict.lookupOrDefault("B", 5.5)),
    yPlusCrit_(dict.lookupOrDefault("yPlusCrit", 11.05)),
    Cmu_(dict.lookupOrDefault("Cmu", 0.09)),
    kappa_(dict.lookupOrDefault("kappa", 0.41)),
    Prt_(dict.lookupOrDefault("Prt", 0.85))
{}


alphatFilmWallFunctionFvPatchScalarField::
alphatFilmWallFunctionFvPatchScalarField
(
    const alphatFilmWallFunctionFvPatchScalarField& fwfpsf
)
:
    fixedValueFvPatchScalarField(fwfpsf),
    B_(fwfpsf.B_),
    yPlusCrit_(fwfpsf.yPlusCrit_),
    Cmu_(fwfpsf.Cmu_),
    kappa_(fwfpsf.kappa_),
    Prt_(fwfpsf.Prt_)
{}


alphatFilmWallFunctionFvPatchScalarField::
alphatFilmWallFunctionFvPatchScalarField
(
    const alphatFilmWallFunctionFvPatchScalarField& fwfpsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedValueFvPatchScalarField(fwfpsf, iF),
    B_(fwfpsf.B_),
    yPlusCrit_(fwfpsf.yPlusCrit_),
    Cmu_(fwfpsf.Cmu_),
    kappa_(fwfpsf.kappa_),
    Prt_(fwfpsf.Prt_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void alphatFilmWallFunctionFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    typedef regionModels::surfaceFilmModels::surfaceFilmRegionModel modelType;

    // Since we're inside initEvaluate/evaluate there might be processor
    // comms underway. Change the tag we use.
    int oldTag = UPstream::msgType();
    UPstream::msgType() = oldTag+1;

    bool foundFilm =
        db().time().foundObject<modelType>("surfaceFilmProperties");

    if (!foundFilm)
    {
        // Do nothing on construction - film model doesn't exist yet
        return;
    }

    const label patchi = patch().index();

    // Retrieve phase change mass from surface film model
    const modelType& filmModel =
        db().time().lookupObject<modelType>("surfaceFilmProperties");

    const label filmPatchi = filmModel.regionPatchID(patchi);

    tmp<volScalarField> mDotFilm(filmModel.primaryMassTrans());
    scalarField mDotFilmp = mDotFilm().boundaryField()[filmPatchi];
    filmModel.toPrimary(filmPatchi, mDotFilmp);

    // Retrieve RAS turbulence model
    const RASModel& turbModel = db().lookupObject<RASModel>("RASProperties");

    const scalarField& y = turbModel.y()[patchi];
    const scalarField& rhow = turbModel.rho().boundaryField()[patchi];
    const tmp<volScalarField> tk = turbModel.k();
    const volScalarField& k = tk();
    const scalarField& muw = turbModel.mu().boundaryField()[patchi];
    const scalarField& alphaw = turbModel.alpha().boundaryField()[patchi];

    const scalar Cmu25 = pow(Cmu_, 0.25);

    // Populate alphat field values
    scalarField& alphat = *this;
    forAll(alphat, facei)
    {
        label faceCelli = patch().faceCells()[facei];

        scalar uTau = Cmu25*sqrt(k[faceCelli]);

        scalar yPlus = y[facei]*uTau/(muw[facei]/rhow[facei]);

        scalar Pr = muw[facei]/alphaw[facei];

        scalar factor = 0.0;
        scalar mStar = mDotFilmp[facei]/(y[facei]*uTau);
        if (yPlus > yPlusCrit_)
        {
            scalar expTerm = exp(min(50.0, yPlusCrit_*mStar*Pr));
            scalar yPlusRatio = yPlus/yPlusCrit_;
            scalar powTerm = mStar*Prt_/kappa_;
            factor =
                mStar/(expTerm*(pow(yPlusRatio, powTerm)) - 1.0 + ROOTVSMALL);
        }
        else
        {
            scalar expTerm = exp(min(50.0, yPlus*mStar*Pr));
            factor = mStar/(expTerm - 1.0 + ROOTVSMALL);
        }

        scalar dx = patch().deltaCoeffs()[facei];

        scalar alphaEff = dx*rhow[facei]*uTau*factor;

        alphat[facei] = max(alphaEff - alphaw[facei], 0.0);
    }

    // Restore tag
    UPstream::msgType() = oldTag;

    fixedValueFvPatchScalarField::updateCoeffs();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void alphatFilmWallFunctionFvPatchScalarField::write(Ostream& os) const
{
    fvPatchField<scalar>::write(os);
    os.writeKeyword("B") << B_ << token::END_STATEMENT << nl;
    os.writeKeyword("yPlusCrit") << yPlusCrit_ << token::END_STATEMENT << nl;
    os.writeKeyword("Cmu") << Cmu_ << token::END_STATEMENT << nl;
    os.writeKeyword("kappa") << kappa_ << token::END_STATEMENT << nl;
    os.writeKeyword("Prt") << Prt_ << token::END_STATEMENT << nl;
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchScalarField,
    alphatFilmWallFunctionFvPatchScalarField
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RASModels
} // End namespace compressible
} // End namespace CML

// ************************************************************************* //
