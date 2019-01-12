/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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
    CML::temperatureDirectedInletOutletVelocityFvPatchVectorField

Description
    CML::temperatureDirectedInletOutletVelocityFvPatchVectorField

SourceFiles
    temperatureDirectedInletOutletVelocityFvPatchVectorField.cpp

Author
    Aleksandar Jemcov
    Oliver Borm

\*---------------------------------------------------------------------------*/

#ifndef temperatureDirectedInletOutletVelocityFvPatchVectorField_HPP
#define temperatureDirectedInletOutletVelocityFvPatchVectorField_HPP

#include "fvPatchFields.hpp"
#include "mixedFvPatchFields.hpp"
#include "Switch.hpp"

namespace CML
{

class temperatureDirectedInletOutletVelocityFvPatchVectorField :
    public mixedFvPatchVectorField
{
    //- Name of flux field
    word phiName_;

    //- Name of static Temperature field
    word TName_;

    //- Total Temperature field
    scalarField T0_;

    //- Inlet direction
    vectorField inletDir_;

    //- Is the supplied inlet value in cartesian or cylindrical coordinates?
    Switch cylindricalCCS_;

    //- Angular velocity of the frame
    vector omega_;

public:

    //- Runtime type information
    TypeName("temperatureDirectedInletOutletVelocity");


    //- Construct from patch and internal field
    temperatureDirectedInletOutletVelocityFvPatchVectorField
    (
        fvPatch const&,
        DimensionedField<vector, volMesh> const&
    );

    //- Construct from patch, internal field and dictionary
    temperatureDirectedInletOutletVelocityFvPatchVectorField
    (
        fvPatch const&,
        DimensionedField<vector, volMesh> const&,
        dictionary const&
    );

    //- Construct by mapping given
    //  temperatureDirectedInletOutletVelocityFvPatchVectorField
    //  onto a new patch
    temperatureDirectedInletOutletVelocityFvPatchVectorField
    (
        temperatureDirectedInletOutletVelocityFvPatchVectorField const&,
        fvPatch const&,
        DimensionedField<vector, volMesh> const&,
        fvPatchFieldMapper const&
    );

    //- Construct as copy
    temperatureDirectedInletOutletVelocityFvPatchVectorField
    (
        temperatureDirectedInletOutletVelocityFvPatchVectorField const&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
        return tmp<fvPatchVectorField>
        (
            new temperatureDirectedInletOutletVelocityFvPatchVectorField
            (
                *this
            )
        );
    }

    //- Construct as copy setting internal field reference
    temperatureDirectedInletOutletVelocityFvPatchVectorField
    (
        temperatureDirectedInletOutletVelocityFvPatchVectorField const&,
        DimensionedField<vector, volMesh> const&
    );

    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchVectorField> clone
    (
        DimensionedField<vector, volMesh> const& iF
    ) const
    {
        return tmp<fvPatchVectorField>
        (
            new temperatureDirectedInletOutletVelocityFvPatchVectorField
            (
                *this,
                iF
            )
        );
    }


    // Member functions

    // Access

    //- Return the name of phi
    word const& phiName() const
    {
         return phiName_;
    }


    // Mapping functions

    //- Map (and resize as needed) from self given a mapping object
    virtual void autoMap
    (
        fvPatchFieldMapper const&
    );

    //- Reverse map the given fvPatchField onto this fvPatchField
    virtual void rmap
    (
        fvPatchVectorField const&,
        labelList const&
    );


    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();

    //- Write
    virtual void write(Ostream&) const;
};


}

#endif
