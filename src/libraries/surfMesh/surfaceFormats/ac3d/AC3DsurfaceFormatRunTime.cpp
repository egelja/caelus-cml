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

#include "AC3DsurfaceFormat.hpp"

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
    AC3DsurfaceFormat,
    face,
    fileExtension,
    ac
);
addNamedTemplatedToRunTimeSelectionTable
(
    MeshedSurface,
    AC3DsurfaceFormat,
    triFace,
    fileExtension,
    ac
);

// write MeshedSurfaceProxy
addNamedTemplatedToMemberFunctionSelectionTable
(
    MeshedSurfaceProxy,
    AC3DsurfaceFormat,
    face,
    write,
    fileExtension,
    ac
);
addNamedTemplatedToMemberFunctionSelectionTable
(
    MeshedSurfaceProxy,
    AC3DsurfaceFormat,
    triFace,
    write,
    fileExtension,
    ac
);


// write UnsortedMeshedSurface
addNamedTemplatedToMemberFunctionSelectionTable
(
    UnsortedMeshedSurface,
    AC3DsurfaceFormat,
    face,
    write,
    fileExtension,
    ac
);
addNamedTemplatedToMemberFunctionSelectionTable
(
    UnsortedMeshedSurface,
    AC3DsurfaceFormat,
    triFace,
    write,
    fileExtension,
    ac
);

}
}

// ************************************************************************* //
