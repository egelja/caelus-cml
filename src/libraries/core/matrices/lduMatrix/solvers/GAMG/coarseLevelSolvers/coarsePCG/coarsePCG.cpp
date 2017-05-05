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

\*---------------------------------------------------------------------------*/

#include "coarsePCG.hpp"

namespace CML
{
    defineTypeNameAndDebug(CoarsePCG, 0);

    lduMatrix::solver::addsymMatrixConstructorToTable<CoarsePCG>
        addCoarsePCGSymMatrixConstructorToTable_;
}

CML::dictionary CML::CoarsePCG::solverDict
(
    const scalar tol,
    const scalar relTol
)
{
    dictionary dict(IStringStream("solver PCG; preconditioner SSGS;")());
    dict.add("tolerance", tol);
    dict.add("relTol", relTol);

    return dict;
}


CML::dictionary CML::CoarsePCG::solverDict
(
    Istream& is
)
{
    scalar tol(readScalar(is));
    scalar relTol(readScalar(is));

    return solverDict(tol, relTol);
}

CML::CoarsePCG::CoarsePCG
(
    word const& fieldName,
    lduMatrix const& matrix,
    FieldField<Field, scalar> const& interfaceBouCoeffs,
    FieldField<Field, scalar> const& interfaceIntCoeffs,
    lduInterfaceFieldPtrsList const& interfaces,
    dictionary const& solverControls
) :
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

CML::CoarsePCG::CoarsePCG
(
    word const& fieldName,
    lduMatrix const& matrix,
    FieldField<Field, scalar> const& interfaceBouCoeffs,
    FieldField<Field, scalar> const& interfaceIntCoeffs,
    lduInterfaceFieldPtrsList const& interfaces,
    scalar const tolerance,
    scalar const relTol
) :
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
