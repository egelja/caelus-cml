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

#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTemplate2TypeNameAndDebug
(
    surfaceScalarField::DimensionedInternalField,
    0
);
defineTemplate2TypeNameAndDebug
(
    surfaceVectorField::DimensionedInternalField,
    0
);
defineTemplate2TypeNameAndDebug
(
    surfaceSphericalTensorField::DimensionedInternalField,
    0
);
defineTemplate2TypeNameAndDebug
(
    surfaceSymmTensorField::DimensionedInternalField,
    0
);
defineTemplate2TypeNameAndDebug
(
    surfaceTensorField::DimensionedInternalField,
    0
);


defineTemplateTypeNameAndDebug(surfaceScalarField, 0);
defineTemplateTypeNameAndDebug(surfaceVectorField, 0);
defineTemplateTypeNameAndDebug(surfaceSphericalTensorField, 0);
defineTemplateTypeNameAndDebug(surfaceSymmTensorField, 0);
defineTemplateTypeNameAndDebug(surfaceTensorField, 0);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
