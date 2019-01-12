/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2016 H. Jasak
Copyright (C) 2016 V. Vukcevic
Copyright (C) 2018 Applied CCM
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
    BlockLduMatrix

Description
    BlockLduMatrix is a general matrix class in which the coefficients are
    stored as three arrays, one for the upper triangle, one for the
    lower triangle and a third for the diagonal.  Addressing object must
    be supplied for the upper and lower triangles.

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved.
    Vuko Vukcevic

\*---------------------------------------------------------------------------*/

#ifndef BlockLduMatrix_HPP
#define BlockLduMatrix_HPP

#include "coeffFields.hpp"
#include "FieldField.hpp"
#include "lduMesh.hpp"
#include "HashSet.hpp"
#include "BlockLduInterfaceFieldPtrsList.hpp"
#include "Map.hpp"
#include "error.hpp"
#include "IOstreams.hpp"
#include "demandDrivenData.hpp"
#include "tmp.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class blockLduMatrix Declaration
\*---------------------------------------------------------------------------*/

class blockLduMatrix
{
public:

    ClassName("blockLduMatrix");

    //- Construct null
    blockLduMatrix()
    {}
};


// * * * * * * Forward declaration of template friend fuctions * * * * * * * //

template<class Type>
class BlockLduMatrix;

template<class Type>
Ostream& operator<<(Ostream&, const BlockLduMatrix<Type>&);

template<class Type>
class BlockConstraint;

/*---------------------------------------------------------------------------*\
                        Class BlockLduMatrix Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockLduMatrix
:
    public blockLduMatrix,
    public refCount
{

public:

    typedef CoeffField<Type> TypeCoeffField;
    typedef Field<Type> TypeField;
    typedef BlockConstraint<Type> ConstraintType;


private:

    // LDU mesh reference
    const lduMesh& lduMesh_;

    //- Eliminated equations
    //  Consider adding solo cells on construction.  HJ, 26/Oct/2012
    labelHashSet eliminatedEqns_;

    // Block matrix elements
    //- Diagonal coefficients
    CoeffField<Type>* diagPtr_;

    //- Upper triangle coefficients.  Also used for symmetric matrix
    CoeffField<Type>* upperPtr_;

    //- Lower triangle coefficients
    CoeffField<Type> *lowerPtr_;

    // Coupling
    //- List of coupled interfaces
    typename BlockLduInterfaceFieldPtrsList<Type>::Type interfaces_;

    //- Coupled interface coefficients, upper
    FieldField<CoeffField, Type> coupleUpper_;

    //- Coupled interface coefficients, lower
    FieldField<CoeffField, Type> coupleLower_;

    // Constraints
    //- Equation triangle map
    mutable Map<ConstraintType> fixedEqns_;

    //- Matrix constraint fill-in
    //  Equals to the estimated fraction of fixed nodes in the matrix
    static int fixFillIn;


    // Decoupled versions of nmatrix operations
    //- Sum off-diagonal coefficients and add to diagonal,
    //  decoupled version
    void decoupledSumDiag();

    //- Sum negative off-diagonal coefficients and add to diagonal,
    //  decoupled version
    void decoupledNegSumDiag();

    //- Check matrix for diagonal dominance, decoupled version
    void decoupledCheck() const;

    //- Relax matrix, decoupled version
    void decoupledRelax
    (
        const TypeField& x,
        TypeField& b,
        const scalar alpha
    );

    //- Matrix scaling with scalar field, decoupled version
    void decoupledMultEqOp(const scalarField& sf);

    //- Matrix multiplication without coupled interface update,
    //  Decoupled version
    void decoupledAmulCore
    (
        TypeField& Ax,
        const TypeField& x
    ) const;

    //- Matrix transpose multiplication without coupled
    //  interface update
    //  Decoupled version
    void decoupledTmulCore
    (
        TypeField& Tx,
        const TypeField& x
    ) const;

    //- Return L-U vector-matrix multiplication in row-form,
    //  Decoupled version
    tmp<TypeField> decoupledH(const TypeField& x) const;

    //- Return L-U  vector-matrix multiplication in off-diagonal form,
    //  Decoupled version
    tmp<TypeField> decoupledFaceH(const TypeField& x) const;


protected:

    // Access to constraints
    //- Return constraint map
    const Map<ConstraintType>& fixedEqns() const
    {
        return fixedEqns_;
    }

    //- Return access constraint map
    Map<ConstraintType>& fixedEqns()
    {
        return fixedEqns_;
    }


public:

    //- Construct given addressing
    explicit BlockLduMatrix(const lduMesh&);

    //- Construct as copy
    BlockLduMatrix(const BlockLduMatrix<Type>&);

    //- Construct as copy or re-use as specified.
    BlockLduMatrix(BlockLduMatrix<Type>&, bool reUse);

    virtual ~BlockLduMatrix();

    // Access to addressing
    //- Return the LDU mesh from which the addressing is obtained
    const lduMesh& mesh() const
    {
        return lduMesh_;
    }

    //- Return eliminated equations
    const labelHashSet& eliminatedEqns() const
    {
        return eliminatedEqns_;
    }

    //- Return access to eliminated equations
    labelHashSet& eliminatedEqns()
    {
        return eliminatedEqns_;
    }

    //- Return the LDU addressing
    const lduAddressing& lduAddr() const
    {
        return lduMesh_.lduAddr();
    }

    //- Return the patch evaluation schedule
    const lduSchedule& patchSchedule() const
    {
        return lduAddr().patchSchedule();
    }

    // Access to coefficients
    //- Return access to diagonal coefficients
    TypeCoeffField& diag();

    //- Return diagonal coefficients
    const TypeCoeffField& diag() const;

    //- Return access to upper coefficients
    //  Also used for symmetric matrices
    TypeCoeffField& upper();

    //- Return upper coefficients
    //  Also used for symmetric matrices
    const TypeCoeffField& upper() const;

    //- Return access to lower coefficients
    TypeCoeffField& lower();

    //- Return lower coefficients
    const TypeCoeffField& lower() const;

    //- Return access to coupled interface coefficients, upper
    FieldField<CoeffField, Type>& coupleUpper()
    {
        return coupleUpper_;
    }

    //- Return coupled interface coefficients, upper
    const FieldField<CoeffField, Type>& coupleUpper() const
    {
        return coupleUpper_;
    }

    //- Return access to coupled interface coefficients, lower
    FieldField<CoeffField, Type>& coupleLower()
    {
        return coupleLower_;
    }

    //- Return coupled interface coefficients, lower
    const FieldField<CoeffField, Type>& coupleLower() const
    {
        return coupleLower_;
    }

    //- Return coupled interfaces
    const typename BlockLduInterfaceFieldPtrsList<Type>::Type&
    interfaces() const
    {
        return interfaces_;
    }

    //- Access to coupled interfaces
    typename BlockLduInterfaceFieldPtrsList<Type>::Type& interfaces()
    {
        return interfaces_;
    }

    //- Clear interfaces.  Destructor helper
    void clearInterfaces();

    // Matrix structure
    //- Return true if there is a diagonal
    bool thereIsDiag() const
    {
        return (diagPtr_);
    }

    //- Return true if upper triangle is allocated
    bool thereIsUpper() const
    {
        return (upperPtr_);
    }

    //- Return true if lower triangle is allocated
    bool thereIsLower() const
    {
        return (lowerPtr_);
    }

    //- Return true if matrix is empty
    bool empty() const;

    //- Return true if matrix is diagonal-only
    bool diagonal() const;

    //- Return true if matrix is symmetric
    bool symmetric() const;

    //- Return true if matrix is asymmetric
    bool asymmetric() const;

    //- Return true if matrix is component-coupled
    bool componentCoupled() const;

    // Operations
    //- Sum off-diagonal coefficients and add to diagonal
    void sumDiag();

    //- Sum negative off-diagonal coefficients and add to diagonal
    void negSumDiag();

    //- Check matrix for diagonal dominance
    void check() const;

    //- Relax matrix
    void relax
    (
        const TypeField& x,
        TypeField& b,
        const scalar alpha
    );

/*
    // Initialise the update of interfaced interfaces
    // for matrix operations
    void initMatrixInterfaces
    (
        FieldField<Field, Type> const& interfaceCoeffs,
        typename BlockLduInterfaceFieldPtrsList<Type>::Type const&,
        Field<Type> const& psiif,
        Field<Type>& result
    ) const;

    // Update interfaced interfaces for matrix operations
    void updateMatrixInterfaces
    (
        FieldField<Field, Type> const& interfaceCoeffs,
        BlockLduInterfaceFieldPtrsList<Type> const& interfaces,
        Field<Type> const& psiif,
        Field<Type>& result
    ) const;
*/
    //- Matrix multiplication
    void Amul
    (
        TypeField& Ax,
        const TypeField& x
    ) const;

    //- Matrix multiplication without coupled interface update
    void AmulCore
    (
        TypeField& Ax,
        const TypeField& x
    ) const;

    //- Matrix transpose multiplication
    void Tmul
    (
        TypeField& Ax,
        const TypeField& x
    ) const;

    //- Matrix transpose multiplication without coupled
    //  interface update
    void TmulCore
    (
        TypeField& Ax,
        const TypeField& x
    ) const;


    //- Return decoupled b
    void segregateB
    (
        TypeField& sMul,
        const TypeField& x
    ) const;

    // Coupled interface functionality
    //- Initialise the update of coupled interfaces
    //  for Amul operations
    void initInterfaces
    (
        const FieldField<CoeffField, Type>& interfaceCoeffs,
        TypeField& result,
        const TypeField& psi
    ) const;

    //- Update coupled interfaces
    void updateInterfaces
    (
        const FieldField<CoeffField, Type>& interfaceCoeffs,
        TypeField& result,
        const TypeField& psi
    ) const;

    // Constraint manipulation
    //- Set constrained value in a prescribed point
    void setValue
    (
        const label eqnIndex,
        const Type& value
    );

    // Residual calculation
    //- Calculate residual
    tmp<TypeField> residual
    (
        const TypeField& x
    ) const;

    tmp<TypeField> residual
    (
        const TypeField& x,
        const TypeField& b
    ) const;

    // H-operations
    //- Return L-U vector-matrix multiplication in row-form
    tmp<TypeField> H(const TypeField&) const;

    //- Return L-U  vector-matrix multiplication in off-diagonal form
    tmp<TypeField> faceH(const TypeField&) const;

    // Member operators
    void operator=(const BlockLduMatrix<Type>&);

    void negate();

    void operator+=(const BlockLduMatrix<Type>&);
    void operator-=(const BlockLduMatrix<Type>&);

    void operator*=(const scalarField&);
    void operator*=(const scalar);

    // Ostream operator
    friend Ostream& operator<< <Type>
    (
        Ostream&,
        const BlockLduMatrix<Type>&
    );
};

// BlockLduMatrix.C
// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class Type>
int BlockLduMatrix<Type>::fixFillIn
(
    debug::optimisationSwitch("matrixConstraintFillIn", 4)
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
BlockLduMatrix<Type>::BlockLduMatrix(const lduMesh& ldu)
:
    lduMesh_(ldu),
    diagPtr_(nullptr),
    upperPtr_(nullptr),
    lowerPtr_(nullptr),
    interfaces_(ldu.interfaces().size()),
    coupleUpper_(ldu.lduAddr().nPatches()),
    coupleLower_(ldu.lduAddr().nPatches()),
    fixedEqns_(ldu.lduAddr().size()/fixFillIn)
{
    const lduAddressing& addr = ldu.lduAddr();

    forAll (coupleUpper_, i)
    {
        coupleUpper_.set(i, new CoeffField<Type>(addr.getPatchAddr(i).size()));
        coupleLower_.set(i, new CoeffField<Type>(addr.getPatchAddr(i).size()));
    }
}


template<class Type>
BlockLduMatrix<Type>::BlockLduMatrix(const BlockLduMatrix<Type>& A)
:
    refCount(),
    lduMesh_(A.lduMesh_),
    diagPtr_(nullptr),
    upperPtr_(nullptr),
    lowerPtr_(nullptr),
    interfaces_(A.interfaces_),
    coupleUpper_(A.coupleUpper_),
    coupleLower_(A.coupleLower_),
    fixedEqns_(A.fixedEqns_)
{
    if (A.diagPtr_)
    {
        diagPtr_ = new TypeCoeffField(*(A.diagPtr_));
    }

    if (A.upperPtr_)
    {
        upperPtr_ = new TypeCoeffField(*(A.upperPtr_));
    }

    if (A.lowerPtr_)
    {
        lowerPtr_ = new TypeCoeffField(*(A.lowerPtr_));
    }
}


//HJ, problematic: memory management. Reconsider.  HJ, 7/Nov/2010
template<class Type>
BlockLduMatrix<Type>::BlockLduMatrix(BlockLduMatrix<Type>& A, bool reUse)
:
    refCount(),
    lduMesh_(A.lduMesh_),
    diagPtr_(nullptr),
    upperPtr_(nullptr),
    lowerPtr_(nullptr),
    interfaces_(A.interfaces_, reUse),
    coupleUpper_(A.coupleUpper_, reUse),
    coupleLower_(A.coupleLower_, reUse),
    fixedEqns_(A.fixedEqns_)
{
    if (reUse)
    {
        if (A.lowerPtr_)
        {
            lowerPtr_ = A.lowerPtr_;
            A.lowerPtr_ = nullptr;
        }

        if (A.diagPtr_)
        {
            diagPtr_ = A.diagPtr_;
            A.diagPtr_ = nullptr;
        }

        if (A.upperPtr_)
        {
            upperPtr_ = A.upperPtr_;
            A.upperPtr_ = nullptr;
        }
    }
    else
    {
        if (A.diagPtr_)
        {
            diagPtr_ = new TypeCoeffField(*(A.diagPtr_));
        }

        if (A.upperPtr_)
        {
            upperPtr_ = new TypeCoeffField(*(A.upperPtr_));
        }

        if (A.lowerPtr_)
        {
            lowerPtr_ = new TypeCoeffField(*(A.lowerPtr_));
        }
    }
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
BlockLduMatrix<Type>::~BlockLduMatrix()
{
    deleteDemandDrivenData(diagPtr_);
    deleteDemandDrivenData(upperPtr_);
    deleteDemandDrivenData(lowerPtr_);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
typename CML::BlockLduMatrix<Type>::TypeCoeffField&
BlockLduMatrix<Type>::diag()
{
    if (not diagPtr_)
    {
        diagPtr_ = new TypeCoeffField(lduAddr().size());
    }

    return *diagPtr_;
}


template<class Type>
const typename CML::BlockLduMatrix<Type>::TypeCoeffField&
BlockLduMatrix<Type>::diag() const
{
    if (not diagPtr_)
    {
        FatalErrorInFunction
            << "diagPtr_ unallocated"
            << abort(FatalError);
    }

    return *diagPtr_;
}


template<class Type>
typename CML::BlockLduMatrix<Type>::TypeCoeffField&
BlockLduMatrix<Type>::upper()
{
    if (not upperPtr_)
    {
        upperPtr_ = new TypeCoeffField(lduAddr().lowerAddr().size());
    }

    return *upperPtr_;
}


template<class Type>
const typename CML::BlockLduMatrix<Type>::TypeCoeffField&
BlockLduMatrix<Type>::upper() const
{
    if (not upperPtr_)
    {
        FatalErrorInFunction
            << "upperPtr_ unallocated"
            << abort(FatalError);
    }

    return *upperPtr_;
}


template<class Type>
typename CML::BlockLduMatrix<Type>::TypeCoeffField&
BlockLduMatrix<Type>::lower()
{
    if (not lowerPtr_)
    {
        if (upperPtr_)
        {
            lowerPtr_ = new TypeCoeffField(upperPtr_->transpose());
        }
        else
        {
            lowerPtr_ = new TypeCoeffField(lduAddr().lowerAddr().size());
        }
    }

    return *lowerPtr_;
}


template<class Type>
const typename CML::BlockLduMatrix<Type>::TypeCoeffField&
BlockLduMatrix<Type>::lower() const
{
    if (not lowerPtr_)
    {
        FatalErrorInFunction
            << "lowerPtr_ unallocated"
            << abort(FatalError);
    }

    return *lowerPtr_;
}


template<class Type>
void BlockLduMatrix<Type>::clearInterfaces()
{
    forAll (interfaces_, i)
    {
        if (interfaces_.set(i))
        {
            delete interfaces_(i);
        }
    }
}


template<class Type>
bool BlockLduMatrix<Type>::empty() const
{
    return (not diagPtr_ && not lowerPtr_ && not upperPtr_);
}


template<class Type>
bool BlockLduMatrix<Type>::diagonal() const
{
    return (diagPtr_ && not lowerPtr_ && not upperPtr_);
}


template<class Type>
bool BlockLduMatrix<Type>::symmetric() const
{
    if (lowerPtr_ && not upperPtr_)
    {
        FatalErrorInFunction
            << "Matrix assembly error: symmetric matrix but only lower "
            << "triangle is allocated.  This is not allowed."
            << abort(FatalError);
    }

    // Note:
    // It is possible that the block matrix is symmetric in sparseness
    // pattern, but asymmetric because the diagonal or upper coefficients are
    // square and asymmetric within the coefficients.
    // In such cases, the symmetric check in this function will falsely claim
    // the matrix is symmetries whereas in its flattened nature it is not
    // For the moment, symmetric check is used to see if the lower() is
    // allocated or not.
    // Please reconsider, especially related to matrix structure in
    // complex cases and in choice of linear equation solver for matrices
    // with asymmetric square coefficients
    // HJ and VV, 16/Mar/2016

    return (diagPtr_ && (not lowerPtr_ && upperPtr_));
}


template<class Type>
bool BlockLduMatrix<Type>::asymmetric() const
{
    return (diagPtr_ && lowerPtr_ && upperPtr_);
}


template<class Type>
bool BlockLduMatrix<Type>::componentCoupled() const
{
    // Return true if the matrix coefficient couple the components
    if (thereIsDiag())
    {
        if (diag().activeType() == blockCoeffBase::SQUARE)
        {
            return true;
        }
    }

    if (thereIsUpper())
    {
        if (upper().activeType() == blockCoeffBase::SQUARE)
        {
            return true;
        }
    }

    if (thereIsLower())
    {
        if (lower().activeType() == blockCoeffBase::SQUARE)
        {
            return true;
        }
    }

    return false;
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Type>
Ostream& operator<<(Ostream& os, const BlockLduMatrix<Type>& ldum)
{
    if (ldum.diagPtr_)
    {
        os.writeKeyword("diagonal")
            << *ldum.diagPtr_ <<  token::END_STATEMENT << nl;
    }
    else
    {
        // Dummy write for consistency
        os.writeKeyword("diagonal")
            << typename BlockLduMatrix<Type>::TypeCoeffField
               (
                   ldum.lduAddr().size()
               )
            << token::END_STATEMENT << nl;
    }

    if (ldum.upperPtr_)
    {
        os.writeKeyword("upper")
            << *ldum.upperPtr_
            << token::END_STATEMENT << nl;
    }
    else
    {
        // Dummy write for consistency
        os.writeKeyword("upper")
            << typename BlockLduMatrix<Type>::TypeCoeffField
               (
                   ldum.lduAddr().lowerAddr().size()
               )
            <<  token::END_STATEMENT << nl;
    }

    if (ldum.lowerPtr_)
    {
        os.writeKeyword("lower")
            << *ldum.lowerPtr_ <<  token::END_STATEMENT << nl;
    }
    else
    {
        // Dummy write for consistency
        os.writeKeyword("lower")
            << typename BlockLduMatrix<Type>::TypeCoeffField
               (
                   ldum.lduAddr().lowerAddr().size()
               )
            <<  token::END_STATEMENT << nl;
    }

    os.writeKeyword("coupleUpper")
        << ldum.coupleUpper_
        << token::END_STATEMENT << endl;

    os.writeKeyword("coupleLower")
        << ldum.coupleLower_
        << token::END_STATEMENT << endl;

    os.check("Ostream& operator<<(Ostream&, const BlockLduMatrix<Type>&");

    return os;
}

// BlockLduMatrixOperations.C
// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void BlockLduMatrix<Type>::sumDiag()
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;
    typedef typename TypeCoeffField::squareTypeField squareTypeField;

    TypeCoeffField& Diag = this->diag();

    const unallocLabelList& l = lduAddr().lowerAddr();
    const unallocLabelList& u = lduAddr().upperAddr();

    if (this->symmetric())
    {
        // Symmetric matrix: re-use upper transpose for lower coefficients
        const TypeCoeffField& Upper =
            const_cast<const BlockLduMatrix<Type>&>(*this).upper();

        if
        (
            Upper.activeType() == blockCoeffBase::SQUARE
         || Diag.activeType() == blockCoeffBase::SQUARE
        )
        {
            const squareTypeField& activeUpper = Upper.asSquare();
            squareTypeField& activeDiag = Diag.asSquare();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] += activeUpper[coeffI].T();
                activeDiag[u[coeffI]] += activeUpper[coeffI];
            }
        }
        else if
        (
            Upper.activeType() == blockCoeffBase::LINEAR
         || Diag.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiag = Diag.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] += activeUpper[coeffI];
                activeDiag[u[coeffI]] += activeUpper[coeffI];
            }
        }
        else if
        (
            Upper.activeType() == blockCoeffBase::SCALAR
         || Diag.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiag = Diag.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] += activeUpper[coeffI];
                activeDiag[u[coeffI]] += activeUpper[coeffI];
            }
        }
    }
    else if (this->asymmetric())
    {
        // Full asymmetric matrix
        const TypeCoeffField& Lower =
            const_cast<const BlockLduMatrix<Type>&>(*this).lower();

        const TypeCoeffField& Upper =
            const_cast<const BlockLduMatrix<Type>&>(*this).upper();

        if
        (
            Lower.activeType() == blockCoeffBase::SQUARE
         || Upper.activeType() == blockCoeffBase::SQUARE
         || Diag.activeType() == blockCoeffBase::SQUARE
        )
        {
            const squareTypeField& activeLower = Lower.asSquare();
            const squareTypeField& activeUpper = Upper.asSquare();
            squareTypeField& activeDiag = Diag.asSquare();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] += activeLower[coeffI];
                activeDiag[u[coeffI]] += activeUpper[coeffI];
            }
        }
        else if
        (
            Lower.activeType() == blockCoeffBase::LINEAR
         || Upper.activeType() == blockCoeffBase::LINEAR
         || Diag.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeLower = Lower.asLinear();
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiag = Diag.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] += activeLower[coeffI];
                activeDiag[u[coeffI]] += activeUpper[coeffI];
            }
        }
        else if
        (
            Lower.activeType() == blockCoeffBase::SCALAR
         || Upper.activeType() == blockCoeffBase::SCALAR
         || Diag.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeLower = Lower.asScalar();
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiag = Diag.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] += activeLower[coeffI];
                activeDiag[u[coeffI]] += activeUpper[coeffI];
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "No off-diagonal available"
            << abort(FatalError);
    }
}


template<class Type>
void BlockLduMatrix<Type>::negSumDiag()
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;
    typedef typename TypeCoeffField::squareTypeField squareTypeField;

    TypeCoeffField& Diag = this->diag();

    const unallocLabelList& l = lduAddr().lowerAddr();
    const unallocLabelList& u = lduAddr().upperAddr();

    if (this->symmetric())
    {
        // Symmetric matrix: re-use upper transpose for lower coefficients
        const TypeCoeffField& Upper =
            const_cast<const BlockLduMatrix<Type>&>(*this).upper();

        if
        (
            Upper.activeType() == blockCoeffBase::SQUARE
         || Diag.activeType() == blockCoeffBase::SQUARE
        )
        {
            const squareTypeField& activeUpper = Upper.asSquare();
            squareTypeField& activeDiag = Diag.asSquare();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] -= activeUpper[coeffI].T();
                activeDiag[u[coeffI]] -= activeUpper[coeffI];
            }
        }
        else if
        (
            Upper.activeType() == blockCoeffBase::LINEAR
         || Diag.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiag = Diag.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] -= activeUpper[coeffI];
                activeDiag[u[coeffI]] -= activeUpper[coeffI];
            }
        }
        else if
        (
            Upper.activeType() == blockCoeffBase::SCALAR
         || Diag.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiag = Diag.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] -= activeUpper[coeffI];
                activeDiag[u[coeffI]] -= activeUpper[coeffI];
            }
        }
    }
    else if (this->asymmetric())
    {
        // Full asymmetric matrix
        const TypeCoeffField& Lower =
            const_cast<const BlockLduMatrix<Type>&>(*this).lower();

        const TypeCoeffField& Upper =
            const_cast<const BlockLduMatrix<Type>&>(*this).upper();

        if
        (
            Lower.activeType() == blockCoeffBase::SQUARE
         || Upper.activeType() == blockCoeffBase::SQUARE
         || Diag.activeType() == blockCoeffBase::SQUARE
        )
        {
            const squareTypeField& activeLower = Lower.asSquare();
            const squareTypeField& activeUpper = Upper.asSquare();
            squareTypeField& activeDiag = Diag.asSquare();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] -= activeLower[coeffI];
                activeDiag[u[coeffI]] -= activeUpper[coeffI];
            }
        }
        else if
        (
            Lower.activeType() == blockCoeffBase::LINEAR
         || Upper.activeType() == blockCoeffBase::LINEAR
         || Diag.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeLower = Lower.asLinear();
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiag = Diag.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] -= activeLower[coeffI];
                activeDiag[u[coeffI]] -= activeUpper[coeffI];
            }
        }
        else if
        (
            Lower.activeType() == blockCoeffBase::SCALAR
         || Upper.activeType() == blockCoeffBase::SCALAR
         || Diag.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeLower = Lower.asScalar();
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiag = Diag.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] -= activeLower[coeffI];
                activeDiag[u[coeffI]] -= activeUpper[coeffI];
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "No off-diagonal available"
            << abort(FatalError);
    }
}


template<class Type>
void BlockLduMatrix<Type>::check() const
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;
    typedef typename TypeCoeffField::squareTypeField squareTypeField;

    // TODO: Account for coupled boundary patch coeffs. HJ, 29/Oct/2015

    // Get original diag
    const TypeCoeffField& d = this->diag();

    // Copy the diagonal.  It is initialised to zero
    TypeCoeffField SumOffDiag(d.size());

    const unallocLabelList& l = lduAddr().lowerAddr();
    const unallocLabelList& u = lduAddr().upperAddr();

    if (this->symmetric())
    {
        // Symmetric matrix: re-use upper transpose for lower coefficients
        const TypeCoeffField& Upper = this->upper();

        if
        (
            Upper.activeType() == blockCoeffBase::SQUARE
         || d.activeType() == blockCoeffBase::SQUARE
        )
        {
            // Note: For a square coefficient, the matrix needs to be analysed
            // row-by-row, including the contribution of the off-diagonal
            // elements in the diagonal matrix

            // Get result as linear
            linearTypeField& activeSumOffDiag = SumOffDiag.asLinear();

            // Do diagonal elements

            // Create the diagonal matrix element without the diagonal
            const squareTypeField& activeDiag = d.asSquare();

            linearTypeField diagDiag(activeDiag.size());
            squareTypeField luDiag(activeDiag.size());

            // Take out the diagonal from the diag as a linear type
            contractLinear(diagDiag, activeDiag);

            // Expand diagonal only to full square type and store into luDiag
            expandLinear(luDiag, diagDiag);

            // Keep only off-diagonal in luDiag
            luDiag = activeDiag - luDiag;

            sumMagToDiag(activeSumOffDiag, luDiag);

            // Do the off-diagonal elements by collapsing each row
            // into the linear form
            const squareTypeField& activeUpper = Upper.asSquare();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeSumOffDiag[l[coeffI]] +=
                    sumMagToDiag(activeUpper[coeffI].T());

                activeSumOffDiag[u[coeffI]] +=
                    sumMagToDiag(activeUpper[coeffI]);
            }

            // Check diagonal dominance
            diagDiag = cmptMag(diagDiag);

            // Divide diagonal with sum of off-diagonals
            cmptDivide(diagDiag, diagDiag, activeSumOffDiag);

            InfoInFunction
                << "Symmetric matrix: " << activeDiag.size()
                << " diagonal dominance sym square: "
                << CML::min(diagDiag)
                << endl;
        }
        else if
        (
            Upper.activeType() == blockCoeffBase::LINEAR
         || d.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeDiag = d.asLinear();

            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeSumOffDiag = SumOffDiag.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeSumOffDiag[l[coeffI]] += cmptMag(activeUpper[coeffI]);
                activeSumOffDiag[u[coeffI]] += cmptMag(activeUpper[coeffI]);
            }

            linearTypeField diagDiag = cmptMag(activeDiag);

            cmptDivide(diagDiag, diagDiag, activeSumOffDiag);

            InfoInFunction
                << "Symmetric matrix: " << activeDiag.size()
                << " diagonal dominance sym linear: "
                << CML::min(diagDiag)
                << endl;
        }
        else if
        (
            Upper.activeType() == blockCoeffBase::SCALAR
         || d.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeDiag = d.asScalar();

            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeSumOffDiag = SumOffDiag.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeSumOffDiag[l[coeffI]] += cmptMag(activeUpper[coeffI]);
                activeSumOffDiag[u[coeffI]] += cmptMag(activeUpper[coeffI]);
            }

            InfoInFunction
                << "Symmetric matrix: " << activeDiag.size()
                << " diagonal dominance sym scalar: "
                << CML::min(mag(activeDiag)/activeSumOffDiag)
                << endl;
        }
    }
    else if (this->asymmetric())
    {
        // Full asymmetric matrix
        const TypeCoeffField& Lower = this->lower();
        const TypeCoeffField& Upper = this->upper();

        if
        (
            Lower.activeType() == blockCoeffBase::SQUARE
         || Upper.activeType() == blockCoeffBase::SQUARE
         || d.activeType() == blockCoeffBase::SQUARE
        )
        {
            // Note: For a square coefficient, the matrix needs to be analysed
            // row-by-row, including the contribution of the off-diagonal
            // elements in the diagonal matrix

            // Get result as linear
            linearTypeField& activeSumOffDiag = SumOffDiag.asLinear();

            // Do diagonal elements

            // Create the diagonal matrix element without the diagonal
            const squareTypeField& activeDiag = d.asSquare();

            linearTypeField diagDiag(activeDiag.size());
            squareTypeField luDiag(activeDiag.size());

            // Take out the diagonal from the diag as a linear type
            contractLinear(diagDiag, activeDiag);

            // Expand diagonal only to full square type and store into luDiag
            expandLinear(luDiag, diagDiag);

            // Keep only off-diagonal in luDiag
            luDiag = activeDiag - luDiag;

            sumMagToDiag(activeSumOffDiag, luDiag);

            // Do the off-diagonal elements by collapsing each row
            // into the linear form
            const squareTypeField& activeLower = Lower.asSquare();
            const squareTypeField& activeUpper = Upper.asSquare();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeSumOffDiag[l[coeffI]] +=
                    sumMagToDiag(activeLower[coeffI]);

                activeSumOffDiag[u[coeffI]] +=
                    sumMagToDiag(activeUpper[coeffI]);
            }

            // Check diagonal dominance
            diagDiag = cmptMag(diagDiag);

            // Divide diagonal with sum of off-diagonals
            cmptDivide(diagDiag, diagDiag, activeSumOffDiag);

            InfoInFunction
                << "Asymmetric matrix: " << activeDiag.size()
                << " diagonal dominance assym square: "
                << CML::min(diagDiag)
                << endl;
        }
        else if
        (
            Lower.activeType() == blockCoeffBase::LINEAR
         || Upper.activeType() == blockCoeffBase::LINEAR
         || d.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeDiag = d.asLinear();

            const linearTypeField& activeLower = Lower.asLinear();
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeSumOffDiag = SumOffDiag.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeSumOffDiag[l[coeffI]] += cmptMag(activeLower[coeffI]);
                activeSumOffDiag[u[coeffI]] += cmptMag(activeUpper[coeffI]);
            }

            linearTypeField diagDiag = cmptMag(activeDiag);

            cmptDivide(diagDiag, diagDiag, activeSumOffDiag);

            InfoInFunction
                << "Asymmetric matrix: " << activeDiag.size()
                << " diagonal dominance assym linear: "
                << CML::min(diagDiag)
                << endl;
        }
        else if
        (
            Lower.activeType() == blockCoeffBase::SCALAR
         || Upper.activeType() == blockCoeffBase::SCALAR
         || d.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeDiag = d.asScalar();

            const scalarTypeField& activeLower = Lower.asScalar();
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeSumOffDiag = SumOffDiag.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeSumOffDiag[l[coeffI]] += cmptMag(activeLower[coeffI]);
                activeSumOffDiag[u[coeffI]] += cmptMag(activeUpper[coeffI]);
            }

            InfoInFunction
                << "Asymmetric matrix: "  << activeDiag.size()
                << " diagonal dominance assym scalar: "
                << CML::min(mag(activeDiag)/activeSumOffDiag)
                << endl;
        }
    }
    else
    {
        InfoInFunction
            << "Diagonal matrix" << endl;
    }
}


template<class Type>
void BlockLduMatrix<Type>::relax
(
    const Field<Type>& x,
    Field<Type>& b,
    const scalar alpha
)
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;
    typedef typename TypeCoeffField::squareTypeField squareTypeField;

    //HJ Missing code: add coupling coefficients to under-relaxation.
    //21/Feb/2008

    if (alpha <= 0)
    {
        return;
    }

    TypeCoeffField& Diag = this->diag();

    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    const unallocLabelList& l = lduAddr().lowerAddr();
    const unallocLabelList& u = lduAddr().upperAddr();

    if (this->symmetric())
    {
        // Symmetric matrix: re-use upper transpose for lower coefficients
        const TypeCoeffField& Upper =
            const_cast<const BlockLduMatrix<Type>&>(*this).upper();

        if
        (
            Upper.activeType() == blockCoeffBase::SQUARE
         || Diag.activeType() == blockCoeffBase::SQUARE
        )
        {
            const squareTypeField& activeUpper = Upper.asSquare();
            squareTypeField& activeDiag = Diag.asSquare();

            // Make a copy of diagonal before relaxation
            squareTypeField activeDiagOld = activeDiag;

            // There are options for under-relaxing the block diagonal
            // coefficient.  Currently, the complete diagonal block is
            // under-relaxed.  There's no checking on the off-diag sum

            squareTypeField sumOff
            (
                activeDiag.size(),
                pTraits<typename TypeCoeffField::squareType>::zero
            );

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                sumOff[u[coeffI]] += cmptMag(activeUpper[coeffI].T());
                sumOff[l[coeffI]] += cmptMag(activeUpper[coeffI]);
            }

            // Reconsider under-relaxation of square blocks.
            // HJ, 23/Sep/2011 (2 places)
            activeDiag = max(activeDiag, sumOff);
            activeDiag *= 1.0/alpha;

            // Add the relaxation contribution to b
            forAll (b, i)
            {
                b[i] += mult(activeDiag[i] - activeDiagOld[i], x[i]);
            }
        }
        else if
        (
            Upper.activeType() == blockCoeffBase::LINEAR
         || Diag.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiag = Diag.asLinear();

            // Make a copy of diagonal before relaxation
            linearTypeField activeDiagOld = activeDiag;

            linearTypeField sumOff
            (
                activeDiag.size(),
                pTraits<typename TypeCoeffField::linearType>::zero
            );

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                sumOff[u[coeffI]] += cmptMag(activeUpper[coeffI]);
                sumOff[l[coeffI]] += cmptMag(activeUpper[coeffI]);
            }

            activeDiag = max(activeDiag, sumOff);
            activeDiag *= 1.0/alpha;

            // Add the relaxation contribution to b
            forAll (b, i)
            {
                b[i] += mult(activeDiag[i] - activeDiagOld[i], x[i]);
            }
        }
        else if
        (
            Upper.activeType() == blockCoeffBase::SCALAR
         || Diag.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiag = Diag.asScalar();

            // Make a copy of diagonal before relaxation
            scalarTypeField activeDiagOld = activeDiag;

            scalarTypeField sumOff
            (
                activeDiag.size(),
                pTraits<typename TypeCoeffField::scalarType>::zero
            );

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                sumOff[u[coeffI]] += mag(activeUpper[coeffI]);
                sumOff[l[coeffI]] += mag(activeUpper[coeffI]);
            }

            activeDiag = max(activeDiag, sumOff);
            activeDiag *= 1.0/alpha;

            // Add the relaxation contribution to b
            forAll (b, i)
            {
                b[i] += (activeDiag[i] - activeDiagOld[i])*x[i];
            }
        }
    }
    else if (this->asymmetric())
    {
        // Full asymmetric matrix
        const TypeCoeffField& Lower =
            const_cast<const BlockLduMatrix<Type>&>(*this).lower();

        const TypeCoeffField& Upper =
            const_cast<const BlockLduMatrix<Type>&>(*this).upper();

        if
        (
            Lower.activeType() == blockCoeffBase::SQUARE
         || Upper.activeType() == blockCoeffBase::SQUARE
         || Diag.activeType() == blockCoeffBase::SQUARE
        )
        {
            const squareTypeField& activeLower = Lower.asSquare();
            const squareTypeField& activeUpper = Upper.asSquare();
            squareTypeField& activeDiag = Diag.asSquare();

            // Make a copy of diagonal before relaxation
            squareTypeField activeDiagOld = activeDiag;

            // There are options for under-relaxing the block diagonal
            // coefficient.  Currently, the complete diagonal block is
            // under-relaxed.  There's no checking on the off-diag sum

            squareTypeField sumOff
            (
                activeDiag.size(),
                pTraits<typename TypeCoeffField::squareType>::zero
            );

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                sumOff[u[coeffI]] += cmptMag(activeLower[coeffI]);
                sumOff[l[coeffI]] += cmptMag(activeUpper[coeffI]);
            }

            // Reconsider under-relaxation of square blocks.
            // HJ, 23/Sep/2011 (2 places)
            activeDiag = max(activeDiag, sumOff);
            activeDiag *= 1.0/alpha;

            // Add the relaxation contribution to b
            forAll (b, i)
            {
                b[i] += mult(activeDiag[i] - activeDiagOld[i], x[i]);
            }
        }
        else if
        (
            Lower.activeType() == blockCoeffBase::LINEAR
         || Upper.activeType() == blockCoeffBase::LINEAR
         || Diag.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeLower = Lower.asLinear();
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiag = Diag.asLinear();

            // Make a copy of diagonal before relaxation
            linearTypeField activeDiagOld = activeDiag;

            linearTypeField sumOff
            (
                activeDiag.size(),
                pTraits<typename TypeCoeffField::linearType>::zero
            );

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                sumOff[u[coeffI]] += cmptMag(activeLower[coeffI]);
                sumOff[l[coeffI]] += cmptMag(activeUpper[coeffI]);
            }

            activeDiag = max(activeDiag, sumOff);
            activeDiag *= 1.0/alpha;

            // Add the relaxation contribution to b
            forAll (b, i)
            {
                b[i] += mult(activeDiag[i] - activeDiagOld[i], x[i]);
            }
        }
        else if
        (
            Lower.activeType() == blockCoeffBase::SCALAR
         || Upper.activeType() == blockCoeffBase::SCALAR
         || Diag.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeLower = Lower.asScalar();
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiag = Diag.asScalar();

            // Make a copy of diagonal before relaxation
            scalarTypeField activeDiagOld = activeDiag;

            scalarTypeField sumOff
            (
                activeDiag.size(),
                pTraits<typename TypeCoeffField::scalarType>::zero
            );

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                sumOff[u[coeffI]] += mag(activeLower[coeffI]);
                sumOff[l[coeffI]] += mag(activeUpper[coeffI]);
            }

            activeDiag = max(activeDiag, sumOff);
            activeDiag *= 1.0/alpha;

            // Add the relaxation contribution to b
            forAll (b, i)
            {
                b[i] += activeDiag[i] - activeDiagOld[i]*x[i];
            }
        }
    }
}


template<class Type>
void BlockLduMatrix<Type>::setValue
(
    const label eqnIndex,
    const Type& value
)
{
    BlockConstraint<Type> cp(eqnIndex, value);

    if (not fixedEqns_.found(eqnIndex))
    {
        fixedEqns_.insert(eqnIndex, cp);
    }
    else
    {
        WarningInFunction
            << "Adding constraint on an already constrained point."
            << "  Point: " << eqnIndex << endl;

        fixedEqns_[eqnIndex].combine(cp);
    }
}


template<class Type>
tmp<Field<Type> > BlockLduMatrix<Type>::residual
(
    const Field<Type>& x
) const
{
    Field<Type> Ax(x.size());
    Amul(Ax, x);
    return -Ax;
}


template<class Type>
typename CML::tmp<Field<Type> > BlockLduMatrix<Type>::residual
(
    const Field<Type>& x,
    const Field<Type>& b
) const
{
    return b + residual(x);
}


template<class Type>
void BlockLduMatrix<Type>::negate()
{
    if (lowerPtr_)
    {
        lowerPtr_->negate();
    }

    if (upperPtr_)
    {
        upperPtr_->negate();
    }

    if (diagPtr_)
    {
        diagPtr_->negate();
    }

    // Do coupling coefficients
    coupleUpper_.negate();
    coupleLower_.negate();
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void BlockLduMatrix<Type>::operator=(const BlockLduMatrix<Type>& A)
{
    if (this == &A)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    if (A.lowerPtr_)
    {
        lower() = A.lower();
    }
    else if (lowerPtr_)
    {
        delete lowerPtr_;
        lowerPtr_ = nullptr;
    }

    if (A.upperPtr_)
    {
        upper() = A.upper();
    }
    else if (upperPtr_)
    {
        delete upperPtr_;
        upperPtr_ = nullptr;
    }

    if (A.diagPtr_)
    {
        diag() = A.diag();
    }

    // Copy interface data
    interfaces_ = A.interfaces_;
    coupleUpper_ = A.coupleUpper_;
    coupleLower_ = A.coupleLower_;

    // Copy constraints
    fixedEqns_ = A.fixedEqns_;
}


template<class Type>
void BlockLduMatrix<Type>::operator+=(const BlockLduMatrix<Type>& A)
{
    // Do diagonal first
    if (A.thereIsDiag())
    {
        diag() += A.diag();
    }

    if (A.symmetric())
    {
        upper() += A.upper();

        if (this->asymmetric())
        {
            lower() += A.upper().transpose();
        }
    }

    if (A.asymmetric())
    {
        upper() += A.upper();
        lower() += A.lower();
    }

    // Interface data
    coupleUpper_ += A.coupleUpper_;
    coupleLower_ += A.coupleLower_;
}


template<class Type>
void BlockLduMatrix<Type>::operator-=(const BlockLduMatrix<Type>& A)
{
    // Do diagonal first
    if (A.thereIsDiag())
    {
        diag() -= A.diag();
    }

    if (A.symmetric())
    {
        upper() -= A.upper();

        if (this->asymmetric())
        {
            lower() -= A.upper().transpose();
        }
    }

    if (A.asymmetric())
    {
        upper() -= A.upper();
        lower() -= A.lower();
    }

    // Interface data
    coupleUpper_ -= A.coupleUpper_;
    coupleLower_ -= A.coupleLower_;
}


template<class Type>
void BlockLduMatrix<Type>::operator*=(const scalarField& sf)
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;
    typedef typename TypeCoeffField::squareTypeField squareTypeField;

    //HJ Missing code: add coupling coefficients op*=
    //   HJ, 21/Feb/2008
    // IC - Complicated because we have to scale across the interfaces
    // We may need to include this functionality in lduInterfaceField
    // as additional initInterfaceScale and scaleInterface functions

    if (diagPtr_)
    {
        *diagPtr_ *= sf;
    }

    if (upperPtr_)
    {
        TypeCoeffField& Upper = *upperPtr_;

        const unallocLabelList& l = lduAddr().lowerAddr();

        if (Upper.activeType() == blockCoeffBase::SCALAR)
        {
            scalarTypeField& activeUpper = Upper.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeUpper[coeffI] *= sf[l[coeffI]];
            }
        }
        else if (Upper.activeType() == blockCoeffBase::LINEAR)
        {
            linearTypeField& activeUpper = Upper.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeUpper[coeffI] *= sf[l[coeffI]];
            }
        }
        else if (Upper.activeType() == blockCoeffBase::SQUARE)
        {
            squareTypeField& activeUpper = Upper.asSquare();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeUpper[coeffI] *= sf[l[coeffI]];
            }
        }
    }

    if (lowerPtr_)
    {
        TypeCoeffField& Lower = *lowerPtr_;

        const unallocLabelList& u = lduAddr().upperAddr();

        if (Lower.activeType() == blockCoeffBase::SCALAR)
        {
            scalarTypeField& activeLower = Lower.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                activeLower[coeffI] *= sf[u[coeffI]];
            }
        }
        else if (Lower.activeType() == blockCoeffBase::LINEAR)
        {
            linearTypeField& activeLower = Lower.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                activeLower[coeffI] *= sf[u[coeffI]];
            }
        }
        else if (Lower.activeType() == blockCoeffBase::SQUARE)
        {
            squareTypeField& activeLower = Lower.asSquare();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                activeLower[coeffI] *= sf[u[coeffI]];
            }
        }
    }
}


template<class Type>
void BlockLduMatrix<Type>::operator*=(const scalar s)
{
    if (diagPtr_)
    {
        *diagPtr_ *= s;
    }

    if (upperPtr_)
    {
        *upperPtr_ *= s;
    }

    if (lowerPtr_)
    {
        *lowerPtr_ *= s;
    }

    // Interface data
    coupleUpper_ *= s;
    coupleLower_ *= s;
}


// BlockLduMatrixUpdateInterfaces.C
template<class Type>
void BlockLduMatrix<Type>::initInterfaces
(
    const FieldField<CoeffField, Type>& interfaceCoeffs,
    TypeField& result,
    const TypeField& psi
) const
{
    if
    (
        Pstream::defaultCommsType == Pstream::blocking
     || Pstream::defaultCommsType == Pstream::nonBlocking
    )
    {
        forAll (interfaces_, interfaceI)
        {
            if (interfaces_.set(interfaceI))
            {
                interfaces_[interfaceI].initInterfaceMatrixUpdate
                (
                    result,
                    psi,
                    *this,
                    interfaceCoeffs[interfaceI],
                    Pstream::defaultCommsType
                );
            }
        }
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        const lduSchedule& patchSchedule = this->patchSchedule();

        // Loop over the "global" patches are on the list of interfaces but
        // beyond the end of the schedule which only handles "normal" patches
        for
        (
            label interfaceI = patchSchedule.size()/2;
            interfaceI < interfaces_.size();
            interfaceI++
        )
        {
            if (interfaces_.set(interfaceI))
            {
                interfaces_[interfaceI].initInterfaceMatrixUpdate
                (
                    result,
                    psi,
                    *this,
                    interfaceCoeffs[interfaceI],
                    Pstream::blocking
                );
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "Unsuported communications type "
            << Pstream::commsTypeNames[Pstream::defaultCommsType]
            << exit(FatalError);
    }
}


template<class Type>
void BlockLduMatrix<Type>::updateInterfaces
(
    const FieldField<CoeffField, Type>& interfaceCoeffs,
    TypeField& result,
    const TypeField& psi
) const
{
    if (Pstream::defaultCommsType == Pstream::blocking)
    {
        forAll (interfaces_, interfaceI)
        {
            if (interfaces_.set(interfaceI))
            {
                interfaces_[interfaceI].updateInterfaceMatrix
                (
                    result,
                    psi,
                    *this,
                    interfaceCoeffs[interfaceI],
                    Pstream::defaultCommsType
                );
            }
        }
    }
    else if (Pstream::defaultCommsType == Pstream::nonBlocking)
    {
        // Block until all sends/receives have been finished
        IPstream::waitRequests();
        OPstream::waitRequests();

        forAll (interfaces_, interfaceI)
        {
            if (interfaces_.set(interfaceI))
            {
                interfaces_[interfaceI].updateInterfaceMatrix
                (
                    result,
                    psi,
                    *this,
                    interfaceCoeffs[interfaceI],
                    Pstream::defaultCommsType
                );
            }
        }
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        const lduSchedule& patchSchedule = this->patchSchedule();

        // Loop over all the "normal" interfaces relating to standard patches
        forAll (patchSchedule, i)
        {
            label interfaceI = patchSchedule[i].patch;

            if (interfaces_.set(interfaceI))
            {
                if (patchSchedule[i].init)
                {
                    interfaces_[interfaceI].initInterfaceMatrixUpdate
                    (
                        result,
                        psi,
                        *this,
                        interfaceCoeffs[interfaceI],
                        Pstream::scheduled
                    );
                }
                else
                {
                    interfaces_[interfaceI].updateInterfaceMatrix
                    (
                        result,
                        psi,
                        *this,
                        interfaceCoeffs[interfaceI],
                        Pstream::scheduled
                    );
                }
            }
        }

        // Loop over the "global" patches are on the list of interfaces but
        // beyond the end of the schedule which only handles "normal" patches
        for
        (
            label interfaceI = patchSchedule.size()/2;
            interfaceI < interfaces_.size();
            interfaceI++
        )
        {
            if (interfaces_.set(interfaceI))
            {
                interfaces_[interfaceI].updateInterfaceMatrix
                (
                    result,
                    psi,
                    *this,
                    interfaceCoeffs[interfaceI],
                    Pstream::blocking
                );
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "Unsuported communications type "
            << Pstream::commsTypeNames[Pstream::defaultCommsType]
            << exit(FatalError);
    }
}

/*
template <typename Type>
void
CML::BlockLduMatrix<Type>::updateMatrixInterfaces
(
    FieldField<Field, Type> const& coupleCoeffs,
    BlockLduInterfaceFieldPtrsList<Type> const& interfaces,
    Field<Type> const& psiif,
    Field<Type>& result
) const
{
    if (Pstream::defaultCommsType == Pstream::blocking)
    {
        forAll(interfaces, interfaceI)
        {
            if (interfaces.set(interfaceI))
            {
                interfaces[interfaceI].updateInterfaceMatrix
                (
                    result,
                    psiif,
                    coupleCoeffs[interfaceI],
                    Pstream::defaultCommsType
                );
            }
        }
    }
    else if (Pstream::defaultCommsType == Pstream::nonBlocking)
    {
        // Try and consume interfaces as they become available
        bool allUpdated = false;

        for (label i = 0; i < UPstream::nPollProcInterfaces; i++)
        {
            allUpdated = true;

            forAll(interfaces, interfaceI)
            {
                if (interfaces.set(interfaceI))
                {
                    if (not interfaces[interfaceI].updatedMatrix())
                    {
                        if (interfaces[interfaceI].ready())
                        {
                            interfaces[interfaceI].updateInterfaceMatrix
                            (
                                result,
                                psiif,
                                coupleCoeffs[interfaceI],
                                Pstream::defaultCommsType
                            );
                        }
                        else
                        {
                            allUpdated = false;
                        }
                    }
                }
            }

            if (allUpdated)
            {
                break;
            }
        }

        // Block for everything
        if (Pstream::parRun())
        {
            if (allUpdated)
            {
                // All received. Just remove all storage of requests
                // Note that we don't know what starting number of requests
                // was before start of sends and receives (since set from
                // initMatrixInterfaces) so set to 0 and loose any in-flight
                // requests.
                UPstream::resetRequests(0);
            }
            else
            {
                // Block for all requests and remove storage
                UPstream::waitRequests();
            }
        }

        // Consume
        forAll(interfaces, interfaceI)
        {
            if
            (
                interfaces.set(interfaceI)
            && not interfaces[interfaceI].updatedMatrix()
            )
            {
                interfaces[interfaceI].updateInterfaceMatrix
                (
                    result,
                    psiif,
                    coupleCoeffs[interfaceI],
                    Pstream::defaultCommsType
                );
            }
        }
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        const lduSchedule& patchSchedule = this->patchSchedule();

        // Loop over all the "normal" interfaces relating to standard patches
        forAll(patchSchedule, i)
        {
            label interfaceI = patchSchedule[i].patch;

            if (interfaces.set(interfaceI))
            {
                if (patchSchedule[i].init)
                {
                    interfaces[interfaceI].initInterfaceMatrixUpdate
                    (
                        result,
                        psiif,
                        coupleCoeffs[interfaceI],
                        Pstream::scheduled
                    );
                }
                else
                {
                    interfaces[interfaceI].updateInterfaceMatrix
                    (
                        result,
                        psiif,
                        coupleCoeffs[interfaceI],
                        Pstream::scheduled
                    );
                }
            }
        }

        // Loop over the "global" patches are on the list of interfaces but
        // beyond the end of the schedule which only handles "normal" patches
        for
        (
            label interfaceI=patchSchedule.size()/2;
            interfaceI<interfaces.size();
            interfaceI++
        )
        {
            if (interfaces.set(interfaceI))
            {
                interfaces[interfaceI].updateInterfaceMatrix
                (
                    result,
                    psiif,
                    coupleCoeffs[interfaceI],
                    Pstream::blocking
                );
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "Unsuported communications type "
            << Pstream::commsTypeNames[Pstream::defaultCommsType]
            << exit(FatalError);
    }
}
*/
/*
template <typename Type>
void
BlockLduMatrix<Type>::initMatrixInterfaces
(
    FieldField<Field, Type> const& coupleCoeffs,
    typename BlockLduInterfaceFieldPtrsList<Type>::Type const& interfaces,
    // BlockLduInterfaceFieldPtrsList<Type> const& interfaces,
    Field<Type> const& psiif,
    Field<Type>& result
) const
{
    if
    (
        Pstream::defaultCommsType == Pstream::blocking
     || Pstream::defaultCommsType == Pstream::nonBlocking
    )
    {
        forAll(interfaces, interfaceI)
        {
            if (interfaces.set(interfaceI))
            {
                interfaces[interfaceI].initInterfaceMatrixUpdate
                (
                    result,
                    psiif,
                    coupleCoeffs[interfaceI],
                    Pstream::defaultCommsType
                );
            }
        }
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        const lduSchedule& patchSchedule = this->patchSchedule();

        // Loop over the "global" patches are on the list of interfaces but
        // beyond the end of the schedule which only handles "normal" patches
        for
        (
            label interfaceI=patchSchedule.size()/2;
            interfaceI<interfaces.size();
            interfaceI++
        )
        {
            if (interfaces.set(interfaceI))
            {
                interfaces[interfaceI].initInterfaceMatrixUpdate
                (
                    result,
                    psiif,
                    coupleCoeffs[interfaceI],
                    Pstream::blocking
                );
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "Unsuported communications type "
            << Pstream::commsTypeNames[Pstream::defaultCommsType]
            << exit(FatalError);
    }
}
*/

// BlockLduMatrixATmul.C
template<class Type>
void
BlockLduMatrix<Type>::Amul
(
    TypeField& Ax,
    const TypeField& x
) const
{
    Ax = pTraits<Type>::zero;

    // Note: changed order of interface update: init after core. HJ, 14/Mar/2016
    AmulCore(Ax, x);

    // Initialise the update of coupled interfaces
    initInterfaces(coupleUpper_, Ax, x);

    // Update coupled interfaces
    updateInterfaces(coupleUpper_, Ax, x);
}


template<class Type>
void BlockLduMatrix<Type>::AmulCore
(
    TypeField& Ax,
    const TypeField& x
) const
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;
    typedef typename TypeCoeffField::squareTypeField squareTypeField;

    const unallocLabelList& u = lduAddr().upperAddr();
    const unallocLabelList& l = lduAddr().lowerAddr();

    const TypeCoeffField& Diag = this->diag();
    const TypeCoeffField& Upper = this->upper();

    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    // Diagonal multiplication, no indirection
    multiply(Ax, Diag, x);

    // Lower multiplication
    {
        const TypeCoeffField& Lower = this->lower();

        if (Lower.activeType() == blockCoeffBase::SCALAR)
        {
            const scalarTypeField& activeLower = Lower.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Ax[u[coeffI]] += mult(activeLower[coeffI], x[l[coeffI]]);
            }
        }
        else if (Lower.activeType() == blockCoeffBase::LINEAR)
        {
            const linearTypeField& activeLower = Lower.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Ax[u[coeffI]] += mult(activeLower[coeffI], x[l[coeffI]]);
            }
        }
        else if (Lower.activeType() == blockCoeffBase::SQUARE)
        {
            const squareTypeField& activeLower = Lower.asSquare();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Ax[u[coeffI]] += mult(activeLower[coeffI], x[l[coeffI]]);
            }
        }
    }

    // Upper multiplication
    if (Upper.activeType() == blockCoeffBase::SCALAR)
    {
        const scalarTypeField& activeUpper = Upper.asScalar();

        for (register label coeffI = 0; coeffI < u.size(); coeffI++)
        {
            Ax[l[coeffI]] += mult(activeUpper[coeffI], x[u[coeffI]]);
        }
    }
    else if (Upper.activeType() == blockCoeffBase::LINEAR)
    {
        const linearTypeField& activeUpper = Upper.asLinear();

        for (register label coeffI = 0; coeffI < u.size(); coeffI++)
        {
            Ax[l[coeffI]] += mult(activeUpper[coeffI], x[u[coeffI]]);
        }
    }
    else if (Upper.activeType() == blockCoeffBase::SQUARE)
    {
        const squareTypeField& activeUpper = Upper.asSquare();

        for (register label coeffI = 0; coeffI < u.size(); coeffI++)
        {
            Ax[l[coeffI]] += mult(activeUpper[coeffI], x[u[coeffI]]);
        }
    }
}


template<class Type>
void BlockLduMatrix<Type>::Tmul
(
    TypeField& Ax,
    const TypeField& x
) const
{
    Ax = pTraits<Type>::zero;

    TmulCore(Ax, x);

    // Note: changed order of interface update: init after core. HJ, 14/Mar/2016

    // Initialise the update of coupled interfaces
    initInterfaces(coupleLower_, Ax, x);

    // Update coupled interfaces
    updateInterfaces(coupleLower_, Ax, x);
}


template<class Type>
void BlockLduMatrix<Type>::TmulCore
(
    TypeField& Tx,
    const TypeField& x
) const
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;
    typedef typename TypeCoeffField::squareTypeField squareTypeField;

    const unallocLabelList& u = lduAddr().upperAddr();
    const unallocLabelList& l = lduAddr().lowerAddr();

    const TypeCoeffField& Diag = this->diag();
    const TypeCoeffField& Upper = this->upper();

    // Diagonal multiplication, no indirection
    multiply(Tx, Diag, x);

    // Upper multiplication
    if (Upper.activeType() == blockCoeffBase::SCALAR)
    {
        const scalarTypeField& activeUpper = Upper.asScalar();

        for (register label coeffI = 0; coeffI < u.size(); coeffI++)
        {
            Tx[u[coeffI]] += mult(activeUpper[coeffI], x[l[coeffI]]);
        }
    }
    else if (Upper.activeType() == blockCoeffBase::LINEAR)
    {
        const linearTypeField& activeUpper = Upper.asLinear();

        for (register label coeffI = 0; coeffI < u.size(); coeffI++)
        {
            Tx[u[coeffI]] += mult(activeUpper[coeffI], x[l[coeffI]]);
        }
    }
    else if (Upper.activeType() == blockCoeffBase::SQUARE)
    {
        const squareTypeField& activeUpper = Upper.asSquare();

        for (register label coeffI = 0; coeffI < u.size(); coeffI++)
        {
            // Bug fix: Missing transpose. VV, 31/Aug/2015.
            Tx[u[coeffI]] += mult(activeUpper[coeffI].T(), x[l[coeffI]]);
        }
    }

    // Lower multiplication
    if (symmetric())
    {
        if (Upper.activeType() == blockCoeffBase::SCALAR)
        {
            const scalarTypeField& activeUpper = Upper.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Tx[l[coeffI]] += mult(activeUpper[coeffI], x[u[coeffI]]);
            }
        }
        else if (Upper.activeType() == blockCoeffBase::LINEAR)
        {
            const linearTypeField& activeUpper = Upper.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Tx[l[coeffI]] += mult(activeUpper[coeffI], x[u[coeffI]]);
            }
        }
        else if (Upper.activeType() == blockCoeffBase::SQUARE)
        {
            const squareTypeField& activeUpper = Upper.asSquare();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                // Use transpose upper coefficient
                Tx[l[coeffI]] +=
                    mult(activeUpper[coeffI].T(), x[u[coeffI]]);
            }
        }
    }
    else // Asymmetric matrix
    {
        const TypeCoeffField& Lower = this->lower();

        if (Lower.activeType() == blockCoeffBase::SCALAR)
        {
            const scalarTypeField& activeLower = Lower.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Tx[l[coeffI]] += mult(activeLower[coeffI], x[u[coeffI]]);
            }
        }
        else if (Lower.activeType() == blockCoeffBase::LINEAR)
        {
            const linearTypeField& activeLower = Lower.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Tx[l[coeffI]] += mult(activeLower[coeffI], x[u[coeffI]]);
            }
        }
        else if (Lower.activeType() == blockCoeffBase::SQUARE)
        {
            const squareTypeField& activeLower = Lower.asSquare();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                // Bug fix: Missing transpose. VV, 31/Aug/2015.
                Tx[l[coeffI]] += mult(activeLower[coeffI].T(), x[u[coeffI]]);
            }
        }
    }
}


template<class Type>
void BlockLduMatrix<Type>::segregateB
(
    TypeField& sMul,
    const TypeField& x
) const
{
    typedef typename TypeCoeffField::linearType linearType;
    typedef typename TypeCoeffField::squareType squareType;

    typedef typename TypeCoeffField::linearTypeField linearTypeField;
    typedef typename TypeCoeffField::squareTypeField squareTypeField;

    const unallocLabelList& u = lduAddr().upperAddr();
    const unallocLabelList& l = lduAddr().lowerAddr();

    // Diagonal multiplication
    if (thereIsDiag())
    {
        if (diag().activeType() == blockCoeffBase::SQUARE)
        {
            const squareTypeField& activeDiag = this->diag().asSquare();
            linearTypeField lf(activeDiag.size());
            squareTypeField sf(activeDiag.size());

            // Expand and contract
            contractLinear(lf, activeDiag);
            expandLinear(sf, lf);

            sMul -= (activeDiag - sf) & x;
        }
    }

    // Lower multiplication
    if (thereIsLower())
    {
        if (lower().activeType() == blockCoeffBase::SQUARE)
        {
            const squareTypeField& activeLower = this->lower().asSquare();

            // Auxiliary variables used in expand/contract
            linearType lt;
            squareType st;

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                contractLinear(lt, activeLower[coeffI]);
                expandLinear(st, lt);

                sMul[u[coeffI]] -= (activeLower[coeffI] - st) & x[l[coeffI]];
            }
        }
    }

    // Upper multiplication
    if (thereIsUpper())
    {
        if (upper().activeType() == blockCoeffBase::SQUARE)
        {
            const squareTypeField& activeUpper = this->upper().asSquare();

            // Auxiliary variables used in expand/contract
            linearType lt;
            squareType st;

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                contractLinear(lt, activeUpper[coeffI]);
                expandLinear(st, lt);

                sMul[l[coeffI]] -= (activeUpper[coeffI] - st) & x[u[coeffI]];
            }

            // If the matrix is symmetric, the lower triangular product
            // is also needed
            if (symmetric())
            {
                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    // Use transpose upper coefficient
                    contractLinear(lt, activeUpper[coeffI]);
                    expandLinear(st, lt);
                    sMul[u[coeffI]] -=
                        (activeUpper[coeffI].T() - st) & x[l[coeffI]];
                }
            }
        }
    }
}


// BlockLduMatrixHOps.C
template<class Type>
tmp<Field<Type> >
BlockLduMatrix<Type>::H(const Field<Type>& x) const
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;
    typedef typename TypeCoeffField::squareTypeField squareTypeField;

    // Create result
    tmp<Field<Type> > tresult
    (
        new Field<Type>(lduAddr().size(), pTraits<Type>::zero)
    );
    Field<Type>& result = tresult();

    if (this->thereIsUpper())
    {
        const unallocLabelList& l = lduAddr().lowerAddr();
        const unallocLabelList& u = lduAddr().upperAddr();
        const TypeCoeffField& Upper = this->upper();

        // Create multiplication function object
        typename BlockCoeff<Type>::multiply mult;

        // Lower multiplication
        if (symmetric())
        {
            if (Upper.activeType() == blockCoeffBase::SCALAR)
            {
                const scalarTypeField& activeUpper = Upper.asScalar();

                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    result[u[coeffI]] -=
                        mult(activeUpper[coeffI], x[l[coeffI]]);
                }
            }
            else if (Upper.activeType() == blockCoeffBase::LINEAR)
            {
                const linearTypeField& activeUpper = Upper.asLinear();

                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    result[u[coeffI]] -=
                        mult(activeUpper[coeffI], x[l[coeffI]]);
                }
            }
            else if (Upper.activeType() == blockCoeffBase::SQUARE)
            {
                const squareTypeField& activeUpper = Upper.asSquare();

                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    // Use transpose upper coefficient
                    result[u[coeffI]] -=
                        mult(activeUpper[coeffI].T(), x[l[coeffI]]);
                }
            }
        }
        else // Asymmetric matrix
        {
            const TypeCoeffField& Lower = this->lower();

            if (Lower.activeType() == blockCoeffBase::SCALAR)
            {
                const scalarTypeField& activeLower = Lower.asScalar();

                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    result[u[coeffI]] -=
                        mult(activeLower[coeffI], x[l[coeffI]]);
                }
            }
            else if (Lower.activeType() == blockCoeffBase::LINEAR)
            {
                const linearTypeField& activeLower = Lower.asLinear();

                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    result[u[coeffI]] -=
                        mult(activeLower[coeffI], x[l[coeffI]]);
                }
            }
            else if (Lower.activeType() == blockCoeffBase::SQUARE)
            {
                const squareTypeField& activeLower = Lower.asSquare();

                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    result[u[coeffI]] -=
                        mult(activeLower[coeffI], x[l[coeffI]]);
                }
            }
        }

        // Upper multiplication
        if (Upper.activeType() == blockCoeffBase::SCALAR)
        {
            const scalarTypeField& activeUpper = Upper.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                result[l[coeffI]] -= mult(activeUpper[coeffI], x[u[coeffI]]);
            }
        }
        else if (Upper.activeType() == blockCoeffBase::LINEAR)
        {
            const linearTypeField& activeUpper = Upper.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                result[l[coeffI]] -= mult(activeUpper[coeffI], x[u[coeffI]]);
            }
        }
        else if (Upper.activeType() == blockCoeffBase::SQUARE)
        {
            const squareTypeField& activeUpper = Upper.asSquare();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                result[l[coeffI]] -= mult(activeUpper[coeffI], x[u[coeffI]]);
            }
        }
    }

    return tresult;
}


template<class Type>
tmp<Field<Type> >
BlockLduMatrix<Type>::faceH(const Field<Type>& x) const
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;
    typedef typename TypeCoeffField::squareTypeField squareTypeField;

    const unallocLabelList& u = lduAddr().upperAddr();
    const unallocLabelList& l = lduAddr().lowerAddr();

    // Create result
    tmp<Field<Type> > tresult(new Field<Type>(u.size(), pTraits<Type>::zero));
    Field<Type>& result = tresult();

    if (this->thereIsUpper())
    {
        const TypeCoeffField& Upper = this->upper();

        // Create multiplication function object
        typename BlockCoeff<Type>::multiply mult;

        // Lower multiplication
        if (symmetric())
        {
            if (Upper.activeType() == blockCoeffBase::SCALAR)
            {
                const scalarTypeField& activeUpper = Upper.asScalar();

                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    // This can be optimised with a subtraction.
                    // Currently not done for clarity.  HJ, 31/Oct/2007
                    result[coeffI] =
                        mult(activeUpper[coeffI], x[u[coeffI]])
                    - mult(activeUpper[coeffI], x[l[coeffI]]);
                }
            }
            else if (Upper.activeType() == blockCoeffBase::LINEAR)
            {
                const linearTypeField& activeUpper = Upper.asLinear();

                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    // This can be optimised with a subtraction.
                    // Currently not done for clarity.  HJ, 31/Oct/2007
                    result[coeffI] =
                        mult(activeUpper[coeffI], x[u[coeffI]])
                    - mult(activeUpper[coeffI], x[l[coeffI]]);
                }
            }
            else if (Upper.activeType() == blockCoeffBase::SQUARE)
            {
                const squareTypeField& activeUpper = Upper.asSquare();

                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    // Use transpose upper coefficient
                    result[coeffI] =
                        mult(activeUpper[coeffI], x[u[coeffI]])
                    - mult(activeUpper[coeffI].T(), x[l[coeffI]]);
                }
            }
        }
        else // Asymmetric matrix
        {
            const TypeCoeffField& Lower = this->lower();

            if (Lower.activeType() == blockCoeffBase::SCALAR)
            {
                const scalarTypeField& activeUpper = Upper.asScalar();
                const scalarTypeField& activeLower = Lower.asScalar();

                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    result[coeffI] =
                        mult(activeUpper[coeffI], x[u[coeffI]])
                    - mult(activeLower[coeffI], x[l[coeffI]]);
                }
            }
            else if (Lower.activeType() == blockCoeffBase::LINEAR)
            {
                const linearTypeField& activeUpper = Upper.asLinear();
                const linearTypeField& activeLower = Lower.asLinear();

                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    result[coeffI] =
                        mult(activeUpper[coeffI], x[u[coeffI]])
                    - mult(activeLower[coeffI], x[l[coeffI]]);
                }
            }
            else if (Lower.activeType() == blockCoeffBase::SQUARE)
            {
                const squareTypeField& activeUpper = Upper.asSquare();
                const squareTypeField& activeLower = Lower.asSquare();

                for (register label coeffI = 0; coeffI < u.size(); coeffI++)
                {
                    result[coeffI] =
                        mult(activeUpper[coeffI], x[u[coeffI]])
                    - mult(activeLower[coeffI], x[l[coeffI]]);
                }
            }
        }
    }

    return tresult;
}


// BlockLduMatrixDecouple.C
template<class Type>
void BlockLduMatrix<Type>::decoupledSumDiag()
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;

    TypeCoeffField& Diag = this->diag();

    const unallocLabelList& l = lduAddr().lowerAddr();
    const unallocLabelList& u = lduAddr().upperAddr();

    if (this->symmetric())
    {
        // Symmetric matrix: re-use upper for lower coefficients
        const TypeCoeffField& Upper =
            const_cast<const BlockLduMatrix<Type>&>(*this).upper();

        if
        (
            Upper.activeType() == blockCoeffBase::LINEAR
         || Diag.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiag = Diag.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] += activeUpper[coeffI];
                activeDiag[u[coeffI]] += activeUpper[coeffI];
            }
        }
        else if
        (
            Upper.activeType() == blockCoeffBase::SCALAR
         || Diag.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiag = Diag.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] += activeUpper[coeffI];
                activeDiag[u[coeffI]] += activeUpper[coeffI];
            }
        }
    }
    else if (this->asymmetric())
    {
        // Full asymmetric matrix
        const TypeCoeffField& Lower =
            const_cast<const BlockLduMatrix<Type>&>(*this).lower();

        const TypeCoeffField& Upper =
            const_cast<const BlockLduMatrix<Type>&>(*this).upper();

        if
        (
            Lower.activeType() == blockCoeffBase::LINEAR
         || Upper.activeType() == blockCoeffBase::LINEAR
         || Diag.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeLower = Lower.asLinear();
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiag = Diag.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] += activeLower[coeffI];
                activeDiag[u[coeffI]] += activeUpper[coeffI];
            }
        }
        else if
        (
            Lower.activeType() == blockCoeffBase::SCALAR
         || Upper.activeType() == blockCoeffBase::SCALAR
         || Diag.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeLower = Lower.asScalar();
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiag = Diag.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] += activeLower[coeffI];
                activeDiag[u[coeffI]] += activeUpper[coeffI];
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "No off-diagonal available"
            << abort(FatalError);
    }
}


template<class Type>
void BlockLduMatrix<Type>::decoupledNegSumDiag()
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;

    TypeCoeffField& Diag = this->diag();

    const unallocLabelList& l = lduAddr().lowerAddr();
    const unallocLabelList& u = lduAddr().upperAddr();

    if (this->symmetric())
    {
        // Symmetric matrix: re-use upper for lower coefficients
        const TypeCoeffField& Upper =
            const_cast<const BlockLduMatrix<Type>&>(*this).upper();

        if
        (
            Upper.activeType() == blockCoeffBase::LINEAR
         || Diag.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiag = Diag.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] -= activeUpper[coeffI];
                activeDiag[u[coeffI]] -= activeUpper[coeffI];
            }
        }
        else if
        (
            Upper.activeType() == blockCoeffBase::SCALAR
         || Diag.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiag = Diag.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] -= activeUpper[coeffI];
                activeDiag[u[coeffI]] -= activeUpper[coeffI];
            }
        }
    }
    else if (this->asymmetric())
    {
        // Full asymmetric matrix
        const TypeCoeffField& Lower =
            const_cast<const BlockLduMatrix<Type>&>(*this).lower();

        const TypeCoeffField& Upper =
            const_cast<const BlockLduMatrix<Type>&>(*this).upper();

        if
        (
            Lower.activeType() == blockCoeffBase::LINEAR
         || Upper.activeType() == blockCoeffBase::LINEAR
         || Diag.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeLower = Lower.asLinear();
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiag = Diag.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] -= activeLower[coeffI];
                activeDiag[u[coeffI]] -= activeUpper[coeffI];
            }
        }
        else if
        (
            Lower.activeType() == blockCoeffBase::SCALAR
         || Upper.activeType() == blockCoeffBase::SCALAR
         || Diag.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeLower = Lower.asScalar();
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiag = Diag.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiag[l[coeffI]] -= activeLower[coeffI];
                activeDiag[u[coeffI]] -= activeUpper[coeffI];
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "No off-diagonal available"
            << abort(FatalError);
    }
}


template<class Type>
void BlockLduMatrix<Type>::decoupledCheck() const
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;

    // Copy the diagonal
    TypeCoeffField DiagCopy(this->diag().size());

    const unallocLabelList& l = lduAddr().lowerAddr();
    const unallocLabelList& u = lduAddr().upperAddr();

    if (this->symmetric())
    {
        // Symmetric matrix: re-use upper for lower coefficients
        const TypeCoeffField& Upper = this->upper();

        if
        (
            Upper.activeType() == blockCoeffBase::LINEAR
         || DiagCopy.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiagCopy = DiagCopy.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiagCopy[l[coeffI]] += activeUpper[coeffI];
                activeDiagCopy[u[coeffI]] += activeUpper[coeffI];
            }

            Info<< "void BlockLduMatrix<Type>::decoupledCheck() const : "
                << "Symmetric matrix: raw matrix difference: "
                << sum(mag(activeDiagCopy))
                << " scaled: "
                << sum(mag(activeDiagCopy))/sum(mag(this->diag().asLinear()))
                << endl;
        }
        else if
        (
            Upper.activeType() == blockCoeffBase::SCALAR
         || DiagCopy.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiagCopy = DiagCopy.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiagCopy[l[coeffI]] += activeUpper[coeffI];
                activeDiagCopy[u[coeffI]] += activeUpper[coeffI];
            }

            Info<< "void BlockLduMatrix<Type>::decoupledCheck() const : "
                << "Symmetric matrix: raw matrix difference: "
                << sum(mag(activeDiagCopy))
                << " scaled: "
                << sum(mag(activeDiagCopy))/sum(mag(this->diag().asScalar()))
                << endl;
        }
    }
    else if (this->asymmetric())
    {
        // Full asymmetric matrix
        const TypeCoeffField& Lower = this->lower();
        const TypeCoeffField& Upper = this->upper();

        if
        (
            Lower.activeType() == blockCoeffBase::LINEAR
         || Upper.activeType() == blockCoeffBase::LINEAR
         || DiagCopy.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeLower = Lower.asLinear();
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiagCopy = DiagCopy.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiagCopy[l[coeffI]] += activeLower[coeffI];
                activeDiagCopy[u[coeffI]] += activeUpper[coeffI];
            }

            Info<< "void BlockLduMatrix<Type>::decoupledCheck() const : "
                << "Asymmetric matrix: raw matrix difference: "
                << sum(mag(activeDiagCopy))
                << " scaled: "
                << sum(mag(activeDiagCopy))/sum(mag(this->diag().asLinear()))
                << endl;
        }
        else if
        (
            Lower.activeType() == blockCoeffBase::SCALAR
         || Upper.activeType() == blockCoeffBase::SCALAR
         || DiagCopy.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeLower = Lower.asScalar();
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiagCopy = DiagCopy.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeDiagCopy[l[coeffI]] += activeLower[coeffI];
                activeDiagCopy[u[coeffI]] += activeUpper[coeffI];
            }

            Info<< "void BlockLduMatrix<Type>::decoupledCheck() const : "
                << "Asymmetric matrix: raw matrix difference: "
                << sum(mag(activeDiagCopy))
                << " scaled: "
                << sum(mag(activeDiagCopy))/sum(mag(this->diag().asScalar()))
                << endl;
        }
    }
    else
    {
        Info<< "void BlockLduMatrix<Type>::decoupledCheck() const : "
            << "Diagonal matrix" << endl;
    }
}


template<class Type>
void BlockLduMatrix<Type>::decoupledRelax
(
    const TypeField& x,
    TypeField& b,
    const scalar alpha
)
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;

    //HJ Missing code: add coupling coefficients to under-relaxation.
    //21/Feb/2008

    if (alpha <= 0)
    {
        return;
    }

    TypeCoeffField& Diag = this->diag();

    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    const unallocLabelList& l = lduAddr().lowerAddr();
    const unallocLabelList& u = lduAddr().upperAddr();

    if (this->symmetric())
    {
        // Symmetric matrix: re-use upper for lower coefficients
        const TypeCoeffField& Upper =
            const_cast<const BlockLduMatrix<Type>&>(*this).upper();

        if
        (
            Upper.activeType() == blockCoeffBase::LINEAR
         || Diag.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiag = Diag.asLinear();

            // Make a copy of diagonal before relaxation
            linearTypeField activeDiagOld = activeDiag;

            linearTypeField sumOff
            (
                activeDiag.size(),
                pTraits<typename TypeCoeffField::linearType>::zero
            );

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                sumOff[u[coeffI]] += cmptMag(activeUpper[coeffI]);
                sumOff[l[coeffI]] += cmptMag(activeUpper[coeffI]);
            }

            activeDiag = max(activeDiag, sumOff);
            activeDiag *= 1.0/alpha;

            // Add the relaxation contribution to b
            forAll (b, i)
            {
                b[i] += mult(activeDiag[i] - activeDiagOld[i], x[i]);
            }
        }
        else if
        (
            Upper.activeType() == blockCoeffBase::SCALAR
         || Diag.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiag = Diag.asScalar();

            // Make a copy of diagonal before relaxation
            scalarTypeField activeDiagOld = activeDiag;

            scalarTypeField sumOff
            (
                activeDiag.size(),
                pTraits<typename TypeCoeffField::scalarType>::zero
            );

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                sumOff[u[coeffI]] += mag(activeUpper[coeffI]);
                sumOff[l[coeffI]] += mag(activeUpper[coeffI]);
            }

            activeDiag = max(activeDiag, sumOff);
            activeDiag *= 1.0/alpha;

            // Add the relaxation contribution to b
            forAll (b, i)
            {
                b[i] += mult(activeDiag[i] - activeDiagOld[i], x[i]);
            }
        }
    }
    else if (this->asymmetric())
    {
        // Full asymmetric matrix
        const TypeCoeffField& Lower =
            const_cast<const BlockLduMatrix<Type>&>(*this).lower();

        const TypeCoeffField& Upper =
            const_cast<const BlockLduMatrix<Type>&>(*this).upper();

        if
        (
            Lower.activeType() == blockCoeffBase::LINEAR
         || Upper.activeType() == blockCoeffBase::LINEAR
         || Diag.activeType() == blockCoeffBase::LINEAR
        )
        {
            const linearTypeField& activeLower = Lower.asLinear();
            const linearTypeField& activeUpper = Upper.asLinear();
            linearTypeField& activeDiag = Diag.asLinear();

            linearTypeField sumOff
            (
                activeDiag.size(),
                pTraits<typename TypeCoeffField::linearType>::zero
            );

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                sumOff[u[coeffI]] += cmptMag(activeLower[coeffI]);
                sumOff[l[coeffI]] += cmptMag(activeUpper[coeffI]);
            }

            activeDiag = max(activeDiag, sumOff);
            activeDiag *= 1.0/alpha;
        }
        else if
        (
            Lower.activeType() == blockCoeffBase::SCALAR
         || Upper.activeType() == blockCoeffBase::SCALAR
         || Diag.activeType() == blockCoeffBase::SCALAR
        )
        {
            const scalarTypeField& activeLower = Lower.asScalar();
            const scalarTypeField& activeUpper = Upper.asScalar();
            scalarTypeField& activeDiag = Diag.asScalar();

            // Make a copy of diagonal before relaxation
            scalarTypeField activeDiagOld = activeDiag;

            scalarTypeField sumOff
            (
                activeDiag.size(),
                pTraits<typename TypeCoeffField::scalarType>::zero
            );

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                sumOff[u[coeffI]] += mag(activeLower[coeffI]);
                sumOff[l[coeffI]] += mag(activeUpper[coeffI]);
            }

            activeDiag = max(activeDiag, sumOff);
            activeDiag *= 1.0/alpha;

            forAll (b, i)
            {
                b[i] += (activeDiag[i] - activeDiagOld[i])*x[i];
            }
        }
    }
}


template<class Type>
void BlockLduMatrix<Type>::decoupledMultEqOp(const scalarField& sf)
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;

    if (diagPtr_)
    {
        *diagPtr_ *= sf;
    }

    if (upperPtr_)
    {
        TypeCoeffField& Upper = *upperPtr_;

        const unallocLabelList& l = lduAddr().lowerAddr();

        if (Upper.activeType() == blockCoeffBase::SCALAR)
        {
            scalarTypeField& activeUpper = Upper.asScalar();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeUpper[coeffI] *= sf[l[coeffI]];
            }
        }
        else if (Upper.activeType() == blockCoeffBase::LINEAR)
        {
            linearTypeField& activeUpper = Upper.asLinear();

            for (register label coeffI = 0; coeffI < l.size(); coeffI++)
            {
                activeUpper[coeffI] *= sf[l[coeffI]];
            }
        }
    }

    if (lowerPtr_)
    {
        TypeCoeffField& Lower = *lowerPtr_;

        const unallocLabelList& u = lduAddr().upperAddr();

        if (Lower.activeType() == blockCoeffBase::SCALAR)
        {
            scalarTypeField& activeLower = Lower.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                activeLower[coeffI] *= sf[u[coeffI]];
            }
        }
        else if (Lower.activeType() == blockCoeffBase::LINEAR)
        {
            linearTypeField& activeLower = Lower.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                activeLower[coeffI] *= sf[u[coeffI]];
            }
        }
    }
}


template<class Type>
void BlockLduMatrix<Type>::decoupledAmulCore
(
    TypeField& Ax,
    const TypeField& x
) const
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;

    const unallocLabelList& u = lduAddr().upperAddr();
    const unallocLabelList& l = lduAddr().lowerAddr();

    // In order to do automatic multiplication, diagonal needs to be recognised
    // as a decoupled coeff field.  HJ, 19/Feb/2008
    const DecoupledCoeffField<Type>& Diag = this->diag();
    const TypeCoeffField& Upper = this->upper();

    // Diagonal multiplication, no indirection
    multiply(Ax, Diag, x);

    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    // Lower multiplication
    if (symmetric())
    {
        if (Upper.activeType() == blockCoeffBase::SCALAR)
        {
            const scalarTypeField& activeUpper = Upper.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Ax[u[coeffI]] += mult(activeUpper[coeffI], x[l[coeffI]]);
            }
        }
        else if (Upper.activeType() == blockCoeffBase::LINEAR)
        {
            const linearTypeField& activeUpper = Upper.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Ax[u[coeffI]] += mult(activeUpper[coeffI], x[l[coeffI]]);
            }
        }
    }
    else // Asymmetric matrix
    {
        const TypeCoeffField& Lower = this->lower();

        if (Lower.activeType() == blockCoeffBase::SCALAR)
        {
            const scalarTypeField& activeLower = Lower.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Ax[u[coeffI]] += mult(activeLower[coeffI], x[l[coeffI]]);
            }
        }
        else if (Lower.activeType() == blockCoeffBase::LINEAR)
        {
            const linearTypeField& activeLower = Lower.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Ax[u[coeffI]] += mult(activeLower[coeffI], x[l[coeffI]]);
            }
        }
    }

    // Upper multiplication
    if (Upper.activeType() == blockCoeffBase::SCALAR)
    {
        const scalarTypeField& activeUpper = Upper.asScalar();

        for (register label coeffI = 0; coeffI < u.size(); coeffI++)
        {
            Ax[l[coeffI]] += mult(activeUpper[coeffI], x[u[coeffI]]);
        }
    }
    else if (Upper.activeType() == blockCoeffBase::LINEAR)
    {
        const linearTypeField& activeUpper = Upper.asLinear();

        for (register label coeffI = 0; coeffI < u.size(); coeffI++)
        {
            Ax[l[coeffI]] += mult(activeUpper[coeffI], x[u[coeffI]]);
        }
    }
}


template<class Type>
void BlockLduMatrix<Type>::decoupledTmulCore
(
    TypeField& Tx,
    const TypeField& x
) const
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;

    const unallocLabelList& u = lduAddr().upperAddr();
    const unallocLabelList& l = lduAddr().lowerAddr();

    // In order to do automatic multiplication, diagonal needs to be recognised
    // as a decoupled coeff field.  HJ, 19/Feb/2008
    const DecoupledCoeffField<Type>& Diag = this->diag();
    const TypeCoeffField& Upper = this->upper();

    // Diagonal multiplication, no indirection
    multiply(Tx, Diag, x);

    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    // Upper multiplication
    if (Upper.activeType() == blockCoeffBase::SCALAR)
    {
        const scalarTypeField& activeUpper = Upper.asScalar();

        for (register label coeffI = 0; coeffI < u.size(); coeffI++)
        {
            Tx[u[coeffI]] += mult(activeUpper[coeffI], x[l[coeffI]]);
        }
    }
    else if (Upper.activeType() == blockCoeffBase::LINEAR)
    {
        const linearTypeField& activeUpper = Upper.asLinear();

        for (register label coeffI = 0; coeffI < u.size(); coeffI++)
        {
            Tx[u[coeffI]] += mult(activeUpper[coeffI], x[l[coeffI]]);
        }
    }

    // Lower multiplication
    if (symmetric())
    {
        if (Upper.activeType() == blockCoeffBase::SCALAR)
        {
            const scalarTypeField& activeUpper = Upper.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Tx[l[coeffI]] += mult(activeUpper[coeffI], x[u[coeffI]]);
            }
        }
        else if (Upper.activeType() == blockCoeffBase::LINEAR)
        {
            const linearTypeField& activeUpper = Upper.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Tx[l[coeffI]] += mult(activeUpper[coeffI], x[u[coeffI]]);
            }
        }
    }
    else // Asymmetric matrix
    {
        const TypeCoeffField& Lower = this->lower();

        if (Lower.activeType() == blockCoeffBase::SCALAR)
        {
            const scalarTypeField& activeLower = Lower.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Tx[l[coeffI]] += mult(activeLower[coeffI], x[u[coeffI]]);
            }
        }
        else if (Lower.activeType() == blockCoeffBase::LINEAR)
        {
            const linearTypeField& activeLower = Lower.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                Tx[l[coeffI]] += mult(activeLower[coeffI], x[u[coeffI]]);
            }
        }
    }
}


// BlockLduMatrixDecoupledHOps.C
template<class Type>
tmp<Field<Type> >
BlockLduMatrix<Type>::decoupledH(const Field<Type>& x) const
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;

    // Create result
    tmp<Field<Type> > tresult
    (
        new Field<Type>(lduAddr().size(), pTraits<Type>::zero)
    );
    Field<Type>& result = tresult();

    const unallocLabelList& u = lduAddr().upperAddr();
    const unallocLabelList& l = lduAddr().lowerAddr();

    const TypeCoeffField& Upper = this->upper();

    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    // Lower multiplication
    if (symmetric())
    {
        if (Upper.activeType() == blockCoeffBase::SCALAR)
        {
            const scalarTypeField& activeUpper = Upper.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                result[u[coeffI]] -= mult(activeUpper[coeffI], x[l[coeffI]]);
            }
        }
        else if (Upper.activeType() == blockCoeffBase::LINEAR)
        {
            const linearTypeField& activeUpper = Upper.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                result[u[coeffI]] -= mult(activeUpper[coeffI], x[l[coeffI]]);
            }
        }
    }
    else // Asymmetric matrix
    {
        const TypeCoeffField& Lower = this->lower();

        if (Lower.activeType() == blockCoeffBase::SCALAR)
        {
            const scalarTypeField& activeLower = Lower.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                result[u[coeffI]] -= mult(activeLower[coeffI], x[l[coeffI]]);
            }
        }
        else if (Lower.activeType() == blockCoeffBase::LINEAR)
        {
            const linearTypeField& activeLower = Lower.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                result[u[coeffI]] -= mult(activeLower[coeffI], x[l[coeffI]]);
            }
        }
    }

    // Upper multiplication
    if (Upper.activeType() == blockCoeffBase::SCALAR)
    {
        const scalarTypeField& activeUpper = Upper.asScalar();

        for (register label coeffI = 0; coeffI < u.size(); coeffI++)
        {
            result[l[coeffI]] -= mult(activeUpper[coeffI], x[u[coeffI]]);
        }
    }
    else if (Upper.activeType() == blockCoeffBase::LINEAR)
    {
        const linearTypeField& activeUpper = Upper.asLinear();

        for (register label coeffI = 0; coeffI < u.size(); coeffI++)
        {
            result[l[coeffI]] -= mult(activeUpper[coeffI], x[u[coeffI]]);
        }
    }

    return tresult;
}


template<class Type>
tmp<Field<Type> >
BlockLduMatrix<Type>::decoupledFaceH(const Field<Type>& x) const
{
    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;

    const unallocLabelList& u = lduAddr().upperAddr();
    const unallocLabelList& l = lduAddr().lowerAddr();

    // Create result
    tmp<Field<Type> > tresult(new Field<Type>(u.size(), pTraits<Type>::zero));
    Field<Type>& result = tresult();

    const TypeCoeffField& Upper = this->upper();

    // Create multiplication function object
    typename BlockCoeff<Type>::multiply mult;

    // Lower multiplication
    if (symmetric())
    {
        if (Upper.activeType() == blockCoeffBase::SCALAR)
        {
            const scalarTypeField& activeUpper = Upper.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                // This can be optimised with a subtraction.
                // Currently not done for clarity.  HJ, 31/Oct/2007
                result[coeffI] =
                    mult(activeUpper[coeffI], x[u[coeffI]])
                  - mult(activeUpper[coeffI], x[l[coeffI]]);
            }
        }
        else if (Upper.activeType() == blockCoeffBase::LINEAR)
        {
            const linearTypeField& activeUpper = Upper.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                // This can be optimised with a subtraction.
                // Currently not done for clarity.  HJ, 31/Oct/2007
                result[coeffI] =
                    mult(activeUpper[coeffI], x[u[coeffI]])
                  - mult(activeUpper[coeffI], x[l[coeffI]]);
            }
        }
    }
    else // Asymmetric matrix
    {
        const TypeCoeffField& Lower = this->lower();

        if (Lower.activeType() == blockCoeffBase::SCALAR)
        {
            const scalarTypeField& activeUpper = Upper.asScalar();
            const scalarTypeField& activeLower = Lower.asScalar();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                result[coeffI] =
                    mult(activeUpper[coeffI], x[u[coeffI]])
                  - mult(activeLower[coeffI], x[l[coeffI]]);
            }
        }
        else if (Lower.activeType() == blockCoeffBase::LINEAR)
        {
            const linearTypeField& activeUpper = Upper.asLinear();
            const linearTypeField& activeLower = Lower.asLinear();

            for (register label coeffI = 0; coeffI < u.size(); coeffI++)
            {
                result[coeffI] =
                    mult(activeUpper[coeffI], x[u[coeffI]])
                  - mult(activeLower[coeffI], x[l[coeffI]]);
            }
        }
    }

    return tresult;
}


} // End namespace CML
#endif
