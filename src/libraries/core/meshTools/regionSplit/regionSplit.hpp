/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::regionSplit

Description
    This class separates the mesh into distinct unconnected regions,
    each of which is then given a label according to globalNumbering().


    Say 6 cells, 3 processors, with single baffle on proc1.

              baffle
                |
    +---+---+---+---+---+---+
    |   |   |   |   |   |   |
    +---+---+---+---+---+---+
      proc0 | proc1 | proc2



    1: determine local regions (uncoupled)

    +---+---+---+---+---+---+
    | 0 | 0 | 0 | 1 | 0 | 0 |
    +---+---+---+---+---+---+
      proc0 | proc1 | proc2



    2: make global

    +---+---+---+---+---+---+
    | 0 | 0 | 1 | 2 | 3 | 3 |
    +---+---+---+---+---+---+
      proc0 | proc1 | proc2



    3: merge connected across procs

    +---+---+---+---+---+---+
    | 0 | 0 | 0 | 2 | 2 | 2 |
    +---+---+---+---+---+---+
      proc0 | proc1 | proc2



    4. determine locally owner regions. determine compact numbering for the
    local regions and send these to all processors that need them:

    proc0 uses regions:
        - 0 which is local to it.
    proc1 uses regions
        - 0 which originates from proc0
        - 2 which is local to it
    proc2 uses regions
        - 2 which originates from proc1

    So proc1 needs to get the compact number for region 0 from proc0 and proc2
    needs to get the compact number for region 2 from proc1:

    +---+---+---+---+---+---+
    | 0 | 0 | 0 | 1 | 1 | 1 |
    +---+---+---+---+---+---+
      proc0 | proc1 | proc2


    Can optionally keep all regions local to the processor.

    Note: does not walk across cyclicAMI/cyclicACMI - since not 'coupled()'
          at the patch level.


SourceFiles
    regionSplit.C

\*---------------------------------------------------------------------------*/

#ifndef regionSplit_HPP
#define regionSplit_HPP

#include "globalIndex.hpp"
#include "labelPair.hpp"
#include "boolList.hpp"
#include "MeshObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class polyMesh;

/*---------------------------------------------------------------------------*\
                         Class regionSplit Declaration
\*---------------------------------------------------------------------------*/

class regionSplit
:
    public MeshObject<polyMesh, regionSplit>,
    public labelList
{
    // Private data

        mutable autoPtr<globalIndex> globalNumberingPtr_;


    // Private Member Functions

        //- Calculate region split in non-compact (global) numbering.
        void calcNonCompactRegionSplit
        (
            const globalIndex& globalFaces,
            const boolList& blockedFace,
            const List<labelPair>& explicitConnections,
            labelList& cellRegion
        ) const;

        //- Calculate global region split. Return globalIndex.
        autoPtr<globalIndex> calcRegionSplit
        (
            const bool doGlobalRegions,
            const boolList& blockedFace,
            const List<labelPair>& explicitConnections,
            labelList& cellRegion
        ) const;


public:

    //- Runtime type information
    ClassName("regionSplit");


    // Constructors

        //- Construct from mesh
        regionSplit
        (
            const polyMesh&,
            const bool doGlobalRegions = Pstream::parRun()
        );

        //- Construct from mesh and whether face is blocked
        //  NOTE: blockedFace has to be consistent across coupled faces!
        regionSplit
        (
            const polyMesh&,
            const boolList& blockedFace,
            const bool doGlobalRegions = Pstream::parRun()
        );

        //- Construct from mesh and whether face is blocked. Additional explicit
        //  connections between normal boundary faces.
        //  NOTE: blockedFace has to be consistent across coupled faces!
        regionSplit
        (
            const polyMesh&,
            const boolList& blockedFace,
            const List<labelPair>&,
            const bool doGlobalRegions = Pstream::parRun()
        );


    // Member Functions

        //- Return global region numbering
        const globalIndex& globalNumbering() const
        {
            return globalNumberingPtr_();
        }

        //- Return local number of regions
        label nLocalRegions() const
        {
            return globalNumbering().localSize(Pstream::myProcNo());
        }

        //- Return total number of regions
        label nRegions() const
        {
            return globalNumbering().size();
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
