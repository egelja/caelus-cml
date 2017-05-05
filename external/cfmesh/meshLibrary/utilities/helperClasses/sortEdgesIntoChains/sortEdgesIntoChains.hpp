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
    sortEdgesIntoChains

Description
    Creates boundary faces from a chain of open edges

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    sortEdgesIntoChains.cpp

\*---------------------------------------------------------------------------*/

#ifndef sortEdgesIntoChains_HPP
#define sortEdgesIntoChains_HPP

#include "DynList.hpp"
#include "labelList.hpp"
#include "edge.hpp"
#include "Map.hpp"
#include "boolList.hpp"

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class sortEdgesIntoChains Declaration
\*---------------------------------------------------------------------------*/

class sortEdgesIntoChains
{
    // Members
        const DynList<edge>& bEdges_;

        bool openEdges_;

        Map<label> newNodeLabel_;

        List<DynList<label> > edgesAtPoint_;

        DynList<labelList> createdChains_;

    // Private member functions
        void createNodeLabels();

        bool findPointsBelongingToTheChain
        (
            const label currPos,
            boolList& chainEdges
        ) const;

        void shrinkEdges(const boolList& chainEdges);

        void createChainFromEdges(const boolList& chainEdges);

        void sortEdges();

    public:

        sortEdgesIntoChains
        (
            const DynList<edge>& bEdges
        );

        ~sortEdgesIntoChains();

    // Member functions

        //- a list of points which have not yet been resolved
                const DynList<labelList>& sortedChains() const;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
