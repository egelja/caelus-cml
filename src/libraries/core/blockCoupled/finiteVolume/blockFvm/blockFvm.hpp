/*---------------------------------------------------------------------------*\
Copyright (C) 2017 Applied CCM Pty Ltd
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

Namespace
    CML::blockFvm

Description
    Namespace of functions to calculate implicit derivatives returning a
    matrix.

    Temporal derivatives are calculated using Euler-implicit, backward
    differencing or Crank-Nicolson. Spatial derivatives are calculated
    using Gauss' Theorem.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

\*---------------------------------------------------------------------------*/

#ifndef blockFvm_HPP
#define blockFvm_HPP

#include "blockFvmDiv.hpp"
#include "blockFvmGrad.hpp"


#endif
