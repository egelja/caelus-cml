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
    CML::oppositeFace

Description
    Class containing opposite face for a prismatic cell with addressing
    and a possibility of failure.

SourceFiles
    oppositeFace.cpp

\*---------------------------------------------------------------------------*/

#ifndef oppositeFace_H
#define oppositeFace_H

#include "face.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class oppositeFace Declaration
\*---------------------------------------------------------------------------*/

class oppositeFace
:
    public face
{
    // Private data

        //- Master face index
        const label masterIndex_;

        //- Opposite face index
        const label oppositeIndex_;


public:

    // Constructors

        //- Construct from components
        oppositeFace
        (
            const face& f,
            const label masterIndex,
            const label oppositeIndex
        )
        :
            face(f),
            masterIndex_(masterIndex),
            oppositeIndex_(oppositeIndex)

        {}


    // Member Functions

        //- Master face index
        inline label masterIndex() const
        {
            return masterIndex_;
        }

        //- Slave face index
        inline label oppositeIndex() const
        {
            return oppositeIndex_;
        }

        //- Does the opposite face exist?
        inline bool found() const
        {
            return oppositeIndex_ >= 0;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
