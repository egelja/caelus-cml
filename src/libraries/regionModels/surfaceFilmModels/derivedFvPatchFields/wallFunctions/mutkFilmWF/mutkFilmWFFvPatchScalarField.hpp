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
    CML::compressible::RASModels::mutkFilmWallFunctionFvPatchScalarField

Description
    This boundary condition provides a turbulent viscosity condition when
    using wall functions, based on turbulence kinetic energy, for use with
    surface film models.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            mutkFilmWallFunction;
        value           uniform 0;
    }
    \endverbatim

See also
    Foam::nutkWallFunctionFvPatchScalarField

SourceFiles
    mutkFilmWallFunctionFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef compressibleMutSpalartAllmarasWallFunctionFvPatchScalarField_H
#define compressibleMutSpalartAllmarasWallFunctionFvPatchScalarField_H

#include "mutkWFFvPatchScalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{
namespace RASModels
{

/*---------------------------------------------------------------------------*\
         Class mutkFilmWallFunctionFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class mutkFilmWallFunctionFvPatchScalarField
:
    public mutkWallFunctionFvPatchScalarField
{
protected:

    // Protected data

        //- B Coefficient (default = 5.5)
        scalar B_;

        //- y+ value for laminar -> turbulent transition (default = 11.05)
        scalar yPlusCrit_;


    // Protected member functions

        //- Calculate the turbulence viscosity
        virtual tmp<scalarField> calcMut() const;

        //- Calculate the friction velocity
        virtual tmp<scalarField> calcUTau(const scalarField& magGradU) const;


public:

    //- Runtime type information
    TypeName("mutkFilmWallFunction");


    // Constructors

        //- Construct from patch and internal field
        mutkFilmWallFunctionFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mutkFilmWallFunctionFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  mutkFilmWallFunctionFvPatchScalarField
        //  onto a new patch
        mutkFilmWallFunctionFvPatchScalarField
        (
            const mutkFilmWallFunctionFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        mutkFilmWallFunctionFvPatchScalarField
        (
            const mutkFilmWallFunctionFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new mutkFilmWallFunctionFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        mutkFilmWallFunctionFvPatchScalarField
        (
            const mutkFilmWallFunctionFvPatchScalarField&,
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
                new mutkFilmWallFunctionFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //- Calculate and return the yPlus at the boundary
            virtual tmp<scalarField> yPlus() const;


        // I-O

            //- Write
            virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RASModels
} // End namespace compressible
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
