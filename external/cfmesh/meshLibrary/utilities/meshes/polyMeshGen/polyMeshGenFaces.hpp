/*---------------------------------------------------------------------------*\
opyright (C) Creative Fields, Ltd.
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
    polyMeshGenFaces

Description
    Mesh with geometry and faces. It can be used for volume and surface meshes

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    polyMeshGenFaces.C

\*---------------------------------------------------------------------------*/

#ifndef polyMeshGenFaces_HPP
#define polyMeshGenFaces_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "polyMeshGenPoints.hpp"
#include "faceListPMG.hpp"
#include "labelIOList.hpp"
#include "processorBoundaryPatch.hpp"
#include "boundaryPatch.hpp"

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class polyMeshGenFaces
: public polyMeshGenPoints
{
protected:

    // Protected data
        //- list of faces
        faceListPMG faces_;

        //- inter-processor boundaries. These faces are located after the other
        //- boundary faces in the list of faces. The processor
        //- boundaries are internal faces in the end.
        PtrList<processorBoundaryPatch> procBoundaries_;

        //- boundary data
        PtrList<boundaryPatch> boundaries_;

        //- face subsets
        std::map<label, meshSubset> faceSubsets_;

    // Addressing data
        //- number of internal faces, owner and neighbour
        mutable label nIntFaces_;
        mutable labelIOList* ownerPtr_;
        mutable labelIOList* neighbourPtr_;

    // Private member functions

        //- calculate owner and neighbour addressing
        virtual void calculateOwnersAndNeighbours() const = 0;

        //- clear all pointer data
        void clearOut() const;

    // Disallow bitwise assignment
        void operator=(const polyMeshGenFaces&);

        polyMeshGenFaces(const polyMeshGenFaces&);

public:

    friend class polyMeshGenModifier;

    // Constructors
        //- Null constructor
        polyMeshGenFaces(const Time&);

        //- Construct from components without the boundary
        polyMeshGenFaces
        (
            const Time&,
            const pointField& points,
            const faceList& faces
        );

        //- Construct from components with the boundary
        polyMeshGenFaces
        (
            const Time&,
            const pointField& points,
            const faceList& faces,
            const wordList& patchNames,
            const labelList& patchStart,
            const labelList& nFacesInPatch
        );

    // Destructor
        virtual ~polyMeshGenFaces();

    // Member functions

        //- access to faces
        inline const faceListPMG& faces() const;

        //- return number of internal faces
        inline label nInternalFaces() const;

        //- owner and neighbour cells for faces
        inline const labelList& owner() const;
        inline const labelList& neighbour() const;

    // Boundary data

        //- inter-processor boundaries
        inline const PtrList<processorBoundaryPatch>& procBoundaries() const;

        //- return processor patch label for the given face label
        label faceIsInProcPatch(const label faceLabel) const;

        //- ordinary boundaries
        inline const PtrList<boundaryPatch>& boundaries() const;

        //- return patch label for the given face label
        label faceIsInPatch(const label faceLabel) const;

        //- return list of patches in the boundary
        wordList patchNames() const;

        //- return the index of a patch given its name
        label getPatchID(const word& patchName) const;

        //- return the name of a patch given its ID
        word getPatchName(const label patchID) const;

        //- return a list of patch indices corresponding to the given
        // name, expanding regular expressions
        labelList findPatches(const word& patchName) const;

    // Subsets

        label addFaceSubset(const word&);
        void removeFaceSubset(const label);
        word faceSubsetName(const label) const;
        label faceSubsetIndex(const word&) const;
        inline void addFaceToSubset(const label, const label);
        inline void removeFaceFromSubset(const label, const label);
        inline void faceInSubsets(const label, DynList<label>&) const;
        inline void faceSubsetIndices(DynList<label>&) const;
        template<class ListType>
        inline void facesInSubset(const label, ListType&) const;
        template<class ListType>
        inline void updateFaceSubsets(const ListType&);
        inline void updateFaceSubsets(const VRWGraph&);

    // Read mesh
        void read();

    // Write mesh
        void write() const;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

# ifdef USE_OMP
#include <omp.h>
# endif

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
        
inline const faceListPMG& polyMeshGenFaces::faces() const
{
    return faces_;
}
        
inline label polyMeshGenFaces::nInternalFaces() const
{
    if( !(ownerPtr_ && neighbourPtr_) )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateOwnersAndNeighbours();
    }

    return nIntFaces_;
}

inline const labelList& polyMeshGenFaces::owner() const
{
    if( !ownerPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateOwnersAndNeighbours();
    }
    
    return *ownerPtr_;
}

inline const labelList& polyMeshGenFaces::neighbour() const
{
    if( !neighbourPtr_ )
    {
        # ifdef USE_OMP
        if( omp_in_parallel() )
            FatalErrorInFunction
                << "Calculating addressing inside a parallel region."
                << " This is not thread safe" << exit(FatalError);
        # endif

        calculateOwnersAndNeighbours();
    }
    
    return *neighbourPtr_;
}

inline const PtrList<processorBoundaryPatch>&
polyMeshGenFaces::procBoundaries() const
{
    return procBoundaries_;
}

inline const PtrList<boundaryPatch>& polyMeshGenFaces::boundaries() const
{
    return boundaries_;
}

inline void polyMeshGenFaces::addFaceToSubset
(
    const label setID,
    const label faceI
)
{
    std::map<label, meshSubset>::iterator it = faceSubsets_.find(setID);
    if( it == faceSubsets_.end() )
        return;
    
    it->second.addElement(faceI);
}

inline void polyMeshGenFaces::removeFaceFromSubset
(
    const label setI,
    const label faceI)
{
    std::map<label, meshSubset>::iterator it = faceSubsets_.find(setI);
    if( it == faceSubsets_.end() )
        return;
    
    it->second.removeElement(faceI);
}

inline void polyMeshGenFaces::faceInSubsets
(
    const label faceI,
    DynList<label>& faceSubsets
) const
{
    faceSubsets.clear();
    
    std::map<label, meshSubset>::const_iterator it;
    for
    (
        it=faceSubsets_.begin();
        it!=faceSubsets_.end();
        ++it
    )
    {
        if( it->second.contains(faceI) )
            faceSubsets.append(it->first);
    }
}

inline void polyMeshGenFaces::faceSubsetIndices(DynList<label>& indices) const
{
    indices.clear();
    
    std::map<label, meshSubset>::const_iterator it;
    for
    (
        it=faceSubsets_.begin();
        it!=faceSubsets_.end();
        ++it
    )
        indices.append(it->first);
}

template<class ListType>
inline void polyMeshGenFaces::facesInSubset
(
    const label setI,
    ListType& faceLabels
) const
{
    faceLabels.clear();
    
    std::map<label, meshSubset>::const_iterator it =
        faceSubsets_.find(setI);
    if( it == faceSubsets_.end() )
        return;
    
    it->second.containedElements(faceLabels);
}

template<class ListType>
inline void polyMeshGenFaces::updateFaceSubsets(const ListType& newFaceLabels)
{
    for
    (
        std::map<label, meshSubset>::iterator it=faceSubsets_.begin();
        it!=faceSubsets_.end();
        ++it
    )
        it->second.updateSubset(newFaceLabels);
}

inline void polyMeshGenFaces::updateFaceSubsets(const VRWGraph& newFacesForFace)
{
    for
    (
        std::map<label, meshSubset>::iterator it=faceSubsets_.begin();
        it!=faceSubsets_.end();
        ++it
    )
        it->second.updateSubset(newFacesForFace);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
