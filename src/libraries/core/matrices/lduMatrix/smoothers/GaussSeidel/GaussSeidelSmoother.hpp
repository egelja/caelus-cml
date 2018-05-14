/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
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
    CML::GaussSeidelSmoother

Description
    Forward Gauss-Seidel smoother. Implements the following equation:
    xNew = inv(L)*(b-U*xOld)

SourceFiles
    GaussSeidelSmoother.cpp

References

    [1] Templates for the Solution of Linear Systems: Building Blocks
        for Iterative Methods, R. Barrett, M. Barry, T.F. Chan, J. Demmel,
        J. Donato, J. Dongarra, V. Eijkhout, R. Pozo, C. Romine, and
        Van der Vorst, SIAM, 1994, Philadephia, PA, 2nd edition

    [2] Iterative Methods for Sparse Linear Systems, Y. Saad, SIAM, 2003,
        Philadephia, PA, 2nd edition

\*---------------------------------------------------------------------------*/

#ifndef GaussSeidelSmoother_H
#define GaussSeidelSmoother_H

#include "lduMatrix.hpp"

namespace CML
{

class GaussSeidelSmoother
:
    public lduMatrix::smoother
{

public:

    //- Runtime type information
    TypeName("GaussSeidel");


    // Constructors

        //- Construct from components
        GaussSeidelSmoother
        (
            const word& fieldName,
            const lduMatrix& matrix,
            const FieldField<Field, scalar>& interfaceBouCoeffs,
            const FieldField<Field, scalar>& interfaceIntCoeffs,
            const lduInterfaceFieldPtrsList& interfaces
        );


    // Member Functions

        //- Smooth for the given number of sweeps
        static void smooth
        (
            const word& fieldName,
            scalarField& x,
            const lduMatrix& matrix,
            const scalarField& b,
            const FieldField<Field, scalar>& interfaceBouCoeffs,
            const lduInterfaceFieldPtrsList& interfaces,
            const direction cmpt,
            const label nSweeps
        );


        //- Smooth the solution for a given number of sweeps
        virtual void smooth
        (
            scalarField& x,
            const scalarField& B,
            const direction cmpt,
            const label nSweeps
        ) const;
};


} // End namespace CML

#endif


