/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "coordSet.hpp"

// * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * * //

namespace CML
{
    template<>
    const char* CML::NamedEnum
    <
        CML::coordSet::coordFormat,
        5
    >::names[] =
    {
        "xyz",
        "x",
        "y",
        "z",
        "distance"
    };
}


const CML::NamedEnum<CML::coordSet::coordFormat, 5>
    CML::coordSet::coordFormatNames_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from components
CML::coordSet::coordSet
(
    const word& name,
    const word& axis
)
:
    pointField(0),
    name_(name),
    axis_(coordFormatNames_[axis]),
    curveDist_(0)
{}


//- Construct from components
CML::coordSet::coordSet
(
    const word& name,
    const word& axis,
    const List<point>& points,
    const scalarList& curveDist
)
:
    pointField(points),
    name_(name),
    axis_(coordFormatNames_[axis]),
    curveDist_(curveDist)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::coordSet::hasVectorAxis() const
{
    return axis_ == XYZ;
}


CML::scalar CML::coordSet::scalarCoord
(
    const label index
)   const
{
    const point& p = operator[](index);

    if (axis_ == X)
    {
        return p.x();
    }
    else if (axis_ == Y)
    {
        return p.y();
    }
    else if (axis_ == Z)
    {
        return p.z();
    }
    else if (axis_ == DISTANCE)
    {
        // Use distance to reference point
        return curveDist_[index];
    }
    else
    {
        FatalErrorInFunction
            << "Illegal axis specification " << axis_
            << " for sampling line " << name_
            << exit(FatalError);

        return 0;
    }
}


CML::point CML::coordSet::vectorCoord(const label index) const
{
    const point& p = operator[](index);

    return p;
}


CML::Ostream& CML::coordSet::write(Ostream& os) const
{
    os  << "name:" << name_ << " axis:" << axis_
        << endl
        << endl << "\t(coord)"
        << endl;

    forAll(*this, sampleI)
    {
        os  << '\t' << operator[](sampleI) << endl;
    }

    return os;
}


// ************************************************************************* //
