/*---------------------------------------------------------------------------*\
Copyright held by the original author
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

\*---------------------------------------------------------------------------*/

#include "polyMeshGenAddressing.hpp"
#include "VRWGraphSMPModifier.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void polyMeshGenAddressing::calcEdgeCells() const
{
    if( ecPtr_ )
    {
        FatalErrorInFunction
            << "edgeCells already calculated"
            << abort(FatalError);
    }
    else
    {
        const VRWGraph& ce = cellEdges();

        ecPtr_ = new VRWGraph();
        VRWGraph& edgeCellAddr = *ecPtr_;

        VRWGraphSMPModifier(edgeCellAddr).reverseAddressing(ce);
        edgeCellAddr.setSize(edges().size());
    }
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const VRWGraph& polyMeshGenAddressing::edgeCells() const
{
    if( !ecPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calcEdgeCells();
    }

    return *ecPtr_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
