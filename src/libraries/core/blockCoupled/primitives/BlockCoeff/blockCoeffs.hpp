/*---------------------------------------------------------------------------*\
Copyright (C) 2004-6 H. Jasak All rights reserved
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus. If not, see <http://www.gnu.org/licenses/>.

Class
    blockCoeffs

Description
    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

\*---------------------------------------------------------------------------*/

#ifndef blockCoeffs_HPP
#define blockCoeffs_HPP

#include "scalarBlockCoeff.hpp"
#include "sphericalTensorBlockCoeff.hpp"
#include "symmTensorBlockCoeff.hpp"
#include "tensorBlockCoeff.hpp"
#include "DecoupledCoeffField.hpp"


namespace CML
{

typedef BlockCoeff<scalar> scalarBlockCoeff;
typedef BlockCoeff<sphericalTensor> sphericalTensorBlockCoeff;
typedef BlockCoeff<symmTensor> symmTensorBlockCoeff;
typedef BlockCoeff<tensor> tensorBlockCoeff;

/* * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * */

template<class Type>
scalar sumMult
(
    const Field<Type>& f1,
    const Field<Type>& f2
)
{
    Type result = pTraits<Type>::zero;

    typename BlockCoeff<Type>::multiply mult;

    for (register label i = 0; i < f1.size(); i++)
    {
        result += mult(f1[i], f2[i]);
    }

    return cmptSum(result);
}


template<class Type>
scalar gSumMult
(
    const Field<Type>& f1,
    const Field<Type>& f2
)
{
    scalar SumProd = sumMult(f1, f2);
    reduce(SumProd, sumOp<scalar>());
    return SumProd;
}


template<>
inline scalar sumMult
(
    const scalarField& f1,
    const scalarField& f2
)
{
    return sumProd(f1, f2);
}


} // End namespace CML
#endif
