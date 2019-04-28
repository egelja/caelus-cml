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
    CML::STLpoint

Description
    A vertex point representation for STL files.

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef STLpoint_H
#define STLpoint_H

#include "point.hpp"
#include "Istream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                          Class STLpoint Declaration
\*---------------------------------------------------------------------------*/

class STLpoint
:
    public Vector<float>
{

public:

    // Constructors

        //- Construct null
        inline STLpoint()
        {}

        //- Construct from components
        inline STLpoint(float x, float y, float z)
        :
            Vector<float>(x, y, z)
        {}

        //- Construct from point
        inline STLpoint(const point& pt)
        :
            Vector<float>(float(pt.x()), float(pt.y()), float(pt.z()))
        {}

        //- Construct from istream
        inline STLpoint(Istream& is)
        :
            Vector<float>(is)
        {}


    // Member Operators

        //- Conversion to point
        inline operator point() const
        {
            return point(x(), y(), z());
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
