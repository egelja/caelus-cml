/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2011 H. Jasak
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
    BlockLduSolver

Description
    Linear equation solver for BlockLduMatrix.

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

   Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#ifndef BlockLduSolver_HPP
#define BlockLduSolver_HPP

#include "BlockLduMatrix.hpp"
#include "scalarBlockConstraint.hpp"
#include "runTimeSelectionTables.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "BlockDiagonalSolver.hpp"


namespace CML
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //

template<class Type>
class BlockSolverPerformance;

/*---------------------------------------------------------------------------*\
                        Class BlockLduSolver Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockLduSolver
{

    //- Name of field being solved for
    word fieldName_;

    //- Control data dictionary
    dictionary dict_;


protected:

    typedef Field<Type> TypeField;

    //- Matrix
    const BlockLduMatrix<Type>& matrix_;

    //- Return dictionary
    const dictionary& dict() const
    {
        return dict_;
    }

    //- Read a control parameter from the dictionary
    template<class T>
    void readControl
    (
        const dictionary& dict,
        T& control,
        const word& controlName
    );


public:

    //- Runtime type information
    TypeName("BlockLduSolver");

    //- Large value for the use in solvers
    static const scalar great_;

    //- Small value for the use in solvers
    static const scalar small_;

    // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        BlockLduSolver,
        symMatrix,
        (
            const word& fieldName,
            BlockLduMatrix<Type>& matrix,
            const dictionary& dict
        ),
        (
            fieldName,
            matrix,
            dict
        )
    );

    declareRunTimeSelectionTable
    (
        autoPtr,
        BlockLduSolver,
        asymMatrix,
        (
            const word& fieldName,
            BlockLduMatrix<Type>& matrix,
            const dictionary& dict
        ),
        (
            fieldName,
            matrix,
            dict
        )
    );


    //- Construct from field name and matrix
    BlockLduSolver
    (
        const word& fieldName,
        const BlockLduMatrix<Type>& matrix
    );

    //- Construct from dictionary
    BlockLduSolver
    (
        const word& fieldName,
        const BlockLduMatrix<Type>& matrix,
        const dictionary& dict
    );

    // Selectors
    //- Return a new solver from a dictionary
    static autoPtr<BlockLduSolver<Type> > New
    (
        const word& fieldName,
        BlockLduMatrix<Type>& matrix,
        const dictionary& dict
    );

    //- Return a new solver given type
    static autoPtr<BlockLduSolver<Type> > New
    (
        const word& solverName,
        const word& fieldName,
        BlockLduMatrix<Type>& matrix,
        const dictionary& dict
    );

    virtual ~BlockLduSolver()
    {}

    // Member functions
    //- Return field name
    const word& fieldName() const
    {
        return fieldName_;
    }

    //- Solve
    virtual BlockSolverPerformance<Type> solve
    (
        TypeField& x,
        const TypeField& b
    ) = 0;
};

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class Type>
const scalar BlockLduSolver<Type>::great_ = 1.0e+20;

template<class Type>
const scalar BlockLduSolver<Type>::small_ = 1.0e-20;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
template<class T>
void BlockLduSolver<Type>::readControl
(
    const dictionary& dict,
    T& control,
    const word& controlName
)
{
    if (dict.found(controlName))
    {
        dict.lookup(controlName) >> control;
    }
    else
    {
        FatalIOErrorInFunction(dict)
            << "Cannot read control " << controlName
            << exit(FatalIOError);
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
BlockLduSolver<Type>::BlockLduSolver
(
    const word& fieldName,
    const BlockLduMatrix<Type>& matrix
)
:
    fieldName_(fieldName),
    dict_(),
    matrix_(matrix)
{}


template<class Type>
BlockLduSolver<Type>::BlockLduSolver
(
    const word& fieldName,
    const BlockLduMatrix<Type>& matrix,
    const dictionary& dict
)
:
    fieldName_(fieldName),
    dict_(dict),
    matrix_(matrix)
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
autoPtr<typename CML::BlockLduSolver<Type> >
BlockLduSolver<Type>::New
(
    const word& fieldName,
    BlockLduMatrix<Type>& matrix,
    const dictionary& dict
)
{
    word solverName(dict.lookup("solver"));

    if (matrix.diagonal())
    {
        return autoPtr<BlockLduSolver<Type> >
        (
            new BlockDiagonalSolver<Type>(fieldName, matrix, dict)
        );
    }
    else if (matrix.symmetric() || matrix.asymmetric())
    {
        return BlockLduSolver<Type>::New
        (
            solverName,
            fieldName,
            matrix,
            dict
        );
    }
    else
    {
        FatalErrorInFunction
            << "cannot solve incomplete matrix, "
               "no diagonal or off-diagonal coefficient"
            << exit(FatalError);

        return autoPtr<BlockLduSolver<Type> >(nullptr);
    }
}


template<class Type>
autoPtr<typename CML::BlockLduSolver<Type> >
BlockLduSolver<Type>::New
(
    const word& solverName,
    const word& fieldName,
    BlockLduMatrix<Type>& matrix,
    const dictionary& dict
)
{
    if (matrix.diagonal())
    {
        return autoPtr<BlockLduSolver<Type> >
        (
            new BlockDiagonalSolver<Type>(fieldName, matrix, dict)
        );
    }
    else if (matrix.symmetric())
    {
        if (not symMatrixConstructorTablePtr_)
        {
            FatalErrorInFunction
                << "Initialization problem." << endl;
        }

        typename symMatrixConstructorTable::iterator constructorIter =
            symMatrixConstructorTablePtr_->find(solverName);

        if (constructorIter == symMatrixConstructorTablePtr_->end())
        {
            FatalIOErrorInFunction(dict)
                << "Unknown symmetric matrix solver " << solverName
                << endl << endl
                << "Valid symmetric matrix solvers are :" << endl
                << symMatrixConstructorTablePtr_->sortedToc()
                << exit(FatalIOError);
        }

        return autoPtr<BlockLduSolver<Type> >
        (
            constructorIter()
            (
                fieldName,
                matrix,
                dict
            )
        );
    }
    else if (matrix.asymmetric())
    {
        if (not asymMatrixConstructorTablePtr_)
        {
            FatalErrorInFunction
                << "Initialization problem." << endl;
        }

        typename asymMatrixConstructorTable::iterator constructorIter =
            asymMatrixConstructorTablePtr_->find(solverName);

        if (constructorIter == asymMatrixConstructorTablePtr_->end())
        {
            FatalIOErrorInFunction(dict)
                << "Unknown asymmetric matrix solver " << solverName
                << endl << endl
                << "Valid asymmetric matrix solvers are :" << endl
                << asymMatrixConstructorTablePtr_->sortedToc()
                << exit(FatalIOError);
        }

        return autoPtr<BlockLduSolver<Type> >
        (
            constructorIter()
            (
                fieldName,
                matrix,
                dict
            )
        );
    }
    else
    {
        FatalErrorInFunction
            << "cannot solve incomplete matrix, "
               "no diagonal or off-diagonal coefficient"
            << exit(FatalError);

        return autoPtr<BlockLduSolver<Type> >(nullptr);
    }
}


} // End namespace CML

#define makeBlockSolverTypeName(typeSolverType)                               \
                                                                              \
defineNamedTemplateTypeNameAndDebug(typeSolverType, 0);


#define addSolverToBlockMatrix(Type, typeSolverType, typeMatrix)              \
                                                                              \
addToRunTimeSelectionTable(block##Type##Solver, typeSolverType, typeMatrix);


#define makeBlockSolverHyperVector(type, Type, args...)                       \
                                                                              \
typedef BlockLduSolver<type > block##Type##Solver;                            \
defineNamedTemplateTypeNameAndDebug(block##Type##Solver, 0);                  \
defineTemplateRunTimeSelectionTable                                           \
(                                                                             \
    block##Type##Solver,                                                      \
    symMatrix                                                                 \
);                                                                            \
                                                                              \
defineTemplateRunTimeSelectionTable                                           \
(                                                                             \
    block##Type##Solver,                                                      \
    asymMatrix                                                                \
);                                                                            \
                                                                              \
typedef BlockDiagonalSolver<type > block##Type##DiagonalSolver;               \
defineNamedTemplateTypeNameAndDebug(block##Type##DiagonalSolver, 0);          \
                                                                              \
typedef BlockBiCGStabSolver<type > block##Type##BiCGStabSolver;               \
makeBlockSolverTypeName(block##Type##BiCGStabSolver);                         \
addSolverToBlockMatrix(Type, block##Type##BiCGStabSolver, symMatrix);         \
addSolverToBlockMatrix(Type, block##Type##BiCGStabSolver, asymMatrix);        \
                                                                              \
typedef BlockCGSolver<type > block##Type##CGSolver;                           \
makeBlockSolverTypeName(block##Type##CGSolver);                               \
addSolverToBlockMatrix(Type, block##Type##CGSolver, symMatrix);               \
                                                                              \
typedef BlockGaussSeidelSolver<type > block##Type##GaussSeidelSolver;         \
makeBlockSolverTypeName(block##Type##GaussSeidelSolver);                      \
addSolverToBlockMatrix(Type, block##Type##GaussSeidelSolver, symMatrix);      \
addSolverToBlockMatrix(Type, block##Type##GaussSeidelSolver, asymMatrix);     \



#endif
