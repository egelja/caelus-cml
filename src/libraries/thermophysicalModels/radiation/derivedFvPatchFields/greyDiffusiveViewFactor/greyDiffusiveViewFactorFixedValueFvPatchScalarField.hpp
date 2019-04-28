/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::greyDiffusiveViewFactorFixedValueFvPatchScalarField

Description
    This boundary condition provides a grey-diffuse condition for radiative
    heat flux, \c qr, for use with the view factor model

Usage
    \table
        Property     | Description             | Required    | Default value
        qro          | external radiative heat flux | yes    |
        emissivityMode | emissivity mode: solidRadiation or lookup | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            greyDiffusiveRadiationViewFactor;
        qro             uniform 0;
        emissivityMode  solidRadiation;
        value           uniform 0;
    }
    \endverbatim

See also
    CML::radiationCoupledBase
    CML::radiation::radiationModel
    CML::radiation::viewFactor
    CML::fixedValueFvPatchField

SourceFiles
    greyDiffusiveViewFactorFixedValueFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef greyDiffusiveViewFactorFixedValueFvPatchScalarField_HPP
#define greyDiffusiveViewFactorFixedValueFvPatchScalarField_HPP

#include "radiationCoupledBase.hpp"
#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace radiation
{

/*---------------------------------------------------------------------------*\
      Class greyDiffusiveViewFactorFixedValueFvPatchScalarField declaration
\*---------------------------------------------------------------------------*/

class greyDiffusiveViewFactorFixedValueFvPatchScalarField
:
    public fixedValueFvPatchScalarField,
    public radiationCoupledBase
{

    //- External radiative heat flux
    scalarField qro_;


public:

    //- Runtime type information
    TypeName("greyDiffusiveRadiationViewFactor");


    //- Construct from patch and internal field
    greyDiffusiveViewFactorFixedValueFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    greyDiffusiveViewFactorFixedValueFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given a
    // greyDiffusiveViewFactorFixedValueFvPatchScalarField onto a new patch
    greyDiffusiveViewFactorFixedValueFvPatchScalarField
    (
        const greyDiffusiveViewFactorFixedValueFvPatchScalarField&,
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    greyDiffusiveViewFactorFixedValueFvPatchScalarField
    (
        const greyDiffusiveViewFactorFixedValueFvPatchScalarField&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
        return tmp<fvPatchScalarField>
        (
            new greyDiffusiveViewFactorFixedValueFvPatchScalarField(*this)
        );
    }

    //- Construct as copy setting internal field reference
    greyDiffusiveViewFactorFixedValueFvPatchScalarField
    (
        const greyDiffusiveViewFactorFixedValueFvPatchScalarField&,
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
            new greyDiffusiveViewFactorFixedValueFvPatchScalarField
            (
                *this,
                iF
            )
        );
    }


    // Member functions

    // Access

    //- Return external radiative heat flux
    const scalarList& qro()
    {
        return qro_;
    }


    // Evaluation functions

    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();


    // I-O

    //- Write
    virtual void write(Ostream&) const;

};


} // End namespace radiation
} // End namespace CML


#endif
