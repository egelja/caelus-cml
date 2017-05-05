/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::cellFeatures

Description
    Cell analysis class.

    Constructs feature edges and feature points, which are edges/points with
    and angle > given specification.
    Can be asked for 'superFaces' which can be used to see if a cell is a
    'splitHex'.

SourceFiles
    cellFeatures.cpp

\*---------------------------------------------------------------------------*/

#ifndef cellFeatures_H
#define cellFeatures_H

#include "faceList.hpp"
#include "labelList.hpp"
#include "boolList.hpp"
#include "HashSet.hpp"
#include "Map.hpp"
#include "DynamicList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class primitiveMesh;

/*---------------------------------------------------------------------------*\
                           Class cellFeatures Declaration
\*---------------------------------------------------------------------------*/

class cellFeatures
{
    // Private data

        const primitiveMesh& mesh_;

        //- cos of angle between two connected faces or two connected edges on
        //  same face before edge/point is 'feature'.
        scalar minCos_;

        label cellI_;

        //- Feature edges
        labelHashSet featureEdge_;

        //- (demand driven) Faces after removing internal points&edges
        mutable faceList* facesPtr_;

        //- New to old face mapping
        mutable List<DynamicList<label> > faceMap_;


    // Private Member Functions

        bool faceAlignedEdge(const label, const label) const;

        label nextEdge
        (
            const Map<label>& toSuperFace,
            const label superFaceI,
            const label thisEdgeI,
            const label thisVertI
        ) const;

        bool isCellFeatureEdge(const scalar, const label) const;

        void walkSuperFace
        (
            const label faceI,
            const label superFaceI,
            Map<label>& toSuperFace
        ) const;

        void calcSuperFaces() const;


        //- Disallow default bitwise copy construct
        cellFeatures(const cellFeatures&);

        //- Disallow default bitwise assignment
        void operator=(const cellFeatures&);

public:

    // Constructors

        //- Construct from cell in mesh
        cellFeatures
        (
            const primitiveMesh&,
            const scalar minCos,    // angle to use for feature recognition.
            const label cellI
        );


    //- Destructor
    ~cellFeatures();


    // Member Functions

        // Access

            const labelHashSet& featureEdge() const
            {
                return featureEdge_;
            }

            const faceList& faces() const
            {
                if (!facesPtr_)
                {
                    calcSuperFaces();
                }
                return *facesPtr_;
            }

            //- New to old faceMap. Guaranteed to be shrunk.
            const List<DynamicList<label> >& faceMap() const
            {
                if (!facesPtr_)
                {
                    calcSuperFaces();
                }
                return faceMap_;
            }


        // Check

            //- Is edge a feature edge (uniquely determined since on cell
            //  only two faces sharing edge)
            bool isFeatureEdge(const label edgeI) const
            {
                return featureEdge().found(edgeI);
            }

            //- Are two edges connected at feature point?
            //  Is local to face since point might be seen as feature point
            //  from one face but not from another.
            bool isFeaturePoint(const label edge0, const label edge1) const;

            //- Is vertexI on faceI used by two edges that form feature
            //  point
            bool isFeatureVertex(const label faceI, const label vertI) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
