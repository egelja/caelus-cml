/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
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

Class
    CML::meshSearchMeshObject

Description
    MeshObject wrapper around meshSearch(mesh).

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef meshSearchMeshObject_H
#define meshSearchMeshObject_H

#include "MeshObject.hpp"
#include "meshSearch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
              Class meshSearchMeshObject Declaration
\*---------------------------------------------------------------------------*/

class meshSearchMeshObject
:
    public MeshObject<polyMesh, meshSearchMeshObject>,
    public meshSearch
{

public:

    // Declare name of the class and its debug switch
    TypeName("meshSearchMeshObject");


    // Constructors

        //- Constructor given polyMesh
        explicit meshSearchMeshObject(const polyMesh& mesh);

    //- Destructor
    virtual ~meshSearchMeshObject()
    {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
