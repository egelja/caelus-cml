/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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
#include "relaxationShape.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationShapes
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

autoPtr<relaxationShape> relaxationShape::New
(
    const word& subDictName,
    const fvMesh& mesh_
)
{
    word relaxationShapeTypeName;

    // Enclose the creation of the waveProp to ensure it is
    // deleted before the relaxationShape is created otherwise the dictionary
    // is entered in the database twice
    {
        const dictionary coeffDict_
        (
            (mesh_.thisDb().lookupObject<IOdictionary>("waveProperties"))
             .subDict(subDictName + "Coeffs")
             .subDict("relaxationZone")
        );

        coeffDict_.lookup("relaxationShape") >> relaxationShapeTypeName;

    }

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find
        (
            "relaxationShape"+relaxationShapeTypeName
        );

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown relaxation shape type 'relaxationShape"
            << relaxationShapeTypeName << "'" << endl << endl
            << "Valid relaxation shape types are :" << endl
            << dictionaryConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<relaxationShape>(cstrIter()(subDictName,mesh_));
}


autoPtr<relaxationShape> relaxationShape::New
(
    const word& subDictName,
    const word relaxationShapeTypeName,
    const fvMesh& mesh_
)
{
    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find
        (
            "relaxationShape"+relaxationShapeTypeName
        );

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown relaxation shape type 'relaxationShape"
            << relaxationShapeTypeName << "'" << endl << endl
            << "Valid relaxation shape types are :" << endl
            << dictionaryConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<relaxationShape>(cstrIter()(subDictName,mesh_));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationShapes
} // End namespace CML

// ************************************************************************* //
