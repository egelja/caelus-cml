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
    CML::lineEdge

Description
    A straight edge between the start point and the end point.

SourceFiles
    lineEdge.cpp

\*---------------------------------------------------------------------------*/

#ifndef lineEdge_H
#define lineEdge_H

#include "curvedEdge.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                          Class lineEdge Declaration
\*---------------------------------------------------------------------------*/


class lineEdge
:
    public curvedEdge
{

public:

    //- Runtime type information
    TypeName("line");

    // Constructors

        //- Construct from components
        lineEdge(const pointField&, const label start, const label end);

        //- Construct from Istream with a pointField
        lineEdge(const pointField&, Istream&);


        //- Destructor
        virtual ~lineEdge();


    // Member Functions

        //- Return the point position corresponding to the curve parameter
        //  0 <= lambda <= 1
        point position(const scalar) const;

        //- Return the length of the curve
        scalar length() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
