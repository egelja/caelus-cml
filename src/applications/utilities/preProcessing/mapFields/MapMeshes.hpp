/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2014 OpenFOAM Foundation
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

#ifndef MapMeshes_H
#define MapMeshes_H

#include "MapVolFields.hpp"
#include "mapLagrangian.hpp"
#include "UnMapped.hpp"
#include "pointMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<template<class> class CombineOp>
void MapMesh
(
    const meshToMesh& interp,
    const HashSet<word>& selectedFields,
    const bool noLagrangian
)
{
    {
        const polyMesh& meshSource = interp.srcRegion();

        // Search for list of objects for this time
        IOobjectList objects(meshSource, meshSource.time().timeName());

        // Map volFields
        // ~~~~~~~~~~~~~
        MapVolFields<scalar>
        (
            objects,
            selectedFields,
            interp,
            CombineOp<scalar>()
        );

        MapVolFields<vector>
        (
            objects,
            selectedFields,
            interp,
            CombineOp<vector>()
        );
        MapVolFields<sphericalTensor>
        (
            objects,
            selectedFields,
            interp,
            CombineOp<sphericalTensor>()
        );
        MapVolFields<symmTensor>
        (
            objects,
            selectedFields,
            interp,
            CombineOp<symmTensor>()
        );
        MapVolFields<tensor>
        (
            objects,
            selectedFields,
            interp,
            CombineOp<tensor>()
        );
    }

    {
        const polyMesh& meshTarget = interp.tgtRegion();

        // Search for list of target objects for this time
        IOobjectList objects(meshTarget, meshTarget.time().timeName());

        // Mark surfaceFields as unmapped
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        UnMapped<surfaceScalarField>(objects);
        UnMapped<surfaceVectorField>(objects);
        UnMapped<surfaceSphericalTensorField>(objects);
        UnMapped<surfaceSymmTensorField>(objects);
        UnMapped<surfaceTensorField>(objects);

        // Mark pointFields as unmapped
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        UnMapped<pointScalarField>(objects);
        UnMapped<pointVectorField>(objects);
        UnMapped<pointSphericalTensorField>(objects);
        UnMapped<pointSymmTensorField>(objects);
        UnMapped<pointTensorField>(objects);
    }

    if (!noLagrangian)
    {
        mapLagrangian(interp);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
