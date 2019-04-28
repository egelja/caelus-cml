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
#include "twoDPointCorrector.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(motionSolver, 0);
    defineRunTimeSelectionTable(motionSolver, dictionary);
}

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

CML::IOobject CML::motionSolver::stealRegistration
(
    const IOdictionary& dict
)
{
    IOobject io(dict);
    if (dict.registerObject())
    {
        // De-register if necessary
        const_cast<IOdictionary&>(dict).checkOut();

        io.registerObject() = true;
    }

    return io;
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
            IOobject::AUTO_WRITE
        )
    ),
    mesh_(mesh)
{}


CML::motionSolver::motionSolver
(
    const polyMesh& mesh,
    const IOdictionary& dict,
    const word& type
)
:
    IOdictionary(stealRegistration(dict), dict),
    mesh_(mesh),
    coeffDict_(dict.subDict(type + "Coeffs"))
{}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::motionSolver> CML::motionSolver::New
(
    const polyMesh& mesh,
    const IOdictionary& solverDict
)
{
    const word solverTypeName(solverDict.lookup("solver"));

    Info<< "Selecting motion solver: " << solverTypeName << endl;

    const_cast<Time&>(mesh.time()).libs().open
    (
        solverDict,
        "motionSolverLibs",
        dictionaryConstructorTablePtr_
    );

    if (!dictionaryConstructorTablePtr_)
    {
        FatalErrorInFunction
            << "solver table is empty"
            << exit(FatalError);
    }

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(solverTypeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown solver type "
            << solverTypeName << nl << nl
            << "Valid solver types are:" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<motionSolver>(cstrIter()(mesh, solverDict));
}


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
            IOobject::AUTO_WRITE
        )
    );

    return New(mesh, solverDict);
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
    twoDPointCorrector::New(mesh_).correctPoints(p);
}


void CML::motionSolver::updateMesh(const mapPolyMesh& mpm)
{}


bool CML::motionSolver::writeObject
(
    IOstream::streamFormat fmt,
    IOstream::versionNumber ver,
    IOstream::compressionType cmp
) const
{
    return true;
}


bool CML::motionSolver::read()
{
    if (regIOobject::read())
    {
        coeffDict_ = subDict(type() + "Coeffs");

        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
