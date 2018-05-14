/*---------------------------------------------------------------------------*\
 Copyright (C) 2016-2017 OpenFOAM Foundation
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
    CML::fixedFluxExtrapolatedPressureFvPatchScalarField

Group
    grpInletBoundaryConditions grpWallBoundaryConditions

Description
    This boundary condition sets the pressure gradient to the provided value
    such that the flux on the boundary is that specified by the velocity
    boundary condition.

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            fixedFluxExtrapolatedPressure;
    }
    \endverbatim

See also
    CML::fixedGradientFvPatchField

SourceFiles
    fixedFluxExtrapolatedPressureFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef fixedFluxExtrapolatedPressureFvPatchScalarField_HPP
#define fixedFluxExtrapolatedPressureFvPatchScalarField_HPP

#include "fixedFluxPressureFvPatchScalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
             Class fixedFluxExtrapolatedPressureFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class fixedFluxExtrapolatedPressureFvPatchScalarField
:
    public fixedFluxPressureFvPatchScalarField
{

public:

    //- Runtime type information
    TypeName("fixedFluxExtrapolatedPressure");


    // Constructors

        //- Construct from patch and internal field
        fixedFluxExtrapolatedPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixedFluxExtrapolatedPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  fixedFluxExtrapolatedPressureFvPatchScalarField onto a new patch
        fixedFluxExtrapolatedPressureFvPatchScalarField
        (
            const fixedFluxExtrapolatedPressureFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        fixedFluxExtrapolatedPressureFvPatchScalarField
        (
            const fixedFluxExtrapolatedPressureFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new fixedFluxExtrapolatedPressureFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        fixedFluxExtrapolatedPressureFvPatchScalarField
        (
            const fixedFluxExtrapolatedPressureFvPatchScalarField&,
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
                new fixedFluxExtrapolatedPressureFvPatchScalarField(*this, iF)
            );
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
