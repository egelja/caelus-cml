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
    CML::leastSquaresVectors

Description
    Least-squares gradient scheme vectors

SourceFiles
    leastSquaresVectors.cpp

\*---------------------------------------------------------------------------*/

#ifndef leastSquaresVectors_H
#define leastSquaresVectors_H

#include "MeshObject.hpp"
#include "fvMesh.hpp"
#include "surfaceFieldsFwd.hpp"
#include "labelPair.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class leastSquaresVectors Declaration
\*---------------------------------------------------------------------------*/

class leastSquaresVectors
:
    public MeshObject<fvMesh, leastSquaresVectors>
{
    // Private data

        //- Least-squares gradient vectors
        mutable surfaceVectorField* pVectorsPtr_;
        mutable surfaceVectorField* nVectorsPtr_;


    // Private Member Functions

        //- Construct Least-squares gradient vectors
        void makeLeastSquaresVectors() const;


public:

    // Declare name of the class and its debug switch
    TypeName("leastSquaresVectors");


    // Constructors

        //- Construct given an fvMesh
        explicit leastSquaresVectors(const fvMesh&);


    //- Destructor
    virtual ~leastSquaresVectors();


    // Member functions

        //- Return reference to owner least square vectors
        const surfaceVectorField& pVectors() const;

        //- Return reference to neighbour least square vectors
        const surfaceVectorField& nVectors() const;


        //- Delete the least square vectors when the mesh moves
        virtual bool movePoints();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
