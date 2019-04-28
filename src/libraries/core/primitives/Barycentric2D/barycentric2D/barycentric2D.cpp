/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "barycentric2D.hpp"
#include "Random.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::barycentric2D CML::barycentric2D01(Random& rndGen)
{
    scalar s = rndGen.scalar01();
    scalar t = rndGen.scalar01();

    // Transform the random point in the unit square to a random point in the
    // unit tri by reflecting across the diagonal

    if (s + t > 1)
    {
        s = 1 - s;
        t = 1 - t;
    }

    return CML::barycentric2D(1 - s - t, s, t);
}


// ************************************************************************* //
