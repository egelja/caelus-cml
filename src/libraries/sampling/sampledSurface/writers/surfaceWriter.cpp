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

#include "surfaceWriter.hpp"

#include "MeshedSurfaceProxy.hpp"
#include "proxySurfaceWriter.hpp"

#include "HashTable.hpp"
#include "word.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(surfaceWriter, 0);
    defineRunTimeSelectionTable(surfaceWriter, word);
    defineRunTimeSelectionTable(surfaceWriter, wordDict);
    addNamedToRunTimeSelectionTable
    (
        surfaceWriter,
        surfaceWriter,
        word,
        null
    );
}


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

CML::autoPtr<CML::surfaceWriter>
CML::surfaceWriter::New(const word& writeType)
{
    wordConstructorTable::iterator cstrIter =
        wordConstructorTablePtr_->find(writeType);

    if (cstrIter == wordConstructorTablePtr_->end())
    {
        if (MeshedSurfaceProxy<face>::canWriteType(writeType))
        {
            // generally unknown, but can be written via MeshedSurfaceProxy
            // use 'proxy' handler instead
            return autoPtr<surfaceWriter>(new proxySurfaceWriter(writeType));
        }

        if (cstrIter == wordConstructorTablePtr_->end())
        {
            FatalErrorInFunction
                << "Unknown write type \"" << writeType << "\"\n\n"
                << "Valid write types : "
                << wordConstructorTablePtr_->sortedToc() << nl
                << "Valid proxy types : "
                << MeshedSurfaceProxy<face>::writeTypes() << endl
                << exit(FatalError);
        }
    }

    return autoPtr<surfaceWriter>(cstrIter()());
}


CML::autoPtr<CML::surfaceWriter>
CML::surfaceWriter::New(const word& writeType, const dictionary& optDict)
{
    // find constructors with dictionary options
    wordDictConstructorTable::iterator cstrIter =
        wordDictConstructorTablePtr_->find(writeType);

    if (cstrIter == wordDictConstructorTablePtr_->end())
    {
        // revert to versions without options
        return CML::surfaceWriter::New(writeType);
    }

    return autoPtr<surfaceWriter>(cstrIter()(optDict));
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::surfaceWriter::surfaceWriter()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::surfaceWriter::~surfaceWriter()
{}


// ************************************************************************* //
