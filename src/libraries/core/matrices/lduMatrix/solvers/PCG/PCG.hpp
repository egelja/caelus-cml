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
    CML::PCG

Description
    Preconditioned conjugate gradient solver for symmetric lduMatrices
    using a run-time selectable preconditioner.

SourceFiles
    PCG.cpp

References

    [1] Templates for the Solution of Linear Systems: Building Blocks
        for Iterative Methods, R. Barrett, M. Barry, T.F. Chan, J. Demmel,
        J. Donato, J. Dongarra, V. Eijkhout, R. Pozo, C. Romine, and
        Van der Vorst, SIAM, 1994, Philadephia, PA, 2nd edition

    [2] Iterative Methods for Sparse Linear Systems, Y. Saad, SIAM, 2003,
        Philadephia, PA, 2nd edition

\*---------------------------------------------------------------------------*/

#ifndef PCG_H
#define PCG_H

#include "lduMatrix.hpp"

namespace CML
{

class PCG : public lduMatrix::solver
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    PCG(const PCG&);

    //- Disallow default bitwise assignment
    void operator=(const PCG&);

public:

    //- Runtime type information
    TypeName("PCG");

    // Constructors

    //- Construct from matrix components and solver controls
    PCG
    (
        word const& fieldName,
        lduMatrix const& matrix,
        FieldField<Field, scalar> const& interfaceBouCoeffs,
        FieldField<Field, scalar> const& interfaceIntCoeffs,
        lduInterfaceFieldPtrsList const& interfaces,
        dictionary const& solverControls
    );

    //- Destructor
    virtual ~PCG() {}

    // Member Functions

    //- Solve the matrix with this solver
    virtual solverPerformance solve
    (
        scalarField& x,
        scalarField const& b,
        direction const cmpt=0
    ) const;
};


}

#endif

