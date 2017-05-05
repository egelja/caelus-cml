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

#include "SIBS.hpp"
#include "simpleMatrix.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void CML::SIBS::SIMPR
(
    const ODE& ode,
    const scalar xStart,
    const scalarField& y,
    const scalarField& dydx,
    const scalarField& dfdx,
    const scalarSquareMatrix& dfdy,
    const scalar deltaX,
    const label nSteps,
    scalarField& yEnd
) const
{
    scalar h = deltaX/nSteps;

    scalarSquareMatrix a(n_);
    for (register label i=0; i<n_; i++)
    {
        for (register label j=0; j<n_; j++)
        {
            a[i][j] = -h*dfdy[i][j];
        }
        ++a[i][i];
    }

    labelList pivotIndices(n_);
    scalarSquareMatrix::LUDecompose(a, pivotIndices);

    for (register label i=0; i<n_; i++)
    {
        yEnd[i] = h*(dydx[i] + h*dfdx[i]);
    }

    scalarSquareMatrix::LUBacksubstitute(a, pivotIndices, yEnd);

    scalarField del(yEnd);
    scalarField ytemp(n_);

    for (register label i=0; i<n_; i++)
    {
        ytemp[i] = y[i] + del[i];
    }

    scalar x = xStart + h;

    ode.derivatives(x, ytemp, yEnd);

    for (register label nn=2; nn<=nSteps; nn++)
    {
        for (register label i=0; i<n_; i++)
        {
            yEnd[i] = h*yEnd[i] - del[i];
        }

        scalarSquareMatrix::LUBacksubstitute(a, pivotIndices, yEnd);

        for (register label i=0; i<n_; i++)
        {
            ytemp[i] += (del[i] += 2.0*yEnd[i]);
        }

        x += h;

        ode.derivatives(x, ytemp, yEnd);
    }
    for (register label i=0; i<n_; i++)
    {
        yEnd[i] = h*yEnd[i] - del[i];
    }

    scalarSquareMatrix::LUBacksubstitute(a, pivotIndices, yEnd);

    for (register label i=0; i<n_; i++)
    {
        yEnd[i] += ytemp[i];
    }
}


// ************************************************************************* //
