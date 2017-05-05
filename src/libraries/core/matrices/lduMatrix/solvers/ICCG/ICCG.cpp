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

#include "ICCG.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(ICCG, 0);

    lduMatrix::solver::addsymMatrixConstructorToTable<ICCG>
        addICCGSymMatrixConstructorToTable_;
}

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

CML::dictionary CML::ICCG::solverDict
(
    const scalar tol,
    const scalar relTol
)
{
    dictionary dict(IStringStream("solver PCG; preconditioner DIC;")());
    dict.add("tolerance", tol);
    dict.add("relTol", relTol);

    return dict;
}


CML::dictionary CML::ICCG::solverDict
(
    Istream& is
)
{
    scalar tol(readScalar(is));
    scalar relTol(readScalar(is));

    return solverDict(tol, relTol);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::ICCG::ICCG
(
    const word& fieldName,
    const lduMatrix& matrix,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const FieldField<Field, scalar>& interfaceIntCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const dictionary& solverControls
)
:
    PCG
    (
        fieldName,
        matrix,
        interfaceBouCoeffs,
        interfaceIntCoeffs,
        interfaces,
        solverControls
    )
{}


CML::ICCG::ICCG
(
    const word& fieldName,
    const lduMatrix& matrix,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const FieldField<Field, scalar>& interfaceIntCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const scalar tolerance,
    const scalar relTol
)
:
    PCG
    (
        fieldName,
        matrix,
        interfaceBouCoeffs,
        interfaceIntCoeffs,
        interfaces,
        solverDict(tolerance, relTol)
    )
{}

// ************************************************************************* //
