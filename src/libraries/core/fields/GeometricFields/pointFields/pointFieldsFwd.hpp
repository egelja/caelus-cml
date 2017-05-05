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
    CML::pointFields

Description

SourceFiles
    pointFields.C

\*---------------------------------------------------------------------------*/

#ifndef pointFieldsFwd_H
#define pointFieldsFwd_H

#include "fieldTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class pointMesh;

template<class Type>
class pointPatchField;

template<class Type, template<class> class PatchField, class GeoMesh>
class GeometricField;

typedef GeometricField<scalar, pointPatchField, pointMesh> pointScalarField;
typedef GeometricField<vector, pointPatchField, pointMesh> pointVectorField;
typedef GeometricField<sphericalTensor, pointPatchField, pointMesh>
    pointSphericalTensorField;
typedef GeometricField<symmTensor, pointPatchField, pointMesh>
    pointSymmTensorField;
typedef GeometricField<tensor, pointPatchField, pointMesh> pointTensorField;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
