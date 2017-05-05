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
    CML::pyrolysisModelCollection

Description
    A centralized pyrolysis collection.

    Container class for a set of pyrolysis with functions implemented
    to loop over the functions for each type.

SourceFiles
    pyrolysisModelCollection.C

\*---------------------------------------------------------------------------*/

#ifndef pyrolysisModelCollection_H
#define pyrolysisModelCollection_H

#include "IOPtrList.hpp"
#include "pyrolysisModel.hpp"
#include "fvc.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators
class fvMesh;

namespace regionModels
{
namespace pyrolysisModels
{

/*---------------------------------------------------------------------------*\
                           Class pyrolysisModelCollection Declaration
\*---------------------------------------------------------------------------*/

class pyrolysisModelCollection
:
    public IOPtrList<pyrolysisModel>
{
    // Private data

        //- Reference to the finite volume mesh this zone is part of
        const fvMesh& mesh_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        pyrolysisModelCollection(const pyrolysisModelCollection&);

        //- Disallow default bitwise assignment
        void operator=(const pyrolysisModelCollection&);


public:


    // Constructors

        //- Construct from mesh
        pyrolysisModelCollection(const fvMesh&);


    // Member Functions

        //- Pre-evolve regions
        void preEvolveRegion();

         //- Evolve the pyrolysis equation regions
        void evolveRegion();

        //- Evolve regions
        void evolve();

        //- Provide some feedback from pyrolysis regions
        void info() const;

        //- Return max diffusivity allowed in the solid
        scalar maxDiff() const;

        //- Mean diffusion number of the solid regions
        scalar solidRegionDiffNo() const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace pyrolysisModels
} // End namespace regionModels
} // End namespace CML


#endif

// ************************************************************************* //
