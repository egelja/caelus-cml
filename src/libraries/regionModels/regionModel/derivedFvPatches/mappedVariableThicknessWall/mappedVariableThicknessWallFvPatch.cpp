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

#include "mappedVariableThicknessWallFvPatch.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "regionModel1D.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(mappedVariableThicknessWallFvPatch, 0);
    addToRunTimeSelectionTable
    (
        fvPatch,
        mappedVariableThicknessWallFvPatch,
        polyPatch
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::mappedVariableThicknessWallFvPatch::
makeDeltaCoeffs(scalarField& dc) const
{
    const mappedVariableThicknessWallPolyPatch& pp =
        refCast<const mappedVariableThicknessWallPolyPatch>(patch());

    const mappedPatchBase& mpp = refCast<const mappedPatchBase>(patch());

    const polyMesh& nbrMesh = mpp.sampleMesh();

    typedef regionModels::regionModel1D modelType;

    const modelType& region1D =
        nbrMesh.objectRegistry::lookupObject<modelType>
        (
            "thermoBaffleProperties"
        );

    dc = 2.0/(pp.thickness()/region1D.nLayers());
}


// ************************************************************************* //
