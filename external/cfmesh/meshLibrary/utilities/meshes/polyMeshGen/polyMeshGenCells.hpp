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
    polyMeshGenCells

Description
    Mesh with geometry and topology information

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    polyMeshGenCells.cpp

\*---------------------------------------------------------------------------*/

#ifndef polyMeshGenCells_HPP
#define polyMeshGenCells_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "polyMeshGenFaces.hpp"
#include "cellListPMG.hpp"

namespace CML
{

class polyMeshGenAddressing;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class polyMeshGenCells
: public polyMeshGenFaces
{
protected:

    // Protected data
        //- list of cells
        cellListPMG cells_;

        //- cell subsets
        std::map<label, meshSubset> cellSubsets_;

        //- primitive mesh which calculates addressing
        mutable polyMeshGenAddressing* addressingDataPtr_;

    // Private member functions
        //- calculate owner and neighbour
        void calculateOwnersAndNeighbours() const;

        //- calculate mesh addressing
        void calculateAddressingData() const;

        //- clear all pointer data
        void clearOut() const;

    // Disallow bitwise assignment
        void operator=(const polyMeshGenCells&);

        polyMeshGenCells(const polyMeshGenCells&);

public:

    friend class polyMeshGenModifier;

    // Constructors
        //- Null constructor
        polyMeshGenCells(const Time&);

        //- Construct from components without the boundary
        polyMeshGenCells
        (
            const Time&,
            const pointField& points,
            const faceList& faces,
            const cellList& cells
        );

        //- Construct from components with the boundary
        polyMeshGenCells
        (
            const Time&,
            const pointField& points,
            const faceList& faces,
            const cellList& cells,
            const wordList& patchNames,
            const labelList& patchStart,
            const labelList& nFacesInPatch
        );

    // Destructor
        ~polyMeshGenCells();

    // Member functions
        //- access to cells
        inline const cellListPMG& cells() const;

        //- addressing which may be needed
        const polyMeshGenAddressing& addressingData() const;

        //- clear addressing data
        void clearAddressingData() const;

    // Subsets
        label addCellSubset(const word&);
        void removeCellSubset(const label);
        word cellSubsetName(const label) const;
        label cellSubsetIndex(const word&) const;
        inline void addCellToSubset(const label, const label);
        inline void removeCellFromSubset(const label, const label);
        inline void cellInSubsets(const label, DynList<label>&) const;
        inline void cellSubsetIndices(DynList<label>&) const;
        template<class ListType>
        inline void cellsInSubset(const label, ListType&) const;
        template<class ListType>
        inline void updateCellSubsets(const ListType&);
        inline void updateCellSubsets(const VRWGraph&);

    // Read mesh
        void read();

    // Write mesh
        void write() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline const cellListPMG& polyMeshGenCells::cells() const
{
    return cells_;
}

inline void polyMeshGenCells::addCellToSubset
(
    const label selID,
    const label cellI
)
{
    std::map<label, meshSubset>::iterator it = cellSubsets_.find(selID);
    if( it == cellSubsets_.end() )
        return;

    it->second.addElement(cellI);
}

inline void polyMeshGenCells::removeCellFromSubset
(
    const label selID,
    const label cellI
)
{
    std::map<label, meshSubset>::iterator it = cellSubsets_.find(selID);
    if( it == cellSubsets_.end() )
        return;

    it->second.removeElement(cellI);
}

inline void polyMeshGenCells::cellInSubsets
(
    const label cellI,
    DynList<label>& cellSubsets
) const
{
    cellSubsets.clear();

    std::map<label, meshSubset>::const_iterator it;
    for
    (
        it=cellSubsets_.begin();
        it!=cellSubsets_.end();
        ++it
    )
    {
        if( it->second.contains(cellI) )
            cellSubsets.append(it->first);
    }
}

inline void polyMeshGenCells::cellSubsetIndices(DynList<label>& indices) const
{
    indices.clear();

    std::map<label, meshSubset>::const_iterator it;
    for
    (
        it=cellSubsets_.begin();
        it!=cellSubsets_.end();
        ++it
    )
        indices.append(it->first);
}

template<class ListType>
inline void polyMeshGenCells::cellsInSubset
(
    const label selID,
    ListType& cellLabels
) const
{
    cellLabels.clear();

    std::map<label, meshSubset>::const_iterator it =
        cellSubsets_.find(selID);
    if( it == cellSubsets_.end() )
        return;

    it->second.containedElements(cellLabels);
}

template<class ListType>
inline void polyMeshGenCells::updateCellSubsets
(
    const ListType& newCellLabels
)
{
    for
    (
        std::map<label, meshSubset>::iterator it=cellSubsets_.begin();
        it!=cellSubsets_.end();
        ++it
    )
        it->second.updateSubset(newCellLabels);
}

inline void polyMeshGenCells::updateCellSubsets
(
    const VRWGraph& newCellsForCell
)
{
    for
    (
        std::map<label, meshSubset>::iterator it=cellSubsets_.begin();
        it!=cellSubsets_.end();
        ++it
    )
        it->second.updateSubset(newCellsForCell);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
