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
    CML::slicedVolFields

\*---------------------------------------------------------------------------*/

#ifndef slicedVolFieldsFwd_H
#define slicedVolFieldsFwd_H

#include "fieldTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class volMesh;

template<class Type>
class fvPatchField;

template<class Type>
class slicedFvPatchField;

template
<
    class Type,
    template<class> class PatchField,
    template<class> class SlicedPatchField,
    class GeoMesh
>
class SlicedGeometricField;

typedef
SlicedGeometricField<scalar, fvPatchField, slicedFvPatchField, volMesh>
    slicedVolScalarField;

typedef
SlicedGeometricField<vector, fvPatchField, slicedFvPatchField, volMesh>
    slicedVolVectorField;

typedef
SlicedGeometricField<sphericalTensor, fvPatchField, slicedFvPatchField, volMesh>
    slicedVolSphericalTensorField;

typedef
SlicedGeometricField<symmTensor, fvPatchField, slicedFvPatchField, volMesh>
    slicedVolSymmTensorField;

typedef
SlicedGeometricField<tensor, fvPatchField, slicedFvPatchField, volMesh>
    slicedVolTensorField;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
