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
    CML::compressible::RASModels::alphatWallFunctionFvPatchScalarField

Description
    Boundary condition for turbulent thermal diffusivity when using wall
    functions
    - Turbulent Prandtl number defaults to 0.85 if unspecified

SourceFiles
    compAlphatWallFunctionFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef compressibleAlphatWallFunctionFvPatchScalarField_H
#define compressibleAlphatWallFunctionFvPatchScalarField_H

#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{

/*---------------------------------------------------------------------------*\
            Class alphatWallFunctionFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class alphatWallFunctionFvPatchScalarField
:
    public fixedValueFvPatchScalarField
{
    // Private data

        //- Name of turbulent viscosity field
        word mutName_;

        //- Turbulent Prandtl number
        scalar Prt_;


public:

    //- Runtime type information
    TypeName("compressible::alphatWallFunction");


    // Constructors

        //- Construct from patch and internal field
        alphatWallFunctionFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        alphatWallFunctionFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  alphatWallFunctionFvPatchScalarField
        //  onto a new patch
        alphatWallFunctionFvPatchScalarField
        (
            const alphatWallFunctionFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        alphatWallFunctionFvPatchScalarField
        (
            const alphatWallFunctionFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new alphatWallFunctionFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        alphatWallFunctionFvPatchScalarField
        (
            const alphatWallFunctionFvPatchScalarField&,
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
                new alphatWallFunctionFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        // I-O

            //- Write
            virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace compressible
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
