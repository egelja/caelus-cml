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
    CML::mixedUnburntEnthalpyFvPatchScalarField

Description
    Mixed boundary condition for unburnt

SourceFiles
    mixedUnburntEnthalpyFvPatchScalarField.cpp

\*---------------------------------------------------------------------------*/

#ifndef mixedUnburntEnthalpyFvPatchScalarField_HPP
#define mixedUnburntEnthalpyFvPatchScalarField_HPP

#include "mixedFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
           Class mixedUnburntEnthalpyFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class mixedUnburntEnthalpyFvPatchScalarField
:
    public mixedFvPatchScalarField
{

public:

    //- Runtime type information
    TypeName("mixedUnburntEnthalpy");


    //- Construct from patch and internal field
    mixedUnburntEnthalpyFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    mixedUnburntEnthalpyFvPatchScalarField
    (
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given mixedUnburntEnthalpyFvPatchScalarField
    // onto a new patch
    mixedUnburntEnthalpyFvPatchScalarField
    (
        const mixedUnburntEnthalpyFvPatchScalarField&,
        const fvPatch&,
        const DimensionedField<scalar, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    mixedUnburntEnthalpyFvPatchScalarField
    (
        const mixedUnburntEnthalpyFvPatchScalarField&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
        return tmp<fvPatchScalarField>
        (
            new mixedUnburntEnthalpyFvPatchScalarField(*this)
        );
    }

    //- Construct as copy setting internal field reference
    mixedUnburntEnthalpyFvPatchScalarField
    (
        const mixedUnburntEnthalpyFvPatchScalarField&,
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
            new mixedUnburntEnthalpyFvPatchScalarField(*this, iF)
        );
    }


    // Member functions

    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();

};


} // End namespace CML


#endif
