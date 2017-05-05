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

#ifndef slicedSurfaceFields_H
#define slicedSurfaceFields_H

#include "SlicedGeometricField.hpp"
#include "slicedFvsPatchField.hpp"
#include "surfaceMesh.hpp"
#include "slicedSurfaceFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<>
inline void
SlicedGeometricField<scalar, fvsPatchField, slicedFvsPatchField, surfaceMesh>::
correctBoundaryConditions()
{}

template<>
inline void
SlicedGeometricField<vector, fvsPatchField, slicedFvsPatchField, surfaceMesh>::
correctBoundaryConditions()
{}

template<>
inline void
SlicedGeometricField
<
    sphericalTensor,
    fvsPatchField,
    slicedFvsPatchField,
    surfaceMesh
>::
correctBoundaryConditions()
{}

template<>
inline void
SlicedGeometricField
<
    symmTensor,
    fvsPatchField,
    slicedFvsPatchField,
    surfaceMesh
>::
correctBoundaryConditions()
{}

template<>
inline void
SlicedGeometricField<tensor, fvsPatchField, slicedFvsPatchField, surfaceMesh>::
correctBoundaryConditions()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
