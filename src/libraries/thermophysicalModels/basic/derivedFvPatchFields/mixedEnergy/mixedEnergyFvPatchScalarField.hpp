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
    CML::mixedEnergyFvPatchScalarField

Description
    This boundary condition provides a mixed condition for internal energy

See also
    CML::mixedFvPatchField

SourceFiles
    mixedEnergyFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef mixedEnergyFvPatchScalarField_HPP
#define mixedEnergyFvPatchScalarField_HPP

#include "mixedFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
            Class mixedEnergyFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class mixedEnergyFvPatchScalarField
:
    public mixedFvPatchScalarField
{

public:

    //- Runtime type information
    TypeName("mixedEnergy");


    //- Construct from patch and internal field
    mixedEnergyFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    mixedEnergyFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given mixedEnergyFvPatchScalarField
    // onto a new patch
    mixedEnergyFvPatchScalarField
    (
        const mixedEnergyFvPatchScalarField&,
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    mixedEnergyFvPatchScalarField
    (
        const mixedEnergyFvPatchScalarField&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
        return tmp<fvPatchScalarField>
        (
            new mixedEnergyFvPatchScalarField(*this)
        );
    }

    //- Construct as copy setting internal field reference
    mixedEnergyFvPatchScalarField
    (
        const mixedEnergyFvPatchScalarField&,
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
            new mixedEnergyFvPatchScalarField(*this, iF)
        );
    }


    // Member functions

    // Access

    //- Allow manipulation of the boundary values
    virtual bool fixesValue() const
    {
        return false;
    }


    // Evaluation functions

    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();

};


} // End namespace CML


#endif
