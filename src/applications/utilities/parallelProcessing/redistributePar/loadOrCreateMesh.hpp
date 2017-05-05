/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

InNamespace
    CML

Description
    Load or create (0 size) a mesh. Used in distributing meshes to a
    larger number of processors

SourceFiles
    loadOrCreateMesh.cpp

\*---------------------------------------------------------------------------*/

#ifndef loadOrCreateMesh_HPP
#define loadOrCreateMesh_HPP

#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

//- Load (if it exists) or create zero cell mesh given an IOobject:
//      name     : regionName
//      instance : exact directory where to find mesh (i.e. does not
//                 do a findInstance
autoPtr<fvMesh> loadOrCreateMesh(const IOobject& io);

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
