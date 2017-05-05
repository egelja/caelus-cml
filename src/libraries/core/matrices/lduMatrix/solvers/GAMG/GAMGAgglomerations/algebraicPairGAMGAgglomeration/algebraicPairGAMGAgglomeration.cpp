/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2016 Applied CCM
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

#include "algebraicPairGAMGAgglomeration.hpp"
#include "lduMatrix.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML
{
    defineTypeNameAndDebug(algebraicPairGAMGAgglomeration, 0);

    addToRunTimeSelectionTable
    (
        GAMGAgglomeration,
        algebraicPairGAMGAgglomeration,
        lduMatrix
    );
}

CML::algebraicPairGAMGAgglomeration::algebraicPairGAMGAgglomeration
(
    lduMatrix const& matrix,
    dictionary const& controlDict
) :
    pairGAMGAgglomeration(matrix.mesh(), controlDict)
{
    if (matrix.hasLower())
        agglomerate
        (
            matrix.mesh(),
            max(mag(matrix.lower()), mag(matrix.upper()))
        );
    else
        agglomerate(matrix.mesh(), mag(matrix.upper()));
}


// ************************************************************************* //
