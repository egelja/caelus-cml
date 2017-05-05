/*---------------------------------------------------------------------------*\
Copyright (C) 2016 Applied CCM
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
    CML::totalTemperatureFvPatchScalarField

Description
    This boundary condition provides a total temperature condition.

    \heading Patch usage

    \table
        Property     | Description             | Required    | Default value
        U            | Velocity field name     | no          | U
        phi          | Flux field name         | no          | phi
        psi          | Compressibility field name | no       | thermo:psi
        gamma        | ratio of specific heats (Cp/Cv) | yes |
        T0           | reference temperature   | yes         |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            totalTemperature;
        T0              uniform 300;
    }
    \endverbatim

SourceFiles
    totalTemperatureFvPatchScalarField.cpp

SeeAlso
    CML::fixedValueFvPatchField

\*---------------------------------------------------------------------------*/

#ifndef totalTemperatureFvPatchScalarField_HPP_
#define totalTemperatureFvPatchScalarField_HPP_

#include "fixedValueFvPatchFields.hpp"

namespace CML
{

class totalTemperatureFvPatchScalarField : public fixedValueFvPatchScalarField
{

    //- Name of the velocity field
    word UName_;

    //- Name of the flux transporting the field
    word phiName_;

    //- Name of the compressibility field used to calculate the wave speed
    word psiName_;

    //- Heat capacity ratio
    scalar gamma_;

    //- Total pressure
    scalarField T0_;

public:

    //- Runtime type information
    TypeName("totalTemperature");


    //- Construct from patch and internal field
    totalTemperatureFvPatchScalarField
    (
        fvPatch const&,
        DimensionedField<scalar, volMesh> const&
    );

    //- Construct from patch, internal field and dictionary
    totalTemperatureFvPatchScalarField
    (
        fvPatch const&,
        DimensionedField<scalar, volMesh> const&,
        dictionary const&
    );

    //- Construct by mapping given totalTemperatureFvPatchScalarField
    //  onto a new patch
    totalTemperatureFvPatchScalarField
    (
        totalTemperatureFvPatchScalarField const&,
        fvPatch const&,
        DimensionedField<scalar, volMesh> const&,
        fvPatchFieldMapper const&
    );

    //- Construct as copy
    totalTemperatureFvPatchScalarField
    (
        totalTemperatureFvPatchScalarField const&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
        return tmp<fvPatchScalarField>
        (
            new totalTemperatureFvPatchScalarField(*this)
        );
    }

    //- Construct as copy setting internal field reference
    totalTemperatureFvPatchScalarField
    (
        totalTemperatureFvPatchScalarField const&,
        DimensionedField<scalar, volMesh> const&
    );

    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
        DimensionedField<scalar, volMesh> const& iF
    ) const
    {
        return tmp<fvPatchScalarField>
        (
            new totalTemperatureFvPatchScalarField(*this, iF)
        );
    }

    //- Return the total pressure
    scalarField const& T0() const
    {
        return T0_;
    }

    //- Return reference to the total pressure to allow adjustment
    scalarField& T0()
    {
        return T0_;
    }

    //- Map (and resize as needed) from self given a mapping object
    virtual void autoMap
    (
        fvPatchFieldMapper const&
    );

    //- Reverse map the given fvPatchField onto this fvPatchField
    virtual void rmap
    (
        fvPatchScalarField const&,
        labelList const&
    );

    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();

    //- Write
    virtual void write(Ostream&) const;
};


}

#endif


