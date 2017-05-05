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

#include "motionSolver.hpp"
#include "Time.hpp"
#include "polyMesh.hpp"
#include "dlLibraryTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(motionSolver, 0);
    defineRunTimeSelectionTable(motionSolver, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::motionSolver::motionSolver(const polyMesh& mesh)
:
    IOdictionary
    (
        IOobject
        (
            "dynamicMeshDict",
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    ),
    mesh_(mesh),
    twoDPointCorrector_(mesh)
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::motionSolver> CML::motionSolver::New(const polyMesh& mesh)
{
    IOdictionary solverDict
    (
        IOobject
        (
            "dynamicMeshDict",
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE,
            false
        )
    );

    Istream& msData = solverDict.lookup("solver");

    const word solverTypeName(msData);

    Info<< "Selecting motion solver: " << solverTypeName << endl;

    const_cast<Time&>(mesh.time()).libs().open
    (
        solverDict,
        "motionSolverLibs",
        dictionaryConstructorTablePtr_
    );

    if (!dictionaryConstructorTablePtr_)
    {
        FatalErrorIn
        (
            "motionSolver::New(const polyMesh& mesh)"
        )   << "solver table is empty"
            << exit(FatalError);
    }

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(solverTypeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "motionSolver::New(const polyMesh& mesh)"
        )   << "Unknown solver type "
            << solverTypeName << nl << nl
            << "Valid solver types are:" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<motionSolver>(cstrIter()(mesh, msData));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::motionSolver::~motionSolver()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::pointField> CML::motionSolver::newPoints()
{
    solve();
    return curPoints();
}


void CML::motionSolver::twoDCorrectPoints(pointField& p) const
{
    twoDPointCorrector_.correctPoints(p);
}


void CML::motionSolver::updateMesh(const mapPolyMesh& mpm)
{
    twoDPointCorrector_.updateMesh();
}


// ************************************************************************* //
