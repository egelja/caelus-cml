/*---------------------------------------------------------------------------*\
Copyright (C) 2010 I. Clifford
Copyright (C) 2016 H. Jasak
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Description
    This file was obtained from Ivor Clifford at the OpenFOAM Workshop
    in PennState, USA, 2011.

    The original file contained "Copyright held by original author",
    but no author was named in the original file. Authorship confirmed by
    Ivor Clifford (2017).

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Ivor Clifford
    Hrvoje Jasak

\*---------------------------------------------------------------------------*/

#include "cyclicFvPatchHyperVectorFields.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "BlockLduMatrix.hpp"


namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

#define hyperVectorMatrixInterfaceFunc(Type)                                  \
template<>                                                                    \
void cyclicFvPatchField<Type>::updateInterfaceMatrix                          \
(                                                                             \
    Field<Type>& result,                                                      \
    const Field<Type>& psiInternal,                                           \
    const BlockLduMatrix<Type>&,                                              \
    const CoeffField<Type>& coeffs,                                           \
    const Pstream::commsTypes commsType                                       \
) const                                                                       \
{                                                                             \
    Field<Type> pnf(this->size());                                            \
                                                                              \
    label sizeby2 = this->size()/2;                                           \
    const unallocLabelList& faceCells = cyclicPatch_.faceCells();             \
                                                                              \
    for (label facei=0; facei<sizeby2; facei++)                               \
    {                                                                         \
        pnf[facei] = psiInternal[faceCells[facei + sizeby2]];                 \
        pnf[facei + sizeby2] = psiInternal[faceCells[facei]];                 \
    }                                                                         \
                                                                              \
    if (coeffs.activeType() == blockCoeffBase::SCALAR)                        \
    {                                                                         \
        pnf = coeffs.asScalar() * pnf;                                        \
    }                                                                         \
    else if (coeffs.activeType() == blockCoeffBase::LINEAR)                   \
    {                                                                         \
        pnf = cmptMultiply(coeffs.asLinear(), pnf);                           \
    }                                                                         \
    else if (coeffs.activeType() == blockCoeffBase::SQUARE)                   \
    {                                                                         \
        pnf = coeffs.asSquare() & pnf;                                        \
    }                                                                         \
                                                                              \
    forAll(faceCells, elemI)                                                  \
    {                                                                         \
        result[faceCells[elemI]] -= pnf[elemI];                               \
    }                                                                         \
}


#define doMakePatchTypeField(type, Type, args...)                             \
                                                                              \
hyperVectorMatrixInterfaceFunc(type)                                          \
                                                                              \
makeTemplatePatchTypeField                                                    \
(                                                                             \
    fvPatch##Type##Field,                                                     \
    cyclicFvPatch##Type##Field                                                \
);

    forAllHyperVectorTypes(doMakePatchTypeField)

#undef doMakePatchTypeField

#undef hyperVectorMatrixInterfaceFunc


} // End namespace CML
