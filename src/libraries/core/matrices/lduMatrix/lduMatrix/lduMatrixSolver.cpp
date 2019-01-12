/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
Copyright (C) 2016 OpenCFD Ltd
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
#include "diagonalSolver.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineRunTimeSelectionTable(lduMatrix::solver, symMatrix);
    defineRunTimeSelectionTable(lduMatrix::solver, asymMatrix);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::lduMatrix::solver> CML::lduMatrix::solver::New
(
    const word& fieldName,
    const lduMatrix& matrix,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const FieldField<Field, scalar>& interfaceIntCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const dictionary& solverControls
)
{
    const word name(solverControls.lookup("solver"));

    if (matrix.diagonal())
    {
        return autoPtr<lduMatrix::solver>
        (
            new diagonalSolver
            (
                fieldName,
                matrix,
                interfaceBouCoeffs,
                interfaceIntCoeffs,
                interfaces,
                solverControls
            )
        );
    }
    else if (matrix.symmetric())
    {
        symMatrixConstructorTable::iterator constructorIter =
            symMatrixConstructorTablePtr_->find(name);

        if (constructorIter == symMatrixConstructorTablePtr_->end())
        {
            FatalIOErrorInFunction(solverControls)
                << "Unknown symmetric matrix solver " << name << nl << nl
                << "Valid symmetric matrix solvers are :" << endl
                << symMatrixConstructorTablePtr_->sortedToc()
                << exit(FatalIOError);
        }

        return autoPtr<lduMatrix::solver>
        (
            constructorIter()
            (
                fieldName,
                matrix,
                interfaceBouCoeffs,
                interfaceIntCoeffs,
                interfaces,
                solverControls
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
                << "Unknown asymmetric matrix solver " << name << nl << nl
                << "Valid asymmetric matrix solvers are :" << endl
                << asymMatrixConstructorTablePtr_->sortedToc()
                << exit(FatalIOError);
        }

        return autoPtr<lduMatrix::solver>
        (
            constructorIter()
            (
                fieldName,
                matrix,
                interfaceBouCoeffs,
                interfaceIntCoeffs,
                interfaces,
                solverControls
            )
        );
    }
    else
    {
        FatalIOErrorInFunction(solverControls)
            << "cannot solve incomplete matrix, "
               "no diagonal or off-diagonal coefficient"
            << exit(FatalIOError);

        return autoPtr<lduMatrix::solver>(nullptr);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::lduMatrix::solver::solver
(
    const word& fieldName,
    const lduMatrix& matrix,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const FieldField<Field, scalar>& interfaceIntCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const dictionary& solverControls
)
:
    fieldName_(fieldName),
    matrix_(matrix),
    interfaceBouCoeffs_(interfaceBouCoeffs),
    interfaceIntCoeffs_(interfaceIntCoeffs),
    interfaces_(interfaces),
    controlDict_(solverControls),
    profiling_("lduMatrix::solver." + fieldName)
{
    readControls();

    if (maxIter_ < minIter_)
    {
        FatalErrorInFunction
            << "maxIter < minIter : Check linear solver controls in fvSolution and make sure to set maxIter value to be larger or equal to minIter"
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::lduMatrix::solver::readControls()
{
    maxIter_   = controlDict_.lookupOrDefault<label>("maxIter", 1000);
    minIter_   = controlDict_.lookupOrDefault<label>("minIter", 1);
    eps_       = controlDict_.lookupOrDefault<scalar>("eps", 1e-6);
    tolerance_ = controlDict_.lookupOrDefault<scalar>("tolerance", 1e-6);
    relTol_    = controlDict_.lookupOrDefault<scalar>("relTol", 0);
}


void CML::lduMatrix::solver::read(const dictionary& solverControls)
{
    controlDict_ = solverControls;
    readControls();
}


CML::scalar CML::lduMatrix::solver::normFactor
(
    const scalarField& psi,
    const scalarField& source,
    const scalarField& Apsi,
    scalarField& tmpField
) const
{
    // --- Calculate A dot reference value of psi
    matrix_.sumA(tmpField, interfaceBouCoeffs_, interfaces_);
    tmpField *= gAverage(psi);

    return gSum(mag(Apsi - tmpField) + mag(source - tmpField)) + matrix_.small_;

    // At convergence this simpler method is equivalent to the above
    // return 2*gSumMag(source) + matrix_.small_;
}


// ************************************************************************* //
