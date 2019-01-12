/*---------------------------------------------------------------------------*\
Copyright (C) 2004-6 H. Jasak
Copyright (C) 2017-2018 Applied CCM Pty Ltd
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
    BlockConstraint

Description
    A storage mechanism which allows setting of the fixed value and
    consequently recovering the equation for a single row of the matrix as
    well as the b. The equation is taken out of the matrix using a
    variant of compact matrix storage mechanism.

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved.
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#ifndef BlockConstraint_HPP
#define BlockConstraint_HPP

#include "BlockCoeff.hpp"
#include "coeffFields.hpp"
#include "scalarBlockConstraint.hpp"

#include "error.hpp"
#include "demandDrivenData.hpp"
#include "BlockLduMatrix.hpp"


namespace CML
{

template<class Type>
class BlockConstraint;

template<class Type>
Ostream& operator<<(Ostream&, const BlockConstraint<Type>&);


/*---------------------------------------------------------------------------*\
                        Class BlockConstraint Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockConstraint
{

    typedef CoeffField<Type> TypeCoeffField;
    typedef Field<Type> TypeField;

    //- Matrix row ID
    label rowID_;

    //- Fixed value
    Type value_;

    //- Fixed components (0-1) 1 = fixed, 0 = free
    Type fixedComponents_;

    //- Are matrix coefficients set?
    bool matrixCoeffsSet_;

    //- Diagonal coefficient
    BlockCoeff<Type> diagCoeff_;

    //- Right-hand side
    Type b_;

    //- Upper coefficients, owner side
    TypeCoeffField* upperCoeffsOwnerPtr_;

    //- Upper coefficients, neighbour side
    TypeCoeffField* upperCoeffsNeighbourPtr_;

    //- Lower coefficients, owner side
    TypeCoeffField* lowerCoeffsOwnerPtr_;

    //- Lower coefficients, neighbour side
    TypeCoeffField* lowerCoeffsNeighbourPtr_;


public:

    //- Construct from components
    BlockConstraint
    (
        const label row,
        const Type value,
        const Type& fixedCmpts = pTraits<Type>::one
    );

    //- Construct as copy
    BlockConstraint(const BlockConstraint&);

    //- Construct from Istream
    BlockConstraint(Istream&);

    ~BlockConstraint();

    // Member Functions
    //- Return matrix row ID
    label rowID() const
    {
        return rowID_;
    }

    //- Return fixed value
    Type value() const
    {
        return value_;
    }

    //- Return map of fixed components
    const Type& fixedComponents() const
    {
        return fixedComponents_;
    }

    //- Return diagonal coefficient
    const BlockCoeff<Type>& diagCoeff() const;

    //- Return right-hand side
    const Type& b() const;

    //- Return off-diagonal coefficients
    const TypeCoeffField& upperCoeffsOwner() const;

    const TypeCoeffField& upperCoeffsNeighbour() const;

    const TypeCoeffField& lowerCoeffsOwner() const;

    const TypeCoeffField& lowerCoeffsNeighbour() const;

    //- Combine with existing equation
    void combine(const BlockConstraint<Type>&);

    //- Set matrix coefficients
    void setMatrix
    (
        const BlockLduMatrix<Type>& matrix,
        const TypeField& x,
        const TypeField& b
    );

    //- Eliminate equation
    void eliminateEquation
    (
        BlockLduMatrix<Type>& matrix,
        TypeField& b
    ) const;

    //- Set x, b and diagonal in eliminated equation
    void setSourceDiag
    (
        BlockLduMatrix<Type>&,
        Field<Type>& x,
        Field<Type>& b
    ) const;

    //- Reconstruct matrix coefficients
    void reconstructMatrix(BlockLduMatrix<Type>&) const;

    //- Clear matrix coefficients
    void clearMatrix();

    // Member Operators
    void operator=(const BlockConstraint<Type>&);

    // Ostream Operator
    friend Ostream& operator<< <Type>
    (
        Ostream&,
        const BlockConstraint<Type>&
    );
};

// BlockConstraint.C
// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
template<class Type>
BlockConstraint<Type>::BlockConstraint
(
    const label row,
    const Type value,
    const Type& fixedCmpts
)
:
    rowID_(row),
    value_(value),
    fixedComponents_(fixedCmpts),
    matrixCoeffsSet_(false),
    diagCoeff_(),
    upperCoeffsOwnerPtr_(nullptr),
    upperCoeffsNeighbourPtr_(nullptr),
    lowerCoeffsOwnerPtr_(nullptr),
    lowerCoeffsNeighbourPtr_(nullptr)
{}


// Construct as copy
template<class Type>
BlockConstraint<Type>::BlockConstraint(const BlockConstraint& e)
:
    rowID_(e.rowID_),
    value_(e.value_),
    fixedComponents_(e.fixedComponents_),
    matrixCoeffsSet_(false),
    upperCoeffsOwnerPtr_(nullptr),
    upperCoeffsNeighbourPtr_(nullptr),
    lowerCoeffsOwnerPtr_(nullptr),
    lowerCoeffsNeighbourPtr_(nullptr)
{}


// Construct from Istream
template<class Type>
BlockConstraint<Type>::BlockConstraint(Istream& is)
:
    rowID_(readLabel(is)),
    value_(pTraits<Type>(is)),
    fixedComponents_(pTraits<Type>(is)),
    matrixCoeffsSet_(false),
    upperCoeffsOwnerPtr_(nullptr),
    upperCoeffsNeighbourPtr_(nullptr),
    lowerCoeffsOwnerPtr_(nullptr),
    lowerCoeffsNeighbourPtr_(nullptr)
{}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
BlockConstraint<Type>::~BlockConstraint()
{
    deleteDemandDrivenData(upperCoeffsOwnerPtr_);
    deleteDemandDrivenData(upperCoeffsNeighbourPtr_);

    deleteDemandDrivenData(lowerCoeffsOwnerPtr_);
    deleteDemandDrivenData(lowerCoeffsNeighbourPtr_);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const BlockCoeff<Type>& BlockConstraint<Type>::diagCoeff() const
{
    if (matrixCoeffsSet_)
    {
        FatalErrorInFunction
            << "matrix coefficients not set"
            << abort(FatalError);
    }

    return diagCoeff_;
}


template<class Type>
const Type& BlockConstraint<Type>::b() const
{
    if (matrixCoeffsSet_)
    {
        FatalErrorInFunction
            << "matrix coefficients not set"
            << abort(FatalError);
    }

    return b_;
}


template<class Type>
const CoeffField<Type>& BlockConstraint<Type>::upperCoeffsOwner() const
{
    if (not upperCoeffsOwnerPtr_ || not matrixCoeffsSet_)
    {
        FatalErrorInFunction
            << "upper matrix coefficients not set"
            << abort(FatalError);
    }

    return *upperCoeffsOwnerPtr_;
}


template<class Type>
const CoeffField<Type>& BlockConstraint<Type>::upperCoeffsNeighbour() const
{
    if (not upperCoeffsNeighbourPtr_ || not matrixCoeffsSet_)
    {
        FatalErrorInFunction
            << "upper matrix coefficients not set"
            << abort(FatalError);
    }

    return *upperCoeffsNeighbourPtr_;
}


template<class Type>
const CoeffField<Type>& BlockConstraint<Type>::lowerCoeffsOwner() const
{
    if (not lowerCoeffsOwnerPtr_ || not matrixCoeffsSet_)
    {
        FatalErrorInFunction
            << "lower matrix coefficients not set"
            << abort(FatalError);
    }

    return *lowerCoeffsOwnerPtr_;
}


template<class Type>
const CoeffField<Type>& BlockConstraint<Type>::lowerCoeffsNeighbour() const
{
    if (not lowerCoeffsNeighbourPtr_ || not matrixCoeffsSet_)
    {
        FatalErrorInFunction
            << "lower matrix coefficients not set"
            << abort(FatalError);
    }

    return *lowerCoeffsNeighbourPtr_;
}


template<class Type>
void BlockConstraint<Type>::combine
(
    const BlockConstraint<Type>& e
)
{
    for
    (
        direction cmptI = 0;
        cmptI < pTraits<Type>::nComponents;
        cmptI++
    )
    {
        if
        (
            e.fixedComponents_.component(cmptI)
          > fixedComponents_.component(cmptI)
        )
        {
            fixedComponents_.component(cmptI) =
                e.fixedComponents_.component(cmptI);

            value_.replace(cmptI, e.value_.component(cmptI));
        }
    }
}


template<class Type>
void BlockConstraint<Type>::clearMatrix()
{
    matrixCoeffsSet_ = false;

    diagCoeff_.clear();

    b_ = pTraits<Type>::zero;

    deleteDemandDrivenData(upperCoeffsOwnerPtr_);
    deleteDemandDrivenData(upperCoeffsNeighbourPtr_);

    deleteDemandDrivenData(lowerCoeffsOwnerPtr_);
    deleteDemandDrivenData(lowerCoeffsNeighbourPtr_);
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void BlockConstraint<Type>::operator=
(
    const BlockConstraint<Type>& rhs
)
{
    // Check for assignment to self
    if (this == &rhs)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    rowID_ = rhs.rowID_;

    value_ = rhs.value_;

    fixedComponents_ = rhs.fixedComponents_;

    clearMatrix();
}

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
Ostream& operator<<(Ostream& os, const BlockConstraint<Type>& e)
{
    os  << e.rowID_ << nl
        << e.value_ << nl
        << e.fixedComponents_ << nl;

    os.check("Ostream& operator<<(Ostream&, BlockConstraint<Type>&");

    return os;
}

// BlockConstraintTools.C
// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void BlockConstraint<Type>::setMatrix
(
    const BlockLduMatrix<Type>& matrix,
    const TypeField& x,
    const TypeField& b
)
{
    if (matrixCoeffsSet_)
    {
        FatalErrorInFunction
            << "matrix coefficients already set"
            << abort(FatalError);
    }

    matrixCoeffsSet_ = true;

    if (matrix.thereIsDiag())
    {
        diagCoeff_ = matrix.diag().getCoeff(rowID_);
    }

    b_ = b[rowID_];

    const label startFaceOwn =
        matrix.lduAddr().ownerStartAddr()[rowID_];
    const label endFaceOwn =
        matrix.lduAddr().ownerStartAddr()[rowID_ + 1];
    const label ownSize = endFaceOwn - startFaceOwn;

    const label startFaceNbr =
        matrix.lduAddr().losortStartAddr()[rowID_];
    const label endFaceNbr =
        matrix.lduAddr().losortStartAddr()[rowID_ + 1];
    const label nbrSize = endFaceNbr - startFaceNbr;

    const unallocLabelList& losort = matrix.lduAddr().losortAddr();

    // Create losort addressing
    labelList losortAddr(nbrSize);

    forAll (losortAddr, laI)
    {
        losortAddr[laI] = losort[startFaceNbr + laI];
    }

    if (matrix.thereIsUpper())
    {
        // Get the upper coefficients
        const TypeCoeffField& matrixUpper = matrix.upper();

        // Owner side
        upperCoeffsOwnerPtr_ = new TypeCoeffField(ownSize);
        TypeCoeffField& uOwn = *upperCoeffsOwnerPtr_;

        matrixUpper.getSubset(uOwn, startFaceOwn, ownSize);

        // Neighbour side
        upperCoeffsNeighbourPtr_ = new TypeCoeffField(nbrSize);
        TypeCoeffField& uNbr = *upperCoeffsNeighbourPtr_;

        matrixUpper.getSubset(uNbr, losortAddr);
    }

    if (matrix.thereIsLower())
    {
        // Get the lower coefficients
        const TypeCoeffField& matrixLower = matrix.lower();

        // Owner side
        lowerCoeffsOwnerPtr_ = new TypeCoeffField(ownSize);
        TypeCoeffField& lOwn = *lowerCoeffsOwnerPtr_;

        matrixLower.getSubset(lOwn, startFaceOwn, ownSize);

        // Neighbour side
        lowerCoeffsNeighbourPtr_ = new TypeCoeffField(nbrSize);
        TypeCoeffField& lNbr = *lowerCoeffsNeighbourPtr_;

        matrixLower.getSubset(lNbr, losortAddr);
    }
}


template<class Type>
void BlockConstraint<Type>::eliminateEquation
(
    BlockLduMatrix<Type>& matrix,
    TypeField& b
) const
{
    const label startFaceOwn =
        matrix.lduAddr().ownerStartAddr()[rowID_];
    const label endFaceOwn =
        matrix.lduAddr().ownerStartAddr()[rowID_ + 1];
    const label ownSize = endFaceOwn - startFaceOwn;

    const label startFaceNbr =
        matrix.lduAddr().losortStartAddr()[rowID_];
    const label endFaceNbr =
        matrix.lduAddr().losortStartAddr()[rowID_ + 1];
    const label nbrSize = endFaceNbr - startFaceNbr;

    const unallocLabelList& owner = matrix.lduAddr().lowerAddr();
    const unallocLabelList& neighbour = matrix.lduAddr().upperAddr();
    const unallocLabelList& losort = matrix.lduAddr().losortAddr();

    // Create losort addressing
    labelList losortAddr(nbrSize);

    forAll (losortAddr, laI)
    {
        losortAddr[laI] = losort[startFaceNbr + laI];
    }

    typename BlockCoeff<Type>::multiply mult;

    // My index = rowID_
    if (matrix.symmetric())
    {
        TypeField bOwn;
        TypeField bNbr;

        TypeCoeffField& upperLower = matrix.upper();
        upperLower.zeroOutSubset(startFaceOwn, ownSize);
        upperLower.zeroOutSubset(losortAddr);

        bOwn = upperCoeffsOwner()*value_;
        bNbr = upperCoeffsNeighbour()*value_;

        // Owner side
        forAll (bOwn, soI)
        {
            // Add contribution to b of the neighbour (I am the owner)
            b[neighbour[startFaceOwn + soI]] -= bOwn[soI];
        }

        // Neighbour side
        forAll (bNbr, snI)
        {
            // Add contribution to b of owner (I am the neighbour)
            b[owner[losort[startFaceNbr + snI]]] -= bNbr[snI];
        }
    }
    else if (matrix.asymmetric())
    {
        // Do upper
        TypeCoeffField& matrixUpper = matrix.upper();

        matrixUpper.zeroOutSubset(startFaceOwn, ownSize);
        TypeField bOwn = lowerCoeffsOwner()*value_;

        // Do lower
        TypeCoeffField& matrixLower = matrix.lower();

        matrixLower.zeroOutSubset(losortAddr);
        TypeField bNbr = upperCoeffsNeighbour()*value_;

        // Owner side
        forAll (bOwn, soI)
        {
            // Add contribution to b of the neighbour (I am the owner)
            b[neighbour[startFaceOwn + soI]] -= bOwn[soI];
        }

        // Neighbour side
        forAll (bNbr, snI)
        {
            // Add contribution to b of owner (I am the neighbour)
            b[owner[losort[startFaceNbr + snI]]] -= bNbr[snI];
        }
    }
}


template<class Type>
void BlockConstraint<Type>::setSourceDiag
(
    BlockLduMatrix<Type>& matrix,
    Field<Type>& x,
    Field<Type>& b
) const
{
    const Type& fc = fixedComponents();

    typename BlockCoeff<Type>::multiply mult;

    if (mag(fc) > SMALL)
    {
        b[rowID()] =
            cmptMultiply
            (
                fc,
                mult(matrix.diag().getCoeff(rowID()), value())
            );

        // Set the solution to the right value as well
        x[rowID()] = cmptMultiply(fc, value());
    }
}


template<class Type>
void BlockConstraint<Type>::reconstructMatrix
(
    BlockLduMatrix<Type>& matrix
) const
{
    if (not matrixCoeffsSet_)
    {
        FatalErrorInFunction
            << "matrix coefficients not set"
            << abort(FatalError);
    }

    if (matrix.thereIsDiag())
    {
        matrix.diag().setCoeff(rowID_, diagCoeff_);
    }

    const label startFaceOwn =
        matrix.lduAddr().ownerStartAddr()[rowID_];
    const label endFaceOwn =
        matrix.lduAddr().ownerStartAddr()[rowID_ + 1];
    const label ownSize = endFaceOwn - startFaceOwn;

    const label startFaceNbr =
        matrix.lduAddr().losortStartAddr()[rowID_];
    const label endFaceNbr =
        matrix.lduAddr().losortStartAddr()[rowID_ + 1];
    const label nbrSize = endFaceNbr - startFaceNbr;

    const unallocLabelList& losort = matrix.lduAddr().losortAddr();

    // Create losort addressing
    labelList losortAddr(nbrSize);

    forAll (losortAddr, laI)
    {
        losortAddr[laI] = losort[startFaceNbr + laI];
    }

    if (matrix.thereIsUpper())
    {
        // Get the upper coefficients
        TypeCoeffField& matrixUpper = matrix.upper();

        // Owner side
        const TypeCoeffField& uOwn = upperCoeffsOwner();

        matrixUpper.setSubset(uOwn, startFaceOwn, ownSize);

        // Neighbour side
        const TypeCoeffField& uNbr = upperCoeffsNeighbour();

        matrixUpper.setSubset(uNbr, losortAddr);
    }

    if (matrix.thereIsLower())
    {
        // Get the lower coefficients
        TypeCoeffField& matrixLower = matrix.lower();

        // Owner side
        const TypeCoeffField& lOwn = lowerCoeffsOwner();

        matrixLower.setSubset(lOwn, startFaceOwn, ownSize);

        // Neighbour side
        const TypeCoeffField& lNbr = lowerCoeffsNeighbour();

        matrixLower.setSubset(lNbr, losortAddr);
    }
}


} // End namespace CML
#endif
