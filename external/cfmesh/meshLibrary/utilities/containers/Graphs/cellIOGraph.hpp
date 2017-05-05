/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Class
    cellIOGraph

Description
    A graph of cells which support automated output

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    cellIOGraph.cpp

\*---------------------------------------------------------------------------*/

#ifndef cellIOGraph_HPP
#define cellIOGraph_HPP

#include "VRWGraph.hpp"
#include "regIOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class cellIOGraph Declaration
\*---------------------------------------------------------------------------*/

class cellIOGraph
:
    public regIOobject,
    public VRWGraph
{

public:

    //- Runtime type information
        TypeName("cellIOGraph");

    // Constructors

        //- Construct from IOobject
        cellIOGraph(const IOobject&);

        //- Construct from IOobject and size of cellIOGraph
        cellIOGraph(const IOobject&, const label);

        //- Construct from IOobject and a VRWGraph
        cellIOGraph(const IOobject&, const VRWGraph&);


    // Member functions

        bool writeData(Ostream&) const;


    // Member operators

        void operator=(const cellIOGraph&);
        void operator=(const VRWGraph&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
