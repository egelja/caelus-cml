/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
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
    CML::refinementHistoryConstraint

Description
    Constraint to keep all cells originating from refining the same cell
    onto the same processor. Reads polyMesh/refinementHistory.

SourceFiles
    refinementHistoryConstraint.cpp

\*---------------------------------------------------------------------------*/

#ifndef refinementHistoryConstraint_HPP
#define refinementHistoryConstraint_HPP

#include "decompositionConstraint.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{


/*---------------------------------------------------------------------------*\
                Class refinementHistoryConstraint Declaration
\*---------------------------------------------------------------------------*/

class refinementHistoryConstraint
:
    public decompositionConstraint
{
    // Private data

public:

    //- Runtime type information
    TypeName("refinementHistory");


    // Constructors

        //- Construct with generic dictionary with optional entry for type
        refinementHistoryConstraint
        (
            const dictionary& constraintsDict,
            const word& type
        );

        //- Construct from components
        refinementHistoryConstraint();


    //- Destructor
    virtual ~refinementHistoryConstraint()
    {}


    // Member Functions

        //- Add my constraints to list of constraints
        virtual void add
        (
            const polyMesh& mesh,
            boolList& blockedFace,
            PtrList<labelList>& specifiedProcessorFaces,
            labelList& specifiedProcessor,
            List<labelPair>& explicitConnections
        ) const;

        //- Apply any additional post-decomposition constraints
        virtual void apply
        (
            const polyMesh& mesh,
            const boolList& blockedFace,
            const PtrList<labelList>& specifiedProcessorFaces,
            const labelList& specifiedProcessor,
            const List<labelPair>& explicitConnections,
            labelList& decomposition
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
