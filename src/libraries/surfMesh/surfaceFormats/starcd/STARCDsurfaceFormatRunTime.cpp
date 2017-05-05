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

\*---------------------------------------------------------------------------*/

#include "STARCDsurfaceFormat.hpp"

#include "addToRunTimeSelectionTable.hpp"
#include "addToMemberFunctionSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

// read MeshedSurface
addNamedTemplatedToRunTimeSelectionTable
(
    MeshedSurface,
    STARCDsurfaceFormat,
    face,
    fileExtension,
    inp
);
addNamedTemplatedToRunTimeSelectionTable
(
    MeshedSurface,
    STARCDsurfaceFormat,
    triFace,
    fileExtension,
    inp
);

// write MeshedSurfaceProxy
addNamedTemplatedToMemberFunctionSelectionTable
(
    MeshedSurfaceProxy,
    STARCDsurfaceFormat,
    face,
    write,
    fileExtension,
    inp
);
addNamedTemplatedToMemberFunctionSelectionTable
(
    MeshedSurfaceProxy,
    STARCDsurfaceFormat,
    triFace,
    write,
    fileExtension,
    inp
);


}
}

// ************************************************************************* //
