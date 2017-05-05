/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::mappedVelocityFluxFixedValueFvPatchField

Description
    Recycles the velocity and flux at a patch to this patch

    mode = NEARESTPATCHFACE : sample nearest face on selected patch
    mode = NEARESTFACE : sample nearest face on any patch. Note: does not
                         warn if nearest actually is on internal face!

SourceFiles
    mappedVelocityFluxFixedValueFvPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef mappedVelocityFluxFixedValueFvPatchField_H
#define mappedVelocityFluxFixedValueFvPatchField_H

#include "fixedValueFvPatchFields.hpp"
#include "mappedFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
           Class mappedVelocityFluxFixedValueFvPatch Declaration
\*---------------------------------------------------------------------------*/

class mappedVelocityFluxFixedValueFvPatchField
:
    public fixedValueFvPatchVectorField
{
    // Private data

        //- Name of flux field
        word phiName_;

public:

    //- Runtime type information
    TypeName("mappedVelocityFlux");


    // Constructors

        //- Construct from patch and internal field
        mappedVelocityFluxFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mappedVelocityFluxFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  mappedVelocityFluxFixedValueFvPatchField
        //  onto a new patch
        mappedVelocityFluxFixedValueFvPatchField
        (
            const mappedVelocityFluxFixedValueFvPatchField&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        mappedVelocityFluxFixedValueFvPatchField
        (
            const mappedVelocityFluxFixedValueFvPatchField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchVectorField> clone() const
        {
            return tmp<fvPatchVectorField>
            (
                new mappedVelocityFluxFixedValueFvPatchField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        mappedVelocityFluxFixedValueFvPatchField
        (
            const mappedVelocityFluxFixedValueFvPatchField&,
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
                new mappedVelocityFluxFixedValueFvPatchField(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

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
