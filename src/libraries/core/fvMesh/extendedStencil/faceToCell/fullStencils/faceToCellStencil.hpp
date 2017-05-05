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
    CML::faceToCellStencil

Description
    baseclass for extended cell centred addressing. Contains per cell a
    list of neighbouring faces in global addressing.

SourceFiles
    faceToCellStencil.cpp

\*---------------------------------------------------------------------------*/

#ifndef faceToCellStencil_H
#define faceToCellStencil_H

#include "globalIndex.hpp"
#include "boolList.hpp"
#include "HashSet.hpp"
#include "indirectPrimitivePatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class polyMesh;

/*---------------------------------------------------------------------------*\
                      Class faceToCellStencil Declaration
\*---------------------------------------------------------------------------*/

class faceToCellStencil
:
    public labelListList
{
    // Private data

        const polyMesh& mesh_;

        //- Global numbering for faces
        const globalIndex globalNumbering_;

public:

    // Constructors

        //- Construct from mesh
        explicit faceToCellStencil(const polyMesh&);


    // Member Functions

        const polyMesh& mesh() const
        {
            return mesh_;
        }

        //- Global numbering for faces
        const globalIndex& globalNumbering() const
        {
            return globalNumbering_;
        }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
