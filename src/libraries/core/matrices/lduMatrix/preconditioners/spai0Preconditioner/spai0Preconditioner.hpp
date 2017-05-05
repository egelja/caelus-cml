/*---------------------------------------------------------------------------*\
Copyright (C) 2016 Applied CCM
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

Class
    CML::spai0Preconditioner

Description
    Spai0 preconditioner for both symmetric and 
    asymmetric matrices. The preconditioner is an alternative to
    Jacobi preconditioner and it is meant to be used for large
    parallel simulations. 

SourceFiles
    spai0Preconditioner.cpp

References

    [1] "Robust Parallel Smoothing for Multigrid via Sparse Approximate 
        Inverses", O. Broker, M.J. Grote, C. Mayer, A. Reusken, SIAM Journal 
        of Scientific Computing, 2001

    [2] "Iterative Methods for Sparse Linear Systems", Y. Saad, SIAM, 2003,
        Philadephia, PA, 2nd edition

Author;

    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef spai0Preconditioner_H
#define spai0Preconditioner_H

#include "lduMatrix.hpp"

namespace CML
{

class spai0Preconditioner : public lduMatrix::preconditioner
{
    // Private data

    //- The reciprocal spai0
    scalarField rD_;

    // Private Member Functions

    //- Disallow default bitwise copy construct
    spai0Preconditioner(const spai0Preconditioner&);

    //- Disallow default bitwise assignment
    void operator=(const spai0Preconditioner&);

public:

    //- Runtime type information
    TypeName("SPAI0");

    //- Construct from matrix components and preconditioner solver controls
    spai0Preconditioner
    (
        lduMatrix::solver const&,
        dictionary const& solverControlsUnused
    );

    virtual ~spai0Preconditioner()
    {}

    // Member Functions

    //- Calculate the reciprocal of the preconditioned diagonal
    static void approximateInverse(scalarField& rD, lduMatrix const& matrix);
  
    //- Return w the preconditioned form of residual r
    virtual void precondition
    (
        scalarField& w,
        scalarField const& r,
        direction const cmpt=0
    ) const;

    //- Return wT the transpose-matrix preconditioned form of residual rT.
    virtual void preconditionT
    (
        scalarField& wT,
        scalarField const& rT,
        direction const cmpt=0
    ) const
    {
        return precondition(wT, rT, cmpt);
    }
};


} // End namespace CML

#endif

