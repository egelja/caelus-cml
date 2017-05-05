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

#ifndef triSurfaceFieldsFwd_H
#define triSurfaceFieldsFwd_H

#include "fieldTypes.hpp"
#include "triSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, class GeoMesh>
class DimensionedField;

class triSurfaceGeoMesh;

typedef CML::DimensionedField<label, triSurfaceGeoMesh>
    triSurfaceLabelField;
typedef CML::DimensionedField<scalar, triSurfaceGeoMesh>
    triSurfaceScalarField;
typedef CML::DimensionedField<vector, triSurfaceGeoMesh>
    triSurfaceVectorField;
typedef CML::DimensionedField<sphericalTensor, triSurfaceGeoMesh>
    triSurfaceSphericalTensorField;
typedef CML::DimensionedField<symmTensor, triSurfaceGeoMesh>
    triSurfaceSymmTensorField;
typedef CML::DimensionedField<tensor, triSurfaceGeoMesh>
    triSurfaceTensorField;

class triSurfacePointGeoMesh;

typedef CML::DimensionedField<label, triSurfacePointGeoMesh>
    triSurfacePointLabelField;
typedef CML::DimensionedField<scalar, triSurfacePointGeoMesh>
    triSurfacePointScalarField;
typedef CML::DimensionedField<vector, triSurfacePointGeoMesh>
    triSurfacePointVectorField;
typedef CML::DimensionedField<sphericalTensor, triSurfacePointGeoMesh>
    triSurfacePointSphericalTensorField;
typedef CML::DimensionedField<symmTensor, triSurfacePointGeoMesh>
    triSurfacePointSymmTensorField;
typedef CML::DimensionedField<tensor, triSurfacePointGeoMesh>
    triSurfacePointTensorField;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
