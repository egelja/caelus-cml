/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

#include "setWaveField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(setWaveField, 0);

// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

setWaveField::setWaveField
(
    const fvMesh& mesh,
    volVectorField& U,
    volScalarField& alpha,
    volScalarField& p
)
:
    convexPolyhedral(mesh, true),

    mesh_(mesh),

    U_(U),

    alpha_(alpha),

    p_(p),

    name_
    (
       (U.db().lookupObject<IOdictionary>("waveProperties"))
      .lookup("initializationName")
    ),

    waveProps_(waveTheories::waveTheory::New(name_, mesh_))
{
    if (waveProps_->conflictTSoftInitialise())
    {
        WarningInFunction
          << "\n    The specified value of Tsoft is non-zero in the waveType:"
          << " `" << waveProps_->type() << "'" << nl
          << "    specified in the sub-dictionary waveProperties::"
          << name_ << "Coeffs"
          << "\n\n    Consequently, the initialised `wave field' is set to a"
          << " horizontal free surface with zero velocity." << nl << endl;
    }

}


setWaveField::setWaveField
(
    const fvMesh& mesh,
    const word name,
    volVectorField& U,
    volScalarField& alpha,
    volScalarField& p
)
:
    convexPolyhedral(mesh, true),

    mesh_(mesh),

    U_(U),

    alpha_(alpha),

    p_(p),

    name_(name),

    waveProps_(waveTheories::waveTheory::New(name_, mesh_))
{

}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void setWaveField::signedPointToSurfaceDistance
(
    const pointField& pp,
    scalarField& sd
)
{
    forAll (pp, pointi)
    {
        sd[pointi] = signedPointToSurfaceDistance(pp[pointi]);
    }
}


scalar setWaveField::signedPointToSurfaceDistance
(
    const point& pp
) const
{
    scalar temp = waveProps_->eta(pp, U_.db().time().value());
    temp += (waveProps_->returnDir() & pp);
    temp *= -1.0;

    return temp;
}


void setWaveField::correct()
{
    const scalarField& V(mesh_.V());

    forAll (U_, celli)
    {
        localCell lc = dividePolyhedral( celli, point::zero, point::one);

        vector UTarget(waveProps_->windVelocity(U_.db().time().value()));
        scalar pTarget(0.0);
        scalar alphaTarget(0.0);

        // If size is less than 4, then one cannot evaluate centre/magnitude
        // without getting an floating point exception error
        if (lc.ccNeg().size() >= 4)
        {
            UTarget = waveProps_->U(lc.centreNeg(), U_.db().time().value());
            pTarget = waveProps_->p(lc.centreNeg(), U_.db().time().value());
            alphaTarget = lc.magNeg()/V[celli];
        }

        U_[celli] = UTarget;
        alpha_[celli] = alphaTarget;
        p_[celli] = pTarget;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
