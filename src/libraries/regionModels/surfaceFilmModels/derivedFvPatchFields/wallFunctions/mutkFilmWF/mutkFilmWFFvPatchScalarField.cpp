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

#include "mutkFilmWFFvPatchScalarField.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "compressibleRASModel.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "surfaceFilmRegionModel.hpp"
#include "mappedWallPolyPatch.hpp"
#include "mapDistribute.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{
namespace RASModels
{

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

tmp<scalarField> mutkFilmWallFunctionFvPatchScalarField::calcUTau
(
    const scalarField& magGradU
) const
{
    tmp<scalarField> tuTau(new scalarField(patch().size(), 0.0));
    scalarField& uTau = tuTau();

    typedef regionModels::surfaceFilmModels::surfaceFilmRegionModel modelType;

    bool foundFilm =
        db().time().foundObject<modelType>("surfaceFilmProperties");

    if (!foundFilm)
    {
        // Do nothing on construction - film model doesn't exist yet
        return tuTau;
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
    const fvPatchScalarField& muw = turbModel.mu().boundaryField()[patchi];
    const tmp<volScalarField> tk = turbModel.k();
    const volScalarField& k = tk();

    const scalar Cmu25 = pow(Cmu_, 0.25);

    forAll(uTau, facei)
    {
        label faceCelli = patch().faceCells()[facei];

        scalar ut = Cmu25*sqrt(k[faceCelli]);

        scalar yPlus = y[facei]*ut/muw[facei];

        scalar mStar = mDotFilmp[facei]/(y[facei]*ut);

        scalar factor = 0.0;
        if (yPlus > yPlusCrit_)
        {
            scalar expTerm = exp(min(50.0, B_*mStar));
            scalar powTerm = pow(yPlus, mStar/kappa_);
            factor = mStar/(expTerm*powTerm - 1.0 + ROOTVSMALL);
        }
        else
        {
            scalar expTerm = exp(min(50.0, mStar));
            factor = mStar/(expTerm*yPlus - 1.0 + ROOTVSMALL);
        }

        uTau[facei] = sqrt(max(0, magGradU[facei]*ut*factor));
    }

    return tuTau;
}


tmp<scalarField> mutkFilmWallFunctionFvPatchScalarField::calcMut() const
{
    const label patchi = patch().index();

    const RASModel& turbModel = db().lookupObject<RASModel>("RASProperties");
    const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
    const scalarField magGradU(mag(Uw.snGrad()));
    const scalarField& rhow = turbModel.rho().boundaryField()[patchi];
    const scalarField& muw = turbModel.mu().boundaryField()[patchi];

    return max
    (
        scalar(0),
        rhow*sqr(calcUTau(magGradU))/(magGradU + ROOTVSMALL) - muw
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

mutkFilmWallFunctionFvPatchScalarField::mutkFilmWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mutkWallFunctionFvPatchScalarField(p, iF),
    B_(5.5),
    yPlusCrit_(11.05)
{}


mutkFilmWallFunctionFvPatchScalarField::mutkFilmWallFunctionFvPatchScalarField
(
    const mutkFilmWallFunctionFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mutkWallFunctionFvPatchScalarField(ptf, p, iF, mapper),
    B_(5.5),
    yPlusCrit_(11.05)
{}


mutkFilmWallFunctionFvPatchScalarField::mutkFilmWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mutkWallFunctionFvPatchScalarField(p, iF, dict),
    B_(dict.lookupOrDefault("B", 5.5)),
    yPlusCrit_(dict.lookupOrDefault("yPlusCrit", 11.05))
{}


mutkFilmWallFunctionFvPatchScalarField::mutkFilmWallFunctionFvPatchScalarField
(
    const mutkFilmWallFunctionFvPatchScalarField& wfpsf
)
:
    mutkWallFunctionFvPatchScalarField(wfpsf),
    B_(wfpsf.B_),
    yPlusCrit_(wfpsf.yPlusCrit_)
{}


mutkFilmWallFunctionFvPatchScalarField::mutkFilmWallFunctionFvPatchScalarField
(
    const mutkFilmWallFunctionFvPatchScalarField& wfpsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mutkWallFunctionFvPatchScalarField(wfpsf, iF),
    B_(wfpsf.B_),
    yPlusCrit_(wfpsf.yPlusCrit_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

tmp<scalarField> mutkFilmWallFunctionFvPatchScalarField::yPlus() const
{
    const label patchi = patch().index();

    const RASModel& turbModel = db().lookupObject<RASModel>("RASProperties");
    const scalarField& y = turbModel.y()[patchi];
    const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
    const scalarField& rhow = turbModel.rho().boundaryField()[patchi];
    const scalarField& muw = turbModel.mu().boundaryField()[patchi];

    return y*calcUTau(mag(Uw.snGrad()))/(muw/rhow);
}


void mutkFilmWallFunctionFvPatchScalarField::write(Ostream& os) const
{
    fvPatchField<scalar>::write(os);
    writeLocalEntries(os);
    os.writeKeyword("B") << B_ << token::END_STATEMENT << nl;
    os.writeKeyword("yPlusCrit") << yPlusCrit_ << token::END_STATEMENT << nl;
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField(fvPatchScalarField, mutkFilmWallFunctionFvPatchScalarField);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RASModels
} // End namespace compressible
} // End namespace CML

// ************************************************************************* //
