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

#include "GAMGSolver.hpp"
#include "vector2D.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::scalar CML::GAMGSolver::scalingFactor
(
    scalarField& field,
    const scalarField& source,
    const scalarField& Acf,
    const scalarField& D
) const
{
    scalar scalingFactorNum = 0.0;
    scalar scalingFactorDenom = 0.0;

    forAll(field, i)
    {
        scalingFactorNum += source[i]*field[i];
        scalingFactorDenom += Acf[i]*field[i];
        field[i] += (source[i] - Acf[i])/D[i];
    }

    vector2D scalingVector(scalingFactorNum, scalingFactorDenom);
    reduce(scalingVector, sumOp<vector2D>());

    if
    (
        scalingVector.x()*scalingVector.y() <= scalar(0.)
        ||
        mag(scalingVector.x()) < mag(scalingVector.y())
    )
    {
        return scalar(1.);
    }
    else if (mag(scalingVector.x()) > scalar(2.)*mag(scalingVector.y()))
    {
        return scalar(2.);
    }
    else
    {
        return scalar(scalingVector.x()/scalingVector.y());
    }
        
}


CML::scalar CML::GAMGSolver::scalingFactor
(
    scalarField& Acf,
    const lduMatrix& A,
    scalarField& field,
    const FieldField<Field, scalar>& interfaceLevelBouCoeffs,
    const lduInterfaceFieldPtrsList& interfaceLevel,
    const scalarField& source,
    const direction cmpt
) const
{
    A.Amul
    (
        Acf,
        field,
        interfaceLevelBouCoeffs,
        interfaceLevel,
        cmpt
    );

    return scalingFactor
    (
        field,
        source,
        Acf,
        A.diag()
    );
}


// ************************************************************************* //
