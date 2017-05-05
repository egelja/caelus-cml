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
    CML::edgeVertex

Description
    Combines edge or vertex in single label. Used to specify cuts across
    cell circumference.

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef edgeVertex_H
#define edgeVertex_H

#include "label.hpp"
#include "polyMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class refineCell;

/*---------------------------------------------------------------------------*\
                           Class edgeVertex Declaration
\*---------------------------------------------------------------------------*/

class edgeVertex
{
    // Private data

        //- Reference to mesh. (could be primitive mesh but keeping polyMesh
        //  here saves storing reference at higher levels where we do need it)
        const polyMesh& mesh_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        edgeVertex(const edgeVertex&);

        //- Disallow default bitwise assignment
        void operator=(const edgeVertex&);


public:

    // Static Functions

        //- Update refine list from map. Used to update cell/face labels
        //  after morphing
        static void updateLabels(const labelList& map, List<refineCell>&);

        //- Update map from map. Used to update cell/face labels
        //  after morphing
        static void updateLabels(const labelList& map, Map<label>&);

        //- Update map from map. Used to update cell/face labels
        //  after morphing
        static void updateLabels(const labelList& map, labelHashSet&);



    // Constructors

        //- Construct from mesh
        edgeVertex(const polyMesh& mesh)
        :
            mesh_(mesh)
        {}


    // Member Functions

        const polyMesh& mesh() const
        {
            return mesh_;
        }


    // EdgeVertex handling

        //- is eVert an edge?
        static bool isEdge(const primitiveMesh& mesh, const label eVert)
        {
            if (eVert < 0 || eVert >= (mesh.nPoints() + mesh.nEdges()))
            {
                FatalErrorIn
                (
                    "edgeVertex::isEdge(const primitiveMesh&, const label)"
                )   << "EdgeVertex " << eVert << " out of range "
                    << mesh.nPoints() << " to "
                    << (mesh.nPoints() + mesh.nEdges() - 1)
                    << abort(FatalError);
            }

            return eVert >= mesh.nPoints();
        }
        bool isEdge(const label eVert) const
        {
            return isEdge(mesh_, eVert);
        }

        //- convert eVert to edge label
        static label getEdge(const primitiveMesh& mesh, const label eVert)
        {
            if (!isEdge(mesh, eVert))
            {
                FatalErrorIn
                (
                    "edgeVertex::getEdge(const primitiveMesh&, const label)"
                )   << "EdgeVertex " << eVert << " not an edge"
                    << abort(FatalError);
            }
            return eVert - mesh.nPoints();
        }
        label getEdge(const label eVert) const
        {
            return getEdge(mesh_, eVert);
        }

        //- convert eVert to vertex label
        static label getVertex(const primitiveMesh& mesh, const label eVert)
        {
            if (isEdge(mesh, eVert) || (eVert < 0))
            {
                FatalErrorIn
                (
                    "edgeVertex::getVertex(const primitiveMesh&, const label)"
                )   << "EdgeVertex " << eVert << " not a vertex"
                    << abort(FatalError);
            }
            return eVert;
        }
        label getVertex(const label eVert) const
        {
            return getVertex(mesh_, eVert);
        }

        //- Convert pointI to eVert
        static label vertToEVert(const primitiveMesh& mesh, const label vertI)
        {
            if ((vertI < 0) || (vertI >= mesh.nPoints()))
            {
                FatalErrorIn
                (
                    "edgeVertex::vertToEVert(const primitiveMesh&, const label)"
                )   << "Illegal vertex number " << vertI
                    << abort(FatalError);
            }
            return vertI;
        }
        label vertToEVert(const label vertI) const
        {
            return vertToEVert(mesh_, vertI);
        }

        //- Convert edgeI to eVert
        static label edgeToEVert(const primitiveMesh& mesh, const label edgeI)
        {
            if ((edgeI < 0) || (edgeI >= mesh.nEdges()))
            {
                FatalErrorIn
                (
                    "edgeVertex::edgeToEVert(const primitiveMesh&const label)"
                )   << "Illegal edge number " << edgeI
                    << abort(FatalError);
            }
            return mesh.nPoints() + edgeI;
        }
        label edgeToEVert(const label edgeI) const
        {
            return edgeToEVert(mesh_, edgeI);
        }

        //- Return coordinate of cut (uses weight if edgeCut)
        static point coord
        (
            const primitiveMesh&,
            const label cut,
            const scalar weight
        );
        point coord(const label cut, const scalar weight) const
        {
            return coord(mesh_, cut, weight);
        }

        //- Find mesh edge (or -1) between two cuts.
        static label cutPairToEdge
        (
            const primitiveMesh&,
            const label cut0,
            const label cut1
        );
        label cutPairToEdge(const label cut0, const label cut1) const
        {
            return cutPairToEdge(mesh_, cut0, cut1);
        }

        //- Write cut description to Ostream
        Ostream& writeCut(Ostream& os, const label cut, const scalar) const;

        //- Write cut descriptions to Ostream
        Ostream& writeCuts(Ostream& os, const labelList&, const scalarField&)
         const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
