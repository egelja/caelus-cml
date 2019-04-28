/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

Class
    CML::regionModels::surfaceFilmModels::filmSubModelBase

Description
    Base class for surface film sub-models

SourceFiles
    filmSubModelBaseI.hpp
    filmSubModelBase.cpp

\*---------------------------------------------------------------------------*/

#ifndef filmSubModelBase_H
#define filmSubModelBase_H

#include "surfaceFilmRegionModel.hpp"
#include "subModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                      Class filmSubModelBase Declaration
\*---------------------------------------------------------------------------*/

class filmSubModelBase
:
    public subModelBase
{
protected:

    // Protected data

        //- Reference to the film surface film model
        surfaceFilmRegionModel& filmModel_;


public:

    // Constructors

        //- Construct null
        filmSubModelBase(surfaceFilmRegionModel& film);

        //- Construct from film film without name
        filmSubModelBase
        (
            surfaceFilmRegionModel& film,
            const dictionary& dict,
            const word& baseName,
            const word& modelType,
            const word& dictExt = "Coeffs"
        );

        //- Construct from film film with name
        filmSubModelBase
        (
            const word& modelName,
            surfaceFilmRegionModel& film,
            const dictionary& dict,
            const word& baseName,
            const word& modelType
        );


    //- Destructor
    virtual ~filmSubModelBase();


    // Member Functions

        // Access

            //- Flag to indicate when to write a property
            virtual bool writeTime() const;

            //- Return const access to the film surface film model
            inline const surfaceFilmRegionModel& film() const;

            //- Return the reference to the film surface film model
            inline surfaceFilmRegionModel& film();

            template<class FilmType>
            inline const FilmType& filmType() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "filmSubModelBaseI.hpp"

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class FilmType>
const FilmType& filmSubModelBase::filmType() const
{
    if (!isA<FilmType>(filmModel_))
    {
        FatalErrorInFunction
            << "Model " << this->modelType() << " requested film type "
            << FilmType::typeName << " but film is type " << filmModel_.type()
            << abort(FatalError);
    }

    return refCast<const FilmType>(filmModel_);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
