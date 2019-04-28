/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
Copyright (C) 2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Author: Franjo Juretic (franjo.juretic@c-fields.com)

\*---------------------------------------------------------------------------*/

#include "objectRefinement.hpp"
#include "dictionary.hpp"
#include "error.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

autoPtr<CML::objectRefinement> CML::objectRefinement::New
(
    const word& name,
    const dictionary& dict
)
{
    if( debug )
    {
        Info<< "objectRefinement::New(const word&, const dictionary&) : "
            << "constructing objectRefinement"
            << endl;
    }

    // default type is self
    word refType(typeName_());
    if( dict.found("type") )
    {
        dict.lookup("type") >> refType;
    }

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(refType);

    if( cstrIter == dictionaryConstructorTablePtr_->end() )
    {
        FatalIOErrorInFunction(dict)
            << "Unknown objectRefinement type " << refType << nl << nl
            << "Valid objectRefinement types are :" << nl
            << "[default: " << typeName_() << "]"
            << dictionaryConstructorTablePtr_->toc()
            << exit(FatalIOError);
    }

    return autoPtr<objectRefinement>(cstrIter()(name, dict));
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
