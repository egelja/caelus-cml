/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011 OpenFOAM Foundation
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

InClass
    CML::fvMatrix

Description
    A scalar instance of fvMatrix

SourceFiles
    fvScalarMatrix.cpp

\*---------------------------------------------------------------------------*/

#ifndef fvScalarMatrix_H
#define fvScalarMatrix_H

#include "fvMatrix.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Set reference level for a component of the solution
// on a given patch face
template<>
void fvMatrix<scalar>::setComponentReference
(
    const label patchi,
    const label facei,
    const direction,
    const scalar value
);

template<>
autoPtr<fvMatrix<scalar>::fvSolver> fvMatrix<scalar>::solver
(
    const dictionary&
);

template<>
solverPerformance fvMatrix<scalar>::fvSolver::solve
(
    const dictionary&
);

template<>
solverPerformance fvMatrix<scalar>::solve
(
    const dictionary&
);

template<>
tmp<scalarField> fvMatrix<scalar>::residual() const;

template<>
tmp<volScalarField> fvMatrix<scalar>::H() const;

template<>
tmp<volScalarField> fvMatrix<scalar>::H1() const;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
