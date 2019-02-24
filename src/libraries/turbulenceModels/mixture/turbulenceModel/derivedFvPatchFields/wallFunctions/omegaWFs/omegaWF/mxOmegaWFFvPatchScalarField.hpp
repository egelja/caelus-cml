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
    CML::mixture::omegaWallFunctionFvPatchScalarField

Description
    Provides a wall function boundary condition/constraint on omega

    Computed value is:

        omega = sqrt(omega_vis^2 + omega_log^2)

    where
        omega_vis = omega in viscous region
        omega_log = omega in logarithmic region

    \vartable
        \omega_{vis} | omega in viscous region
        \omega_{log} | omega in logarithmic region
    \endvartable

    Model described by Eqs.(14, 15) of:
    \verbatim
        Menter, F., Esch, T.
        "Elements of Industrial Heat Transfer Prediction"
        16th Brazilian Congress of Mechanical Engineering (COBEM),
        Nov. 2001
    \endverbatim

    \heading Patch usage

    \table
        Property     | Description             | Required    | Default value
        Cmu          | model coefficient       | no          | 0.09
        kappa        | Von Karman constant     | no          | 0.41
        E            | model coefficient       | no          | 9.8
        beta1        | model coefficient       | no          | 0.075
    \endtable

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            omegaWallFunction;
    }
    \endverbatim

SourceFiles
    mxOmegaWallFunctionFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef mxOmegaWallFunctionFvPatchScalarField_H
#define mxOmegaWallFunctionFvPatchScalarField_H

#include "fixedInternalValueFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace mixture
{

class turbulenceModel;

/*---------------------------------------------------------------------------*\
           Class omegaWallFunctionFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class omegaWallFunctionFvPatchScalarField
:
    public fixedInternalValueFvPatchField<scalar>
{
protected:

    //- Cmu coefficient
    scalar Cmu_;

    //- Von Karman constant
    scalar kappa_;

    //- E coefficient
    scalar E_;

    //- beta1 coefficient
    scalar beta1_;

    //- Y+ at the edge of the laminar sublayer
    scalar yPlusLam_;


    //- Check the type of the patch
    virtual void checkType();

    //- Calculate the Y+ at the edge of the laminar sublayer
    virtual scalar calcYPlusLam(const scalar kappa, const scalar E) const;

    //- Write local wall function variables
    virtual void writeLocalEntries(Ostream&) const;


public:

    //- Runtime type information
    TypeName("mixture::omegaWallFunction");


    //- Construct from patch and internal field
    omegaWallFunctionFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    omegaWallFunctionFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given
    // omegaWallFunctionFvPatchScalarField
    //  onto a new patch
    omegaWallFunctionFvPatchScalarField
    (
        const omegaWallFunctionFvPatchScalarField&,
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    omegaWallFunctionFvPatchScalarField
    (
        const omegaWallFunctionFvPatchScalarField&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
        return tmp<fvPatchScalarField>
        (
            new omegaWallFunctionFvPatchScalarField(*this)
        );
    }

    //- Construct as copy setting internal field reference
    omegaWallFunctionFvPatchScalarField
    (
        const omegaWallFunctionFvPatchScalarField&,
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
            new omegaWallFunctionFvPatchScalarField(*this, iF)
        );
    }


    // Member functions

    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();

    //- Write
    virtual void write(Ostream&) const;
};


} // End namespace mixture
} // End namespace CML


#endif
