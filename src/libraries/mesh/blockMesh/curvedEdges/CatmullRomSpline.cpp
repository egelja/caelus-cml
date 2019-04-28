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

#include "error.hpp"
#include "CatmullRomSpline.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::CatmullRomSpline::CatmullRomSpline
(
    const pointField& knots,
    const bool closed
)
:
    polyLine(knots, closed)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::point CML::CatmullRomSpline::position(const scalar mu) const
{
    // endpoints
    if (mu < SMALL)
    {
        return points().first();
    }
    else if (mu > 1 - SMALL)
    {
        return points().last();
    }

    scalar lambda = mu;
    label segment = localParameter(lambda);
    return position(segment, lambda);
}


CML::point CML::CatmullRomSpline::position
(
    const label segment,
    const scalar mu
) const
{
    // out-of-bounds
    if (segment < 0)
    {
        return points().first();
    }
    else if (segment > nSegments())
    {
        return points().last();
    }

    const point& p0 = points()[segment];
    const point& p1 = points()[segment+1];

    // special cases - no calculation needed
    if (mu <= 0.0)
    {
        return p0;
    }
    else if (mu >= 1.0)
    {
        return p1;
    }


    // determine the end points
    point e0;
    point e1;

    if (segment == 0)
    {
        // end: simple reflection
        e0 = 2*p0 - p1;
    }
    else
    {
        e0 = points()[segment-1];
    }

    if (segment+1 == nSegments())
    {
        // end: simple reflection
        e1 = 2*p1 - p0;
    }
    else
    {
        e1 = points()[segment+2];
    }


    return 0.5 *
    (
        ( 2*p0 )
      + mu *
        (
            ( -e0 + p1 )
          + mu *
            (
                ( 2*e0 - 5*p0 + 4*p1 - e1 )
              + mu *
                ( -e0 + 3*p0 - 3*p1 + e1 )
            )
        )
    );
}


CML::scalar CML::CatmullRomSpline::length() const
{
    NotImplemented;
    return 1.0;
}


// ************************************************************************* //
