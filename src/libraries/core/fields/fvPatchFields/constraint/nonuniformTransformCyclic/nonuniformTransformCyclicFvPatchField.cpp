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

#include "nonuniformTransformCyclicFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    cyclicFvPatchField<Type>(p, iF)
{}


template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
    const nonuniformTransformCyclicFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    cyclicFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    cyclicFvPatchField<Type>(p, iF, dict)
{}


template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
    const nonuniformTransformCyclicFvPatchField<Type>& ptf
)
:
    cyclicFvPatchField<Type>(ptf)
{}


template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
    const nonuniformTransformCyclicFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    cyclicFvPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
