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

#include "topoSetSource.hpp"
#include "polyMesh.hpp"
#include "topoSet.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(topoSetSource, 0);
    defineRunTimeSelectionTable(topoSetSource, word);
    defineRunTimeSelectionTable(topoSetSource, istream);

    template<>
    const char* CML::NamedEnum
    <
        CML::topoSetSource::setAction,
        8
    >::names[] =
    {
        "clear",
        "new",
        "invert",
        "add",
        "delete",
        "subset",
        "list",
        "remove"
    };
}


CML::HashTable<CML::string>* CML::topoSetSource::usageTablePtr_ = nullptr;


const CML::NamedEnum<CML::topoSetSource::setAction, 8>
    CML::topoSetSource::actionNames_;


const CML::string CML::topoSetSource::illegalSource_
(
    "Illegal topoSetSource name"
);



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::topoSetSource> CML::topoSetSource::New
(
    const word& topoSetSourceType,
    const polyMesh& mesh,
    const dictionary& dict
)
{
    wordConstructorTable::iterator cstrIter =
        wordConstructorTablePtr_->find(topoSetSourceType);

    if (cstrIter == wordConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown topoSetSource type " << topoSetSourceType
            << endl << endl
            << "Valid topoSetSource types : " << endl
            << wordConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<topoSetSource>(cstrIter()(mesh, dict));
}


CML::autoPtr<CML::topoSetSource> CML::topoSetSource::New
(
    const word& topoSetSourceType,
    const polyMesh& mesh,
    Istream& is
)
{
    istreamConstructorTable::iterator cstrIter =
        istreamConstructorTablePtr_->find(topoSetSourceType);

    if (cstrIter == istreamConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown topoSetSource type " << topoSetSourceType
            << endl << endl
            << "Valid topoSetSource types : " << endl
            << istreamConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<topoSetSource>(cstrIter()(mesh, is));
}


CML::Istream& CML::topoSetSource::checkIs(Istream& is)
{
    if (is.good() && !is.eof())
    {
        return is;
    }
    else
    {
        FatalErrorInFunction << "Istream not good"
            << exit(FatalError);

        return is;
    }
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::topoSetSource::addOrDelete
(
    topoSet& set,
    const label cellI,
    const bool add
) const
{
    if (add)
    {
        set.insert(cellI);
    }
    else
    {
        set.erase(cellI);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::topoSetSource::topoSetSource(const polyMesh& mesh)
:
    mesh_(mesh)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::topoSetSource::~topoSetSource()
{}


// ************************************************************************* //
