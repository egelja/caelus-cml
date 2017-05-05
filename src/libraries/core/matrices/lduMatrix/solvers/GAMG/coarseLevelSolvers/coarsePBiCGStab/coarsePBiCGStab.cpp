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

\*---------------------------------------------------------------------------*/

#include "coarsePBiCGStab.hpp"

namespace CML
{
    defineTypeNameAndDebug(CoarsePBiCGStab, 0);

    lduMatrix::solver::addasymMatrixConstructorToTable<CoarsePBiCGStab>
        addCoarsePBiCGStabSymMatrixConstructorToTable_;
}


CML::dictionary CML::CoarsePBiCGStab::solverDict
(
    scalar const tol,
    scalar const relTol
)
{
    dictionary dict(IStringStream("solver PBiCGStab; preconditioner USGS;")());
    dict.add("tolerance", tol);
    dict.add("relTol", relTol);

    return dict;
}

CML::dictionary CML::CoarsePBiCGStab::solverDict
(
    Istream& is
)
{
    scalar tol(readScalar(is));
    scalar relTol(readScalar(is));

    return solverDict(tol, relTol);
}

CML::CoarsePBiCGStab::CoarsePBiCGStab
(
    word const& fieldName,
    lduMatrix const& matrix,
    FieldField<Field, scalar> const& interfaceBouCoeffs,
    FieldField<Field, scalar> const& interfaceIntCoeffs,
    lduInterfaceFieldPtrsList const& interfaces,
    dictionary const& solverControls
) :
    PBiCGStab
    (
        fieldName,
        matrix,
        interfaceBouCoeffs,
        interfaceIntCoeffs,
        interfaces,
        solverControls
    )
{}

CML::CoarsePBiCGStab::CoarsePBiCGStab
(
    word const& fieldName,
    lduMatrix const& matrix,
    FieldField<Field, scalar> const& interfaceBouCoeffs,
    FieldField<Field, scalar> const& interfaceIntCoeffs,
    lduInterfaceFieldPtrsList const& interfaces,
    scalar const tolerance,
    scalar const relTol
) :
    PBiCGStab
    (
        fieldName,
        matrix,
        interfaceBouCoeffs,
        interfaceIntCoeffs,
        interfaces,
        solverDict(tolerance, relTol)
    )
{}
