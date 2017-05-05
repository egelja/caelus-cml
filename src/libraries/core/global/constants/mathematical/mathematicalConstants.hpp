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
    CML::constant::mathematical

Description
    mathematical constants.

\*---------------------------------------------------------------------------*/

#ifndef mathematicalConstants_H
#define mathematicalConstants_H

#include "scalar.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace constant
{
namespace mathematical
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    static const char* const group = "mathematical";
	
#ifdef windows
    const scalar e = 2.718281828459045;
	const scalar pi = 3.141592653589793;

#else
    const scalar e(M_E);
    const scalar pi(M_PI);
	
#endif
	
    const scalar twoPi(2*pi);
    const scalar piByTwo(0.5*pi);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace mathematical
} // End namespace constant

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
