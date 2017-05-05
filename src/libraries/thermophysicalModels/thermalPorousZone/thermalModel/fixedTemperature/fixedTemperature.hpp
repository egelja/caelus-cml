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
    CML::porousMedia::fixedTemperature

Description
    Fixed temperature model for porous media

\*---------------------------------------------------------------------------*/

#ifndef fixedTemperature_H
#define fixedTemperature_H

#include "thermalModel.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace porousMedia
{

/*---------------------------------------------------------------------------*\
                      Class fixedTemperature Declaration
\*---------------------------------------------------------------------------*/

class fixedTemperature
:
    public thermalModel
{

protected:

    // Protected data

        //- Fixed temperature
        const scalar T_;


public:

    //- Runtime type information
    TypeName("fixedTemperature");


    // Constructors

        //- Construct from porous zone
        fixedTemperature(const porousZone& pZone);


    //- Destructor
    virtual ~fixedTemperature();


    // Member Functions

        //- Add the thermal source to the enthalpy equation
        virtual void addEnthalpySource
        (
            const basicThermo&,
            const volScalarField& rho,
            fvScalarMatrix& hEqn
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace porousMedia

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

