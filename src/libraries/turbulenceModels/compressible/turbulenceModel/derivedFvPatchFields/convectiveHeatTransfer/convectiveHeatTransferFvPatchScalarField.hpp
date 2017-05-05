/*---------------------------------------------------------------------------*\
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
    CML::compressible::RASModels::convectiveHeatTransferFvPatchScalarField

Description
    Convective heat transfer boundary condition

SourceFiles
    convectiveHeatTransferFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef compressibleMutRoughWallFunctionFvPatchScalarField_H
#define compressibleMutRoughWallFunctionFvPatchScalarField_H

#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{

/*---------------------------------------------------------------------------*\
                 Class convectiveHeatTransferFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class convectiveHeatTransferFvPatchScalarField
:
    public fixedValueFvPatchScalarField
{
protected:

    // Protected data

        //- L Length scale [m]
        const scalar L_;


public:

    //- Runtime type information
    TypeName("convectiveHeatTransfer");


    // Constructors

        //- Construct from patch and internal field
        convectiveHeatTransferFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        convectiveHeatTransferFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  convectiveHeatTransferFvPatchScalarField
        //  onto a new patch
        convectiveHeatTransferFvPatchScalarField
        (
            const convectiveHeatTransferFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        convectiveHeatTransferFvPatchScalarField
        (
            const convectiveHeatTransferFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new convectiveHeatTransferFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        convectiveHeatTransferFvPatchScalarField
        (
            const convectiveHeatTransferFvPatchScalarField&,
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
                new convectiveHeatTransferFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        // I-O

            //- Write
            virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace compressible
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
