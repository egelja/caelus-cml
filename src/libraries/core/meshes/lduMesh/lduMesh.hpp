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
    CML::lduMesh

Description
    Abstract base class for meshes which provide LDU addressing for the
    construction of lduMatrix and LDU-solvers.

\*---------------------------------------------------------------------------*/

#ifndef lduMesh_H
#define lduMesh_H

#include "lduAddressing.hpp"
#include "lduInterfacePtrsList.hpp"
#include "typeInfo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class objectRegistry;

/*---------------------------------------------------------------------------*\
                           Class lduMesh Declaration
\*---------------------------------------------------------------------------*/

class lduMesh
{

public:

    //- Runtime type information
    TypeName("lduMesh");


    // Constructors

    //- Destructor
    virtual ~lduMesh()
    {}


    // Member Functions

        // Access

            //- Return the object registry
            virtual const objectRegistry& thisDb() const;

            //- Return ldu addressing
            virtual const lduAddressing& lduAddr() const = 0;

            //- Return a list of pointers for each patch
            //  with only those pointing to interfaces being set
            virtual lduInterfacePtrsList interfaces() const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
