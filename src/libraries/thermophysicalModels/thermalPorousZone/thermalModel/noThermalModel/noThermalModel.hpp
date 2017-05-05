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
    CML::porousMedia::noThermalModel

Description
    A dummy thermal model porous media, corresponding to the 'none' option

\*---------------------------------------------------------------------------*/

#ifndef noThermalModel_H
#define noThermalModel_H

#include "thermalModel.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace porousMedia
{

/*---------------------------------------------------------------------------*\
                       Class noThermalModel Declaration
\*---------------------------------------------------------------------------*/

class noThermalModel
:
    public thermalModel
{

public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from porous zone
        noThermalModel(const porousZone&);


    //- Destructor
    virtual ~noThermalModel();


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

