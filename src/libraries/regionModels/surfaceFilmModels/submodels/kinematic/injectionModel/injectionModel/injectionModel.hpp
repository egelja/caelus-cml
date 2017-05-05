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

Class
    CML::injectionModel

Description
    Base class for film injection models, handling mass transfer from the
    film.

SourceFiles
    injectionModel.C
    injectionModelNew.C
\*---------------------------------------------------------------------------*/

#ifndef injectionModel_H
#define injectionModel_H

#include "filmSubModelBase.hpp"
#include "runTimeSelectionTables.hpp"
#include "scalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                      Class injectionModel Declaration
\*---------------------------------------------------------------------------*/

class injectionModel
:
    public filmSubModelBase
{
private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        injectionModel(const injectionModel&);

        //- Disallow default bitwise assignment
        void operator=(const injectionModel&);


public:

    //- Runtime type information
    TypeName("injectionModel");


    // Declare runtime constructor selection table

         declareRunTimeSelectionTable
         (
             autoPtr,
             injectionModel,
             dictionary,
             (
                const surfaceFilmModel& owner,
                const dictionary& dict
             ),
             (owner, dict)
         );

    // Constructors

        //- Construct null
        injectionModel(const surfaceFilmModel& owner);

        //- Construct from type name, dictionary and surface film model
        injectionModel
        (
            const word& type,
            const surfaceFilmModel& owner,
            const dictionary& dict
        );


    // Selectors

        //- Return a reference to the selected injection model
        static autoPtr<injectionModel> New
        (
            const surfaceFilmModel& owner,
            const dictionary& dict,
            const word& mdoelType
        );


    //- Destructor
    virtual ~injectionModel();


    // Member Functions

        // Evolution

            //- Correct
            virtual void correct
            (
                scalarField& availableMass,
                scalarField& massToInject,
                scalarField& diameterToInject
            ) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
