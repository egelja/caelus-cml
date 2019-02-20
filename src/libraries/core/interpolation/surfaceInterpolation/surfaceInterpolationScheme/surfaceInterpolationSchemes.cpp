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

#include "surfaceInterpolationScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeBaseSurfaceInterpolationScheme(Type)                               \
                                                                               \
defineNamedTemplateTypeNameAndDebug(surfaceInterpolationScheme<Type>, 0);      \
                                                                               \
defineTemplateRunTimeSelectionTable                                            \
(                                                                              \
    surfaceInterpolationScheme<Type>,                                          \
    Mesh                                                                       \
);                                                                             \
                                                                               \
defineTemplateRunTimeSelectionTable                                            \
(                                                                              \
    surfaceInterpolationScheme<Type>,                                          \
    MeshFlux                                                                   \
);

makeBaseSurfaceInterpolationScheme(scalar)
makeBaseSurfaceInterpolationScheme(vector)
makeBaseSurfaceInterpolationScheme(sphericalTensor)
makeBaseSurfaceInterpolationScheme(symmTensor)
makeBaseSurfaceInterpolationScheme(tensor)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


template<>
CML::tmp
<
    CML::GeometricField
    <
        typename CML::innerProduct<CML::vector, CML::scalar>::type,
        CML::fvsPatchField,
        CML::surfaceMesh
    >
>
CML::surfaceInterpolationScheme<CML::scalar>::dotInterpolate
(
    const surfaceVectorField& Sf,
    const GeometricField<scalar, fvPatchField, volMesh>&
) const
{
    NotImplemented;

    return
        tmp
        <
            GeometricField
            <
                typename innerProduct<vector, scalar>::type,
                fvsPatchField,
                surfaceMesh
            >
        >
        (
            GeometricField
            <
                typename innerProduct<vector, scalar>::type,
                fvsPatchField,
                surfaceMesh
            >::null()
        );
}


// ************************************************************************* //
