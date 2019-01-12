/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "lduMatrix.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineRunTimeSelectionTable(lduMatrix::smoother, symMatrix);
    defineRunTimeSelectionTable(lduMatrix::smoother, asymMatrix);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::word
CML::lduMatrix::smoother::getName
(
    const dictionary& solverControls
)
{
    word name;

    // handle primitive or dictionary entry
    const entry& e = solverControls.lookupEntry("smoother", false, false);
    if (e.isDict())
    {
        e.dict().lookup("smoother") >> name;
    }
    else
    {
        e.stream() >> name;
    }

    return name;
}


CML::autoPtr<CML::lduMatrix::smoother> CML::lduMatrix::smoother::New
(
    const word& fieldName,
    const lduMatrix& matrix,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const FieldField<Field, scalar>& interfaceIntCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const dictionary& solverControls
)
{
    word name;

    // handle primitive or dictionary entry
    const entry& e = solverControls.lookupEntry("smoother", false, false);
    if (e.isDict())
    {
        e.dict().lookup("smoother") >> name;
    }
    else
    {
        e.stream() >> name;
    }

    // not (yet?) needed:
    // const dictionary& controls = e.isDict() ? e.dict() : dictionary::null;

    if (matrix.symmetric())
    {
        symMatrixConstructorTable::iterator constructorIter =
            symMatrixConstructorTablePtr_->find(name);

        if (constructorIter == symMatrixConstructorTablePtr_->end())
        {
            FatalIOErrorInFunction(solverControls)
                << "Unknown symmetric matrix smoother "
                << name << nl << nl
                << "Valid symmetric matrix smoothers are :" << endl
                << symMatrixConstructorTablePtr_->sortedToc()
                << exit(FatalIOError);
        }

        return autoPtr<lduMatrix::smoother>
        (
            constructorIter()
            (
                fieldName,
                matrix,
                interfaceBouCoeffs,
                interfaceIntCoeffs,
                interfaces
            )
        );
    }
    else if (matrix.asymmetric())
    {
        asymMatrixConstructorTable::iterator constructorIter =
            asymMatrixConstructorTablePtr_->find(name);

        if (constructorIter == asymMatrixConstructorTablePtr_->end())
        {
            FatalIOErrorInFunction(solverControls)
                << "Unknown asymmetric matrix smoother "
                << name << nl << nl
                << "Valid asymmetric matrix smoothers are :" << endl
                << asymMatrixConstructorTablePtr_->sortedToc()
                << exit(FatalIOError);
        }

        return autoPtr<lduMatrix::smoother>
        (
            constructorIter()
            (
                fieldName,
                matrix,
                interfaceBouCoeffs,
                interfaceIntCoeffs,
                interfaces
            )
        );
    }
    else
    {
        FatalIOErrorInFunction(solverControls)
            << "cannot solve incomplete matrix, "
               "no diagonal or off-diagonal coefficient"
            << exit(FatalIOError);

        return autoPtr<lduMatrix::smoother>(nullptr);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::lduMatrix::smoother::smoother
(
    const word& fieldName,
    const lduMatrix& matrix,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const FieldField<Field, scalar>& interfaceIntCoeffs,
    const lduInterfaceFieldPtrsList& interfaces
)
:
    fieldName_(fieldName),
    matrix_(matrix),
    interfaceBouCoeffs_(interfaceBouCoeffs),
    interfaceIntCoeffs_(interfaceIntCoeffs),
    interfaces_(interfaces)
{}


// ************************************************************************* //
