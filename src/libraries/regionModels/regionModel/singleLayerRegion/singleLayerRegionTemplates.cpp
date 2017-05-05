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

#include "singleLayerRegion.hpp"
#include "zeroGradientFvPatchFields.hpp"
#include "mappedFixedInternalValueFvPatchFields.hpp"
#include "mappedFixedPushedInternalValueFvPatchFields.hpp"

// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //

template<class Type>
CML::wordList
CML::regionModels::singleLayerRegion::mappedFieldAndInternalPatchTypes() const
{
    wordList bTypes(regionMesh().boundaryMesh().size());

    bTypes = zeroGradientFvPatchField<Type>::typeName;

    forAll(intCoupledPatchIDs_, i)
    {
        const label patchI = intCoupledPatchIDs_[i];
        bTypes[patchI] = mappedFixedInternalValueFvPatchField<Type>::typeName;
    }

    return bTypes;
}


template<class Type>
CML::wordList
CML::regionModels::singleLayerRegion::mappedPushedFieldPatchTypes() const
{
    wordList bTypes(regionMesh().boundaryMesh().size());

    bTypes = zeroGradientFvPatchField<Type>::typeName;

    forAll(intCoupledPatchIDs_, i)
    {
        const label patchI = intCoupledPatchIDs_[i];
        bTypes[patchI] =
            mappedFixedPushedInternalValueFvPatchField<Type>::typeName;
    }

    return bTypes;
}


// ************************************************************************* //
