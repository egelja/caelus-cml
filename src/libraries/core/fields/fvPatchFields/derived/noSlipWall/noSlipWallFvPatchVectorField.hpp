/*---------------------------------------------------------------------------*\
Copyright (C) 2016 Applied CCM
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
    CML::noSlipWallFvPatchVectorField

Description
    
    Specialization of the fixedValue boundary condition applicable for
    the imposition of the no-slip wall boundary condition for vectors.

Authors

    Aleksandar Jemcov
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#ifndef noSlipWallFvPatchVectorField_H
#define noSlipWallFvPatchVectorField_H

#include "fvPatchFields.hpp"
#include "fixedValueFvPatchFields.hpp"

namespace CML
{

class noSlipWallFvPatchVectorField : public fixedValueFvPatchVectorField
{

public:

    //- Runtime type information
    TypeName("noSlipWall");

    //- Construct from patch and internal field
    noSlipWallFvPatchVectorField
    (
        fvPatch const&,
        DimensionedField<vector, volMesh> const&
    );

    //- Construct from patch, internal field and dictionary
    noSlipWallFvPatchVectorField
    (
        fvPatch const&,
        DimensionedField<vector, volMesh> const&,
        dictionary const&
    );

    //- Construct by mapping the given noSlipWallFvPatchVectorField<vector>
    //  onto a new patch
    noSlipWallFvPatchVectorField
    (
        noSlipWallFvPatchVectorField const&,
        fvPatch const&,
        DimensionedField<vector, volMesh> const&,
        fvPatchFieldMapper const&
    );

    //- Construct as copy
    noSlipWallFvPatchVectorField
    (
        noSlipWallFvPatchVectorField const&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchField<vector> > clone() const
    {
        return tmp<fvPatchField<vector> >
        (
            new noSlipWallFvPatchVectorField(*this)
        );
    }

    //- Construct as copy setting internal field reference
    noSlipWallFvPatchVectorField
    (
        noSlipWallFvPatchVectorField const&,
        DimensionedField<vector, volMesh> const&
    );

    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<vector> > clone
    (
        DimensionedField<vector, volMesh> const& iF
    ) const
    {
        return tmp<fvPatchField<vector> >
        (
            new noSlipWallFvPatchVectorField(*this, iF)
        );
    }

    // Member functions

    // Evaluation functions

    //- Return the matrix diagonal coefficients corresponding to the
    //  evaluation of the value of this patchField with given weights
    virtual tmp<Field<vector> > valueInternalCoeffs
    (
        tmp<scalarField> const&
    ) const;

    //- Return the matrix source coefficients corresponding to the
    //  evaluation of the value of this patchField with given weights
    virtual tmp<Field<vector> > valueBoundaryCoeffs
    (
        tmp<scalarField> const&
    ) const;

    //- Return the matrix diagonal coefficients corresponding to the
    //  evaluation of the gradient of this patchField
    virtual tmp<Field<vector> > gradientInternalCoeffs() const;

    //- Return the matrix source coefficients corresponding to the
    //  evaluation of the gradient of this patchField
    virtual tmp<Field<vector> > gradientBoundaryCoeffs() const;

    //- Write
    virtual void write(Ostream&) const;

};


}


#endif

