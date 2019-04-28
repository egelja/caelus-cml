/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::regionModels::surfaceFilmModels::solidification

Description
    Solidification phase change model where all film mass is converted when the
    local temperature > activation temperature.  The latent heat is
    assumed to be removed by heat-transfer to the wall.

SourceFiles
    solidification.cpp

\*---------------------------------------------------------------------------*/

#ifndef solidification_HPP
#define solidification_HPP

#include "phaseChangeModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                       Class solidification Declaration
\*---------------------------------------------------------------------------*/

class solidification
:
    public phaseChangeModel
{
    // Private member functions

        //- Disallow default bitwise copy construct
        solidification(const solidification&);

        //- Disallow default bitwise assignment
        void operator=(const solidification&);


protected:

    // Protected data

        //- Temperature at which solidification starts
        scalar T0_;

        //- Solidification limiter
        //  Maximum fraction of film which can solidify in a time-step
        scalar maxSolidificationFrac_;

        //- Solidification limiter
        //  Maximum rate at which the film can solidify
        dimensionedScalar maxSolidificationRate_;

        //- Accumulated solid mass [kg]
        volScalarField mass_;

        //- Accumulated solid thickness [m]
        volScalarField thickness_;


public:

    //- Runtime type information
    TypeName("solidification");


    // Constructors

        //- Construct from surface film model
        solidification(surfaceFilmRegionModel& film, const dictionary& dict);


    //- Destructor
    virtual ~solidification();


    // Member Functions

        // Evolution

            //- Correct
            virtual void correctModel
            (
                const scalar dt,
                scalarField& availableMass,
                scalarField& dMass,
                scalarField& dEnergy
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
