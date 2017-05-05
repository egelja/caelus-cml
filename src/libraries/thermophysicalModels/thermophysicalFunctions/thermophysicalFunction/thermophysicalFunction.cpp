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

#include "error.hpp"

#include "thermophysicalFunction.hpp"
#include "HashTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(thermophysicalFunction, 0);
    defineRunTimeSelectionTable(thermophysicalFunction, Istream);
    defineRunTimeSelectionTable(thermophysicalFunction, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::autoPtr<CML::thermophysicalFunction> CML::thermophysicalFunction::New
(
    Istream& is
)
{
    if (debug)
    {
        Info<< "thermophysicalFunction::New(Istream&) : "
            << "constructing thermophysicalFunction"
            << endl;
    }

    const word thermophysicalFunctionType(is);

    IstreamConstructorTable::iterator cstrIter =
        IstreamConstructorTablePtr_->find(thermophysicalFunctionType);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalErrorIn("thermophysicalFunction::New(Istream&)")
            << "Unknown thermophysicalFunction type "
            << thermophysicalFunctionType
            << nl << nl
            << "Valid thermophysicalFunction types are :" << endl
            << IstreamConstructorTablePtr_->sortedToc()
            << abort(FatalError);
    }

    return autoPtr<thermophysicalFunction>(cstrIter()(is));
}


CML::autoPtr<CML::thermophysicalFunction> CML::thermophysicalFunction::New
(
    const dictionary& dict
)
{
    if (debug)
    {
        Info<< "thermophysicalFunction::New(const dictionary&) : "
            << "constructing thermophysicalFunction"
            << endl;
    }

    const word thermophysicalFunctionType(dict.lookup("functionType"));

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(thermophysicalFunctionType);

    if (cstrIter == IstreamConstructorTablePtr_->end())
    {
        FatalErrorIn("thermophysicalFunction::New(const dictionary&)")
            << "Unknown thermophysicalFunction type "
            << thermophysicalFunctionType
            << nl << nl
            << "Valid thermophysicalFunction types are :" << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << abort(FatalError);
    }

    return autoPtr<thermophysicalFunction>(cstrIter()(dict));
}


// ************************************************************************* //
