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
    CML::regionModels::surfaceFilmModels::injectionModelList

Description
    List container for film injection models

SourceFiles
    injectionModelList.C

\*---------------------------------------------------------------------------*/

#ifndef injectionModelList_H
#define injectionModelList_H

#include "PtrList.hpp"
#include "injectionModel.hpp"
#include "filmSubModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                    Class injectionModelList Declaration
\*---------------------------------------------------------------------------*/

class injectionModelList
:
    public PtrList<injectionModel>,
    public filmSubModelBase
{
private:

    // Private data

        //- List of mass injected per patch
        scalarField massInjected_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        injectionModelList(const injectionModelList&);

        //- Disallow default bitwise assignment
        void operator=(const injectionModelList&);


public:

    // Constructors

        //- Construct null
        injectionModelList(surfaceFilmRegionModel& film);

        //- Construct from type name, dictionary and surface film model
        injectionModelList
        (
            surfaceFilmRegionModel& film,
            const dictionary& dict
        );


    //- Destructor
    virtual ~injectionModelList();


    // Member Functions

        // Evolution

            //- Correct
            virtual void correct
            (
                scalarField& availableMass,
                volScalarField& massToInject,
                volScalarField& diameterToInject
            );


        // I-O

            //- Provide some info
            virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
