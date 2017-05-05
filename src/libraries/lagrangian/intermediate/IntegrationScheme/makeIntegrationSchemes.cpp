/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
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

#include "IntegrationScheme.hpp"
#include "Euler.hpp"
#include "Analytical.hpp"

#include "scalar.hpp"
#include "vector.hpp"

#ifdef windows
//Symbol to force loading at runtime
extern "C"
void lagrangianIntermediateLoad()
{}
#endif

namespace CML
{
    makeIntegrationScheme(scalar);
    makeIntegrationSchemeType(Euler, scalar);
    makeIntegrationSchemeType(Analytical, scalar);

    makeIntegrationScheme(vector);
    makeIntegrationSchemeType(Euler, vector);
    makeIntegrationSchemeType(Analytical, vector);
}


// ************************************************************************* //
