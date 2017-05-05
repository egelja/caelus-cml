/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    atmBoundaryLayerInletVelocityFvPatchVectorField

Description
    Boundary condition specifies a atmospheric boundary layer (ABL)
    velocity inlet profile given the friction velocity value,
    flow direction n and direction of the parabolic coordinate z.

    \verbatim
        U = (Ustar/K) ln((z - zGround + z0)/z0)

    where:

        Ustar is the frictional velocity
        K is karman's constant
        z0 is the surface roughness lenght
        z is the verical coordinate
        zGround is the minumum coordinate value in z direction.

    and:

        Ustar = K Uref/ln((Zref + z0)/z0)

    where:

        Uref is the reference velocity at Zref
        Zref is the reference height.

    \endverbatim

    Reference:
    D.M. Hargreaves and N.G. Wright
    "On the use of the k-epsilon model in commercial CFD software to model the
     neutral atmospheric boundary layer"
    Journal of Wind Engineering and Industrial Aerodynamics 95(2007) 355-369.

NOTE: D.M. Hargreaves and N.G. Wright recommend Gamma epsilon in the k-epsilon
      model should be changed from 1.3 to 1.11 for consistency.
      The roughness height (Er) is given by Er = 20 z0 following the same
      reference

SourceFiles
    atmBoundaryLayerInletVelocityFvPatchVectorField.cpp

\*---------------------------------------------------------------------------*/

#ifndef atmBoundaryLayerInletVelocityFvPatchVectorField_H
#define atmBoundaryLayerInletVelocityFvPatchVectorField_H

#include "fvPatchFields.hpp"
#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace incompressible
{

/*---------------------------------------------------------------------------*\
       Class atmBoundaryLayerInletVelocityFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class atmBoundaryLayerInletVelocityFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    // Private data

        //- Frictional velocity
        scalarField Ustar_;

        //- Flow direction
        vector n_;

        //- Direction of the z-coordinate
        vector z_;

        //- Surface roughness lenght
        scalarField z0_;

        //- Von Karman constant
        const scalar kappa_;

        //- Reference velocity
        const scalar Uref_;

        //- Reference hight
        const scalar Href_;

        //- Minimum corrdinate value in z direction
        scalarField zGround_;


public:

    //- Runtime type information
    TypeName("atmBoundaryLayerInletVelocity");


    // Constructors

        //- Construct from patch and internal field
        atmBoundaryLayerInletVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        atmBoundaryLayerInletVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        // atmBoundaryLayerInletVelocityFvPatchVectorField onto a new patch
        atmBoundaryLayerInletVelocityFvPatchVectorField
        (
            const atmBoundaryLayerInletVelocityFvPatchVectorField&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchVectorField> clone() const
        {
            return tmp<fvPatchVectorField>
            (
                new atmBoundaryLayerInletVelocityFvPatchVectorField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        atmBoundaryLayerInletVelocityFvPatchVectorField
        (
            const atmBoundaryLayerInletVelocityFvPatchVectorField&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchVectorField> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return tmp<fvPatchVectorField>
            (
                new atmBoundaryLayerInletVelocityFvPatchVectorField(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return Ustar
            const scalarField& Ustar() const
            {
                return Ustar_;
            }

            //- Return flow direction
            const vector& n() const
            {
                return n_;
            }

            //- Return z direction
            const vector& z() const
            {
                return z_;
            }


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchVectorField&,
                const labelList&
            );


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace incompressible
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
