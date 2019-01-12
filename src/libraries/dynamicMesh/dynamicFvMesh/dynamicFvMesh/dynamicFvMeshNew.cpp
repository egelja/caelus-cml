/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

#include "staticFvMesh.hpp"

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::dynamicFvMesh> CML::dynamicFvMesh::New(const IOobject& io)
{
    // Note: - do not register the dictionary since dynamicFvMeshes themselves
    // do this.
    // - defaultRegion (region0) gets loaded from constant, other ones
    //   get loaded from constant/<regionname>. Normally we'd use
    //   polyMesh::dbDir() but we haven't got a polyMesh yet ...
    IOobject dictHeader
    (
        "dynamicMeshDict",
        io.time().constant(),
        (io.name() == polyMesh::defaultRegion ? "" : io.name()),
        io.db(),
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
    );

    if (dictHeader.headerOk())
    {
        IOdictionary dict(dictHeader);

        const word dynamicFvMeshTypeName(dict.lookup("dynamicFvMesh"));

        Info<< "Selecting dynamicFvMesh " << dynamicFvMeshTypeName << endl;

        const_cast<Time&>(io.time()).libs().open
        (
            dict,
            "dynamicFvMeshLibs",
            IOobjectConstructorTablePtr_
        );

        if (!IOobjectConstructorTablePtr_)
        {
            FatalErrorInFunction
                << "dynamicFvMesh table is empty"
                << exit(FatalError);
        }

        IOobjectConstructorTable::iterator cstrIter =
            IOobjectConstructorTablePtr_->find(dynamicFvMeshTypeName);

        if (cstrIter == IOobjectConstructorTablePtr_->end())
        {
            FatalErrorInFunction
                << "Unknown dynamicFvMesh type "
                << dynamicFvMeshTypeName << nl << nl
                << "Valid dynamicFvMesh types are :" << endl
                << IOobjectConstructorTablePtr_->sortedToc()
                << exit(FatalError);
        }

        return autoPtr<dynamicFvMesh>(cstrIter()(io));
    }
    else
    {
        Info<< "Selecting staticFvMesh " << endl;
        return autoPtr<dynamicFvMesh>(new staticFvMesh(io));
    }
}


// ************************************************************************* //
