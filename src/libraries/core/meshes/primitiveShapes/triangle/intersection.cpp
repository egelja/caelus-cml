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

#include "intersection.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

CML::scalar CML::intersection::planarTol_ = 0.2;

namespace CML
{
    template<>
    const char* CML::NamedEnum
    <
        CML::intersection::direction,
        2
    >::names[] =
    {
        "vector",
        "contactSphere"
    };

    template<>
    const char* CML::NamedEnum
    <
        CML::intersection::algorithm,
        3
    >::names[] =
    {
        "fullRay",
        "halfRay",
        "visible"
    };
}

const CML::NamedEnum<CML::intersection::direction, 2>
CML::intersection::directionNames_;

const CML::NamedEnum<CML::intersection::algorithm, 3>
CML::intersection::algorithmNames_;


// ************************************************************************* //
