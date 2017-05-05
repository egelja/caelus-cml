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

    CML::SPAI0Smoother

Description

    Efficient smoother based on ILU(0) algorithm with modified diagonal

Author

    Aleksandar Jemcov

SourceFiles

    SPAI0Smoother.cpp

References

    [1] Templates for the Solution of Linear Systems: Building Blocks
        for Iterative Methods, R. Barrett, M. Barry, T.F. Chan, J. Demmel,
        J. Donato, J. Dongarra, V. Eijkhout, R. Pozo, C. Romine, and
        Van der Vorst, SIAM, 1994, Philadephia, PA, 2nd edition

    [2] Iterative Methods for Sparse Linear Systems, Y. Saad, SIAM, 2003,
        Philadephia, PA, 2nd edition


\*---------------------------------------------------------------------------*/

#ifndef SPAI0Smoother_H
#define SPAI0Smoother_H

#include "lduMatrix.hpp"

namespace CML
{

class SPAI0Smoother : public lduMatrix::smoother
{
    //- The reciprocal preconditioned diagonal
    scalarField rD_;

public:

    //- Runtime type information
    TypeName("SPAI0");


    // Constructors

    //- Construct from matrix components
    SPAI0Smoother
    (
        word const& fieldName,
        lduMatrix const& matrix,
        FieldField<Field, scalar> const& interfaceBouCoeffs,
        FieldField<Field, scalar> const& interfaceIntCoeffs,
        lduInterfaceFieldPtrsList const& interfaces
    );

    // Member Functions

    //- Smooth the solution for a given number of sweeps
    void smooth
    (
        scalarField& psi,
        scalarField const& source,
        direction const cmpt,
        label const nSweeps
    ) const;
};

} // End namespace CML

#endif

