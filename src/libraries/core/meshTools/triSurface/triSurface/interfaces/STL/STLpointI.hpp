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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline STLpoint::STLpoint()
{}

inline STLpoint::STLpoint(float x, float y, float z)
:
    Vector<float>(x, y, z)
{}

inline STLpoint::STLpoint(const point& pt)
:
    Vector<float>(float(pt.x()), float(pt.y()), float(pt.z()))
{}

inline STLpoint::STLpoint(Istream& is)
:
    Vector<float>(is)
{}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline STLpoint::operator point() const
{
    return point(x(), y(), z());
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
