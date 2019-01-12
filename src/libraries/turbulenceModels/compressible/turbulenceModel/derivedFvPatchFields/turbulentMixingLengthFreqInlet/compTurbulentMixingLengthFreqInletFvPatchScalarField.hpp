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
    CML::compressible::turbulentMixingLengthFrequencyInletFvPatchScalarField

Description
    This boundary condition provides a turbulence specific dissipation,
    \f$\omega\f$ (omega) inlet condition based on a specified mixing length.
    The patch values are calculated using:

        \f[
            \omega_p = \frac{k^{0.5}}{C_{\mu}^{0.25} L}
        \f]
   
    where

    \vartable
        \omega_p | patch omega values
        C_{\mu} | Model coefficient, set to 0.09
        k       | turbulence kinetic energy
        L       | length scale
    \endvartable

    \heading Patch usage

    \table
        Property     | Description             | Required    | Default value
        mixingLength | Length scale [m]        | yes         |
        phi          | flux field name         | no          | phi
        k            | turbulence kinetic energy field name | no | k
    \endtable

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            compressible::turbulentMixingLengthFrequencyInlet;
        mixingLength    0.005;
        value           uniform 200;   // placeholder
    }
    \endverbatim

Note
    In the event of reverse flow, a zero-gradient condition is applied

SeeAlso
    CML::inletOutletFvPatchField

SourceFiles
    compTurbulentMixingLengthFrequencyInletFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef compressibleturbulentMixingLengthFrequencyInletFvPatchScalarField_H
#define compressibleturbulentMixingLengthFrequencyInletFvPatchScalarField_H

#include "inletOutletFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{

/*---------------------------------------------------------------------------*\
    Class turbulentMixingLengthFrequencyInletFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class turbulentMixingLengthFrequencyInletFvPatchScalarField
:
    public inletOutletFvPatchScalarField
{
    // Private data

        //- Turbulent length scale
        scalar mixingLength_;

        //- Name of the turbulent kinetic energy field
        word kName_;


public:

    //- Runtime type information
    TypeName("compressible::turbulentMixingLengthFrequencyInlet");


    // Constructors

        //- Construct from patch and internal field
        turbulentMixingLengthFrequencyInletFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        turbulentMixingLengthFrequencyInletFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  turbulentMixingLengthFrequencyInletFvPatchScalarField
        //  onto a new patch
        turbulentMixingLengthFrequencyInletFvPatchScalarField
        (
            const turbulentMixingLengthFrequencyInletFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        turbulentMixingLengthFrequencyInletFvPatchScalarField
        (
            const turbulentMixingLengthFrequencyInletFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new turbulentMixingLengthFrequencyInletFvPatchScalarField
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        turbulentMixingLengthFrequencyInletFvPatchScalarField
        (
            const turbulentMixingLengthFrequencyInletFvPatchScalarField&,
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
                new turbulentMixingLengthFrequencyInletFvPatchScalarField
                (
                    *this,
                    iF
                )
            );
        }


    // Member functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace compressible
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
