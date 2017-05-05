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

InClass
    CML::slicedSurfaceFields

\*---------------------------------------------------------------------------*/

#ifndef slicedSurfaceFieldsFwd_H
#define slicedSurfaceFieldsFwd_H

#include "fieldTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class surfaceMesh;

template<class Type>
class fvsPatchField;

template<class Type>
class slicedFvsPatchField;

template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
class SlicedGeometricField;

typedef
SlicedGeometricField<scalar, fvsPatchField, slicedFvsPatchField, surfaceMesh>
    slicedSurfaceScalarField;

typedef
SlicedGeometricField<vector, fvsPatchField, slicedFvsPatchField, surfaceMesh>
    slicedSurfaceVectorField;

typedef
SlicedGeometricField
<
    sphericalTensor,
    fvsPatchField,
    slicedFvsPatchField,
    surfaceMesh
>
    slicedSurfaceSphericalTensorField;

typedef
SlicedGeometricField
<
    symmTensor,
    fvsPatchField,
    slicedFvsPatchField,
    surfaceMesh
>
    slicedSurfaceSymmTensorField;

typedef
SlicedGeometricField<tensor, fvsPatchField, slicedFvsPatchField, surfaceMesh>
    slicedSurfaceTensorField;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
