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
    CML::inverseFaceDistanceDiffusivity

Description
    Inverse distance to the given patches motion diffusivity.

SourceFiles
    inverseFaceDistanceDiffusivity.cpp

\*---------------------------------------------------------------------------*/

#ifndef inverseFaceDistanceDiffusivity_H
#define inverseFaceDistanceDiffusivity_H

#include "uniformDiffusivity.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class inverseFaceDistanceDiffusivity Declaration
\*---------------------------------------------------------------------------*/

class inverseFaceDistanceDiffusivity
:
    public uniformDiffusivity
{
    // Private data

        //- Patches selected to base the distance on
        wordList patchNames_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        inverseFaceDistanceDiffusivity(const inverseFaceDistanceDiffusivity&);

        //- Disallow default bitwise assignment
        void operator=(const inverseFaceDistanceDiffusivity&);


public:

    //- Runtime type information
    TypeName("inverseFaceDistance");


    // Constructors

        //- Construct for the given fvMesh and data Istream
        inverseFaceDistanceDiffusivity(const fvMesh& mesh, Istream& mdData);


    //- Destructor
    virtual ~inverseFaceDistanceDiffusivity();


    // Member Functions

        //- Correct the motion diffusivity
        virtual void correct();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
