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

#include "coordinateSystem.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::autoPtr<CML::coordinateSystem> CML::coordinateSystem::New
(
    const objectRegistry& obr,
    const dictionary& dict
)
{
    const dictionary& coordDict = dict.subDict(typeName_());
    word coordType = coordDict.lookup("type");

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(coordType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(dict)
            << "Unknown coordinateSystem type "
            << coordType << nl << nl
            << "Valid coordinateSystem types are :" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return autoPtr<coordinateSystem>(cstrIter()(obr, coordDict));
}


CML::autoPtr<CML::coordinateSystem> CML::coordinateSystem::New
(
    const dictionary& dict
)
{
    const dictionary& coordDict = dict.subDict(typeName_());

    return autoPtr<coordinateSystem>(new coordinateSystem(coordDict));
}


CML::autoPtr<CML::coordinateSystem> CML::coordinateSystem::New
(
    Istream& is
)
{
    const word name(is);
    const dictionary dict(is);

    return autoPtr<coordinateSystem>(new coordinateSystem(name, dict));
}


// ************************************************************************* //
