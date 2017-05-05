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
    CML::diagonalPreconditioner

Description
    Diagonal (Jacobi) preconditioner for both symmetric and 
    asymmetric matrices.

    The reciprocal of the diagonal is calculated and stored for reuse
    because on most systems '*' is faster than '/'.

SourceFiles
    diagonalPreconditioner.cpp

References

    [1] Templates for the Solution of Linear Systems: Building Blocks
        for Iterative Methods, R. Barrett, M. Barry, T.F. Chan, J. Demmel,
        J. Donato, J. Dongarra, V. Eijkhout, R. Pozo, C. Romine, and
        Van der Vorst, SIAM, 1994, Philadephia, PA, 2nd edition

    [2] Iterative Methods for Sparse Linear Systems, Y. Saad, SIAM, 2003,
        Philadephia, PA, 2nd edition

\*---------------------------------------------------------------------------*/

#ifndef diagonalPreconditioner_H
#define diagonalPreconditioner_H

#include "lduMatrix.hpp"

namespace CML
{

class diagonalPreconditioner : public lduMatrix::preconditioner
{
    // Private data

    //- The reciprocal diagonal
    scalarField rD_;

    // Private Member Functions

    //- Disallow default bitwise copy construct
    diagonalPreconditioner(const diagonalPreconditioner&);

    //- Disallow default bitwise assignment
    void operator=(const diagonalPreconditioner&);

public:

    //- Runtime type information
    TypeName("diagonal");

    //- Construct from matrix components and preconditioner solver controls
    diagonalPreconditioner
    (
        lduMatrix::solver const&,
        dictionary const& solverControlsUnused
    );

    virtual ~diagonalPreconditioner()
    {}

    // Member Functions

    void appInv();
  
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

