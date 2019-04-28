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
    CML::regionModels::surfaceFilmModels::laminar

Description
    Film laminar turbulence model.

SourceFiles
    laminar.cpp

\*---------------------------------------------------------------------------*/

#ifndef laminar_HPP
#define laminar_HPP

#include "filmTurbulenceModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                           Class laminar Declaration
\*---------------------------------------------------------------------------*/

class laminar
:
    public filmTurbulenceModel
{
private:

    // Private Data

        //- Surface roughness coefficient
        scalar Cf_;


    // Private member functions

        //- Disallow default bitwise copy construct
        laminar(const laminar&);

        //- Disallow default bitwise assignment
        void operator=(const laminar&);


public:

    //- Runtime type information
    TypeName("laminar");


    // Constructors

        //- Construct from surface film model
        laminar(surfaceFilmRegionModel& film, const dictionary& dict);


    //- Destructor
    virtual ~laminar();


    // Member Functions

        // Evolution

            //- Return the film surface velocity
            virtual tmp<volVectorField> Us() const;

            //- Return the film turbulence viscosity
            virtual tmp<volScalarField> mut() const;

            //- Correct/update the model
            virtual void correct();

            //- Return the source for the film momentum equation
            virtual tmp<fvVectorMatrix> Su(volVectorField& U) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
