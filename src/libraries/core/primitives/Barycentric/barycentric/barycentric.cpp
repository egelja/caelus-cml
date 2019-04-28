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

#include "barycentric.hpp"
#include "Random.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::barycentric CML::barycentric01(Random& rndGen)
{
    scalar s = rndGen.scalar01();
    scalar t = rndGen.scalar01();
    scalar u = rndGen.scalar01();

    // Transform the random point in the unit cube to a random point in the
    // unit tet by means of a series of reflections. See
    // <http://vcg.isti.cnr.it/jgt/tetra.htm> for details.

    if (s + t > 1)
    {
        s = 1 - s;
        t = 1 - t;
    }

    if (s + t + u > 1)
    {
        CML::scalar temp = u;

        if (t + u > 1)
        {
            u = 1 - s - t;
            t = 1 - temp;
        }
        else
        {
            u = s + t + u - 1;
            s = 1 - t - temp;
        }
    }

    return CML::barycentric(1 - s - t - u, s, t, u);
}


// ************************************************************************* //
