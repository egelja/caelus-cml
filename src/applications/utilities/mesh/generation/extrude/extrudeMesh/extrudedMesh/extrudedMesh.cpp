/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "extrudedMesh.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool CML::extrudedMesh::sameOrder(const face& f, const edge& e)
{
    label i = findIndex(f, e[0]);

    label nextI = (i == f.size()-1 ? 0 : i+1);

    return f[nextI] == e[1];
}


// ************************************************************************* //
