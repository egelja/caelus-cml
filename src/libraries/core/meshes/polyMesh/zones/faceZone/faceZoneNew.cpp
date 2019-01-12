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

#include "faceZone.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::autoPtr<CML::faceZone> CML::faceZone::New
(
    const word& name,
    const dictionary& dict,
    const label index,
    const faceZoneMesh& zm
)
{
    if (debug)
    {
        Info<< "faceZone::New(const word&, const dictionary&, const label, "
               "const faceZoneMesh&) : constructing faceZone " << name
            << endl;
    }

    const word zoneType(dict.lookup("type"));

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(zoneType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(dict)
            << "Unknown faceZone type "
            << zoneType << nl << nl
            << "Valid faceZone types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return autoPtr<faceZone>(cstrIter()(name, dict, index, zm));
}


// ************************************************************************* //
