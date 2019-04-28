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
    CML::mixture::mutUSpaldingWallFunctionFvPatchScalarField

Description
    This boundary condition provides a turbulent kinematic viscosity condition
    when using wall functions for rough walls, based on velocity,  using
    Spalding's law to give a continuous nut profile to the wall (y+ = 0)

        \f[
            y^+ = u^+ + \frac{1}{E} \left[exp(\kappa u^+) - 1 - \kappa u^+\,
                - 0.5 (\kappa u^+)^2 - \frac{1}{6} (\kappa u^+)^3\right]
        \f]

    where
    \vartable
        y^+     | non-dimensional position
        u^+     | non-dimensional velocity
        \kappa  | Von Karman constant
    \endvartable


    \heading Patch usage

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            nutUSpaldingWallFunction;
    }
    \endverbatim

SeeAlso
    CML::nutWallFunctionFvPatchScalarField

SourceFiles
    mxmutUSpaldingWFFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef mxMutUSpaldingWallFunctionFvPatchScalarField_HPP
#define mxMutUSpaldingWallFunctionFvPatchScalarField_HPP

#include "mxmutWFFvPatchScalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace mixture
{

/*---------------------------------------------------------------------------*\
          Class mutUSpaldingWallFunctionFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class mutUSpaldingWallFunctionFvPatchScalarField
:
    public mutWallFunctionFvPatchScalarField
{
protected:

    //- Calculate the turbulence viscosity
    virtual tmp<scalarField> calcMut() const;

    //- Calculate the friction velocity
    virtual tmp<scalarField> calcUTau(const scalarField& magGradU) const;


public:

    //- Runtime type information
    TypeName("mixture::mutUSpaldingWallFunction");


    //- Construct from patch and internal field
    mutUSpaldingWallFunctionFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    mutUSpaldingWallFunctionFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given
    //  mutUSpaldingWallFunctionFvPatchScalarField
    //  onto a new patch
    mutUSpaldingWallFunctionFvPatchScalarField
    (
        const mutUSpaldingWallFunctionFvPatchScalarField&,
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    mutUSpaldingWallFunctionFvPatchScalarField
    (
        const mutUSpaldingWallFunctionFvPatchScalarField&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
        return tmp<fvPatchScalarField>
        (
            new mutUSpaldingWallFunctionFvPatchScalarField(*this)
        );
    }

    //- Construct as copy setting internal field reference
    mutUSpaldingWallFunctionFvPatchScalarField
    (
        const mutUSpaldingWallFunctionFvPatchScalarField&,
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
            new mutUSpaldingWallFunctionFvPatchScalarField(*this, iF)
        );
    }


    // Member functions

    //- Calculate and return the yPlus at the boundary
    virtual tmp<scalarField> yPlus() const;

    //- Write
    virtual void write(Ostream& os) const;
};


} // End namespace mixture
} // End namespace CML


#endif
