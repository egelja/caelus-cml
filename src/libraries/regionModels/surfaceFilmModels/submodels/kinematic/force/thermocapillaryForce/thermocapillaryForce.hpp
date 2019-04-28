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
    CML::regionModels::surfaceFilmModels::thermocapillaryForce

Description
    Thermocapillary force

SourceFiles
    thermocapillaryForce.C

\*---------------------------------------------------------------------------*/

#ifndef thermocapillaryForce_H
#define thermocapillaryForce_H

#include "force.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                  Class thermocapillaryForce Declaration
\*---------------------------------------------------------------------------*/

class thermocapillaryForce
:
    public force
{
private:

    // Private member functions

        //- Disallow default bitwise copy construct
        thermocapillaryForce(const thermocapillaryForce&);

        //- Disallow default bitwise assignment
        void operator=(const thermocapillaryForce&);


public:

    //- Runtime type information
    TypeName("thermocapillary");


    // Constructors

        //- Construct from surface film model
        thermocapillaryForce
        (
            surfaceFilmRegionModel& film,
            const dictionary& dict
        );


    //- Destructor
    virtual ~thermocapillaryForce();


    // Member Functions

        // Evolution

            //- Correct
            virtual tmp<fvVectorMatrix> correct(volVectorField& U);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
