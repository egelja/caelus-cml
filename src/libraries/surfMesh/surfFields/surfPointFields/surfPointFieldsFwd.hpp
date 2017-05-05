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

#ifndef surfPointFieldsFwd_H
#define surfPointFieldsFwd_H

#include "fieldTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class surfPointGeoMesh;

template<class Type, class GeoMesh>
class DimensionedField;

typedef DimensionedField<label, surfPointGeoMesh>
    surfPointLabelField;
typedef DimensionedField<scalar, surfPointGeoMesh>
    surfPointScalarField;
typedef DimensionedField<vector, surfPointGeoMesh>
    surfPointVectorField;
typedef DimensionedField<sphericalTensor, surfPointGeoMesh>
    surfPointSphericalTensorField;
typedef DimensionedField<symmTensor, surfPointGeoMesh>
    surfPointSymmTensorField;
typedef DimensionedField<tensor, surfPointGeoMesh>
    surfPointTensorField;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
