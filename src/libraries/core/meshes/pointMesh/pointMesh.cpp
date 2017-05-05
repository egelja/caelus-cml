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

#include "pointMesh.hpp"
#include "globalMeshData.hpp"
#include "pointMeshMapper.hpp"
#include "pointFields.hpp"
#include "MapGeometricFields.hpp"
#include "MapPointField.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

defineTypeNameAndDebug(pointMesh, 0);

}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::pointMesh::mapFields(const mapPolyMesh& mpm)
{
    if (debug)
    {
        Pout<< "void pointMesh::mapFields(const mapPolyMesh&): "
            << "Mapping all registered pointFields."
            << endl;
    }
    // Create a mapper
    const pointMeshMapper m(*this, mpm);

    MapGeometricFields<scalar, pointPatchField, pointMeshMapper, pointMesh>(m);
    MapGeometricFields<vector, pointPatchField, pointMeshMapper, pointMesh>(m);
    MapGeometricFields
    <
        sphericalTensor,
        pointPatchField,
        pointMeshMapper,
        pointMesh
    >(m);
    MapGeometricFields<symmTensor, pointPatchField, pointMeshMapper, pointMesh>
    (m);
    MapGeometricFields<tensor, pointPatchField, pointMeshMapper, pointMesh>(m);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::pointMesh::pointMesh(const polyMesh& pMesh)
:
    MeshObject<polyMesh, pointMesh>(pMesh),
    GeoMesh<polyMesh>(pMesh),
    boundary_(*this, pMesh.boundaryMesh())
{
    if (debug)
    {
        Pout<< "pointMesh::pointMesh(const polyMesh&): "
            << "Constructing from polyMesh " << pMesh.name()
            << endl;
    }

    // Calculate the geometry for the patches (transformation tensors etc.)
    boundary_.calcGeometry();
}


void CML::pointMesh::movePoints(const pointField& newPoints)
{
    if (debug)
    {
        Pout<< "pointMesh::movePoints(const pointField&): "
            << "Moving points." << endl;
    }

    boundary_.movePoints(newPoints);
}


void CML::pointMesh::updateMesh(const mapPolyMesh& mpm)
{
    if (debug)
    {
        Pout<< "pointMesh::updateMesh(const mapPolyMesh&): "
            << "Updating for topology changes." << endl;
        Pout<< endl;
    }
    boundary_.updateMesh();

    // Map all registered point fields
    mapFields(mpm);
}


// ************************************************************************* //
