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

InNamespace
    CML

Description
    For cases which do no have a pressure boundary adjust the balance of
    fluxes to obey continuity.  Return true if the domain is closed.

SourceFiles
    adjustPhi.cpp

\*---------------------------------------------------------------------------*/

#ifndef adjustPhi_H
#define adjustPhi_H

#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

//- Adjust the balance of fluxes to obey continuity.
//  For cases which do not have a pressure boundary.
//  Return true if the domain is closed.
bool adjustPhi
(
    surfaceScalarField& phi,
    const volVectorField& U,
    volScalarField& p
);

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
