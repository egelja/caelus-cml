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
#include "polyLine.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::polyLine::calcParam()
{
    param_.setSize(points_.size());

    if (param_.size())
    {
        param_[0] = 0.0;

        for (label i=1; i < param_.size(); i++)
        {
            param_[i] = param_[i-1] + mag(points_[i] - points_[i-1]);
        }

        // normalize on the interval 0-1
        lineLength_ = param_.last();
        for (label i=1; i < param_.size() - 1; i++)
        {
            param_[i] /= lineLength_;
        }
        param_.last() = 1.0;
    }
    else
    {
        lineLength_ = 0.0;
    }
}



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::polyLine::polyLine(const pointField& ps, const bool)
:
    points_(ps),
    lineLength_(0.0),
    param_(0)
{
    calcParam();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::pointField& CML::polyLine::points() const
{
    return points_;
}


CML::label CML::polyLine::nSegments() const
{
    return points_.size()-1;
}


CML::label CML::polyLine::localParameter(scalar& lambda) const
{
    // check endpoints
    if (lambda < SMALL)
    {
        lambda = 0;
        return 0;
    }
    else if (lambda > 1 - SMALL)
    {
        lambda = 1;
        return nSegments();
    }

    // search table of cumulative distances to find which line-segment
    // we are on. Check the upper bound.

    label segmentI = 1;
    while (param_[segmentI] < lambda)
    {
        segmentI++;
    }
    segmentI--;   // we want the corresponding lower bound

    // the local parameter [0-1] on this line segment
    lambda =
    (
        ( lambda - param_[segmentI] )
      / ( param_[segmentI+1] - param_[segmentI] )
    );

    return segmentI;
}


CML::point CML::polyLine::position(const scalar mu) const
{
    // check endpoints
    if (mu < SMALL)
    {
        return points_.first();
    }
    else if (mu > 1 - SMALL)
    {
        return points_.last();
    }


    scalar lambda = mu;
    label segment = localParameter(lambda);
    return position(segment, lambda);
}


CML::point CML::polyLine::position
(
    const label segment,
    const scalar mu
) const
{
    // out-of-bounds
    if (segment < 0)
    {
        return points_.first();
    }
    else if (segment > nSegments())
    {
        return points_.last();
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
    else
    {
        // linear interpolation
        return points_[segment] + mu * (p1 - p0);
    }
}


CML::scalar CML::polyLine::length() const
{
    return lineLength_;
}


// ************************************************************************* //
