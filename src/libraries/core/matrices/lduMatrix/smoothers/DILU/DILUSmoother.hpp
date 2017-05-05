/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::DILUSmoother

Description
    Simplified diagonal-based incomplete LU smoother for asymmetric matrices.

    To improve efficiency, the residual is evaluated after every nSweeps
    sweeps.

SourceFiles
    DILUSmoother.cpp

\*---------------------------------------------------------------------------*/

#ifndef DILUSmoother_H
#define DILUSmoother_H

#include "lduMatrix.hpp"

namespace CML
{

class DILUSmoother : public lduMatrix::smoother
{
    // Private data

    //- The reciprocal preconditioned diagonal
    scalarField rD_;

public:

    //- Runtime type information
    TypeName("DILU");

    // Constructors

    //- Construct from matrix components
    DILUSmoother
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
