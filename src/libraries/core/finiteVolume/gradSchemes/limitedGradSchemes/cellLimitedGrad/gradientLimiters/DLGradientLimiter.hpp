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
    CML::fv::gradientLimiters::DL

Description
    Differential gradient limiter

    To be used with the CML::fv::cellLimitedGrad limited gradient.

    Reference:
    \verbatim
        Venkatakrishnan, V. (1993).
        On the accuracy of limiters and convergence to steady state solutions.
        In 31st Aerospace Sciences Meeting (p. 880).
    \endverbatim

    Example:
    \verbatim
    gradSchemes
    {
        default Gauss linear;
        limited cellLimited<DL> Gauss linear 1;
    }
    \endverbatim

    Note: this limiter formally allows the limiter function to slightly exceed 1
    which is generally not a good idea and can cause unboundedness.
    This issue is resolved in a consistent and differentiable manner by the
    CML::fv::gradientLimiters::MG limiter which is recommended in
    preference to the CML::fv::gradientLimiters::DL limiter.

See also
    CML::fv::cellLimitedGrad
    CML::fv::gradientLimiters::MG

\*---------------------------------------------------------------------------*/

#ifndef DLGradientLimiter_HPP
#define DLGradientLimiter_HPP

#include "Istream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fv
{
namespace gradientLimiters
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class DL
{

public:

    // Constructors
    DL(Istream&)
    {}


    // Member Functions
    inline scalar limiter(const scalar r) const
    {
        return (sqr(r) + 2*r)/(sqr(r) + r + 2);
    }
};


} // End namespace gradientLimiters
} // End namespace fv
} // End namespace CML


#endif
