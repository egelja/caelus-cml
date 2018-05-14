/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2013 H. Jasak
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

Class
    BlockCholeskyPrecon

Description
    Incomplete Cholesky preconditioning with no fill-in.

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Minor corrections have been ported from FOAM-Extend code.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved.

\*---------------------------------------------------------------------------*/

#ifndef BlockCholeskyPrecon_HPP
#define BlockCholeskyPrecon_HPP

#include "BlockLduPrecon.hpp"
#include "error.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class BlockCholeskyPrecon Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockCholeskyPrecon
:
    public BlockLduPrecon<Type>
{

    //- Preconditioned diagonal
    mutable CoeffField<Type> preconDiag_;

    //- Disallow default bitwise copy construct
    BlockCholeskyPrecon(const BlockCholeskyPrecon&);

    //- Disallow default bitwise assignment
    void operator=(const BlockCholeskyPrecon&);

    //- Precondition the diagonal
    void calcPreconDiag();

    // Diagonal multiplication, symmetric matrix
    template<class DiagType, class ULType>
    void diagMultiply
    (
        Field<DiagType>& dDiag,
        const Field<ULType>& upper
    );

    //- Diagonal multiplication with transpose upper square coeff
    //  for the symmetric matrix
    template<class DiagType, class ULType>
    void diagMultiplyCoeffT
    (
        Field<DiagType>& dDiag,
        const Field<ULType>& upper
    );

    //- Diagonal multiplication, asymmetric matrix
    template<class DiagType, class ULType>
    void diagMultiply
    (
        Field<DiagType>& dDiag,
        const Field<ULType>& lower,
        const Field<ULType>& upper
    );

    //- ILU multiplication, symmetric matrix
    template<class DiagType, class ULType>
    void ILUmultiply
    (
        Field<Type>& x,
        const Field<DiagType>& dDiag,
        const Field<ULType>& upper,
        const Field<Type>& b
    ) const;

    //- ILU multiplication, with transpose upper square coeff
    //  for a symmetric matrix
    template<class DiagType, class ULType>
    void ILUmultiplyCoeffT
    (
        Field<Type>& x,
        const Field<DiagType>& dDiag,
        const Field<ULType>& upper,
        const Field<Type>& b
    ) const;

    //- ILU multiplication, asymmetric matrix
    template<class DiagType, class ULType>
    void ILUmultiply
    (
        Field<Type>& x,
        const Field<DiagType>& dDiag,
        const Field<ULType>& lower,
        const Field<ULType>& upper,
        const Field<Type>& b
    ) const;

    //- ILU multiplication transposed asymmetric matrix
    template<class DiagType, class ULType>
    void ILUmultiplyTranspose
    (
        Field<Type>& x,
        const Field<DiagType>& dDiag,
        const Field<ULType>& lower,
        const Field<ULType>& upper,
        const Field<Type>& b
    ) const;

    // Decoupled operations, used in template specialisation
    //- Precondition the diagonal, decoupled version
    void calcDecoupledPreconDiag();

    //- Execute preconditioning, decoupled version
    void decoupledPrecondition
    (
        Field<Type>& x,
        const Field<Type>& b
    ) const;

    //- Execute preconditioning with matrix transpose,
    //  decoupled version
    void decoupledPreconditionT
    (
        Field<Type>& xT,
        const Field<Type>& bT
    ) const;


public:

    //- Runtime type information
    TypeName("Cholesky");

    //- Construct from matrix for smoother use
    BlockCholeskyPrecon
    (
        const BlockLduMatrix<Type>& matrix
    );

    //- Construct from components
    BlockCholeskyPrecon
    (
        const BlockLduMatrix<Type>& matrix,
        const dictionary& dict
    );

    virtual ~BlockCholeskyPrecon();

    // Member Functions
    //- Execute preconditioning
    virtual void precondition
    (
        Field<Type>& x,
        const Field<Type>& b
    ) const;

    //- Execute preconditioning with matrix transpose
    virtual void preconditionT
    (
        Field<Type>& xT,
        const Field<Type>& bT
    ) const;
};

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void BlockCholeskyPrecon<Type>::calcPreconDiag()
{
    typedef CoeffField<Type> TypeCoeffField;

    // Note: Assuming lower and upper triangle have the same active type
    if (this->matrix_.symmetric())
    {
        const TypeCoeffField& UpperCoeff = this->matrix_.upper();

        if (preconDiag_.activeType() == blockCoeffBase::SCALAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                diagMultiply
                (
                    preconDiag_.asScalar(),
                    UpperCoeff.asScalar()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                diagMultiply
                (
                    preconDiag_.asLinear(),
                    UpperCoeff.asLinear()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                // Transpose multiplication
                diagMultiplyCoeffT
                (
                    preconDiag_.asSquare(),
                    UpperCoeff.asSquare()
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::LINEAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                diagMultiply
                (
                    preconDiag_.asLinear(),
                    UpperCoeff.asScalar()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                diagMultiply
                (
                    preconDiag_.asLinear(),
                    UpperCoeff.asLinear()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                // Transpose multiplication
                diagMultiplyCoeffT
                (
                    preconDiag_.asSquare(),
                    UpperCoeff.asSquare()
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::SQUARE)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                diagMultiply
                (
                    preconDiag_.asSquare(),
                    UpperCoeff.asScalar()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                diagMultiply
                (
                    preconDiag_.asSquare(),
                    UpperCoeff.asLinear()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                // Transpose multiplication
                diagMultiplyCoeffT
                (
                    preconDiag_.asSquare(),
                    UpperCoeff.asSquare()
                );
            }
        }
    }
    else // Asymmetric matrix
    {
        const TypeCoeffField& LowerCoeff = this->matrix_.lower();
        const TypeCoeffField& UpperCoeff = this->matrix_.upper();

        if (preconDiag_.activeType() == blockCoeffBase::SCALAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                diagMultiply
                (
                    preconDiag_.asScalar(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                diagMultiply
                (
                    preconDiag_.asLinear(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                diagMultiply
                (
                    preconDiag_.asSquare(),
                    LowerCoeff.asSquare(),
                    UpperCoeff.asSquare()
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::LINEAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                diagMultiply
                (
                    preconDiag_.asLinear(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                diagMultiply
                (
                    preconDiag_.asLinear(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                diagMultiply
                (
                    preconDiag_.asSquare(),
                    LowerCoeff.asSquare(),
                    UpperCoeff.asSquare()
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::SQUARE)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                diagMultiply
                (
                    preconDiag_.asSquare(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                diagMultiply
                (
                    preconDiag_.asSquare(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                diagMultiply
                (
                    preconDiag_.asSquare(),
                    LowerCoeff.asSquare(),
                    UpperCoeff.asSquare()
                );
            }
        }
    }

    // Invert the diagonal
    preconDiag_ = inv(preconDiag_);
}


template<class Type>
template<class DiagType, class ULType>
void BlockCholeskyPrecon<Type>::diagMultiply
(
    Field<DiagType>& dDiag,
    const Field<ULType>& upper
)
{
    // Precondition the diagonal
    // Get addressing
    const unallocLabelList& upperAddr = this->matrix_.lduAddr().upperAddr();
    const unallocLabelList& lowerAddr = this->matrix_.lduAddr().lowerAddr();

    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    forAll (upper, coeffI)
    {
        dDiag[upperAddr[coeffI]] -=
            mult.tripleProduct
            (
                upper[coeffI],
                dDiag[lowerAddr[coeffI]],
                upper[coeffI]
            );
    }
}


template<class Type>
template<class DiagType, class ULType>
void BlockCholeskyPrecon<Type>::diagMultiplyCoeffT
(
    Field<DiagType>& dDiag,
    const Field<ULType>& upper
)
{
    // Precondition the diagonal
    // Get addressing
    const unallocLabelList& upperAddr = this->matrix_.lduAddr().upperAddr();
    const unallocLabelList& lowerAddr = this->matrix_.lduAddr().lowerAddr();

    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    forAll (upper, coeffI)
    {
        dDiag[upperAddr[coeffI]] -=
            mult.tripleProduct
            (
                upper[coeffI].T(),        // Upper coefficient transposed
                dDiag[lowerAddr[coeffI]],
                upper[coeffI]
            );
    }
}


template<class Type>
template<class DiagType, class ULType>
void BlockCholeskyPrecon<Type>::diagMultiply
(
    Field<DiagType>& dDiag,
    const Field<ULType>& lower,
    const Field<ULType>& upper
)
{
    // Precondition the diagonal
    // Get addressing
    const unallocLabelList& upperAddr = this->matrix_.lduAddr().upperAddr();
    const unallocLabelList& lowerAddr = this->matrix_.lduAddr().lowerAddr();

    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    forAll (upper, coeffI)
    {
        dDiag[upperAddr[coeffI]] -=
            mult.tripleProduct
            (
                lower[coeffI],
                dDiag[lowerAddr[coeffI]],
                upper[coeffI]
            );
    }
}


template<class Type>
template<class DiagType, class ULType>
void BlockCholeskyPrecon<Type>::ILUmultiply
(
    Field<Type>& x,
    const Field<DiagType>& dDiag,
    const Field<ULType>& upper,
    const Field<Type>& b
) const
{
    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    forAll(x, i)
    {
        x[i] = mult(dDiag[i], b[i]);
    }

    const unallocLabelList& upperAddr = this->matrix_.lduAddr().upperAddr();
    const unallocLabelList& lowerAddr = this->matrix_.lduAddr().lowerAddr();

    forAll (upper, coeffI)
    {
        x[upperAddr[coeffI]] -=
            mult
            (
                dDiag[upperAddr[coeffI]],
                mult(upper[coeffI], x[lowerAddr[coeffI]])
            );
    }

    forAllReverse (upper, coeffI)
    {
        x[lowerAddr[coeffI]] -=
            mult
            (
                dDiag[lowerAddr[coeffI]],
                mult(upper[coeffI], x[upperAddr[coeffI]])
            );
    }
}


template<class Type>
template<class DiagType, class ULType>
void BlockCholeskyPrecon<Type>::ILUmultiplyCoeffT
(
    Field<Type>& x,
    const Field<DiagType>& dDiag,
    const Field<ULType>& upper,
    const Field<Type>& b
) const
{
    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    forAll(x, i)
    {
        x[i] = mult(dDiag[i], b[i]);
    }

    const unallocLabelList& upperAddr = this->matrix_.lduAddr().upperAddr();
    const unallocLabelList& lowerAddr = this->matrix_.lduAddr().lowerAddr();

    forAll (upper, coeffI)
    {
        x[upperAddr[coeffI]] -=
            mult
            (
                dDiag[upperAddr[coeffI]],
                mult(upper[coeffI].T(), x[lowerAddr[coeffI]])
            );
    }

    forAllReverse (upper, coeffI)
    {
        x[lowerAddr[coeffI]] -=
            mult
            (
                dDiag[lowerAddr[coeffI]],
                mult(upper[coeffI], x[upperAddr[coeffI]])
            );
    }
}


template<class Type>
template<class DiagType, class ULType>
void BlockCholeskyPrecon<Type>::ILUmultiply
(
    Field<Type>& x,
    const Field<DiagType>& dDiag,
    const Field<ULType>& lower,
    const Field<ULType>& upper,
    const Field<Type>& b
) const
{
    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    forAll(x, i)
    {
        x[i] = mult(dDiag[i], b[i]);
    }

    const unallocLabelList& upperAddr = this->matrix_.lduAddr().upperAddr();
    const unallocLabelList& lowerAddr = this->matrix_.lduAddr().lowerAddr();
    const unallocLabelList& losortAddr = this->matrix_.lduAddr().losortAddr();

    register label losortCoeff;

    forAll (lower, coeffI)
    {
        losortCoeff = losortAddr[coeffI];

        x[upperAddr[losortCoeff]] -=
            mult
            (
                dDiag[upperAddr[losortCoeff]],
                mult(lower[losortCoeff], x[lowerAddr[losortCoeff]])
            );
    }

    forAllReverse (upper, coeffI)
    {
        x[lowerAddr[coeffI]] -=
            mult
            (
                dDiag[lowerAddr[coeffI]],
                mult(upper[coeffI], x[upperAddr[coeffI]])
            );
    }
}


template<class Type>
template<class DiagType, class ULType>
void BlockCholeskyPrecon<Type>::ILUmultiplyTranspose
(
    Field<Type>& x,
    const Field<DiagType>& dDiag,
    const Field<ULType>& lower,
    const Field<ULType>& upper,
    const Field<Type>& b
) const
{
    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    forAll(x, i)
    {
        x[i] = mult(dDiag[i], b[i]);
    }

    const unallocLabelList& upperAddr = this->matrix_.lduAddr().upperAddr();
    const unallocLabelList& lowerAddr = this->matrix_.lduAddr().lowerAddr();
    const unallocLabelList& losortAddr = this->matrix_.lduAddr().losortAddr();

    register label losortCoeff;

    //HJ Not sure if the coefficient itself needs to be transposed. 30/Oct/2007
    forAll (lower, coeffI)
    {
        x[upperAddr[coeffI]] -=
            mult
            (
                dDiag[upperAddr[coeffI]],
                mult(upper[coeffI], x[lowerAddr[coeffI]])
            );
    }

    forAllReverse (upper, coeffI)
    {
        losortCoeff = losortAddr[coeffI];

        x[lowerAddr[losortCoeff]] -=
            mult
            (
                dDiag[lowerAddr[losortCoeff]],
                mult(lower[losortCoeff], x[upperAddr[losortCoeff]])
            );
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
BlockCholeskyPrecon<Type>::BlockCholeskyPrecon
(
    const BlockLduMatrix<Type>& matrix
)
:
    BlockLduPrecon<Type>(matrix),
    preconDiag_(matrix.diag())
{
    calcPreconDiag();
}


template<class Type>
BlockCholeskyPrecon<Type>::BlockCholeskyPrecon
(
    const BlockLduMatrix<Type>& matrix,
    const dictionary& dict
)
:
    BlockLduPrecon<Type>(matrix),
    preconDiag_(matrix.diag())
{
    calcPreconDiag();
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
BlockCholeskyPrecon<Type>::~BlockCholeskyPrecon()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void BlockCholeskyPrecon<Type>::precondition
(
    Field<Type>& x,
    const Field<Type>& b
) const
{
    typedef CoeffField<Type> TypeCoeffField;

    // Note: Assuming lower and upper triangle have the same active type
    if (this->matrix_.symmetric())
    {
        const TypeCoeffField& UpperCoeff = this->matrix_.upper();

        if (preconDiag_.activeType() == blockCoeffBase::SCALAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asScalar(),
                    UpperCoeff.asScalar(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asScalar(),
                    UpperCoeff.asLinear(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                // Multiplication using transposed upper square coefficient
                ILUmultiplyCoeffT
                (
                    x,
                    preconDiag_.asScalar(),
                    UpperCoeff.asSquare(),
                    b
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::LINEAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asLinear(),
                    UpperCoeff.asScalar(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asLinear(),
                    UpperCoeff.asLinear(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                // Multiplication using transposed upper square coefficient
                ILUmultiplyCoeffT
                (
                    x,
                    preconDiag_.asLinear(),
                    UpperCoeff.asSquare(),
                    b
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::SQUARE)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asSquare(),
                    UpperCoeff.asScalar(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asSquare(),
                    UpperCoeff.asLinear(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                // Multiplication using transposed upper square coefficient
                ILUmultiplyCoeffT
                (
                    x,
                    preconDiag_.asSquare(),
                    UpperCoeff.asSquare(),
                    b
                );
            }
        }
    }
    else // Asymmetric matrix
    {
        const TypeCoeffField& LowerCoeff = this->matrix_.lower();
        const TypeCoeffField& UpperCoeff = this->matrix_.upper();

        if (preconDiag_.activeType() == blockCoeffBase::SCALAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asScalar(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asScalar(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asScalar(),
                    LowerCoeff.asSquare(),
                    UpperCoeff.asSquare(),
                    b
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::LINEAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asLinear(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asLinear(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asLinear(),
                    LowerCoeff.asSquare(),
                    UpperCoeff.asSquare(),
                    b
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::SQUARE)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asSquare(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asSquare(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asSquare(),
                    LowerCoeff.asSquare(),
                    UpperCoeff.asSquare(),
                    b
                );
            }
        }
    }
}


template<class Type>
void BlockCholeskyPrecon<Type>::preconditionT
(
    Field<Type>& xT,
    const Field<Type>& bT
) const
{
    typedef CoeffField<Type> TypeCoeffField;

    // Note: Assuming lower and upper triangle have the same active type
    if (this->matrix_.symmetric())
    {
        precondition(xT, bT);
    }
    else // Asymmetric matrix
    {
        const TypeCoeffField& LowerCoeff = this->matrix_.lower();
        const TypeCoeffField& UpperCoeff = this->matrix_.upper();

        if (preconDiag_.activeType() == blockCoeffBase::SCALAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asScalar(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar(),
                    bT
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asScalar(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear(),
                    bT
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asScalar(),
                    LowerCoeff.asSquare(),
                    UpperCoeff.asSquare(),
                    bT
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::LINEAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asLinear(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar(),
                    bT
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asLinear(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear(),
                    bT
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asLinear(),
                    LowerCoeff.asSquare(),
                    UpperCoeff.asSquare(),
                    bT
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::SQUARE)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asSquare(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar(),
                    bT
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asSquare(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear(),
                    bT
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::SQUARE)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asSquare(),
                    LowerCoeff.asSquare(),
                    UpperCoeff.asSquare(),
                    bT
                );
            }
        }
    }
}

// BlockCholeskyPreconDecoupled.C
// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void BlockCholeskyPrecon<Type>::calcDecoupledPreconDiag()
{
    typedef CoeffField<Type> TypeCoeffField;

    // Note: Assuming lower and upper triangle have the same active type
    if (this->matrix_.symmetric())
    {
        const TypeCoeffField& UpperCoeff = this->matrix_.upper();

        if (preconDiag_.activeType() == blockCoeffBase::SCALAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                diagMultiply
                (
                    preconDiag_.asScalar(),
                    UpperCoeff.asScalar()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                diagMultiply
                (
                    preconDiag_.asLinear(),
                    UpperCoeff.asLinear()
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::LINEAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                diagMultiply
                (
                    preconDiag_.asLinear(),
                    UpperCoeff.asScalar()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                diagMultiply
                (
                    preconDiag_.asLinear(),
                    UpperCoeff.asLinear()
                );
            }
        }
    }
    else // Asymmetric matrix
    {
        const TypeCoeffField& LowerCoeff = this->matrix_.lower();
        const TypeCoeffField& UpperCoeff = this->matrix_.upper();

        if (preconDiag_.activeType() == blockCoeffBase::SCALAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                diagMultiply
                (
                    preconDiag_.asScalar(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                diagMultiply
                (
                    preconDiag_.asLinear(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear()
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::LINEAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                diagMultiply
                (
                    preconDiag_.asLinear(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar()
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                diagMultiply
                (
                    preconDiag_.asLinear(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear()
                );
            }
        }
    }

    // Invert the diagonal
    // Note: since square diag type does not exist, simple inversion
    // covers all cases
    preconDiag_ = inv(preconDiag_);
}


template<class Type>
void BlockCholeskyPrecon<Type>::decoupledPrecondition
(
    Field<Type>& x,
    const Field<Type>& b
) const
{
    typedef CoeffField<Type> TypeCoeffField;

    // Note: Assuming lower and upper triangle have the same active type
    if (this->matrix_.symmetric())
    {
        const TypeCoeffField& UpperCoeff = this->matrix_.upper();

        if (preconDiag_.activeType() == blockCoeffBase::SCALAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asScalar(),
                    UpperCoeff.asScalar(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asScalar(),
                    UpperCoeff.asLinear(),
                    b
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::LINEAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asLinear(),
                    UpperCoeff.asScalar(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asLinear(),
                    UpperCoeff.asLinear(),
                    b
                );
            }
        }
    }
    else // Asymmetric matrix
    {
        const TypeCoeffField& LowerCoeff = this->matrix_.lower();
        const TypeCoeffField& UpperCoeff = this->matrix_.upper();

        if (preconDiag_.activeType() == blockCoeffBase::SCALAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asScalar(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asScalar(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear(),
                    b
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::LINEAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asLinear(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar(),
                    b
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiply
                (
                    x,
                    preconDiag_.asLinear(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear(),
                    b
                );
            }
        }
    }
}


template<class Type>
void BlockCholeskyPrecon<Type>::decoupledPreconditionT
(
    Field<Type>& xT,
    const Field<Type>& bT
) const
{
    typedef CoeffField<Type> TypeCoeffField;

    // Note: Assuming lower and upper triangle have the same active type
    if (this->matrix_.symmetric())
    {
        precondition(xT, bT);
    }
    else // Asymmetric matrix
    {
        const TypeCoeffField& LowerCoeff = this->matrix_.lower();
        const TypeCoeffField& UpperCoeff = this->matrix_.upper();

        if (preconDiag_.activeType() == blockCoeffBase::SCALAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asScalar(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar(),
                    bT
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asScalar(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear(),
                    bT
                );
            }
        }
        else if (preconDiag_.activeType() == blockCoeffBase::LINEAR)
        {
            if (UpperCoeff.activeType() == blockCoeffBase::SCALAR)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asLinear(),
                    LowerCoeff.asScalar(),
                    UpperCoeff.asScalar(),
                    bT
                );
            }
            else if (UpperCoeff.activeType() == blockCoeffBase::LINEAR)
            {
                ILUmultiplyTranspose
                (
                    xT,
                    preconDiag_.asLinear(),
                    LowerCoeff.asLinear(),
                    UpperCoeff.asLinear(),
                    bT
                );
            }
        }
    }
}


} // End namespace CML
#endif
