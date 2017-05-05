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

Class
    CML::staticFvMesh

Description
    CML::staticFvMesh

SourceFiles
    staticFvMesh.cpp

\*---------------------------------------------------------------------------*/

#ifndef staticFvMesh_H
#define staticFvMesh_H

#include "dynamicFvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class staticFvMesh Declaration
\*---------------------------------------------------------------------------*/

class staticFvMesh
:
    public dynamicFvMesh
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        staticFvMesh(const staticFvMesh&);

        //- Disallow default bitwise assignment
        void operator=(const staticFvMesh&);


public:

    //- Runtime type information
    TypeName("staticFvMesh");


    // Constructors

        //- Construct from IOobject
        staticFvMesh(const IOobject& io);


    //- Destructor
    ~staticFvMesh();


    // Member Functions

        //- Dummy update function which does not change the mesh
        virtual bool update();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
