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

Class
    CML::treeDataTriSurface

Description
    Encapsulates data for (indexedOc)tree searches on triSurface.

SourceFiles
    treeDataTriSurface.cpp

\*---------------------------------------------------------------------------*/

#ifndef treeDataTriSurface_H
#define treeDataTriSurface_H

#include "treeDataPrimitivePatch.hpp"
#include "triSurface.hpp"
#include "point.hpp"
#include "indexedOctree.hpp"
#include "volumeType.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    typedef treeDataPrimitivePatch<triSurface> treeDataTriSurface;

    //- Template specialisation of getVolumeType for treeDataTriSurface
    template<>
    volumeType treeDataPrimitivePatch<triSurface>::getVolumeType
    (
        const indexedOctree<treeDataPrimitivePatch<triSurface> >& oc,
        const point& sample
    ) const;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif

// ************************************************************************* //
