/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    CML::fixedShearStressFvPatchVectorField

Description
    Set a constant shear stress as tau0 = -nuEff dU/dn.

SourceFiles
    fixedShearStressFvPatchVectorField.cpp

\*---------------------------------------------------------------------------*/

#ifndef fixedShearStressFvPatchVectorField_H
#define fixedShearStressFvPatchVectorField_H

#include "fvPatchFields.hpp"
#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
            Class fixedShearStressFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class fixedShearStressFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    // Private data

        //- Name of flux field (default = phi)
        const word phiName_;

        //- Name of density field (default = rho)
        const word rhoName_;

        //- Constant shear stress
        const vector tau0_;


public:

    //- Runtime type information
    TypeName("fixedShearStress");


    // Constructors

        //- Construct from patch and internal field
        fixedShearStressFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixedShearStressFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        fixedShearStressFvPatchVectorField
        (
            const fixedShearStressFvPatchVectorField&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        fixedShearStressFvPatchVectorField
        (
            const fixedShearStressFvPatchVectorField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchVectorField> clone() const
        {
            return tmp<fvPatchVectorField>
            (
                new fixedShearStressFvPatchVectorField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        fixedShearStressFvPatchVectorField
        (
            const fixedShearStressFvPatchVectorField&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchVectorField> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return tmp<fvPatchVectorField>
            (
                new fixedShearStressFvPatchVectorField(*this, iF)
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
