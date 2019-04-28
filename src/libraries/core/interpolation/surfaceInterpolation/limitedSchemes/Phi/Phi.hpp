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

Class
    CML::PhiLimiter

Description
    Class with limiter function which returns the limiter for the
    Phi differencing scheme.

    Used in conjunction with the template class PhiScheme.

SourceFiles
    Phi.cpp

\*---------------------------------------------------------------------------*/

#ifndef Phi_H
#define Phi_H

#include "vector.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class PhiLimiter Declaration
\*---------------------------------------------------------------------------*/

class PhiLimiter
{
    scalar k_;

public:

    PhiLimiter(Istream& is)
    :
        k_(readScalar(is))
    {
        if (k_ < 0 || k_ > 1)
        {
            FatalIOErrorInFunction(is)
                << "coefficient = " << k_
                << " should be >= 0 and <= 1"
                << exit(FatalIOError);
        }
    }

    scalar limiter
    (
        const scalar cdWeight,
        const scalar faceFlux,
        const vector& PhiP,
        const vector& PhiN,
        const vector& Sf,
        const scalar&
    ) const
    {
        scalar phiP = Sf&PhiP;
        scalar phiN = Sf&PhiN;

        scalar phiU;

        if (faceFlux > 0)
        {
            phiU = phiP;
        }
        else
        {
            phiU = phiN;
        }

        scalar phiCD = cdWeight*phiP + (1 - cdWeight)*phiN;

        // Calculate the effective limiter for the Phi interpolation
        //scalar PLimiter =
        //    (1.0 - k_) + k_*(faceFlux - phiU)/stabilise(phiCD - phiU, SMALL);

        scalar PLimiter =
            ((faceFlux - phiU)/stabilise(phiCD - phiU, SMALL) + k_);

        // Limit the limiter between upwind and central
        return max(min(PLimiter, 1), 0);
    }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
