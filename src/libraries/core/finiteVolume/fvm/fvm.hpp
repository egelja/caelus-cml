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

Namespace
    CML::fvm

Description
    Namespace of functions to calculate implicit derivatives returning a
    matrix.

    Temporal derivatives are calculated using Euler-implicit, backward
    differencing or Crank-Nicolson. Spatial derivatives are calculated
    using Gauss' Theorem.


\*---------------------------------------------------------------------------*/

#ifndef fvm_H
#define fvm_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvmDdt.hpp"
#include "fvmD2dt2.hpp"
#include "fvmDiv.hpp"
#include "fvmLaplacian.hpp"
#include "fvmSup.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
