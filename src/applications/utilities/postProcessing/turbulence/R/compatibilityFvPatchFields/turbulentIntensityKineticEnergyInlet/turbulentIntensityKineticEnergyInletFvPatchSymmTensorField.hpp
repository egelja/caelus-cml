/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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
    CML::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField

Description
    Dummy version of turbulentIntensityKineticEnergyInlet patch type for
    symmetric tensors provided for compatibility with R utility.

    Value is given by a fixed-value condition.


SourceFiles
    turbulentIntensityKineticEnergyInletFvPatchSymmTensorField.cpp

\*---------------------------------------------------------------------------*/

#ifndef turbulentIntensityKineticEnergyInletFvPatchSymmTensorField_H
#define turbulentIntensityKineticEnergyInletFvPatchSymmTensorField_H

#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
 Class turbulentIntensityKineticEnergyInletFvPatchSymmTensorField Declaration
\*---------------------------------------------------------------------------*/

class turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
:
    public fixedValueFvPatchSymmTensorField
{
public:

    //- Runtime type information
    TypeName("turbulentIntensityKineticEnergyInlet");


    // Constructors

        //- Construct from patch and internal field
        turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
        (
            const fvPatch&,
            const DimensionedField<symmTensor, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
        (
            const fvPatch&,
            const DimensionedField<symmTensor, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
        //  onto a new patch
        turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
        (
            const turbulentIntensityKineticEnergyInletFvPatchSymmTensorField&,
            const fvPatch&,
            const DimensionedField<symmTensor, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
        (
            const turbulentIntensityKineticEnergyInletFvPatchSymmTensorField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchSymmTensorField> clone() const
        {
            return tmp<fvPatchSymmTensorField>
            (
                new turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
        (
            const turbulentIntensityKineticEnergyInletFvPatchSymmTensorField&,
            const DimensionedField<symmTensor, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchSymmTensorField> clone
        (
            const DimensionedField<symmTensor, volMesh>& iF
        ) const
        {
            return tmp<fvPatchSymmTensorField>
            (
                new turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
                (
                    *this,
                    iF
                )
            );
        }


    // Member functions

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
