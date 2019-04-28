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

#include "relaxationZone.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(relaxationZone, 0);

// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //


relaxationZone::relaxationZone
(
    const fvMesh& mesh,
    volVectorField& U,
    volScalarField& alpha
)
:
    mesh_(mesh),
    U_(U),
    alpha_(alpha),
    relaxNames_((mesh_.thisDb().lookupObject<IOdictionary>("waveProperties"))
                .lookup("relaxationNames")),
    relaxSchemePtr_(relaxNames_.size()),
    targetAlpha_(nullptr),
    targetVelocity_(nullptr)
{
    forAll (relaxNames_, relaxi)
    {
        relaxSchemePtr_[relaxi] = relaxationSchemes::relaxationScheme::
            New(relaxNames_[relaxi], mesh_, U_, alpha_);
    }
}


// * * * * * * * * * * * * * * * Member functions  * * * * * * * * * * * * * //


void relaxationZone::resetTargetFields()
{
    if (targetAlpha_ != nullptr)
    {
        (*targetAlpha_).internalField() = 0.0;
    }

    if (targetVelocity_ != nullptr)
    {
        (*targetVelocity_).internalField() = vector::zero;
    }
}


void relaxationZone::correctBoundaries()
{
    alpha_.correctBoundaryConditions();

    U_.correctBoundaryConditions();

    if (targetAlpha_ != nullptr)
    {
        (*targetAlpha_).correctBoundaryConditions();
    }

    if (targetVelocity_ != nullptr)
    {
        (*targetVelocity_).correctBoundaryConditions();
    }
}


void relaxationZone::correct()
{
    scalar preTime = mesh_.time().elapsedCpuTime();

    resetTargetFields();

    forAll (relaxSchemePtr_, relaxi)
    {
        relaxSchemePtr_[relaxi]->correct();
    }

    correctBoundaries();

    Info << "Relaxing time: " << mesh_.time().elapsedCpuTime() - preTime
         << " s" << endl;
}

void relaxationZone::addSup(fvMatrix<vector>& eqn)
{
    forAll (relaxSchemePtr_, relaxi)
    {
        relaxSchemePtr_[relaxi]->addSup(eqn);
    }
}

void relaxationZone::constrain(fvMatrix<vector>& eqn)
{
    forAll (relaxSchemePtr_, relaxi)
    {
        relaxSchemePtr_[relaxi]->constrain(eqn);
    }
}

tmp<volScalarField> relaxationZone::numericalBeach()
{
    // Return field
    tmp<volScalarField> tartificialViscotity
    (
        new volScalarField
        (
            IOobject
            (
                "muArti",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("null", dimless/dimTime, 0.0),
            "fixedValue"
        )
    );

    volScalarField& artificialViscosity(tartificialViscotity());

    forAll (relaxSchemePtr_, relaxi)
    {
        relaxSchemePtr_[relaxi]->numericalBeach(artificialViscosity);
    }

    return tartificialViscotity;
}


const volScalarField& relaxationZone::targetAlphaField()
{
    if (!targetAlpha_)
    {
        targetAlpha_ = new volScalarField
        (
            IOobject
            (
                "targetAlphaField",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh_,
            dimensionedScalar("null", dimless, 0.0),
            "zeroGradient"
        );
    }

    forAll (relaxSchemePtr_, relaxi)
    {
        relaxSchemePtr_[relaxi]->targetAlphaField(*targetAlpha_);
    }

    return *targetAlpha_;
}


const volVectorField& relaxationZone::targetVelocityField()
{
    if (!targetVelocity_)
    {
        targetVelocity_ = new volVectorField
        (
            IOobject
            (
                "targetVelocityField",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            mesh_,
            dimensionedVector("null", dimVelocity, vector::zero),
            "zeroGradient"
        );
    }

    forAll (relaxSchemePtr_, relaxi)
    {
        relaxSchemePtr_[relaxi]->targetVelocityField(*targetVelocity_);
    }

    return *targetVelocity_;
}


} // end namespace CML
