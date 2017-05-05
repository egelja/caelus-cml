/*---------------------------------------------------------------------------*\
Copyright (C) 2015-2016 OpenFOAM Foundation
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

#include "localEulerDdtScheme.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

CML::word CML::fv::localEulerDdt::rDeltaTName("rDeltaT");
CML::word CML::fv::localEulerDdt::rDeltaTfName("rDeltaTf");
CML::word CML::fv::localEulerDdt::rSubDeltaTName("rSubDeltaT");

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

bool CML::fv::localEulerDdt::enabled(const fvMesh& mesh)
{
    return
        word(mesh.ddtScheme("default"))
     == fv::localEulerDdtScheme<scalar>::typeName;
}


const CML::volScalarField& CML::fv::localEulerDdt::localRDeltaT
(
    const fvMesh& mesh
)
{
    return mesh.objectRegistry::lookupObject<volScalarField>
    (
        mesh.time().subCycling() ? rSubDeltaTName : rDeltaTName
    );
}


const CML::surfaceScalarField& CML::fv::localEulerDdt::localRDeltaTf
(
    const fvMesh& mesh
)
{
    return mesh.objectRegistry::lookupObject<surfaceScalarField>
    (
        rDeltaTfName
    );
}


CML::tmp<CML::volScalarField> CML::fv::localEulerDdt::localRSubDeltaT
(
    const fvMesh& mesh,
    const label nAlphaSubCycles
)
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            rSubDeltaTName,
            nAlphaSubCycles
           *mesh.objectRegistry::lookupObject<volScalarField>
            (
                rDeltaTName
            )
        )
    );
}


// ************************************************************************* //
