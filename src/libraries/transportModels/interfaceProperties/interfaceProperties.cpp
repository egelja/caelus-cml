/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
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

#include "interfaceProperties.hpp"
#include "alphaContactAngleFvPatchScalarField.hpp"
#include "mathematicalConstants.hpp"
#include "surfaceInterpolate.hpp"
#include "fvcDiv.hpp"
#include "fvcGrad.hpp"
#include "fvcSnGrad.hpp"

// * * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * //
#ifdef windows
//Symbol to force loading at runtime
extern "C"
void interfacePropertiesLoad()
{}
#endif

const CML::scalar CML::interfaceProperties::convertToRad =
    CML::constant::mathematical::pi/180.0;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Correction for the boundary condition on the unit normal nHat on
// walls to produce the correct contact angle.

// The dynamic contact angle is calculated from the component of the
// velocity on the direction of the interface, parallel to the wall.

void CML::interfaceProperties::correctContactAngle
(
    surfaceVectorField::GeometricBoundaryField& nHatb,
    surfaceVectorField::GeometricBoundaryField& gradAlphaf
) const
{
    const volScalarField::GeometricBoundaryField& abf = alpha1_.boundaryField();

    const fvBoundaryMesh& boundary = mesh_.boundary();

    forAll(boundary, patchi)
    {
        if (isA<alphaContactAngleFvPatchScalarField>(abf[patchi]))
        {
            alphaContactAngleFvPatchScalarField& acap =
                const_cast<alphaContactAngleFvPatchScalarField&>
                (
                    refCast<const alphaContactAngleFvPatchScalarField>
                    (
                        abf[patchi]
                    )
                );

            fvsPatchVectorField& nHatp = nHatb[patchi];
            const scalarField theta
            (
                convertToRad*acap.theta(U_.boundaryField()[patchi], nHatp)
            );

            const vectorField nf
            (
                boundary[patchi].nf()
            );

            // Reset nHatp to correspond to the contact angle

            const scalarField a12(nHatp & nf);
            const scalarField b1(cos(theta));

            scalarField b2(nHatp.size());
            forAll(b2, facei)
            {
                b2[facei] = cos(acos(a12[facei]) - theta[facei]);
            }

            const scalarField det(1.0 - a12*a12);

            scalarField a((b1 - a12*b2)/det);
            scalarField b((b2 - a12*b1)/det);

            nHatp = a*nf + b*nHatp;
            nHatp /= (mag(nHatp) + deltaN_.value());

            acap.gradient() = (nf & nHatp)*mag(gradAlphaf[patchi]);
            acap.evaluate();
        }
    }
}


void CML::interfaceProperties::calculateK()
{
    const surfaceVectorField& Sf = mesh_.Sf();

    // Cell gradient of alpha
    const volVectorField gradAlpha(fvc::grad(alpha1_, "nHat"));

    // Interpolated face-gradient of alpha
    surfaceVectorField gradAlphaf(fvc::interpolate(gradAlpha));

    // Face unit interface normal
    surfaceVectorField nHatfv(gradAlphaf/(mag(gradAlphaf) + deltaN_));
    correctContactAngle(nHatfv.boundaryField(), gradAlphaf.boundaryField());

    // Face unit interface normal flux
    nHatf_ = nHatfv & Sf;

    // Simple expression for curvature
    K_ = -fvc::div(nHatf_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::interfaceProperties::interfaceProperties
(
    const volVectorField& U,
    const surfaceScalarField& phi,
    const volScalarField& alpha1,
    const IOdictionary& dict
)
:
    mesh_(alpha1.mesh()),
    phi_(phi),
    U_(U),
    alpha1_(alpha1),
    transportPropertiesDict_(dict),
    dynCompPtr_(dynCompressionModel::New(mesh_, U, phi, alpha1)),
    sigma_(dict.lookup("sigma")),
    deltaN_
    (
        "deltaN",
        1e-8/pow(average(mesh_.V()), 1.0/3.0)
    ),
    nHatf_
    (
        IOobject
        (
            "nHatf",
            mesh_.time().timeName(),
            mesh_
        ),
        mesh_,
        dimensionedScalar("nHatf", dimArea, 0.0)
    ),
    K_
    (
        IOobject
        (
            "interfaceProperties:K",
            mesh_.time().timeName(),
            mesh_
        ),
        mesh_,
        dimensionedScalar("K", dimless/dimLength, 0.0)
    )
{
    calculateK();
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::tmp<CML::surfaceScalarField>
CML::interfaceProperties::surfaceTensionForce() const
{
    return fvc::interpolate(sigmaK())*fvc::snGrad(alpha1_);
}


CML::tmp<CML::volScalarField>
CML::interfaceProperties::nearInterface() const
{
    return pos(alpha1_ - 0.01)*pos(0.99 - alpha1_);
}

CML::tmp<CML::surfaceScalarField>
CML::interfaceProperties::phir()
{
    return nHatf_*dynCompPtr_->phic();
}


bool CML::interfaceProperties::read()
{
    transportPropertiesDict_.lookup("sigma") >> sigma_;

    return true;
}


// ************************************************************************* //
