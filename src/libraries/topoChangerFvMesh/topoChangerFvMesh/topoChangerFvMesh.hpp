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
    CML::topoChangerFvMesh

Description
    Abstract base class for a topology changing fvMesh

SourceFiles
    topoChangerFvMesh.cpp
    newTopoFvMesh.cpp

\*---------------------------------------------------------------------------*/

#ifndef topoChangerFvMesh_H
#define topoChangerFvMesh_H

#include "dynamicFvMesh.hpp"
#include "polyTopoChanger.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class topoChangerFvMesh Declaration
\*---------------------------------------------------------------------------*/

class topoChangerFvMesh
:
    public dynamicFvMesh
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        topoChangerFvMesh(const topoChangerFvMesh&);

        //- Disallow default bitwise assignment
        void operator=(const topoChangerFvMesh&);


protected:

    // Protected Data

        polyTopoChanger topoChanger_;


public:

    //- Runtime type information
    TypeName("topoChangerFvMesh");


    // Constructors

        //- Construct from objectRegistry, and read/write options
        explicit topoChangerFvMesh(const IOobject& io);


    //- Destructor
    virtual ~topoChangerFvMesh();


    // Member Functions

        //- Update the mesh for both mesh motion and topology change
        virtual bool update() = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
