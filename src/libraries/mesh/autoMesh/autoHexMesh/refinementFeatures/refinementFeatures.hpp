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
    CML::refinementFeatures

Description
    Encapsulates queries for features.

SourceFiles
    refinementFeatures.C

\*---------------------------------------------------------------------------*/

#ifndef refinementFeatures_H
#define refinementFeatures_H

#include "featureEdgeMesh.hpp"
#include "indexedOctree.hpp"
#include "treeDataEdge.hpp"
#include "treeDataPoint.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class refinementFeatures Declaration
\*---------------------------------------------------------------------------*/

class refinementFeatures
:
    public PtrList<featureEdgeMesh>
{
private:

    // Private data

        //- Refinement levels
        labelList levels_;

        //- Edge
        PtrList<indexedOctree<treeDataEdge> > edgeTrees_;

        //- Features points
        PtrList<indexedOctree<treeDataPoint> > pointTrees_;


    // Private Member Functions

public:

    // Constructors

        //- Construct from components
        refinementFeatures
        (
            const objectRegistry& io,
            const PtrList<dictionary>& featDicts
        );


    // Member Functions

        // Access

            const labelList& levels() const
            {
                return levels_;
            }

            const PtrList<indexedOctree<treeDataEdge> >& edgeTrees() const
            {
                return edgeTrees_;
            }

            const PtrList<indexedOctree<treeDataPoint> >& pointTrees() const
            {
                return pointTrees_;
            }


        // Query

            //- Find nearest point on nearest feature edge
            void findNearestEdge
            (
                const pointField& samples,
                const scalarField& nearestDistSqr,
                labelList& nearFeature,
                List<pointIndexHit>& nearInfo
            ) const;

            //- Find nearest feature point. Is an index into feature points
            //  which itself is an index into the edgeMesh points.
            //  So the point index is
            //      pointTrees()[nearFeature].shapes().pointLabels()[nearIndex]
            //  Wip.
            void findNearestPoint
            (
                const pointField& samples,
                const scalarField& nearestDistSqr,
                labelList& nearFeature,
                labelList& nearIndex
            ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
