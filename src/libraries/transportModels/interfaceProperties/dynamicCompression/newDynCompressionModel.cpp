/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2017 Applied CCM
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "error.hpp"
#include "dynCompressionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::dynCompressionModel> CML::dynCompressionModel::New
(
    const fvMesh& mesh,
    const volVectorField& U,
    const surfaceScalarField& phi,
    const volScalarField& alpha1
)
{
    word typeName = mesh.solverDict(alpha1.name()).lookup("dynamicCompressionModel");

    Info<< "Selecting Dynamic Interface Compression model " << typeName << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(typeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown Dynamic Interface Compression model type " << typeName
            << endl << endl
            << "Valid Dynamic Interface Compression model types are :" << endl
            << dictionaryConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<dynCompressionModel>(cstrIter()(mesh, U, phi, alpha1));
}


// ************************************************************************* //
