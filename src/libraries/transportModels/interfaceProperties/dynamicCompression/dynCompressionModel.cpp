/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2017 Applied CCM
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

#include "dynCompressionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(dynCompressionModel, 0);
    defineRunTimeSelectionTable(dynCompressionModel, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::dynCompressionModel::dynCompressionModel
(
    const fvMesh& mesh,
    const volVectorField& U,
    const surfaceScalarField& phi,
    const volScalarField& alpha1
)
:
    mesh_(mesh),
    U_(U),
    phi_(phi),
    alpha1_(alpha1),
    cAlpha0_
    (
        mesh_.solverDict(alpha1.name()).lookupOrDefault<scalar>("cAlpha", 0)
    ),
    icAlpha_
    (
        mesh_.solverDict(alpha1.name()).lookupOrDefault<scalar>("icAlpha", 0)
    ),
    phic_
    (
        IOobject
        (
            "phic",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mag(phi_/mesh_.magSf())
    ),
    curTimeIndex_(-1)
{
    Info<<"  cAlpha = "<<cAlpha0_<<endl;
    Info<<"  icAlpha = "<<icAlpha_<<endl;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


//- Calculate the interface compression flux
void CML::dynCompressionModel::updateCompression()
{
    // Reference to cAlpha field
    const surfaceScalarField& cAlphaF = cAlphaf();

    // Standard face-flux compression coefficient
    phic_ = cAlphaF*mag(phi_/mesh_.magSf());

    // Add the optional isotropic compression contribution
    if (icAlpha_ > 0)
    {
        phic_ *= (1.0 - icAlpha_);
        phic_ += (cAlphaF*icAlpha_)*fvc::interpolate(mag(U_));
    }

    // Do not compress interface at non-coupled boundary faces
    // (inlets, outlets etc.)
    forAll(phic_.boundaryField(), patchi)
    {
        fvsPatchScalarField& phicp = phic_.boundaryField()[patchi];

        if (!phicp.coupled())
        {
            phicp == 0;
        }
    }
}


//- Return the face compression speed
const CML::surfaceScalarField& CML::dynCompressionModel::phic()
{
    // Manage multiple calls within a single time-step
    label presTimeIndex;

    if (mesh_.time().subCycling())
    {
        presTimeIndex = mesh_.time().prevTimeState().timeIndex();
    }
    else
    {
        presTimeIndex = mesh_.time().timeIndex();
    }

    if (curTimeIndex_ < presTimeIndex)
    {
        // Re-read coefficients
        read();

        // Correct cAlpha
        correct();

        // Update compression speed
        updateCompression();

        curTimeIndex_ = presTimeIndex;
    }

    return phic_;
}


// - Read the compression coefficients
bool CML::dynCompressionModel::read()
{
    cAlpha0_ = mesh_.solverDict(alpha1_.name()).lookupOrDefault<scalar>("cAlpha", 0);
    icAlpha_ = mesh_.solverDict(alpha1_.name()).lookupOrDefault<scalar>("icAlpha", 0);

    return true;
}


// ************************************************************************* //
