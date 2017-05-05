/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

\*---------------------------------------------------------------------------*/

#include "thermoBaffle2D.hpp"
#include "zeroGradientFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace thermoBaffleModels
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


inline tmp<scalarField> thermoBaffle2D::hs
(
    const scalarField& T,
    const label patchI
) const
{
    const scalarField Cp(thermo_->Cp(patchI));
    return Cp*(T - 298.15);
}


inline tmp<volScalarField> thermoBaffle2D::hs
(
    const volScalarField& T
) const
{
    const volScalarField Cp = thermo_->Cp()();
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "hs(" + T.name() + ")",
                time().timeName(),
                regionMesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            Cp*(T - (dimensionedScalar("Tstd", dimTemperature, 298.15))),
            zeroGradientFvPatchScalarField::typeName
        )
    );
}


inline tmp<volScalarField> thermoBaffle2D::T
(
    const volScalarField& hs
) const
{
    const volScalarField Cp = thermo_->Cp()();
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                "T(" + hs.name() + ")",
                time().timeName(),
                regionMesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            hs/Cp + dimensionedScalar("Tstd", dimTemperature, 298.15),
            zeroGradientFvPatchScalarField::typeName
        )
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace thermoBaffleModels
} // End namespace regionModels
} // End namespace CML

// ************************************************************************* //
