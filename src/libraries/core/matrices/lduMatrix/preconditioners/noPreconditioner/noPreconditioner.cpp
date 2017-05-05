/*---------------------------------------------------------------------------*\
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

\*---------------------------------------------------------------------------*/

#include "noPreconditioner.hpp"
#include "restrict.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(noPreconditioner, 0);

    lduMatrix::preconditioner::
        addsymMatrixConstructorToTable<noPreconditioner>
        addnoPreconditionerSymMatrixConstructorToTable_;

    lduMatrix::preconditioner::
        addasymMatrixConstructorToTable<noPreconditioner>
        addnoPreconditionerAsymMatrixConstructorToTable_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::noPreconditioner::noPreconditioner
(
    const lduMatrix::solver& sol,
    const dictionary&
)
:
    lduMatrix::preconditioner(sol)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::noPreconditioner::precondition
(
    scalarField& wA,
    const scalarField& rA,
    const direction
) const
{
    scalar* RESTRICT wAPtr = wA.begin();
    const scalar* RESTRICT rAPtr = rA.begin();

    register label nCells = wA.size();

    for (register label cell=0; cell<nCells; cell++)
    {
        wAPtr[cell] = rAPtr[cell];
    }
}


// ************************************************************************* //
