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
    CML::structuredDecomp

Description
    Decomposition given using consecutive application of decomposers.

SourceFiles
    structuredDecomp.cpp

\*---------------------------------------------------------------------------*/

#ifndef structuredDecomp_H
#define structuredDecomp_H

#include "decompositionMethod.hpp"

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class structuredDecomp Declaration
\*---------------------------------------------------------------------------*/

class structuredDecomp
:
    public decompositionMethod
{
    // Private data

        dictionary methodDict_;

        autoPtr<decompositionMethod> method_;

        wordList patches_;


    // Private Member Functions

        //- Disallow default bitwise copy construct and assignment
        void operator=(const structuredDecomp&);
        structuredDecomp(const structuredDecomp&);


public:

    //- Runtime type information
    TypeName("structured");


    // Constructors

        //- Construct given the decomposition dictionary
        structuredDecomp(const dictionary& decompositionDict);


    //- Destructor
    virtual ~structuredDecomp()
    {}


    // Member Functions

        //- Is method parallel aware (i.e. does it synchronize domains across
        //  proc boundaries)
        virtual bool parallelAware() const;

        //- Return for every coordinate the wanted processor number. Use the
        //  mesh connectivity (if needed)
        virtual labelList decompose
        (
            const polyMesh& mesh,
            const pointField& points,
            const scalarField& pointWeights
        );

        //- Return for every coordinate the wanted processor number. Explicitly
        //  provided connectivity - does not use mesh_.
        virtual labelList decompose
        (
            const labelListList& globalCellCells,
            const pointField& cc,
            const scalarField& cWeights
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
