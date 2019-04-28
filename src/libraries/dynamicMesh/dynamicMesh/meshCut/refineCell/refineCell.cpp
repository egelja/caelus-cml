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

#include "refineCell.hpp"
#include "error.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::refineCell::refineCell()
:
    cellNo_(-1),
    direction_(vector::zero)
{}


CML::refineCell::refineCell(const label cellI, const vector& direction)
:
    cellNo_(cellI),
    direction_(direction)
{
    scalar magDir = mag(direction_);

    if (magDir < SMALL)
    {
        FatalErrorInFunction
            << "(almost)zero vector as direction for cell " << cellNo_
            << abort(FatalError);
    }
    else if (mag(magDir - 1) > SMALL)
    {
        // Normalize
        direction_ /= mag(direction_);
    }
}


CML::refineCell::refineCell(Istream& is)
:
    cellNo_(readLabel(is)),
    direction_(is)
{
    scalar magDir = mag(direction_);

    if (magDir < SMALL)
    {
        FatalErrorInFunction
            << "(almost)zero vector as direction for cell " << cellNo_
            << abort(FatalError);
    }
    else if (mag(magDir - 1) > SMALL)
    {
        // Normalize
        direction_ /= mag(direction_);
    }
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const refineCell& r)
{
    if (os.format() == IOstream::ASCII)
    {
        os << r.cellNo() << token::SPACE << r.direction();
    }
    else
    {
        os << r.cellNo() << r.direction();
    }
    return os;
}


// ************************************************************************* //

