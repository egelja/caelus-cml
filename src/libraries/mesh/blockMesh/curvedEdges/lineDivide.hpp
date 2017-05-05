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
    CML::lineDivide

Description
    Divides a line into segments

SourceFiles
    lineDivide.cpp

\*---------------------------------------------------------------------------*/

#ifndef lineDivide_H
#define lineDivide_H

#include "pointField.hpp"
#include "scalarList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class curvedEdge;

/*---------------------------------------------------------------------------*\
                         Class lineDivide Declaration
\*---------------------------------------------------------------------------*/

class lineDivide
{
    // Private data

        pointField points_;

        scalarList divisions_;

public:

    // Constructors

        //- Construct from components with discretization and expansion ratio
        lineDivide
        (
            const curvedEdge&,
            const label ndiv,
            const scalar xratio = 1.0
        );


    // Member Functions

        //- Return the points
        const pointField& points() const;

        //- Return the list of lambda values
        const scalarList& lambdaDivisions() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
