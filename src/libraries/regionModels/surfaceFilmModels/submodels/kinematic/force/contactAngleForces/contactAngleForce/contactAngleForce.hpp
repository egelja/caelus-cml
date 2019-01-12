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
    CML::regionModels::surfaceFilmModels::contactAngleForce

Description
    Base-class for film contact angle force models.

    The effect of the contact angle force can be ignored over a specified
    distance from patches.

SourceFiles
    contactAngleForce.cpp

\*---------------------------------------------------------------------------*/

#ifndef contactAngleForce_H
#define contactAngleForce_H

#include "force.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                      Class contactAngleForce Declaration
\*---------------------------------------------------------------------------*/

class contactAngleForce
:
    public force
{
    // Private Data

        //- Coefficient applied to the contact angle force
        scalar Ccf_;

        //- Mask over which force is applied
        volScalarField mask_;


    // Private member functions

        //- Initialise
        void initialise();

        //- Disallow default bitwise copy construct
        contactAngleForce(const contactAngleForce&);

        //- Disallow default bitwise assignment
        void operator=(const contactAngleForce&);


protected:

        //- Return the contact angle field
        virtual tmp<volScalarField> theta() const = 0;


public:

    //- Runtime type information
    TypeName("contactAngle");


    // Constructors

        //- Construct from surface film model
        contactAngleForce
        (
            const word& typeName,
            surfaceFilmRegionModel& film,
            const dictionary& dict
        );


    //- Destructor
    virtual ~contactAngleForce();


    // Member Functions

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
