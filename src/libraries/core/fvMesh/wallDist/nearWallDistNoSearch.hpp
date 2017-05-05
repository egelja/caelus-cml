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
    CML::nearWallDistNoSearch

Description
    Distance calculation for cells with face on a wall. Does not search
    anything, just takes normal component of distance.

SourceFiles
    nearWallDistNoSearch.cpp

\*---------------------------------------------------------------------------*/

#ifndef nearWallDistNoSearch_H
#define nearWallDistNoSearch_H

#include "volFields.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;

/*---------------------------------------------------------------------------*\
                           Class nearWallDistNoSearch Declaration
\*---------------------------------------------------------------------------*/

class nearWallDistNoSearch
:
    public volScalarField::GeometricBoundaryField
{
    // Private data

        //- Reference to mesh
        const fvMesh& mesh_;


    // Private Member Functions

        //- Do all calculations.
        void doAll();

        //- Disallow default bitwise copy construct
        nearWallDistNoSearch(const nearWallDistNoSearch&);

        //- Disallow default bitwise assignment
        void operator=(const nearWallDistNoSearch&);


public:

    // Constructors

        //- Construct from components
        nearWallDistNoSearch(const fvMesh& mesh);


    //- Destructor
    virtual ~nearWallDistNoSearch();


    // Member Functions

        //- Correct for mesh geom/topo changes
        virtual void correct();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
