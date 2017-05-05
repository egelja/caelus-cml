/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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

Class
    CML::attachPolyTopoChanger

Description
    This class is derived from polyMesh and serves as a tool for
    statically connecting pieces of a mesh by executing the mesh
    modifiers and cleaning the mesh.

    The idea is that a mesh can be built from pieces and put together
    using various mesh modifiers (mainly sliding interfaces) which are
    not needed during the run.  Therefore, once the mesh is assembled
    and mesh modification triggered, the newly created point, face and
    cell zones can be cleared together with the mesh modifiers thus
    creating a singly connected static mesh.

    Note:
    All point, face and cell zoning will be lost!  Do it after
    attaching the parts of the mesh, as the point, face and cell
    numbering changes.

\*---------------------------------------------------------------------------*/

#ifndef attachPolyTopoChanger_H
#define attachPolyTopoChanger_H

#include "polyTopoChanger.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class attachPolyTopoChanger Declaration
\*---------------------------------------------------------------------------*/

class attachPolyTopoChanger
:
    public polyTopoChanger
{
    // Private data

    // Private Member Functions

        //- Disallow default bitwise copy construct
        attachPolyTopoChanger(const attachPolyTopoChanger&);

        //- Disallow default bitwise assignment
        void operator=(const attachPolyTopoChanger&);


public:

    // Constructors

        //- Read constructor from IOobject and a polyMesh
        attachPolyTopoChanger(const IOobject& io, polyMesh&);

        //- Read constructor for given polyMesh
        explicit attachPolyTopoChanger(polyMesh&);


    //- Destructor
    virtual ~attachPolyTopoChanger()
    {}


    // Member Functions

        //- Attach mesh. By default filter out empty patches.
        void attach(const bool removeEmptyPatches = true);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
