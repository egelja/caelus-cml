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

#include "cyclicAMIFvPatch.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvMesh.hpp"
#include "transform.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(cyclicAMIFvPatch, 0);
    addToRunTimeSelectionTable(fvPatch, cyclicAMIFvPatch, polyPatch);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::cyclicAMIFvPatch::makeWeights(scalarField& w) const
{
    const cyclicAMIFvPatch& nbrPatch = neighbFvPatch();

    const scalarField deltas(nf() & fvPatch::delta());

    const scalarField nbrDeltas
    (
        interpolate(nbrPatch.nf() & nbrPatch.fvPatch::delta())
    );

    forAll(deltas, faceI)
    {
        scalar di = deltas[faceI];
        scalar dni = nbrDeltas[faceI];

        w[faceI] = dni/(di + dni);
    }
}


CML::tmp<CML::vectorField> CML::cyclicAMIFvPatch::delta() const
{
    const vectorField patchD(fvPatch::delta());

    const cyclicAMIFvPatch& nbrPatch = neighbFvPatch();
    const vectorField nbrPatchD(interpolate(nbrPatch.fvPatch::delta()));

    tmp<vectorField> tpdv(new vectorField(patchD.size()));
    vectorField& pdv = tpdv();

    // do the transformation if necessary
    if (parallel())
    {
        forAll(patchD, faceI)
        {
            vector ddi = patchD[faceI];
            vector dni = nbrPatchD[faceI];

            pdv[faceI] = ddi - dni;
        }
    }
    else
    {
        forAll(patchD, faceI)
        {
            vector ddi = patchD[faceI];
            vector dni = nbrPatchD[faceI];

            pdv[faceI] = ddi - transform(forwardT()[0], dni);
        }
    }

    return tpdv;
}


CML::tmp<CML::labelField> CML::cyclicAMIFvPatch::interfaceInternalField
(
    const labelUList& internalData
) const
{
    return patchInternalField(internalData);
}


CML::tmp<CML::labelField> CML::cyclicAMIFvPatch::internalFieldTransfer
(
    const Pstream::commsTypes commsType,
    const labelUList& iF
) const
{
    return neighbFvPatch().patchInternalField(iF);
}


// ************************************************************************* //
