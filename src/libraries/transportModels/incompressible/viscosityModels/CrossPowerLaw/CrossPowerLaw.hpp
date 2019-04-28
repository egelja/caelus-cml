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
    CML::viscosityModels::CrossPowerLaw

Description
    An incompressible Cross-Power law non-Newtonian viscosity model.

SourceFiles
    CrossPowerLaw.cpp

\*---------------------------------------------------------------------------*/

#ifndef CrossPowerLaw_H
#define CrossPowerLaw_H

#include "viscosityModel.hpp"
#include "dimensionedScalar.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace viscosityModels
{

/*---------------------------------------------------------------------------*\
                           Class CrossPowerLaw Declaration
\*---------------------------------------------------------------------------*/

class CrossPowerLaw
:
    public viscosityModel
{
    // Private data

        dictionary CrossPowerLawCoeffs_;

        dimensionedScalar mu0_;
        dimensionedScalar muInf_;
        dimensionedScalar lambda_;
        dimensionedScalar n_;
        dimensionedScalar rhoRef_;

        volScalarField nu_;

    // Private Member Functions

        //- Calculate and return the laminar viscosity
        tmp<volScalarField> calcNu() const;


public:

    //- Runtime type information
    TypeName("CrossPowerLaw");


    // Constructors

        //- Construct from components
        CrossPowerLaw
        (
            const word& name,
            const dictionary& viscosityProperties,
            const volVectorField& U,
            const surfaceScalarField& phi
        );


    //- Destructor
    ~CrossPowerLaw()
    {}


    // Member Functions

        //- Return the laminar viscosity
        tmp<volScalarField> nu() const
        {
            return nu_;
        }

        //- Correct the laminar viscosity
        void correct();

        //- Read transportProperties dictionary
        bool read(const dictionary& viscosityProperties);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace viscosityModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
