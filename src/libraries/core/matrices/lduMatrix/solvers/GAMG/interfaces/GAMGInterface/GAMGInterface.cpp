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

#include "GAMGInterface.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(GAMGInterface, 0);
    defineRunTimeSelectionTable(GAMGInterface, lduInterface);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::GAMGInterface::combine(const GAMGInterface& coarseGi)
{
    const labelList& coarseFra = coarseGi.faceRestrictAddressing_;

    forAll(faceRestrictAddressing_, ffi)
    {
        faceRestrictAddressing_[ffi] = coarseFra[faceRestrictAddressing_[ffi]];
    }

    faceCells_ = coarseGi.faceCells_;
}


CML::tmp<CML::labelField> CML::GAMGInterface::interfaceInternalField
(
    const labelUList& internalData
) const
{
    return interfaceInternalField<label>(internalData);
}


CML::tmp<CML::scalarField> CML::GAMGInterface::agglomerateCoeffs
(
    const scalarField& fineCoeffs
) const
{
    tmp<scalarField> tcoarseCoeffs(new scalarField(size(), 0.0));
    scalarField& coarseCoeffs = tcoarseCoeffs();

    forAll(faceRestrictAddressing_, ffi)
    {
        coarseCoeffs[faceRestrictAddressing_[ffi]] += fineCoeffs[ffi];
    }

    return tcoarseCoeffs;
}


// ************************************************************************* //
