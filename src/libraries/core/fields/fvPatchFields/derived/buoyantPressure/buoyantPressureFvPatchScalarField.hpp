/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::buoyantPressureFvPatchScalarField

Description
    Set the pressure gradient boundary condition appropriately for buoyant flow.

    If the variable name is "pd" assume it is p - rho*g.h and set the gradient
    appropriately.  Otherwise assume the variable is the static pressure.

SourceFiles
    buoyantPressureFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef buoyantPressureFvPatchScalarFields_H
#define buoyantPressureFvPatchScalarFields_H

#include "fvPatchFields.hpp"
#include "fixedGradientFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class buoyantPressureFvPatch Declaration
\*---------------------------------------------------------------------------*/

class buoyantPressureFvPatchScalarField
:
    public fixedGradientFvPatchScalarField
{
    // Private data

        //- Name of the density field used to calculate the buoyancy force
        word rhoName_;


public:

    //- Runtime type information
    TypeName("buoyantPressure");


    // Constructors

        //- Construct from patch and internal field
        buoyantPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        buoyantPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  buoyantPressureFvPatchScalarField onto a new patch
        buoyantPressureFvPatchScalarField
        (
            const buoyantPressureFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        buoyantPressureFvPatchScalarField
        (
            const buoyantPressureFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new buoyantPressureFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        buoyantPressureFvPatchScalarField
        (
            const buoyantPressureFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new buoyantPressureFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
