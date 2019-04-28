/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2019 Applied CCM Pty Ltd
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
    CML::mixture::mutWallFunctionFvPatchScalarField

Description
    This boundary condition provides a turbulent kinematic viscosity condition
    when using wall functions, based on turbulence kinetic energy.

    \heading Patch usage

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            nutWallFunction;
    }
    \endverbatim

SeeAlso
    CML::fixedValueFvPatchField

SourceFiles
    mutWallFunctionFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef mxMutWallFunctionFvPatchScalarField_HPP
#define mxMutWallFunctionFvPatchScalarField_HPP

#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace mixture
{

/*---------------------------------------------------------------------------*\
             Class mutWallFunctionFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class mutWallFunctionFvPatchScalarField
:
    public fixedValueFvPatchScalarField
{
protected:

    //- Cmu coefficient
    scalar Cmu_;

    //- Von Karman constant
    scalar kappa_;

    //- E coefficient
    scalar E_;

    //- Y+ at the edge of the laminar sublayer
    scalar yPlusLam_;


    //- Check the type of the patch
    virtual void checkType();

    //- Calculate the turbulence viscosity
    virtual tmp<scalarField> calcMut() const = 0;

    //- Write local wall function variables
    virtual void writeLocalEntries(Ostream&) const;


public:

    //- Runtime type information
    TypeName("mixture::mutWallFunction");


    //- Construct from patch and internal field
    mutWallFunctionFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    mutWallFunctionFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given
    //  mutWallFunctionFvPatchScalarField
    //  onto a new patch
    mutWallFunctionFvPatchScalarField
    (
        const mutWallFunctionFvPatchScalarField&,
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    mutWallFunctionFvPatchScalarField
    (
        const mutWallFunctionFvPatchScalarField&
    );

    //- Construct as copy setting internal field reference
    mutWallFunctionFvPatchScalarField
    (
        const mutWallFunctionFvPatchScalarField&,
        const DimensionedField<scalar, volMesh>&
    );


    // Member functions

    //- Calculate the Y+ at the edge of the laminar sublayer
    static scalar yPlusLam(const scalar kappa, const scalar E);

    //- Calculate and return the yPlus at the boundary
    virtual tmp<scalarField> yPlus() const = 0;

    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();

    //- Write
    virtual void write(Ostream&) const;
};


} // End namespace mixture
} // End namespace CML


#endif
