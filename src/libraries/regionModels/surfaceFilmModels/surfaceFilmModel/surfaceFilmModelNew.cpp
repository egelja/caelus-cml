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

#include "surfaceFilmModel.hpp"
#include "noFilm.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

autoPtr<surfaceFilmModel> surfaceFilmModel::New
(
    const fvMesh& mesh,
    const dimensionedVector& g,
    const word& regionType
)
{
    word modelType;

    {
        IOobject surfaceFilmPropertiesDictHeader
        (
            regionType + "Properties",
            mesh.time().constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE,
            false
        );

        if (surfaceFilmPropertiesDictHeader.headerOk())
        {
            IOdictionary surfaceFilmPropertiesDict
            (
                surfaceFilmPropertiesDictHeader
            );

            surfaceFilmPropertiesDict.lookup("surfaceFilmModel") >> modelType;
        }
        else
        {
            modelType = surfaceFilmModels::noFilm::typeName;
        }
    }

    Info<< "Selecting surfaceFilmModel " << modelType << endl;

    meshConstructorTable::iterator cstrIter =
        meshConstructorTablePtr_->find(modelType);

    if (cstrIter == meshConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown surfaceFilmModel type " << modelType
            << nl << nl << "Valid surfaceFilmModel types are:" << nl
            << meshConstructorTablePtr_->toc()
            << exit(FatalError);
    }

    return autoPtr<surfaceFilmModel>
    (
        cstrIter()
        (
            modelType,
            mesh,
            g,
            regionType
        )
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace regionModels
} // End namespace CML

// ************************************************************************* //
