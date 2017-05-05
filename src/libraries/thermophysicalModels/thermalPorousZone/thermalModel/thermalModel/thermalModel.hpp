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
    CML::porousMedia::thermalModel

Description
    Base class to select the temperature specification models for porousMedia

\*---------------------------------------------------------------------------*/

#ifndef thermalModel_H
#define thermalModel_H

#include "porousZone.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "volFieldsFwd.hpp"
#include "fvMatricesFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class basicThermo;

namespace porousMedia
{

/*---------------------------------------------------------------------------*\
                        Class thermalModel Declaration
\*---------------------------------------------------------------------------*/

class thermalModel
{

protected:

    // Protected data

        //- Reference to the porous zone
        const porousZone& pZone_;

        //- Thermal model coefficients dictionary
        const dictionary thermalCoeffs_;


public:

    //- Runtime type information
    TypeName("thermalModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        thermalModel,
        pZone,
        (
            const porousZone& pZone
        ),
        (pZone)
    );


    // Constructors

        //- Construct from porous zone, coefficients from "thermalModel" entry
        thermalModel(const porousZone&);

        //- Construct from porous zone and thermal model coefficients
        thermalModel(const porousZone&, const dictionary& thermalCoeffs);


    //- Destructor
    virtual ~thermalModel();


    //- Selector
    static autoPtr<thermalModel> New(const porousZone&);


    // Member Functions

        //- Add the thermal source to the enthalpy equation
        virtual void addEnthalpySource
        (
            const basicThermo&,
            const volScalarField& rho,
            fvScalarMatrix& hEqn
        ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace porousMedia

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

