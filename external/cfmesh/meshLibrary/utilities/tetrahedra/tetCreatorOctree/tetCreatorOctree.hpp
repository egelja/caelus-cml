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
    tetCreatorOctree

Description
    A class which creates tets from the octree structure

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    tetCreatorOctree.cpp

\*---------------------------------------------------------------------------*/

#ifndef tetCreatorOctree_HPP
#define tetCreatorOctree_HPP

#include "polyMeshGenModifier.hpp"
#include "partTet.hpp"
#include "meshOctreeAddressing.hpp"
#include "meshOctreeCubeCoordinates.hpp"
#include "VRWGraph.hpp"
#include "labelLongList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class tetCreatorOctree Declaration
\*---------------------------------------------------------------------------*/

class tetCreatorOctree
{
    // Private data
        //- helper for searching coordinates of cubes around an edge
        static const meshOctreeCubeCoordinates edgeCoordinates_[12][4];
    
        //- helper for finding face centres of cubes sharing an edge
        static const label faceCentreHelper_[3][4];
    
        //- reference to the octree
        meshOctreeAddressing octreeCheck_;
    
        //- points of the tetrahedrisation
        LongList<point> tetPoints_;
    
        //- tetrahedra
        LongList<partTet> tets_;
        
        //- octree leaves sorted according to their level
        List<labelLongList> sortedLeaves_;
    
        //- node labels of vertices created inside split-hex boxes
        VRWGraph* subNodeLabelsPtr_;
        
        //- cube centre label
        labelList* cubeLabelPtr_;
        
        //- cube face label
        VRWGraph* faceCentreLabelPtr_;
        
        //- are tets created or not
        bool created_;
        
    // Private member functions
        //- deletes all pointer data
        void clearOut();
        
        //- find elements which will be used as mesh cells
        void selectElements();
        
    // Private member functions for creating tetrahedra
        //- create tetPoints_ and necessary addressing
        void createPointsAndAddressing();
        
        //- create faceCentreLabelPtr_
        void createFaceCentreLabels();
        
        //- create tetrahedra from faces, owner and neighbour
        void createTetsAroundEdges();
        
        //- create tetrahedra from split faces
        void createTetsFromSplitFaces();
        
        //- create tetrahedra from faces which share at least one edge
        //- with a refined cube
        void createTetsFromFacesWithCentreNode();
        
        //- create tetrahedra from faces with split edges
        void createTetsAroundSplitEdges();
        
        //- helper funtion which checks validity of a created tet and appends
        //- it if valid
        inline void checkAndAppendTet(const partTet);
        
        //- function containing the workflow
        void createTets();

    // Private copy constructor
        //- Disallow default bitwise copy construct
        tetCreatorOctree(const tetCreatorOctree&);

        //- Disallow default bitwise assignment
        void operator=(const tetCreatorOctree&);

public:

    // Constructors

        //- Construct from octree and IOdictionary
        tetCreatorOctree
        (
            const meshOctree& octree,
            const IOdictionary& meshDict
        );

    // Destructor

        ~tetCreatorOctree();


    // Member Functions
        const LongList<point>& tetPoints() const
        {
            if( !created_ )
                FatalErrorInFunction
                  << "Tets are not created!" << exit(FatalError);
            
            return tetPoints_;
        }
        
        const LongList<partTet>& tets() const
        {
            if( !created_ )
                FatalErrorInFunction
                  << "Tets are not created!" << exit(FatalError);
            
            return tets_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline void tetCreatorOctree::checkAndAppendTet(const partTet pt)
{
    bool valid(true);
    for(label n=0;n<4;++n)
        if( pt[n] < 0 )
        {
            valid = false;
            break;
        }
        
    if( valid )
    {
        tets_.append(pt);
        
        # ifdef DEBUGTets
        Info << "Added tet " << (tets_.size()-1) << " is "
            << tets_[tets_.size()-1] << endl;
        # endif
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
