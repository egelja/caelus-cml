/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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

Class
    CML::MeshesRepository

Description

SourceFiles
    MeshesRepositoryI.hpp
    MeshesRepository.cpp
    MeshesRepositoryIO.cpp

Contributors/Copyright:
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef MeshesRepository_H
#define MeshesRepository_H

#include "PtrMap.hpp"
#include "fvMesh.hpp"
#include "meshToMesh.hpp"

#include "MeshInterpolationOrder.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class MeshesRepository Declaration
\*---------------------------------------------------------------------------*/

class MeshesRepository
{
    // Private data

    HashPtrTable<Time,word> times_;

    HashPtrTable<fvMesh,word> meshes_;

    HashPtrTable<meshToMesh,word> meshInterpolations_;

    HashTable<meshToMeshOrder,word> interpolationOrder_;

    // Constructors

    //- Construct null
    MeshesRepository();

    //- The only instance of this repository
    static MeshesRepository *repositoryInstance;

public:
     //- Runtime type information
    TypeName("MeshesRepository");

    //- Destructor

    virtual ~MeshesRepository();

    //- Get the Singleton of this class
    static MeshesRepository &getRepository();

    //- add a mesh and construct a new time for it
    fvMesh &addMesh(
        const word &name,
        const fileName &caseName,
        const word &region=polyMesh::defaultRegion
    );

    //- add a mesh that is in the same case as another mesh
    fvMesh &addCoupledMesh(
        const word &name,
        const word &masterName,
        const word &region=polyMesh::defaultRegion
    );

    //- get a set using its name (mesh must exist)
    fvMesh &getMesh(
        const word &name
    );

    //- does the mesh exist
    bool hasMesh(
        const word &name
    );

    //- get a mesh to mesh interpolation from mesh name to the other mesg
    meshToMesh &getMeshToMesh(
        const word &name,
        const fvMesh &mesh
    );

    //- set the time that is used for a mesh
    scalar setTime(
        const word &name,
        const string &time,
        label timeIndex=0
    );

    //- set the time that is used for a mesh
    scalar setTime(
        const word &name,
        scalar time,
        label timeIndex=0
    );

    //- set interpolation order for a mesh
    void setInterpolationOrder(
        const word &name,
        meshToMeshOrder val
    );

    //- get interpolation order
    meshToMeshOrder getInterpolationOrder(
        const word &name
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
