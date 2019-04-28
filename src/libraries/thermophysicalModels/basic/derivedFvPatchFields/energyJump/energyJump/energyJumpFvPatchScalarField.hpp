/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
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
    CML::energyJumpFvPatchScalarField

Description
    This boundary condition provides an energy jump condition across a pair
    of coupled patches.  It is not applied directly, but is employed on-the-fly
    when converting temperature boundary conditions into energy.

See also
    CML::fixedJumpFvPatchField

SourceFiles
    energyJumpFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef energyJumpFvPatchScalarField_HPP
#define energyJumpFvPatchScalarField_HPP

#include "fixedJumpFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                Class energyJumpFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class energyJumpFvPatchScalarField
:
    public fixedJumpFvPatchField<scalar>
{

public:

    //- Runtime type information
    TypeName("energyJump");


    //- Construct from patch and internal field
    energyJumpFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    energyJumpFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given energyJumpFvPatchScalarField onto a
    //  new patch
    energyJumpFvPatchScalarField
    (
        const energyJumpFvPatchScalarField&,
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    energyJumpFvPatchScalarField
    (
        const energyJumpFvPatchScalarField&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchField<scalar> > clone() const
    {
        return tmp<fvPatchField<scalar> >
        (
            new energyJumpFvPatchScalarField(*this)
        );
    }

    //- Construct as copy setting internal field reference
    energyJumpFvPatchScalarField
    (
        const energyJumpFvPatchScalarField&,
        const DimensionedField<scalar, volMesh>&
    );

    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<scalar> > clone
    (
        const DimensionedField<scalar, volMesh>& iF
    ) const
    {
        return tmp<fvPatchField<scalar> >
        (
            new energyJumpFvPatchScalarField(*this, iF)
        );
    }


    // Member functions

    // Evaluation functions

    //- Update the coefficients
    virtual void updateCoeffs();


    //- Write
    virtual void write(Ostream&) const;
};


} // End namespace CML


#endif
