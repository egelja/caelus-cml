/*---------------------------------------------------------------------------*\
Copyright (C) 2004-2014 H. Jasak
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
    BlockSolverPerformance

Description
    Class returned by a solver.

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

   Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved.


\*---------------------------------------------------------------------------*/

#ifndef BlockSolverPerformance_HPP
#define BlockSolverPerformance_HPP

#include "error.hpp"
#include "BlockLduMatrix.hpp"


namespace CML
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //

template<class Type>
class BlockSolverPerformance;

template<class Type>
Istream& operator>>(Istream&, BlockSolverPerformance<Type>&);

template<class Type>
Ostream& operator<<(Ostream&, const BlockSolverPerformance<Type>&);


/*---------------------------------------------------------------------------*\
                   Class BlockSolverPerformance Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockSolverPerformance
{

    //- Solver name
    word solverName_;

    //- Field name
    word fieldName_;

    //- Initial residual
    Type initialResidual_;

    //- Final residual
    Type finalResidual_;

    //- Number of iterations
    label nIterations_;

    //- Has the solver converged?
    bool converged_;

    //- Is the matrix singular?
    bool singular_;


public:

    //- Construct without solver and field name
    BlockSolverPerformance()
    :
        initialResidual_(pTraits<Type>::zero),
        finalResidual_(pTraits<Type>::zero),
        nIterations_(0),
        converged_(false),
        singular_(false)
    {}

    //- Construct with solver and field name
    BlockSolverPerformance
    (
        const word& solverName,
        const word& fieldName,
        const Type& iRes = pTraits<Type>::zero,
        const Type& fRes = pTraits<Type>::zero,
        const label nIter = 0,
        const bool converged = false,
        const bool singular = false
    )
    :
        solverName_(solverName),
        fieldName_(fieldName),
        initialResidual_(iRes),
        finalResidual_(fRes),
        nIterations_(nIter),
        converged_(converged),
        singular_(singular)
    {}

    // Member functions
    //- Return solver name
    const word& solverName() const
    {
        return solverName_;
    }

    //- Return solver name
    word& solverName()
    {
        return solverName_;
    }

    //- Return field name
    const word& fieldName() const
    {
        return fieldName_;
    }

    //- Return initial residual
    const Type& initialResidual() const
    {
        return initialResidual_;
    }

    //- Return initial residual
    Type& initialResidual()
    {
        return initialResidual_;
    }

    //- Return final residual
    const Type& finalResidual() const
    {
        return finalResidual_;
    }

    //- Return final residual
    Type& finalResidual()
    {
        return finalResidual_;
    }

    //- Return number of iterations
    label nIterations() const
    {
        return nIterations_;
    }

    //- Return number of iterations
    label& nIterations()
    {
        return nIterations_;
    }

    //- Has the solver converged?
    bool converged() const
    {
        return converged_;
    }

    //- Has the solver converged?
    bool& converged()
    {
        return converged_;
    }

    //- Is the matrix singular?
    bool singular() const
    {
        return singular_;
    }

    //- Is the matrix singular?
    bool& singular()
    {
        return singular_;
    }

    //- Convergence test
    bool checkConvergence
    (
        const scalar tolerance,
        const scalar relTolerance
    );

    //- Singularity test
    bool checkSingularity(const scalar& residual);

    //- Print summary of solver performance
    void print() const;

    // Member Operators
    bool operator!=(const BlockSolverPerformance<Type>&) const;

    // Friend functions
    friend Istream& operator>> <Type>
    (
        Istream&,
        BlockSolverPerformance<Type>&
    );

    friend Ostream& operator<< <Type>
    (
        Ostream&,
        const BlockSolverPerformance<Type>&
    );
};


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
bool BlockSolverPerformance<Type>::checkConvergence
(
    const scalar Tolerance,
    const scalar RelTolerance
)
{
    if (blockLduMatrix::debug >= 2)
    {
        Info<< solverName_
            << ":  Iteration " << nIterations_
            << " residual = " << finalResidual_
            << " mag = " << mag(finalResidual_)
            << " tol = "
            << CML::max(Tolerance, RelTolerance*mag(initialResidual_))
            << endl;
    }

    // Reconsider evaluation of the final residual residualVector
    // - mag(residualVector) = sqrt(sum(sqr(cmpt))).  Currently used - strict
    // - cmptSum(residualVector) = consistent with 1-norm
    // - cmptMax(residualVector) = consistent with inftyNorm
    // HJ, 29/May/2014
    if
    (
        mag(finalResidual_) < Tolerance
     || (
            RelTolerance > SMALL
         && mag(finalResidual_) <= RelTolerance*mag(initialResidual_)
        )
    )
    {
        converged_ = true;
    }
    else
    {
        converged_ = false;
    }

    return converged_;
}


template<class Type>
bool BlockSolverPerformance<Type>::checkSingularity
(
    const scalar& residual
)
{
    if (mag(residual) > VSMALL)
    {
        singular_ = false;
    }
    else
    {
        singular_ = true;
    }

    return singular_;
}


template<class Type>
void BlockSolverPerformance<Type>::print() const
{
    if (blockLduMatrix::debug)
    {
        Info<< solverName_ << ":  Solving for " << fieldName_;

        if (singular())
        {
            Info<< ":  solution singularity" << endl;
        }
        else
        {
            Info<< ", Initial residual = " << initialResidual_
                << ", Final residual = " << finalResidual_
                << ", No Iterations " << nIterations_
                << endl;
        }
    }
}


template<class Type>
bool BlockSolverPerformance<Type>::operator!=
(
    const BlockSolverPerformance<Type>& bsp
) const
{
    return
    (
        solverName()      != bsp.solverName()
     || fieldName()       != bsp.fieldName()
     || initialResidual() != bsp.initialResidual()
     || finalResidual()   != bsp.finalResidual()
     || nIterations()     != bsp.nIterations()
     || converged()       != bsp.converged()
     || singular()        != bsp.singular()
    );
}


template<class Type>
Istream& operator>>
(
    Istream& is,
    typename CML::BlockSolverPerformance<Type>& bsp
)
{
    is.readBeginList("BlockSolverPerformance<Type>");
    is  >> bsp.solverName_
        >> bsp.fieldName_
        >> bsp.initialResidual_
        >> bsp.finalResidual_
        >> bsp.nIterations_
        >> bsp.converged_
        >> bsp.singular_;
    is.readEndList("BlockSolverPerformance<Type>");

    return is;
}


template<class Type>
Ostream& operator<<
(
    Ostream& os,
    const typename CML::BlockSolverPerformance<Type>& bsp
)
{
    os  << token::BEGIN_LIST
        << bsp.solverName_ << token::SPACE
        << bsp.fieldName_ << token::SPACE
        << bsp.initialResidual_ << token::SPACE
        << bsp.finalResidual_ << token::SPACE
        << bsp.nIterations_ << token::SPACE
        << bsp.converged_ << token::SPACE
        << bsp.singular_ << token::SPACE
        << token::END_LIST;

    return os;
}


} // End namespace CML
#endif
