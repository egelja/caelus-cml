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
    CML::cellPointWeightWallModified

Description
    CML::cellPointWeightWallModified

SourceFiles
    cellPointWeightWallModified.cpp

\*---------------------------------------------------------------------------*/

#ifndef cellPointWeightWallModified_H
#define cellPointWeightWallModified_H

#include "cellPointWeight.hpp"
#include "wallPolyPatch.hpp"
#include "polyMesh.hpp"
#include "polyBoundaryMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class polyMesh;

/*---------------------------------------------------------------------------*\
                 Class cellPointWeightWallModified Declaration
\*---------------------------------------------------------------------------*/

class cellPointWeightWallModified
:
    public cellPointWeight
{
public:

    // Constructors

        //- Construct from components
        cellPointWeightWallModified
        (
            const polyMesh& mesh,
            const vector& position,
            const label cellI,
            const label faceI = -1
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
