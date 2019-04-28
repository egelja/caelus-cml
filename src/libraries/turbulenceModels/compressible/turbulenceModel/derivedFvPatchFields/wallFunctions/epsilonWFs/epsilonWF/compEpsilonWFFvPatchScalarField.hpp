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
    CML::compressible::RASModels::epsilonWallFunctionFvPatchScalarField

Description
    Boundary condition for epsilon when using wall functions
    - calculates epsilon and G
    - epsilon values added directly into the matrix to act as a constraint

SourceFiles
    compEpsilonWallFunctionFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef compressibleEpsilonWallFunctionFvPatchScalarField_H
#define compressibleEpsilonWallFunctionFvPatchScalarField_H

#include "fixedInternalValueFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{

/*---------------------------------------------------------------------------*\
           Class epsilonWallFunctionFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class epsilonWallFunctionFvPatchScalarField
:
    public fixedInternalValueFvPatchField<scalar>
{
protected:

    // Protected data

        //- Cmu coefficient
        scalar Cmu_;

        //- Von Karman constant
        scalar kappa_;

        //- E coefficient
        scalar E_;

        //- Y+ at the edge of the laminar sublayer
        scalar yPlusLam_;


    // Protected Member Functions

        //- Check the type of the patch
        virtual void checkType();

        //- Calculate the Y+ at the edge of the laminar sublayer
        virtual scalar calcYPlusLam(const scalar kappa, const scalar E) const;

        //- Write local wall function variables
        virtual void writeLocalEntries(Ostream&) const;


public:

    //- Runtime type information
    TypeName("compressible::epsilonWallFunction");


    // Constructors

        //- Construct from patch and internal field
        epsilonWallFunctionFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        epsilonWallFunctionFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  epsilonWallFunctionFvPatchScalarField
        //  onto a new patch
        epsilonWallFunctionFvPatchScalarField
        (
            const epsilonWallFunctionFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        epsilonWallFunctionFvPatchScalarField
        (
            const epsilonWallFunctionFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new epsilonWallFunctionFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        epsilonWallFunctionFvPatchScalarField
        (
            const epsilonWallFunctionFvPatchScalarField&,
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
                new epsilonWallFunctionFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();

            //- Evaluate the patchField
            virtual void evaluate(const Pstream::commsTypes);


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
