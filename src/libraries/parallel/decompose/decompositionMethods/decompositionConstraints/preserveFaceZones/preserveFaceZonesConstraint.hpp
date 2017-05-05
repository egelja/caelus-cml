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
    CML::preserveFaceZonesConstraint

Description
    Constraint to keep/move owner and neighbour of faceZone onto same
    processor.

SourceFiles
    preserveFaceZonesConstraint.cpp

\*---------------------------------------------------------------------------*/

#ifndef preserveFaceZonesConstraint_HPP
#define preserveFaceZonesConstraint_HPP

#include "decompositionConstraint.hpp"
#include "wordReList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace decompositionConstraints
{

/*---------------------------------------------------------------------------*\
                Class preserveFaceZonesConstraint Declaration
\*---------------------------------------------------------------------------*/

class preserveFaceZonesConstraint
:
    public decompositionConstraint
{
    // Private data

        //- List of zones to keep together
        wordReList zones_;


public:

    //- Runtime type information
    TypeName("preserveFaceZones");


    // Constructors

        //- Construct with generic dictionary with optional entry for type
        preserveFaceZonesConstraint
        (
            const dictionary& constraintsDict,
            const word& type
        );

        //- Construct from components
        preserveFaceZonesConstraint(const wordReList& zones);


    //- Destructor
    virtual ~preserveFaceZonesConstraint()
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

} // End namespace decompositionConstraints
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
