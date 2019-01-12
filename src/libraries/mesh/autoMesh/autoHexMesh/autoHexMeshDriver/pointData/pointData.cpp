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

\*---------------------------------------------------------------------------*/

#include "pointData.hpp"

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const pointData& wDist)
{
    if (os.format() == IOstream::ASCII)
    {
        return os
            << static_cast<const pointEdgePoint&>(wDist)
            << token::SPACE << wDist.s() << token::SPACE << wDist.v();
    }
    else
    {
        return os
            << static_cast<const pointEdgePoint&>(wDist)
            << wDist.s() << wDist.v();
    }
}


CML::Istream& CML::operator>>(Istream& is, pointData& wDist)
{
    return is >> static_cast<pointEdgePoint&>(wDist) >> wDist.s_ >> wDist.v_;
}


// ************************************************************************* //
