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

\*----------------------------------------------------------------------------*/

#include "surfaceLocation.hpp"
#include "triSurface.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::vector CML::surfaceLocation::normal(const triSurface& s) const
{
    const vectorField& n = s.faceNormals();

    if (elementType_ == triPointRef::NONE)
    {
        return n[index()];
    }
    else if (elementType_ == triPointRef::EDGE)
    {
        const labelList& eFaces = s.edgeFaces()[index()];

        if (eFaces.size() == 1)
        {
            return n[eFaces[0]];
        }
        else
        {
            vector edgeNormal(vector::zero);

            forAll(eFaces, i)
            {
                edgeNormal += n[eFaces[i]];
            }
            return edgeNormal/(mag(edgeNormal) + VSMALL);
        }
    }
    else
    {
        return s.pointNormals()[index()];
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

void CML::surfaceLocation::write(Ostream& os, const triSurface& s) const
{
    if (elementType_ == triPointRef::NONE)
    {
        os  << "trianglecoords:" << s[index()].tri(s.points());
    }
    else if (elementType() == triPointRef::EDGE)
    {
        const edge& e = s.edges()[index()];

        os  << "edgecoords:" << e.line(s.localPoints());
    }
    else
    {
        os  << "pointcoord:" << s.localPoints()[index()];
    }
}


CML::Istream& CML::operator>>(Istream& is, surfaceLocation& sl)
{
    label elType;
    is  >> static_cast<pointIndexHit&>(sl)
        >> elType >> sl.triangle_;
    sl.elementType_ = triPointRef::proxType(elType);
    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const surfaceLocation& sl)
{
    return os
        << static_cast<const pointIndexHit&>(sl)
        << token::SPACE << label(sl.elementType_)
        << token::SPACE << sl.triangle_;
}


CML::Ostream& CML::operator<<
(
    Ostream& os,
    const InfoProxy<surfaceLocation>& ip
)
{
    const surfaceLocation& sl = ip.t_;

    if (sl.elementType() == triPointRef::NONE)
    {
        os  << "coord:" << sl.rawPoint()
            << " inside triangle:" << sl.index()
            << " excludeTriangle:" << sl.triangle();
    }
    else if (sl.elementType() == triPointRef::EDGE)
    {
        os  << "coord:" << sl.rawPoint()
            << " on edge:" << sl.index()
            << " excludeTriangle:" << sl.triangle();
    }
    else
    {
        os  << "coord:" << sl.rawPoint()
            << " on point:" << sl.index()
            << " excludeTriangle:" << sl.triangle();
    }

    if (sl.hit())
    {
        os  << " (hit)";
    }
    else
    {
        os  << " (miss)";
    }

    return os;
}


// ************************************************************************* //
