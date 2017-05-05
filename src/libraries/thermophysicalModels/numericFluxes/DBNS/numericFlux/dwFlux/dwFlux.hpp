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

    Riemann solver based on dominant wave scheme

Reference

    [1] "High Resolution Central Scheme with Dominant Wave Dissipation for
         Cell Centererd Unstructured Meshes", A. Jemcov, J.P. Maruszewski,
         51st AIAA Aerospace Sciences Meeting Including the New Horizons
         and Aerospace Exposition, January 2013, Grapevine, Texas

    [2] "The Dominant Wave-Capturing Flux: A Finite Volume Scheme Withouth
         Decomposition for Systems of Hyperbolic Laws", M.G. Edwards,
         Journal of Computational Physics, 218 (2006), pp 275-294     
    
Author

    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef dwFlux_HPP
#define dwFlux_HPP

#include "fvCFD.hpp"

namespace CML
{

class dwFlux
{

public:

    dwFlux() {}

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

