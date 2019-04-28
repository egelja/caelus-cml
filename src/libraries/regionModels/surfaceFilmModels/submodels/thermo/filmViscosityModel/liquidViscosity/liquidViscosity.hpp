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
    CML::regionModels::surfaceFilmModels::liquidViscosity

Description
    liquidViscosity viscosity model

SourceFiles
    liquidViscosity.cpp

\*---------------------------------------------------------------------------*/

#ifndef liquidViscosity_HPP
#define liquidViscosity_HPP

#include "filmViscosityModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                       Class liquidViscosity Declaration
\*---------------------------------------------------------------------------*/

class liquidViscosity
:
    public filmViscosityModel
{
private:

    // Private member functions

        //- Disallow default bitwise copy construct
        liquidViscosity(const liquidViscosity&);

        //- Disallow default bitwise assignment
        void operator=(const liquidViscosity&);


protected:

    // Protected data



public:

    //- Runtime type information
    TypeName("liquid");


    // Constructors

        //- Construct from surface film model
        liquidViscosity
        (
            surfaceFilmRegionModel& film,
            const dictionary& dict,
            volScalarField& mu
        );


    //- Destructor
    virtual ~liquidViscosity();


    // Member Functions

        // Evolution

            //- Correct
            virtual void correct
            (
                const volScalarField& p,
                const volScalarField& T
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
