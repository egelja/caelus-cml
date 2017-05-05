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
    CML::mixedEnthalpyFvPatchScalarField

Description
    Mixed boundary conditions for enthalpy

SourceFiles
    mixedEnthalpyFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef mixedEnthalpyFvPatchScalarField_H
#define mixedEnthalpyFvPatchScalarField_H

#include "mixedFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
               Class mixedEnthalpyFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class mixedEnthalpyFvPatchScalarField
:
    public mixedFvPatchScalarField
{

public:

    //- Runtime type information
    TypeName("mixedEnthalpy");


    // Constructors

        //- Construct from patch and internal field
        mixedEnthalpyFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mixedEnthalpyFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given mixedEnthalpyFvPatchScalarField
        // onto a new patch
        mixedEnthalpyFvPatchScalarField
        (
            const mixedEnthalpyFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        mixedEnthalpyFvPatchScalarField
        (
            const mixedEnthalpyFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new mixedEnthalpyFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        mixedEnthalpyFvPatchScalarField
        (
            const mixedEnthalpyFvPatchScalarField&,
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
                new mixedEnthalpyFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
