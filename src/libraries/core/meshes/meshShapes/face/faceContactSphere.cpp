/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

Description
    Return location of contact sphere on the face

\*---------------------------------------------------------------------------*/

#include "face.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::scalar CML::face::contactSphereDiameter
(
    const point& p,
    const vector& n,
    const pointField& meshPoints
) const
{
    scalar magN = CML::mag(n);

    vector n1 = n/(magN + SMALL);
    vector n2 = area(meshPoints);

    n2 /= CML::mag(n2) + SMALL;

    return 2*((centre(meshPoints) - p) & n2)/((n1 & n2) - 1.0);
}


// ************************************************************************* //
