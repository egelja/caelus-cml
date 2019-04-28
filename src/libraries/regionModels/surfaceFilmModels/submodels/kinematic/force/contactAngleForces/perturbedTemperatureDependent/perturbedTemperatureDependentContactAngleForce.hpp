/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
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
    CML::regionModels::surfaceFilmModels::
        perturbedTemperatureDependentContactAngleForce

Description
    Temperature dependent contact angle force with a stochastic perturbation

    The contact angle in degrees is specified as a CML::Function1 type, to
    enable the use of, e.g.  constant, polynomial, table values and the
    stochastic perturbation obtained from a
    CML::distributionModels::distributionModel

See also
    CML::regionModels::surfaceFilmModels::contactAngleForce
    CML::regionModels::surfaceFilmModels::temperatureDependentContactAngleForce
    CML::regionModels::surfaceFilmModels::distributionContactAngleForce
    CML::Function1Types
    CML::distributionModel

SourceFiles
    perturbedTemperatureDependentContactAngleForce.cpp

\*---------------------------------------------------------------------------*/

#ifndef perturbedTemperatureDependentContactAngleForce_HPP
#define perturbedTemperatureDependentContactAngleForce_HPP

#include "contactAngleForce.hpp"
#include "DataEntry.hpp"
#include "distributionModel.hpp"
#include "Random.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
       Class perturbedTemperatureDependentContactAngleForce Declaration
\*---------------------------------------------------------------------------*/

class perturbedTemperatureDependentContactAngleForce
:
    public contactAngleForce
{
    // Private Data

        //- Contact angle function
        autoPtr<DataEntry<scalar>> thetaPtr_;

        //- Random number generator
        Random rndGen_;

        //- Parcel size PDF model
        const autoPtr<distributionModel> distribution_;


    // Private member functions

        //- Disallow default bitwise copy construct
        perturbedTemperatureDependentContactAngleForce
        (
            const perturbedTemperatureDependentContactAngleForce&
        );

        //- Disallow default bitwise assignment
        void operator=(const perturbedTemperatureDependentContactAngleForce&);


protected:

        //- Return the contact angle field
        virtual tmp<volScalarField> theta() const;


public:

    //- Runtime type information
    TypeName("perturbedTemperatureDependentContactAngle");


    // Constructors

        //- Construct from surface film model
        perturbedTemperatureDependentContactAngleForce
        (
            surfaceFilmRegionModel& film,
            const dictionary& dict
        );


    //- Destructor
    virtual ~perturbedTemperatureDependentContactAngleForce();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
