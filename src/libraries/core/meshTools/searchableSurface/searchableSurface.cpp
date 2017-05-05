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

#include "searchableSurface.hpp"
#include "long.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(searchableSurface, 0);
    defineRunTimeSelectionTable(searchableSurface, dict);

    template<>
    const char* CML::NamedEnum
    <
        CML::searchableSurface::volumeType,
        4
    >::names[] =
    {
        "unknown",
        "mixed",
        "inside",
        "outside"
    };
}

const CML::NamedEnum<CML::searchableSurface::volumeType, 4>
    CML::searchableSurface::volumeTypeNames;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


CML::autoPtr<CML::searchableSurface> CML::searchableSurface::New
(
    const word& searchableSurfaceType,
    const IOobject& io,
    const dictionary& dict
)
{
    dictConstructorTable::iterator cstrIter =
        dictConstructorTablePtr_->find(searchableSurfaceType);

    if (cstrIter == dictConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "searchableSurface::New(const word&,"
            " const IOobject&, const dictionary&)"
        )   << "Unknown searchableSurface type " << searchableSurfaceType
            << endl << endl
            << "Valid searchableSurface types : " << endl
            << dictConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<searchableSurface>(cstrIter()(io, dict));
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::searchableSurface::searchableSurface(const IOobject& io)
:
    regIOobject(io)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::searchableSurface::~searchableSurface()
{}


// ************************************************************************* //
