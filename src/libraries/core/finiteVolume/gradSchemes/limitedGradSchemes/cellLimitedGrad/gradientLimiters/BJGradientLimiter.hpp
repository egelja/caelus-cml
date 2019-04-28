/*---------------------------------------------------------------------------*\
Copyright (C) 2018 OpenFOAM Foundation
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

Class
    CML::fv::gradientLimiters::BJ

Description
    Barth-Jespersen gradient limiter

    Reference:
    \verbatim
        Barth, T. J. and Jespersen, D. (1989)
        The design and application of upwind schemes on unstructured meshes.
        Technical Report AIAA-89-0366.
        AIAA 27th Aerospace Sciences Meeting, Reno, Nevada.
    \endverbatim

    This is the default gradient limiter which clips the gradient to remove cell
    to face extrapolation unboundedness.  It corresponds to the original
    cellLimitedGrad implementation before the addition of run-time select-able
    limiter functions.

    Example:
    \verbatim
    gradSchemes
    {
        default Gauss linear;
        limited cellLimited Gauss linear 1;
    }
    \endverbatim

See also
    CML::fv::cellLimitedGrad

\*---------------------------------------------------------------------------*/

#ifndef BJGradientLimiter_HPP
#define BJGradientLimiter_HPP

#include "Istream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fv
{
namespace gradientLimiters
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class BJ
{

public:

    // Constructors
    BJ(Istream&)
    {}


    // Member Functions
    inline scalar limiter(const scalar r) const
    {
        return min(r, 1);
    }
};


} // End namespace gradientLimiters
} // End namespace fv
} // End namespace CML


#endif
