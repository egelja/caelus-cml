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
    CML::pairGAMGAgglomeration

Description
    Agglomerate using the pair algorithm.

SourceFiles
    pairGAMGAgglomeration.cpp
    pairGAMGAgglomerate.cpp
    pairGAMGAgglomerationCombineLevels.cpp

\*---------------------------------------------------------------------------*/

#ifndef pairGAMGAgglomeration_H
#define pairGAMGAgglomeration_H

#include "GAMGAgglomeration.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class pairGAMGAgglomeration Declaration
\*---------------------------------------------------------------------------*/

class pairGAMGAgglomeration
:
    public GAMGAgglomeration
{
    // Private data

        //- Number of levels to merge, 1 = don't merge, 2 = merge pairs etc.
        label mergeLevels_;


protected:

    // Protected Member Functions

        //- Calculate and return agglomeration of given level
        tmp<labelField> agglomerate
        (
            label& nCoarseCells,
            const lduAddressing& fineMatrixAddressing,
            const scalarField& faceWeights
        );

        //- Agglomerate all levels starting from the given face weights
        void agglomerate
        (
            const lduMesh& mesh,
            const scalarField& faceWeights
        );

        void combineLevels(const label curLevel);

        //- Disallow default bitwise copy construct
        pairGAMGAgglomeration(const pairGAMGAgglomeration&);

        //- Disallow default bitwise assignment
        void operator=(const pairGAMGAgglomeration&);


public:

    //- Runtime type information
    TypeName("pair");


    // Constructors

        //- Construct given mesh and controls
        pairGAMGAgglomeration
        (
            const lduMesh& mesh,
            const dictionary& controlDict
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
