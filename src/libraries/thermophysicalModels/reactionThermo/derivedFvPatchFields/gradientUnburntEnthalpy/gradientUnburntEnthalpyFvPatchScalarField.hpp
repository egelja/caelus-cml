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
    CML::gradientUnburntEnthalpyFvPatchScalarField

Description
    gradient boundary condition for unburnt

SourceFiles
    gradientUnburntEnthalpyFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef gradientUnburntEnthalpyFvPatchScalarField_HPP
#define gradientUnburntEnthalpyFvPatchScalarField_HPP

#include "fixedGradientFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
         Class gradientUnburntEnthalpyFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class gradientUnburntEnthalpyFvPatchScalarField
:
    public fixedGradientFvPatchScalarField
{

public:

    //- Runtime type information
    TypeName("gradientUnburntEnthalpy");


    //- Construct from patch and internal field
    gradientUnburntEnthalpyFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    gradientUnburntEnthalpyFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given gradientUnburntEnthalpyFvPatchScalarField
    // onto a new patch
    gradientUnburntEnthalpyFvPatchScalarField
    (
        const gradientUnburntEnthalpyFvPatchScalarField&,
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    gradientUnburntEnthalpyFvPatchScalarField
    (
        const gradientUnburntEnthalpyFvPatchScalarField&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
        return tmp<fvPatchScalarField>
        (
            new gradientUnburntEnthalpyFvPatchScalarField(*this)
        );
    }

    //- Construct as copy setting internal field reference
    gradientUnburntEnthalpyFvPatchScalarField
    (
        const gradientUnburntEnthalpyFvPatchScalarField&,
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
            new gradientUnburntEnthalpyFvPatchScalarField(*this, iF)
        );
    }


    // Member functions

    // Evaluation functions

    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();

};


} // End namespace CML


#endif
