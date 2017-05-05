/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::pointPatchDist

Description
    Calculation of distance to nearest patch for all points

SourceFiles
    pointPatchDist.cpp

\*---------------------------------------------------------------------------*/

#ifndef pointPatchDist_HPP
#define pointPatchDist_HPP

#include "pointFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class pointMesh;

/*---------------------------------------------------------------------------*\
                          Class pointPatchDist Declaration
\*---------------------------------------------------------------------------*/

class pointPatchDist
:
    public pointScalarField
{

private:

    // Private Member Data

        //- Reference to pointField
        const pointField& points_;

        //- Set of patch IDs
        const labelHashSet patchIDs_;

        //- Number of unset points
        label nUnset_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        pointPatchDist(const pointPatchDist&);

        //- Disallow default bitwise assignment
        void operator=(const pointPatchDist&);


public:

    // Constructors

        //- Construct from mesh and set of patches
        pointPatchDist
        (
            const pointMesh& pMesh,
            const labelHashSet& patchIDs,
            const pointField& points
        );


    //- Destructor
    virtual ~pointPatchDist();


    // Member Functions

        const pointScalarField& y() const
        {
            return *this;
        }

        label nUnset() const
        {
            return nUnset_;
        }

        //- Correct for mesh geom/topo changes
        void correct();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
