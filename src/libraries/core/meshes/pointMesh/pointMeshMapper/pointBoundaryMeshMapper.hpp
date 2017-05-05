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
    CML::pointBoundaryMeshMapper

Description
    CML::pointBoundaryMeshMapper

\*---------------------------------------------------------------------------*/

#ifndef pointBoundaryMeshMapper_H
#define pointBoundaryMeshMapper_H

#include "PtrList.hpp"
#include "pointPatchMapper.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class pointBoundaryMeshMapper Declaration
\*---------------------------------------------------------------------------*/

class pointBoundaryMeshMapper
:
    public PtrList<pointPatchMapper>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        pointBoundaryMeshMapper(const pointBoundaryMeshMapper&);

        //- Disallow default bitwise assignment
        void operator=(const pointBoundaryMeshMapper&);


public:

    // Constructors

        //- Construct from components
        pointBoundaryMeshMapper
        (
            const pointMesh& mesh,
            const pointMapper& pointMap,
            const mapPolyMesh& mpm
        )
        :
            PtrList<pointPatchMapper>(mesh.boundary().size())
        {
            const pointBoundaryMesh& patches = mesh.boundary();

            forAll(patches, patchI)
            {
                set
                (
                    patchI,
                    new pointPatchMapper
                    (
                        patches[patchI],
                        pointMap,
                        mpm
                    )
                );
            }
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
