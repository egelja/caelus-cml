/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

#include "fvScalarMatrix.hpp"
#include "extrapolatedCalculatedFvPatchFields.hpp"
#include "profiling.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<>
void CML::fvMatrix<CML::scalar>::setComponentReference
(
    const label patchi,
    const label facei,
    const direction,
    const scalar value
)
{
    if (psi_.needReference())
    {
        if (Pstream::master())
        {
            internalCoeffs_[patchi][facei] +=
                diag()[psi_.mesh().boundary()[patchi].faceCells()[facei]];

            boundaryCoeffs_[patchi][facei] +=
                diag()[psi_.mesh().boundary()[patchi].faceCells()[facei]]
               *value;
        }
    }
}


template<>
CML::autoPtr<CML::fvMatrix<CML::scalar>::fvSolver>
CML::fvMatrix<CML::scalar>::solver
(
    const dictionary& solverControls
)
{
    addProfiling(solve, "fvMatrix::solve." + psi_.name());

    if (debug)
    {
        Info<< "fvMatrix<scalar>::solver(const dictionary& solverControls) : "
               "solver for fvMatrix<scalar>"
            << endl;
    }

    scalarField saveDiag(diag());
    addBoundaryDiag(diag(), 0);

    autoPtr<fvMatrix<scalar>::fvSolver> solverPtr
    (
        new fvMatrix<scalar>::fvSolver
        (
            *this,
            lduMatrix::solver::New
            (
                psi_.name(),
                *this,
                boundaryCoeffs_,
                internalCoeffs_,
                psi_.boundaryField().interfaces(),
                solverControls
            )
        )
    );

    diag() = saveDiag;

    return solverPtr;
}


template<>
CML::solverPerformance CML::fvMatrix<CML::scalar>::fvSolver::solve
(
    const dictionary& solverControls
)
{
    GeometricField<scalar, fvPatchField, volMesh>& psi =
        const_cast<GeometricField<scalar, fvPatchField, volMesh>&>
        (fvMat_.psi());

    scalarField saveDiag(fvMat_.diag());
    fvMat_.addBoundaryDiag(fvMat_.diag(), 0);

    scalarField totalSource(fvMat_.source());
    fvMat_.addBoundarySource(totalSource, false);

    // assign new solver controls
    solver_->read(solverControls);

    solverPerformance solverPerf = solver_->solve
    (
        psi.internalField(),
        totalSource
    );

    solverPerf.print();

    fvMat_.diag() = saveDiag;

    psi.correctBoundaryConditions();

    psi.mesh().setSolverPerformance(psi.name(), solverPerf);

    return solverPerf;
}


template<>
CML::solverPerformance CML::fvMatrix<CML::scalar>::solve
(
    const dictionary& solverControls
)
{
    if (debug)
    {
        Info<< "fvMatrix<scalar>::solve(const dictionary& solverControls) : "
               "solving fvMatrix<scalar>"
            << endl;
    }

    GeometricField<scalar, fvPatchField, volMesh>& psi =
       const_cast<GeometricField<scalar, fvPatchField, volMesh>&>(psi_);

    scalarField saveDiag(diag());
    addBoundaryDiag(diag(), 0);

    scalarField totalSource(source_);
    addBoundarySource(totalSource, false);

    // Solver call
    solverPerformance solverPerf = lduMatrix::solver::New
    (
        psi.name(),
        *this,
        boundaryCoeffs_,
        internalCoeffs_,
        psi.boundaryField().interfaces(),
        solverControls
    )->solve(psi.internalField(), totalSource);

    solverPerf.print();

    diag() = saveDiag;

    psi.correctBoundaryConditions();

    psi.mesh().setSolverPerformance(psi.name(), solverPerf);

    return solverPerf;
}


template<>
CML::tmp<CML::scalarField> CML::fvMatrix<CML::scalar>::residual() const
{
    scalarField boundaryDiag(psi_.size(), 0.0);
    addBoundaryDiag(boundaryDiag, 0);

    tmp<scalarField> tres
    (
        lduMatrix::residual
        (
            psi_.internalField(),
            source_ - boundaryDiag*psi_.internalField(),
            boundaryCoeffs_,
            psi_.boundaryField().interfaces(),
            0
        )
    );

    addBoundarySource(tres());

    return tres;
}


template<>
CML::tmp<CML::volScalarField> CML::fvMatrix<CML::scalar>::H() const
{
    tmp<volScalarField> tHphi
    (
        new volScalarField
        (
            IOobject
            (
                "H("+psi_.name()+')',
                psi_.instance(),
                psi_.mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            psi_.mesh(),
            dimensions_/dimVol,
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );
    volScalarField& Hphi = tHphi();

    Hphi.internalField() = (lduMatrix::H(psi_.internalField()) + source_);
    addBoundarySource(Hphi.internalField());

    Hphi.internalField() /= psi_.mesh().V();
    Hphi.correctBoundaryConditions();

    return tHphi;
}


template<>
CML::tmp<CML::volScalarField> CML::fvMatrix<CML::scalar>::H1() const
{
    tmp<volScalarField> tH1
    (
        new volScalarField
        (
            IOobject
            (
                "H(1)",
                psi_.instance(),
                psi_.mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            psi_.mesh(),
            dimensions_/(dimVol*psi_.dimensions()),
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );
    volScalarField& H1_ = tH1();

    H1_.internalField() = lduMatrix::H1();
    //addBoundarySource(Hphi.internalField());

    H1_.internalField() /= psi_.mesh().V();
    H1_.correctBoundaryConditions();

    return tH1;
}


// ************************************************************************* //
