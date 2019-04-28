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
    CML::fixedInternalEnergyFvPatchScalarField

Description
    This boundary condition provides a fixed condition for internal energy

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            fixedEnergy;
        value           uniform 100;
    }
    \endverbatim

See also
    CML::fixedValueFvPatchField

SourceFiles
    fixedEnergyFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef fixedEnergyFvPatchScalarField_HPP
#define fixedEnergyFvPatchScalarField_HPP

#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
               Class fixedEnergyFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class fixedEnergyFvPatchScalarField
:
    public fixedValueFvPatchScalarField
{

public:

    //- Runtime type information
    TypeName("fixedEnergy");


    //- Construct from patch and internal field
    fixedEnergyFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    fixedEnergyFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given fixedEnergyFvPatchScalarField
    // onto a new patch
    fixedEnergyFvPatchScalarField
    (
        const fixedEnergyFvPatchScalarField&,
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    fixedEnergyFvPatchScalarField
    (
        const fixedEnergyFvPatchScalarField&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
        return tmp<fvPatchScalarField>
        (
            new fixedEnergyFvPatchScalarField(*this)
        );
    }

    //- Construct as copy setting internal field reference
    fixedEnergyFvPatchScalarField
    (
        const fixedEnergyFvPatchScalarField&,
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
            new fixedEnergyFvPatchScalarField(*this, iF)
        );
    }


    // Member functions

    // Evaluation functions

    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
};


} // End namespace CML


#endif
