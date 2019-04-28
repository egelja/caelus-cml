/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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
    CML::isentropicTotalTemperatureFvPatchScalarField

Description
    CML::isentropicTotalTemperatureFvPatchScalarField

SourceFiles
    isentropicTotalTemperatureFvPatchScalarField.cpp

Author
    Aleksandar Jemcov
    Oliver Borm

\*---------------------------------------------------------------------------*/

#ifndef isentropicTotalTemperatureFvPatchScalarField_HPP
#define isentropicTotalTemperatureFvPatchScalarField_HPP

#include "fixedValueFvPatchFields.hpp"

namespace CML
{

class isentropicTotalTemperatureFvPatchScalarField :
    public fixedValueFvPatchScalarField
{

    //- Name of the static pressure field
    word pName_;

    //- Total temperature field
    scalarField T0_;

    //- Total pressure field
    scalarField p0_;

public:

    //- Runtime type information
    TypeName("isentropicTotalTemperature");


    //- Construct from patch and internal field
    isentropicTotalTemperatureFvPatchScalarField
    (
        fvPatch const&,
        DimensionedField<scalar, volMesh> const&
    );

    //- Construct from patch, internal field and dictionary
    isentropicTotalTemperatureFvPatchScalarField
    (
        fvPatch const&,
        DimensionedField<scalar, volMesh> const&,
        dictionary const&
    );

    //- Construct by mapping given isentropicTotalTemperatureFvPatchScalarField
    //  onto a new patch
    isentropicTotalTemperatureFvPatchScalarField
    (
        isentropicTotalTemperatureFvPatchScalarField const&,
        fvPatch const&,
        DimensionedField<scalar, volMesh> const&,
        fvPatchFieldMapper const&
    );

    //- Construct as copy
    isentropicTotalTemperatureFvPatchScalarField
    (
        isentropicTotalTemperatureFvPatchScalarField const&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
        return tmp<fvPatchScalarField>
        (
            new isentropicTotalTemperatureFvPatchScalarField(*this)
        );
    }

    //- Construct as copy setting internal field reference
    isentropicTotalTemperatureFvPatchScalarField
    (
        isentropicTotalTemperatureFvPatchScalarField const&,
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
            new isentropicTotalTemperatureFvPatchScalarField(*this, iF)
        );
    }


    // Member functions

    // Access

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


    // Mapping functions

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


    // Evaluation functions
    
    //- Inherit updateCoeffs from fixedValueFvPatchScalarField
    using fixedValueFvPatchScalarField::updateCoeffs;
    
    //- Update the coefficients associated with the patch field
    //  using the given patch velocity field
    virtual void updateCoeffs(vectorField const& Up);

    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();

    //- Write
    virtual void write(Ostream&) const;

};


}


#endif
