/*---------------------------------------------------------------------------*\
Copyright (C) 2004-6 H. Jasak
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
    BlockBiCGStabSolver

Description
    Preconditioned Bi-Conjugate Gradient stabilised solver.

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*---------------------------------------------------------------------------*/

#ifndef BlockBiCGStabSolver_HPP
#define BlockBiCGStabSolver_HPP

#include "BlockLduSolver.hpp"
#include "BlockIterativeSolver.hpp"
#include "BlockLduPrecon.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class BlockBiCGStabSolver Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockBiCGStabSolver
:
    public BlockIterativeSolver<Type>
{

    //- Preconditioner
    autoPtr<BlockLduPrecon<Type> > preconPtr_;

    //- Disallow default bitwise copy construct
    BlockBiCGStabSolver(const BlockBiCGStabSolver<Type>&);

    //- Disallow default bitwise assignment
    void operator=(const BlockBiCGStabSolver<Type>&);


public:

    //- Runtime type information
    TypeName("BiCGStab");

    //- Construct from matrix components and solver data stream
    BlockBiCGStabSolver
    (
        const word& fieldName,
        const BlockLduMatrix<Type>& matrix,
        const dictionary& dict
    );

    virtual ~BlockBiCGStabSolver()
    {}

    // Member Functions
    //- Solve the matrix with this solver
    virtual BlockSolverPerformance<Type> solve
    (
        Field<Type>& x,
        const Field<Type>& b
    );
};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from matrix and solver data stream
template<class Type>
BlockBiCGStabSolver<Type>::BlockBiCGStabSolver
(
    const word& fieldName,
    const BlockLduMatrix<Type>& matrix,
    const dictionary& dict
)
:
    BlockIterativeSolver<Type>
    (
        fieldName,
        matrix,
        dict
    ),
    preconPtr_
    (
        BlockLduPrecon<Type>::New
        (
            matrix,
            this->dict()
        )
    )
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
typename CML::BlockSolverPerformance<Type>
BlockBiCGStabSolver<Type>::solve
(
    Field<Type>& x,
    const Field<Type>& b
)
{
    // Create local references to avoid the spread this-> ugliness
    const BlockLduMatrix<Type>& matrix = this->matrix_;

    // Prepare solver performance
    BlockSolverPerformance<Type> solverPerf
    (
        typeName,
        this->fieldName()
    );

    scalar norm = this->normFactor(x, b);

    // Multiplication helper
    typename BlockCoeff<Type>::multiply mult;

    Field<Type> p(x.size());

    // Calculate initial residual
    matrix.Amul(p, x);
    Field<Type> r(b - p);

    solverPerf.initialResidual() = gSum(cmptMag(r))/norm;
    solverPerf.finalResidual() = solverPerf.initialResidual();

    // Check convergence, solve if not converged

    if (not this->stop(solverPerf))
    {
        scalar rho = this->great_;
        scalar rhoOld = rho;

        scalar alpha = 0;
        scalar omega = this->great_;
        scalar beta;

        p = pTraits<Type>::zero;
        Field<Type> ph(x.size(), pTraits<Type>::zero);
        Field<Type> v(x.size(), pTraits<Type>::zero);
        Field<Type> s(x.size(), pTraits<Type>::zero);
        Field<Type> sh(x.size(), pTraits<Type>::zero);
        Field<Type> t(x.size(), pTraits<Type>::zero);

        // Calculate transpose residual
        Field<Type> rw(r);

        do
        {
            rhoOld = rho;

            // Update search directions
            rho = gSumProd(rw, r);

            beta = rho/rhoOld*(alpha/omega);

            // Restart if breakdown occurs
            if (rho == 0)
            {
                rw = r;
                rho = gSumProd(rw, r);

                alpha = 0;
                omega = 0;
                beta = 0;
            }

            forAll (p, i)
            {
                p[i] = r[i] + beta*p[i] - beta*omega*v[i];
            }

            preconPtr_->precondition(ph, p);
            matrix.Amul(v, ph);
            alpha = rho/gSumProd(rw, v);

            forAll (s, i)
            {
                s[i] = r[i] - alpha*v[i];
            }

            // Bug fix, Alexander Monakov, 11/Jul/2012
            preconPtr_->precondition(sh, s);
            matrix.Amul(t, sh);
            omega = gSumProd(t, s)/gSumProd(t, t);

            forAll (x, i)
            {
                x[i] = x[i] + alpha*ph[i] + omega*sh[i];
            }

            forAll (r, i)
            {
                r[i] = s[i] - omega*t[i];
            }

            solverPerf.finalResidual() = gSum(cmptMag(r))/norm;
            solverPerf.nIterations()++;
        } while (not this->stop(solverPerf));
    }

    return solverPerf;
}


} // End namespace CML
#endif
