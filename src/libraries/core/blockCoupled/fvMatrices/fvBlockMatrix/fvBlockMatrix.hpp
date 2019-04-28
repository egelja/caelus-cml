/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2016 V. Vukcevic
Copyright (C) 2015 H. Jasak
Copyright (C) 2018 Applied CCM Pty Ltd
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
    CML::fvBlockMatrix

Description
    fvBlockMatrix is an extension of fvMatrix for block coupled types. It holds
    general insertion and retrieval tools for block coupling and specific
    functions for p-U coupling.

    This file has been obtained from FOAM-Extend-4.0.
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Vuko Vukcevic, FMENA Zagreb.
    Update by Hrvoje Jasak

\*---------------------------------------------------------------------------*/

#ifndef fvBlockMatrix_HPP
#define fvBlockMatrix_HPP

#include "BlockLduSystem.hpp"
#include "fvMatrices.hpp"
#include "BlockLduSolver.hpp"
#include "IOstreams.hpp"


namespace CML
{

// Forward declaration of friend functions and operators

template<class Type>
class fvBlockMatrix;

template<class Type>
Ostream& operator<<(Ostream&, const fvBlockMatrix<Type>&);


/*---------------------------------------------------------------------------*\
                        Class fvBlockMatrix Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fvBlockMatrix
:
    public BlockLduSystem<Type, Type>
{
    //- Internal field reference
    GeometricField<Type, fvPatchField, volMesh>& psi_;

    // Insertion functions for fvMatrix<matrixType> into diagonal positions
    //- Insert internal field into this fvBlockMatrix
    template<class fieldType>
    void insertSolutionVector
    (
        const direction dir,
        const Field<fieldType>& xSingle
    );

    //- Insert matrix diagonal and source into this fvBlockMatrix
    template<class matrixType>
    void insertDiagSource
    (
        const direction dir,
        fvMatrix<matrixType>& matrix
    );

    //- Insert upper and lower part into this fvBlockMatrix
    template<class matrixType>
    void insertUpperLower
    (
        const direction dir,
        const fvMatrix<matrixType>& matrix
    );

    //- Update coupling coefficients in this fvBlockMatrix
    template<class matrixType>
    void updateCouplingCoeffs
    (
        const direction dir,
        const fvMatrix<matrixType>& matrix
    );

    // Insertion functions for fvScalarMatrix into off-diagonal positions
    // (coupling matrices)
    //- Insert coupling matrix diag element into this fvBlockMatrix
    void insertCouplingDiag
    (
        const direction dirI,
        const direction dirJ,
        const scalarField& coeffIJ
    );

    //- Insert coupling matrix diag and source into this fvBlockMatrix
    void insertCouplingDiagSource
    (
        const direction dirI,
        const direction dirJ,
        const fvScalarMatrix& matrix
    );

    //- Insert coupling matrix lower and upper into this fvBlockMatrix
    void insertCouplingUpperLower
    (
        const direction dirI,
        const direction dirJ,
        const fvScalarMatrix& matrix
    );

    // Pressure - velocity (p-U) coupling specific functions
    //- Insert BlockLduSystem (obtained by implicit grad/div operator)
    // into this fvBlockMatrix
    template<class blockType, class fieldType>
    void insertBlock
    (
       const direction dirI,
       const direction dirJ,
       const BlockLduSystem<blockType, fieldType>& blockSystem,
       const bool incrementColumnDir
    );

    //- Insert source and coupling coeffs of BlockLduSystem
    // (eg. obtained by implicit grad/div operator)
    template<class blockType, class fieldType>
    void insertBoundaryContributions
    (
        const direction dirI,
        const direction dirJ,
        const BlockLduSystem<blockType, fieldType>& blockSystem,
        const bool incrementColumnDir
    );


public:

    //- Construct given a field to solve for
    fvBlockMatrix(GeometricField<Type, fvPatchField, volMesh>&);

    //- Construct as copy
    fvBlockMatrix(const fvBlockMatrix<Type>&);

    virtual ~fvBlockMatrix()
    {}

    //- Access to GeometricField
    GeometricField<Type, fvPatchField, volMesh>& psi()
    {
        return psi_;
    }

    //- const reference to GeometricField
    const GeometricField<Type, fvPatchField, volMesh>& psi() const
    {
        return psi_;
    }

    // Insertion and retrieval public tools
    //- Retrieve part of internal field from this fvBlockMatrix
    template<class fieldType>
    void retrieveSolution
    (
        const direction dir,
        Field<fieldType>& xSingle
    ) const;

    //- Insert matrix into this fvBlockMatrix
    template<class matrixType>
    void insertEquation
    (
        const direction dir,
        fvMatrix<matrixType>& matrix
    );

    //- Insert existing block system (obtained by implicit grad/div
    // operator) into this fvBlockMatrix
    template<class blockType, class fieldType>
    void insertBlockCoupling
    (
        const direction dirI,
        const direction dirJ,
        const BlockLduSystem<blockType, fieldType>& blockSystem,
        const bool incrementColumnDir
    );

    //- Insert diagonal only equation coupling into this fvBlockMatrix
    //  Source compensation is done in function updateSourceCoupling()
    //  after all coupling terms are added.  HJ, 27/Apr/2015
    void insertEquationCoupling
    (
        const direction dirI,
        const direction dirJ,
        const scalarField& coeffIJ
    );

    //- Insert scalar equation coupling into this fvBlockMatrix
    void insertEquationCoupling
    (
        const direction dirI,
        const direction dirJ,
        const fvScalarMatrix& matrix
    );

    //- Add field into block field
    void blockAdd
    (
        const direction dir,
        const scalarField& xSingle,
        Field<Type>& blockX
    );

    //- Update coupling of block system.
    //  Subtracts the block-coefficient coupling as specified by the
    //  user from the source, leaving the implicit update given by
    //  linearisation
    void updateSourceCoupling();

    //- Insert adjoint convection tensor term. Hard coded to correspond
    //  to Gauss gradient discretisation with linear interpolation. This
    //  needs to be reorganised. VV, 7/Apr/2016.
    void insertAdjointConvection
    (
        const direction UEqnDir,
        const volVectorField& U,
        const volVectorField& UStar
    );

    //- Insert Picard tensor term that comes from Picard linearisation
    // of convection term in momentum equation. VV, 21/July/2014.
    void insertPicardTensor
    (
        const direction UEqnDir,
        const volVectorField& U,
        const surfaceScalarField& phi
    );

    // Solver calls for fvBlockMatrix
    //- Solve returning the solution statistics.
    // Use the given solver controls
    BlockSolverPerformance<Type> solve(const dictionary&);

    //- Solve returning the solution statistics.
    // Solver controls read from fvSolution
    BlockSolverPerformance<Type> solve();

    // Member operators
    void operator=(const fvBlockMatrix<Type>&);

    void negate();

    void operator+=(const fvBlockMatrix<Type>&);
    void operator-=(const fvBlockMatrix<Type>&);

    void operator*=(const scalarField&);
    void operator*=(const scalar);


    // Ostream operator
    friend Ostream& operator<< <Type>
    (
        Ostream&,
        const fvBlockMatrix<Type>&
    );
};

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
template<class fieldType>
void fvBlockMatrix<Type>::insertSolutionVector
(
    const direction dir,
    const Field<fieldType>& xSingle
)
{
    // Get number of field components and local copy of direction, for
    // consistency with member functions where directions need to be reset.
    const direction nCmpts = pTraits<fieldType>::nComponents;
    direction localDir = dir;

    Field<Type>& psiIn = psi_.internalField();

    for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
    {
        scalarField xSingleCurr(xSingle.component(cmptI));

        forAll (xSingleCurr, cellI)
        {
            psiIn[cellI](localDir) = xSingleCurr[cellI];
        }

        localDir++;
    }
}


template<class Type>
template<class matrixType>
void fvBlockMatrix<Type>::insertDiagSource
(
    const direction dir,
    fvMatrix<matrixType>& matrix
)
{
    //matrix.completeAssembly(); Not needed in OF

    // Save a copy for different components
    scalarField diag(matrix.diag());
    scalarField saveDiag(diag);

    // Add source boundary contribution
    Field<matrixType> source(matrix.source());
    matrix.addBoundarySource(source, false);

    const direction nCmpts = pTraits<matrixType>::nComponents;
    direction localDir = dir;

    // Get reference to this source field of block system
    Field<Type>& b = this->source();

    if
    (
        // This is needed if the matrixType is <vector>, then you need to grab
        // coeffs as linear. Consider doing a matrixType check also.
        // VV, 17/March/2014
        this->diag().activeType() != blockCoeffBase::SQUARE
    )
    {
        typename CoeffField<Type>::linearTypeField& blockDiag =
            this->diag().asLinear();

        for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
        {
            matrix.addBoundaryDiag(diag, cmptI);
            scalarField sourceCmpt(source.component(cmptI));

            forAll (diag, cellI)
            {
                blockDiag[cellI](localDir) = diag[cellI];
                b[cellI](localDir) += sourceCmpt[cellI];
            }

            localDir++;

            // Reset diagonal
            diag = saveDiag;
        }
    }
    else if (this->diag().activeType() == blockCoeffBase::SQUARE)
    {
        typename CoeffField<Type>::squareTypeField& blockDiag =
            this->diag().asSquare();

        for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
        {
            matrix.addBoundaryDiag(diag, cmptI);
            scalarField sourceCmpt(source.component(cmptI));

            forAll (diag, cellI)
            {
                blockDiag[cellI](localDir, localDir) = diag[cellI];
                b[cellI](localDir) += sourceCmpt[cellI];
            }

            localDir++;

            // Reset diagonal
            diag = saveDiag;
        }
    }
}


template<class Type>
template<class matrixType>
void fvBlockMatrix<Type>::insertUpperLower
(
    const direction dir,
    const fvMatrix<matrixType>& matrix
)
{
    if (matrix.diagonal())
    {
        // Matrix for insertion is diagonal-only: nothing to do
        return;
    }

    const direction nCmpts = pTraits<matrixType>::nComponents;
    direction localDir = dir;

    if (matrix.hasUpper())
    {
        const scalarField& upper = matrix.upper();

        if (this->upper().activeType() == blockCoeffBase::UNALLOCATED)
        {
            this->upper().asScalar() = upper;
        }
        else if
        (
            this->upper().activeType() == blockCoeffBase::SCALAR
         || this->upper().activeType() == blockCoeffBase::LINEAR
        )
        {
            typename CoeffField<Type>::linearTypeField& blockUpper =
                this->upper().asLinear();

            for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
            {
                forAll (upper, faceI)
                {
                    blockUpper[faceI](localDir) = upper[faceI];
                }

                localDir++;
            }
        }
        else if (this->upper().activeType() == blockCoeffBase::SQUARE)
        {
            typename CoeffField<Type>::squareTypeField& blockUpper =
                this->upper().asSquare();

            for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
            {
                forAll (upper, faceI)
                {
                    blockUpper[faceI](localDir, localDir) = upper[faceI];
                }

                localDir++;
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "Error in matrix insertion: problem with block structure."
            << abort(FatalError);
    }

    if (matrix.symmetric() && this->symmetric())
    {
        Info<< "Both matrices are symmetric: inserting only upper triangle"
            << endl;
    }
    else
    {
        // Reset localDir
        localDir = dir;

        // Either scalar or block matrix is asymmetric: insert lower triangle
        const scalarField& lower = matrix.lower();

        if (this->lower().activeType() == blockCoeffBase::UNALLOCATED)
        {
            this->lower().asScalar() = lower;
        }
        else if
        (
            this->lower().activeType() == blockCoeffBase::SCALAR
         || this->lower().activeType() == blockCoeffBase::LINEAR
        )
        {
            typename CoeffField<Type>::linearTypeField& blockLower =
                this->lower().asLinear();

            for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
            {
                forAll (lower, faceI)
                {
                    blockLower[faceI](localDir) = lower[faceI];
                }

                localDir++;
            }
        }
        else if (this->lower().activeType() == blockCoeffBase::SQUARE)
        {
            typename CoeffField<Type>::squareTypeField& blockLower =
                this->lower().asSquare();

            for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
            {
                forAll (lower, faceI)
                {
                    blockLower[faceI](localDir, localDir) = lower[faceI];
                }

                localDir++;
            }
        }
    }
}


template<class Type>
template<class matrixType>
void fvBlockMatrix<Type>::updateCouplingCoeffs
(
    const direction dir,
    const fvMatrix<matrixType>& matrix
)
{
    const direction nCmpts = pTraits<matrixType>::nComponents;
    direction localDir = dir;

    const GeometricField<matrixType, fvPatchField, volMesh>& psi =
        matrix.psi();

    forAll (psi.boundaryField(), patchI)
    {
        const fvPatchField<matrixType>& pf = psi.boundaryField()[patchI];
        const fvPatch& patch = pf.patch();

        if (patch.coupled())
        {
            const Field<matrixType>& icp = matrix.internalCoeffs()[patchI];
            const Field<matrixType>& bcp = matrix.boundaryCoeffs()[patchI];

            if
            (
                this->coupleUpper()[patchI].activeType()
             != blockCoeffBase::SQUARE
            )
            {
                typename CoeffField<Type>::linearTypeField& pcoupleUpper =
                    this->coupleUpper()[patchI].asLinear();

                typename CoeffField<Type>::linearTypeField& pcoupleLower =
                    this->coupleLower()[patchI].asLinear();

                for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
                {
                    scalarField icpCmpt(icp.component(cmptI));
                    scalarField bcpCmpt(bcp.component(cmptI));

                    forAll (pf, faceI)
                    {
                        pcoupleUpper[faceI](localDir) = bcpCmpt[faceI];
                        pcoupleLower[faceI](localDir) = icpCmpt[faceI];
                    }

                    localDir++;
                }

                localDir = dir;
            }
            else if
            (
                this->coupleUpper()[patchI].activeType()
             == blockCoeffBase::SQUARE
            )
            {
                typename CoeffField<Type>::squareTypeField& pcoupleUpper =
                    this->coupleUpper()[patchI].asSquare();

                typename CoeffField<Type>::squareTypeField& pcoupleLower =
                    this->coupleLower()[patchI].asSquare();

                for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
                {
                    scalarField icpCmpt(icp.component(cmptI));
                    scalarField bcpCmpt(bcp.component(cmptI));

                    forAll (pf, faceI)
                    {
                        pcoupleUpper[faceI](localDir, localDir) =
                            bcpCmpt[faceI];

                        pcoupleLower[faceI](localDir, localDir) =
                            icpCmpt[faceI];
                    }

                    localDir++;
                }

                localDir = dir;
            }
        }
    }
}


template<class Type>
template<class blockType, class fieldType>
void fvBlockMatrix<Type>::insertBlock
(
    const direction dirI,
    const direction dirJ,
    const BlockLduSystem<blockType, fieldType>& blockSystem,
    const bool incrementColumnDir
)
{
    // Sanity checks
    {
        const direction blockMatrixSize = pTraits<blockType>::nComponents;
        const direction blockMatrixASize = pTraits<Type>::nComponents;

        if (blockMatrixSize > blockMatrixASize)
        {
            FatalErrorInFunction
                << "Trying to insert a block matrix from BlockLduSystem into "
                << "smaller one from fvBlockMatrix."
                << abort(FatalError);
        }

        if (dirI == dirJ)
        {
            FatalErrorInFunction
                << "Trying to insert coupling in the position where equation "
                << "should be, since dirI = dirJ. Try using insertEquation "
                << "member function."
                << abort(FatalError);
        }
    }

    const direction nCmpts = pTraits<blockType>::nComponents;
    direction localDirI = dirI;
    direction localDirJ = dirJ;

    // Get references to ldu fields of blockMatrix always as linear
    const typename CoeffField<blockType>::linearTypeField& bmd =
        blockSystem.diag().asLinear();
    const typename CoeffField<blockType>::linearTypeField& bmu =
        blockSystem.upper().asLinear();
    const typename CoeffField<blockType>::linearTypeField& bml =
        blockSystem.lower().asLinear();

    // Get references to ldu fields of this block matrix always as square
    typename CoeffField<Type>::squareTypeField& blockDiag =
         this->diag().asSquare();
    typename CoeffField<Type>::squareTypeField& blockUpper =
         this->upper().asSquare();
    typename CoeffField<Type>::squareTypeField& blockLower =
         this->lower().asSquare();

    // Insert blockMatrix that represents coupling into larger system matrix
    for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
    {
        forAll (bmd, cellI)
        {
            blockDiag[cellI](localDirI, localDirJ) +=
                bmd[cellI].component(cmptI);
        }

        forAll (bmu, faceI)
        {
            blockUpper[faceI](localDirI, localDirJ) +=
                bmu[faceI].component(cmptI);
            blockLower[faceI](localDirI, localDirJ) +=
                bml[faceI].component(cmptI);
        }

        if (incrementColumnDir)
        {
            localDirI++;
        }
        else
        {
            localDirJ++;
        }
    }
}


template<class Type>
template<class blockType, class fieldType>
void fvBlockMatrix<Type>::insertBoundaryContributions
(
    const direction dirI,
    const direction dirJ,
    const BlockLduSystem<blockType, fieldType>& blockSystem,
    const bool incrementColumnDir
)
{
    // Need to get reference to fvMesh instead of lduMesh
    const fvBoundaryMesh& bmesh =
        refCast<const fvMesh>(this->mesh()).boundary();

    const direction nCmpts = pTraits<blockType>::nComponents;
    const direction nSrcCmpts = pTraits<fieldType>::nComponents;
    direction localDirI = dirI;
    direction localDirJ = dirJ;

    const Field<fieldType>& source = blockSystem.source();

    // Get reference to this source field of block system
    Field<Type>& b = this->source();

    // Insert source from block system to this system's rhs
    for (direction cmptI = 0; cmptI < nSrcCmpts; cmptI++)
    {
        scalarField sourceCmpt(source.component(cmptI));

        forAll (b, cellI)
        {
            b[cellI](localDirI) += sourceCmpt[cellI];
        }

        if (incrementColumnDir)
        {
            localDirI++;
        }
        else
        {
            localDirJ++;
        }
    }

    // Reset local directions for coupling contributions
    localDirI = dirI;
    localDirJ = dirJ;

    // Insert coupling contributions into block matrix
    forAll (bmesh, patchI)
    {
        if (bmesh[patchI].coupled())
        {
            typename CoeffField<Type>::squareTypeField& pcoupleUpper =
                this->coupleUpper()[patchI].asSquare();

            typename CoeffField<Type>::squareTypeField& pcoupleLower =
                this->coupleLower()[patchI].asSquare();

            const typename CoeffField<blockType>::linearTypeField& bmcu =
                blockSystem.coupleUpper()[patchI].asLinear();

            const typename CoeffField<blockType>::linearTypeField& bmcl =
                blockSystem.coupleLower()[patchI].asLinear();

            for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
            {
                forAll (bmcu, faceI)
                {
                    pcoupleUpper[faceI](localDirI, localDirJ) +=
                          bmcu[faceI].component(cmptI);

                    pcoupleLower[faceI](localDirI, localDirJ) +=
                          bmcl[faceI].component(cmptI);
                }

                if (incrementColumnDir)
                {
                    localDirI++;
                }
                else
                {
                    localDirJ++;
                }
            }

            // Reset local directions for other patches
            localDirI = dirI;
            localDirJ = dirJ;
        }
    }
}


template<class Type>
void fvBlockMatrix<Type>::insertCouplingDiag
(
    const direction dirI,
    const direction dirJ,
    const scalarField& coeffIJ
)
{
    // Get reference to block diagonal of the block system
    typename CoeffField<Type>::squareTypeField& blockDiag =
        this->diag().asSquare();

    // Set off-diagonal coefficient
    forAll (coeffIJ, cellI)
    {
        blockDiag[cellI](dirI, dirJ) += coeffIJ[cellI];
    }

    // Source compensation is done in function updateSourceCoupling()
    // after all coupling terms are added.  HJ, 27/Apr/2015
}


template<class Type>
void fvBlockMatrix<Type>::insertCouplingDiagSource
(
    const direction dirI,
    const direction dirJ,
    const fvScalarMatrix& matrix
)
{
    // Prepare the diagonal and source
    scalarField diag = matrix.diag();
    scalarField source = matrix.source();

    // Add boundary source contribution
    matrix.addBoundaryDiag(diag, 0);
    matrix.addBoundarySource(source, false);

    // Get reference to block diagonal of the block system
    typename CoeffField<Type>::squareTypeField& blockDiag =
        this->diag().asSquare();

    // Get reference to this source field of the block system
    Field<Type>& b = this->source();

    // Set off-diagonal coefficient
    forAll(diag, cellI)
    {
        blockDiag[cellI](dirI, dirJ) += diag[cellI];
        b[cellI](dirI) += source[cellI];
    }
}


template<class Type>
void fvBlockMatrix<Type>::insertCouplingUpperLower
(
    const direction dirI,
    const direction dirJ,
    const fvScalarMatrix& matrix
)
{
    if (matrix.diagonal())
    {
        // Matrix for insertion is diagonal-only: nothing to do
        return;
    }

    if (matrix.symmetric() && this->symmetric())
    {
        Info<< "Both fvScalarMatrix and block matrix are symmetric: " << nl
            << "inserting only upper triangle"
            << endl;
    }
    else
    {
        // Either scalar or block matrix is asymmetric: insert lower triangle
        const scalarField& lower = matrix.lower();

        typename CoeffField<Type>::squareTypeField& blockLower =
            this->lower().asSquare();

        forAll (lower, cellI)
        {
            blockLower[cellI](dirI, dirJ) = lower[cellI];
        }
    }

    if (matrix.hasUpper())
    {
        const scalarField& upper = matrix.upper();

        typename CoeffField<Type>::squareTypeField& blockUpper =
            this->upper().asSquare();

        forAll (upper, cellI)
        {
            blockUpper[cellI](dirI, dirJ) = upper[cellI];
        }
    }
    else
    {
        FatalErrorInFunction
            << "Error in matrix insertion: problem with block structure."
            << abort(FatalError);
    }

    // Insert block interface fields
    forAll (this->interfaces(), patchI)
    {
        if (this->interfaces().set(patchI))
        {
            // Couple upper and lower
            const scalarField& cUpper = matrix.boundaryCoeffs()[patchI];
            const scalarField& cLower = matrix.internalCoeffs()[patchI];

            typename CoeffField<Type>::squareTypeField& blockUpper =
                this->coupleUpper()[patchI].asSquare();

            typename CoeffField<Type>::squareTypeField& blockLower =
                this->coupleLower()[patchI].asSquare();

            forAll (cUpper, faceI)
            {
                blockUpper[faceI](dirI, dirJ) = cUpper[faceI];
                blockLower[faceI](dirI, dirJ) = cLower[faceI];
            }
        }
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
fvBlockMatrix<Type>::fvBlockMatrix
(
    GeometricField<Type, fvPatchField, volMesh>& psi
)
:
    BlockLduSystem<Type, Type>(psi.mesh()),
    psi_(psi)
{
    this->interfaces() = psi.boundaryField().blockInterfaces();
}


template<class Type>
fvBlockMatrix<Type>::fvBlockMatrix
(
    const fvBlockMatrix<Type>& bxs
)
:
    BlockLduSystem<Type, Type>(bxs),
    psi_(bxs.psi())
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
template<class fieldType>
void fvBlockMatrix<Type>::retrieveSolution
(
    const direction dir,
    Field<fieldType>& xSingle
) const
{
    const direction nCmpts = pTraits<fieldType>::nComponents;
    direction localDir = dir;

    const Field<Type> psiIn = psi_.internalField();

    for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
    {
        scalarField xSingleCurr(xSingle.component(cmptI));

        forAll (xSingleCurr, cellI)
        {
            xSingleCurr[cellI] = psiIn[cellI](localDir);
        }

        xSingle.replace(cmptI, xSingleCurr);

        localDir++;
    }
}


template<class Type>
template<class matrixType>
void fvBlockMatrix<Type>::insertEquation
(
    const direction dir,
    fvMatrix<matrixType>& matrix
)
{
    insertSolutionVector(dir, matrix.psi().internalField());
    insertDiagSource(dir, matrix);
    insertUpperLower(dir, matrix);
    updateCouplingCoeffs(dir, matrix);
}


template<class Type>
template<class blockType, class fieldType>
void fvBlockMatrix<Type>::insertBlockCoupling
(
    const direction dirI,
    const direction dirJ,
    const BlockLduSystem<blockType, fieldType>& blockSystem,
    const bool incrementColumnDir
)
{
    insertBlock(dirI, dirJ, blockSystem, incrementColumnDir);
    insertBoundaryContributions(dirI, dirJ, blockSystem, incrementColumnDir);
}


template<class Type>
void fvBlockMatrix<Type>::insertEquationCoupling
(
    const direction dirI,
    const direction dirJ,
    const scalarField& coeffIJ
)
{
    // Multiply coefficients by volume
    scalarField coeffIJVol(coeffIJ*psi_.mesh().V());

    insertCouplingDiag(dirI, dirJ, coeffIJVol);
}


template<class Type>
void fvBlockMatrix<Type>::insertEquationCoupling
(
    const direction dirI,
    const direction dirJ,
    const fvScalarMatrix& matrix
)
{
    insertCouplingDiagSource(dirI, dirJ, matrix);
    insertCouplingUpperLower(dirI, dirJ, matrix);
}


template<class Type>
void fvBlockMatrix<Type>::blockAdd
(
    const direction dir,
    const scalarField& xSingle,
    Field<Type>& blockX
)
{
    forAll (xSingle, cellI)
    {
        blockX[cellI](dir) += xSingle[cellI];
    }
}


template<class Type>
void fvBlockMatrix<Type>::updateSourceCoupling()
{
    // Eliminate off-diagonal block coefficients from the square diagonal
    // With this change, the segregated matrix can be assembled with complete
    // source terms and linearisation can be provided independently.
    // Once the linearisation coefficients are set (off-diagonal entries
    // in the square block matrix, they are multiplied by the current value
    // of the field and subtracted from the source term

    if (this->diag().activeType() == blockCoeffBase::SQUARE)
    {
        typename CoeffField<Type>::squareTypeField& blockDiag =
            this->diag().asSquare();

        typename CoeffField<Type>::linearTypeField lf(blockDiag.size());
        typename CoeffField<Type>::squareTypeField sf(blockDiag.size());

        // Expand and contract
        // Take out the diagonal entries from the square coefficient
        contractLinear(lf, blockDiag);

        // Expand the diagonal for full square, with zeroes in the off-diagonal
        expandLinear(sf, lf);

        // Subtract from the source the difference between the full block
        // diagonal and the diagonal terms only
        // Sign is the same as in the derivative
        this->source() += (blockDiag - sf) & psi_.internalField();
    }
}


template<class Type>
void fvBlockMatrix<Type>::insertAdjointConvection
(
    const direction UEqnDir,
    const volVectorField& U,
    const volVectorField& UStar
)
{
    const direction nCmpts = pTraits<vector>::nComponents;
    direction localDirI = UEqnDir;
    direction localDirJ = UEqnDir;

    // Sanity check
    {
        const direction blockMatrixSize = pTraits<Type>::nComponents;

        if (nCmpts > blockMatrixSize)
        {
            FatalErrorInFunction
                << "Trying to insert adjoint convection term into smaller "
                << "fvBlockMatrix. Do you have momentum equation?"
                << abort(FatalError);
        }
    }

    const fvMesh& mesh = UStar.mesh();

    // Get owner/neighbour addressing
    const unallocLabelList& owner = mesh.owner();
    const unallocLabelList& neighbour = mesh.neighbour();

    // Get surface area vectors
    const surfaceVectorField& Sf = mesh.Sf();
    const vectorField& SfIn = Sf.internalField();

    // Get reference to primal internal velocity field
    const vectorField& UIn = U.internalField();

    // Get weights for UStar needed for implicit discretisation
    tmp<surfaceInterpolationScheme<vector> > tinterpScheme =
        fvc::scheme<vector>(mesh, UStar.name());
    tmp<surfaceScalarField> tweights = tinterpScheme().weights(U);
    const scalarField& wIn = tweights().internalField();

    // Initialise the block system for adjoint convection
    BlockLduSystem<vector, vector> acSystem(mesh);

    // Get references to ldu fields of adjoint convection block system always as
    // square
    typename CoeffField<vector>::squareTypeField& acDiag =
         acSystem.diag().asSquare();
    typename CoeffField<vector>::squareTypeField& acUpper =
         acSystem.upper().asSquare();
    typename CoeffField<vector>::squareTypeField& acLower =
         acSystem.lower().asSquare();

    vectorField& acSource = acSystem.source();

    // Note: not sure about the signs for both implicit and explicit
    // (boundary) contributions - need to check. VV, 7/Apr/2016.

    // Loop through faces
    register label own, nei;
    forAll (neighbour, faceI)
    {
        own = owner[faceI];
        nei = neighbour[faceI];

        // Get references
        const scalar& wf = wIn[faceI];
        const vector& Sff = SfIn[faceI];

        // Calculate tensorial helper variables
        const tensor acOwn = Sff*UIn[own];
        const tensor acNei = Sff*UIn[nei];

        // Add lower/upper contributions
        acUpper[faceI] = (1 - wf)*acOwn;
        acLower[faceI] = -wf*acNei;

        // Warning: this is not negSumDiag(). VV, 7/Apr/2016
        acDiag[own] += wf*acOwn;
        acDiag[nei] -= (1 - wf)*acNei;
    }

    // Boundary contributions - hard coded or explicit because of the problem
    // with inconsistent return type of internalCoeffs. VV, 7/Apr/2016.
    forAll(UStar.boundaryField(), patchI)
    {
        // Get references to velocity field and the patch
        const fvPatchVectorField& UStarp = UStar.boundaryField()[patchI];
        const fvPatch& patch = UStarp.patch();

        // Check for empty patches. Needed since the boundary conditions are
        // hard coded. VV. 7/Apr/2016.
        if (patch.type() == "empty")
        {
            continue;
        }

        // Get additional references
        const fvsPatchScalarField& wp = tweights().boundaryField()[patchI];
        const unallocLabelList& fc = patch.faceCells();
        const fvsPatchVectorField Sfp = Sf.boundaryField()[patchI];
        const fvPatchVectorField& Up = U.boundaryField()[patchI];

        // Hard coded implicit zeroGradient if patch does not fix value
        if (not UStarp.fixesValue())
        {
            // Get velocity patch internal field (primal, not adjoint)
            const vectorField UpIn(Up.patchInternalField());

            forAll(UpIn, faceI)
            {
                acDiag[fc[faceI]] += Sfp[faceI]*UpIn[faceI];
            }
        }
        // Coupled patches
        else if (patch.coupled())
        {
            typename CoeffField<vector>::squareTypeField& acpCoupleUpper =
                acSystem.coupleUpper()[patchI].asSquare();
            typename CoeffField<vector>::squareTypeField& acpCoupleLower =
                acSystem.coupleLower()[patchI].asSquare();

            // Get velocity patch internal field (primal, not adjoint)
            const vectorField UpIn(Up.patchInternalField());

            // Loop through boundary faces
            forAll (acpCoupleUpper, faceI)
            {
                // Get reference to this weight
                const scalar& wpf = wp[faceI];

                // Calculate tensorial helper variable
                const tensor acOwn = Sfp[faceI]*UpIn[faceI];

                // Add upper and diagonal (lower) contributions
                acpCoupleUpper[faceI] -= (1 - wpf)*acOwn;
                acpCoupleLower[faceI] -= wpf*acOwn;
            }
        }
        else if (UStarp.fixesValue())
        {
            // Get velocity patch internal field (primal, not adjoint)
            const vectorField UpIn(Up.patchInternalField());

            // Get boundary coeffs for the UStarp fieldi
            const vectorField boundaryCoeffs(UStarp.valueBoundaryCoeffs(wp));

            // Boundary contribution
            forAll(boundaryCoeffs, faceI)
            {
                acSource[fc[faceI]] -= Sfp[faceI]*
                    (UpIn[faceI] & boundaryCoeffs[faceI]);
            }
        }
        else
        {
            FatalErrorInFunction
                << "Patch does not fix value, nor doesn't fix value nor is"
                << " coupled."
                << abort(FatalError);
        }
    }

    // Finally add adjoint convection system into this fvBlockMatrix
    typename CoeffField<Type>::squareTypeField& blockDiag =
         this->diag().asSquare();
    typename CoeffField<Type>::squareTypeField& blockUpper =
         this->upper().asSquare();
    typename CoeffField<Type>::squareTypeField& blockLower =
         this->lower().asSquare();

    Field<Type>& blockSource = this->source();

    // Add diagonal, source, lower and upper
    for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
    {
        for (direction cmptJ = 0; cmptJ < nCmpts; cmptJ++)
        {
            forAll(acDiag, cellI)
            {
                blockDiag[cellI](localDirI, localDirJ) +=
                    acDiag[cellI](cmptI, cmptJ);
            }

            forAll(acUpper, faceI)
            {
                blockUpper[faceI](localDirI, localDirJ) +=
                    acUpper[faceI](cmptI, cmptJ);
                blockLower[faceI](localDirI, localDirJ) +=
                    acLower[faceI](cmptI, cmptJ);
            }

            localDirJ++;
        }

        forAll(acSource, cellI)
        {
            blockSource[cellI](localDirI) += acSource[cellI](cmptI);
        }

        localDirI++;

        // Reset localDirJ
        localDirJ = UEqnDir;
    }

    // Reset local direction for coupling contributions
    localDirI = UEqnDir;
    localDirJ = UEqnDir;

    // Add coupling contributions
    forAll(UStar.boundaryField(), patchI)
    {
        if (UStar.boundaryField()[patchI].patch().coupled())
        {
            typename CoeffField<Type>::squareTypeField& pcoupleUpper =
                this->coupleUpper()[patchI].asSquare();
            typename CoeffField<Type>::squareTypeField& pcoupleLower =
                this->coupleLower()[patchI].asSquare();

            const typename CoeffField<vector>::squareTypeField& acpcu =
                acSystem.coupleUpper()[patchI].asSquare();
            const typename CoeffField<vector>::squareTypeField& acpcl =
                acSystem.coupleLower()[patchI].asSquare();

            for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
            {
                for (direction cmptJ = 0; cmptJ < nCmpts; cmptJ++)
                {
                    forAll(acpcu, faceI)
                    {
                        pcoupleUpper[faceI](localDirI, localDirJ) +=
                              acpcu[faceI](cmptI, cmptJ);
                        pcoupleLower[faceI](localDirI, localDirJ) +=
                              acpcl[faceI](cmptI, cmptJ);
                    }
                }
            }

            // Reset local directions for other patches
            localDirI = UEqnDir;
            localDirJ = UEqnDir;
        }
    }
}


template<class Type>
void fvBlockMatrix<Type>::insertPicardTensor
(
    const direction UEqnDir,
    const volVectorField& U,
    const surfaceScalarField& phi
)
{
    const direction nCmpts = pTraits<vector>::nComponents;
    direction localDirI = UEqnDir;
    direction localDirJ = UEqnDir;

    // Sanity check
    {
        const direction blockMatrixSize = pTraits<Type>::nComponents;

        if (nCmpts > blockMatrixSize)
        {
            FatalErrorInFunction
                << "Trying to insert Picard tensor term into smaller "
                << "fvBlockMatrix. Do you have momentum equation?"
                << abort(FatalError);
        }
    }

    // Get weights for U which makes the implicit flux part
    const fvMesh& mesh = U.mesh();

    tmp<surfaceInterpolationScheme<vector> > tinterpScheme =
        fvc::scheme<vector>(mesh, U.name());

    tmp<surfaceScalarField> tweights = tinterpScheme().weights(U);
    const scalarField& wIn = tweights().internalField();

    // Calculate the Pi tensor. Consider hard coding the interpolation scheme to
    // correspond to the div(phi, U) interpolation scheme for consistency.
    // VV, 21/July/2014.
    const surfaceTensorField pi
    (
        fvc::interpolate(U, -phi, "Uconvection")*mesh.Sf()
    );
    const tensorField& piIn = pi.internalField();

    BlockLduSystem<vector, vector> piSystem(mesh);

    // Get references to ldu fields of pi block system always as square
    typename CoeffField<vector>::squareTypeField& piDiag =
         piSystem.diag().asSquare();
    typename CoeffField<vector>::squareTypeField& piUpper =
         piSystem.upper().asSquare();
    typename CoeffField<vector>::squareTypeField& piLower =
         piSystem.lower().asSquare();

    vectorField& piSource = piSystem.source();

    piLower = -wIn*piIn;
    piUpper = piLower + piIn;
    piSystem.negSumDiag();

    // Boundary contributions - hard coded or explicit because of the problem
    // with inconsistent return type of internalCoeffs. VV, 21/July/2014.
    forAll(U.boundaryField(), patchI)
    {
        const fvPatchVectorField& Ub = U.boundaryField()[patchI];
        const fvPatch& patch = Ub.patch();
        const fvsPatchTensorField& pib = pi.boundaryField()[patchI];
        const fvsPatchScalarField& wb = tweights().boundaryField()[patchI];
        const unallocLabelList& fc = patch.faceCells();

        // Check for empty patches. Needed since the boundary conditions are
        // hard coded. VV. 18/Sep/2014.
        if (patch.type() == "empty")
        {
            continue;
        }

        // Hard coded zeroGradient if patch does not fix value
        if (not Ub.fixesValue())
        {
            forAll(Ub, faceI)
            {
                piDiag[fc[faceI]] += pib[faceI];
            }
        }
        // Coupled patches
        else if (patch.coupled())
        {
            typename CoeffField<vector>::squareTypeField& pipCoupleUpper =
                piSystem.coupleUpper()[patchI].asSquare();
            typename CoeffField<vector>::squareTypeField& pipCoupleLower =
                piSystem.coupleLower()[patchI].asSquare();

            const tensorField pcl(-wb*pib);
            const tensorField pcu(pcl + pib);

            // Coupling  contributions
            pipCoupleLower -= pcl;
            pipCoupleUpper -= pcu;
        }
        else if (Ub.fixesValue())
        {
            const vectorField boundaryCoeffs(Ub.valueBoundaryCoeffs(wb));

            // Boundary contribution
            forAll(Ub, faceI)
            {
                piSource[fc[faceI]] -= pib[faceI] & boundaryCoeffs[faceI];
            }
        }
        else
        {
            FatalErrorInFunction
                << "Patch does not fix value, nor doesn't fix value nor is"
                << " coupled."
                << abort(FatalError);
        }
    }

    // Consider chucking the above code into fvm::picardTensor operator.
    // VV, 21/July/2014.

    // Finally add Picard piSystem into this fvBlockMatrix
    typename CoeffField<Type>::squareTypeField& blockDiag =
         this->diag().asSquare();
    typename CoeffField<Type>::squareTypeField& blockUpper =
         this->upper().asSquare();
    typename CoeffField<Type>::squareTypeField& blockLower =
         this->lower().asSquare();

    Field<Type>& blockSource = this->source();

    // Add diagonal, source, lower and upper
    for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
    {
        for (direction cmptJ = 0; cmptJ < nCmpts; cmptJ++)
        {
            forAll(piDiag, cellI)
            {
                blockDiag[cellI](localDirI, localDirJ) +=
                    piDiag[cellI](cmptI, cmptJ);
            }

            forAll(piUpper, faceI)
            {
                blockUpper[faceI](localDirI, localDirJ) +=
                    piUpper[faceI](cmptI, cmptJ);
                blockLower[faceI](localDirI, localDirJ) +=
                    piLower[faceI](cmptI, cmptJ);
            }

            localDirJ++;
        }

        forAll(piSource, cellI)
        {
            blockSource[cellI](localDirI) += piSource[cellI](cmptI);
        }

        localDirI++;

        // Reset localDirJ
        localDirJ = UEqnDir;
    }

    // Reset local direction for coupling contributions
    localDirI = UEqnDir;
    localDirJ = UEqnDir;

    // Add coupling contributions
    forAll(U.boundaryField(), patchI)
    {
        if (U.boundaryField()[patchI].patch().coupled())
        {
            typename CoeffField<Type>::squareTypeField& pcoupleUpper =
                this->coupleUpper()[patchI].asSquare();
            typename CoeffField<Type>::squareTypeField& pcoupleLower =
                this->coupleLower()[patchI].asSquare();

            const typename CoeffField<vector>::squareTypeField& pipcu =
                piSystem.coupleUpper()[patchI].asSquare();
            const typename CoeffField<vector>::squareTypeField& pipcl =
                piSystem.coupleLower()[patchI].asSquare();

            for (direction cmptI = 0; cmptI < nCmpts; cmptI++)
            {
                for (direction cmptJ = 0; cmptJ < nCmpts; cmptJ++)
                {
                    forAll(pipcu, faceI)
                    {
                        pcoupleUpper[faceI](localDirI, localDirJ) +=
                              pipcu[faceI](cmptI, cmptJ);
                        pcoupleLower[faceI](localDirI, localDirJ) +=
                              pipcl[faceI](cmptI, cmptJ);
                    }
                }
            }

            // Reset local directions for other patches
            localDirI = UEqnDir;
            localDirJ = UEqnDir;
        }
    }
}


template<class Type>
BlockSolverPerformance<Type> fvBlockMatrix<Type>::solve
(
    const dictionary& solverControls
)
{
    // Solver call
    BlockSolverPerformance<Type> solverPerf =
        BlockLduSolver<Type>::New
        (
            psi_.name(),
            *this,
            solverControls
        )->solve(psi_.internalField(), this->source());

    // Print performance
    solverPerf.print();

    return solverPerf;
}


template<class Type>
BlockSolverPerformance<Type> fvBlockMatrix<Type>::solve()
{
    return solve(psi_.mesh().solverDict(psi_.name()));
}

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
Ostream& operator<<
(
    Ostream& os,
    const fvBlockMatrix<Type>& bxs
)
{
    os  << static_cast<const BlockLduSystem<Type, Type>&>(bxs) << nl
        << bxs.psi() << endl;

    return os;
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::fvBlockMatrix<Type>::negate()
{
    BlockLduSystem<Type, Type>::negate();
    psi_.negate();
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void fvBlockMatrix<Type>::operator=
(
    const fvBlockMatrix<Type>& bxs
)
{
    if (this == &bxs)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    BlockLduSystem<Type, Type>::operator=(bxs);
    psi_ = bxs.psi();
}


template<class Type>
void fvBlockMatrix<Type>::operator+=
(
    const fvBlockMatrix<Type>& bxs
)
{
    BlockLduSystem<Type, Type>::operator+=(bxs);
    psi_ += bxs.psi();
}

template<class Type>
void fvBlockMatrix<Type>::operator-=
(
    const fvBlockMatrix<Type>& bxs
)
{
    BlockLduSystem<Type, Type>::operator-=(bxs);
    psi_ -= bxs.psi();
}

template<class Type>
void fvBlockMatrix<Type>::operator*=
(
    const scalarField& sf
)
{
    BlockLduSystem<Type, Type>::operator*=(sf);
    psi_ *= sf;
}

template<class Type>
void fvBlockMatrix<Type>::operator*=
(
    const scalar s
)
{
    BlockLduSystem<Type, Type>::operator*=(s);
    psi_ *= s;
}


} // End namespace CML
#endif
