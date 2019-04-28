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
    CML::mapDistributeLagrangian

Description
    Class containing mesh-to-mesh mapping information for particles

SourceFiles
    mapDistributeLagrangian.cpp

\*---------------------------------------------------------------------------*/

#ifndef mapDistributeLagrangian_H
#define mapDistributeLagrangian_H

#include "mapDistribute.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                           Class mapDistributeLagrangian Declaration
\*---------------------------------------------------------------------------*/

class mapDistributeLagrangian
{
    // Private data

        //- Map to distribute particles
        const mapDistribute particleMap_;

        //- Per element in subsetted mesh the cell label
        const labelListList constructCellLabels_;


public:

    // Constructors

        //- Construct from components
        mapDistributeLagrangian
        (
            const label nNewParticles,
            const Xfer<labelListList>& subParticleMap,
            const Xfer<labelListList>& constructParticleMap,
            const Xfer<labelListList>& constructCellLabels
        )
        :
            particleMap_(nNewParticles, subParticleMap, constructParticleMap),
            constructCellLabels_(constructCellLabels)
        {}


    // Member Functions

        // Access

            //- Distribution map
            const mapDistribute& particleMap() const
            {
                return particleMap_;
            }

            //- Per received particle the destination cell label
            const labelListList& constructCellLabels() const
            {
                return constructCellLabels_;
            }


        // Edit

            //- distribute list of lagrangian data
            template<class T>
            void distributeLagrangianData(List<T>& lst) const
            {
                particleMap_.distribute(lst);
            }

            //- Correct for topo change.
            void updateMesh(const mapPolyMesh&)
            {
                NotImplemented;
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
