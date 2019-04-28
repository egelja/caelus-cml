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

#include "polyPatch.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::autoPtr<CML::polyPatch> CML::polyPatch::New
(
    const word& patchType,
    const word& name,
    const label size,
    const label start,
    const label index,
    const polyBoundaryMesh& bm
)
{
    if (debug)
    {
        Info<< "polyPatch::New(const word&, const word&, const label, "
               "const label, const label, const polyBoundaryMesh&) : "
               "constructing polyPatch"
            << endl;
    }

    wordConstructorTable::iterator cstrIter =
        wordConstructorTablePtr_->find(patchType);

    if (cstrIter == wordConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown polyPatch type "
            << patchType << " for patch " << name << nl << nl
            << "Valid polyPatch types are :" << endl
            << wordConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<polyPatch>
    (
        cstrIter()
        (
            name,
            size,
            start,
            index,
            bm,
            patchType
        )
    );
}


CML::autoPtr<CML::polyPatch> CML::polyPatch::New
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyBoundaryMesh& bm
)
{
    if (debug)
    {
        Info<< "polyPatch::New(const word&, const dictionary&, const label, "
               "const polyBoundaryMesh&) : constructing polyPatch"
            << endl;
    }

    word patchType(dict.lookup("type"));
    dict.readIfPresent("geometricType", patchType);

    return polyPatch::New(patchType, name, dict, index, bm);
}


CML::autoPtr<CML::polyPatch> CML::polyPatch::New
(
    const word& patchType,
    const word& name,
    const dictionary& dict,
    const label index,
    const polyBoundaryMesh& bm
)
{
    if (debug)
    {
        Info<< "polyPatch::New(const word&, const word&, const dictionary&, "
               "const label, const polyBoundaryMesh&) : constructing polyPatch"
            << endl;
    }

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(patchType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        if (!disallowGenericPolyPatch)
        {
            cstrIter = dictionaryConstructorTablePtr_->find("genericPatch");
        }

        if (cstrIter == dictionaryConstructorTablePtr_->end())
        {
            FatalIOErrorInFunction(dict)
                << "Unknown polyPatch type "
                << patchType << " for patch " << name << nl << nl
                << "Valid polyPatch types are :" << endl
                << dictionaryConstructorTablePtr_->sortedToc()
                << exit(FatalIOError);
        }
    }

    return autoPtr<polyPatch>(cstrIter()(name, dict, index, bm, patchType));
}


// ************************************************************************* //
