/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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

Description
    Riemann solver based on local Kurganov-Tadmor-Petrova 
    central-upwind scheme

Reference
    [1] A. Kurganov, S. Noelle and G. Petrova: "Semi-discrete central-upwind 
        scheme for hyperbolic conservation laws and Hamilton-Jacobi equations. 
        In: SIAM J. Sci. Comput., 23, 2001, pp. 707–740
    [2] A. Kurganov and E. Tadmor: "New high-resolution central schemes for 
        nonlinear conservation laws and convection-diffusion equations". 
        In: J. Comput. Phys., 160, 2000, pp. 241–282.
Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef ktpFlux_HPP
#define ktpFlux_HPP

#include "fvCFD.hpp"

namespace CML
{

class ktpFlux
{

public:

    ktpFlux() {}

    void evaluateFlux
    (
        scalar& rhoFlux,
        vector& rhoUFlux,
        scalar& rhoEFlux,
        scalar const pLeft,
        scalar const pRight,
        vector const ULeft,
        vector const URight,
        scalar const TLeft,
        scalar const TRight,
        scalar const RLeft,
        scalar const RRight,
        scalar const CvLeft,
        scalar const CvRight,
        vector const Sf,
        scalar const magSf,
        vector const dotX
    ) const;
};

}

#endif

