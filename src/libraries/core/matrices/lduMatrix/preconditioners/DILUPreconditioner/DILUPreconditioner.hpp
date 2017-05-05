/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 - 2016 Applied CCM
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

    CML::DILUPreconditioner

Description

    Simplified diagonal-based incomplete LU preconditioner for asymmetric
    matrices. Better known under ILU(0) name.
    The reciprocal of the preconditioned diagonal is calculated
    and stored.

SourceFiles

    DILUPreconditioner.cpp

References

    [1] Templates for the Solution of Linear Systems: Building Blocks
        for Iterative Methods, R. Barrett, M. Barry, T.F. Chan, J. Demmel,
        J. Donato, J. Dongarra, V. Eijkhout, R. Pozo, C. Romine, and
        Van der Vorst, SIAM, 1994, Philadephia, PA, 2nd edition

    [2] Iterative Methods for Sparse Linear Systems, Y. Saad, SIAM, 2003,
        Philadephia, PA, 2nd edition

\*---------------------------------------------------------------------------*/

#ifndef DILUPreconditioner_H
#define DILUPreconditioner_H

#include "lduMatrix.hpp"

namespace CML
{

class DILUPreconditioner
:
    public lduMatrix::preconditioner
{
    // Private data

    //- The reciprocal preconditioned diagonal
    scalarField rD_;

public:

    //- Runtime type information
    TypeName("DILU");

    //- Construct from matrix components and preconditioner solver controls
    DILUPreconditioner
    (
        lduMatrix::solver const&,
        dictionary const& solverControlsUnused
    );

    //- Destructor
    virtual ~DILUPreconditioner()
    {}

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
    ) const;
};

} // End namespace CML

#endif

