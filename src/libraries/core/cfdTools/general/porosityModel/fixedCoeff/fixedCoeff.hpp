/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::fixedCoeff

Description
    Fixed coefficient form of porosity model

        \f[
            S = - \rho_ref (\alpha + \beta |U|) U
        \f]

    In the case of compressible flow, a value for the reference density is
    required

SourceFiles
    fixedCoeff.cpp

\*---------------------------------------------------------------------------*/

#ifndef fixedCoeff_H
#define fixedCoeff_H

#include "porosityModel.hpp"
#include "coordinateSystem.hpp"
#include "dimensionedTensor.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace porosityModels
{

/*---------------------------------------------------------------------------*\
                      Class fixedCoeff Declaration
\*---------------------------------------------------------------------------*/

class fixedCoeff
:
    public porosityModel
{
private:

    // Private data

        //- Local co-ordinate system
        coordinateSystem coordSys_;

        //- Model alpha coefficient [1/s]
        dimensionedTensor alpha_;

        //- Model beta coefficient [1/m]
        dimensionedTensor beta_;


    // Private Member Functions

        //- Apply
        void apply
        (
            scalarField& Udiag,
            vectorField& Usource,
            const scalarField& V,
            const vectorField& U,
            const scalar rho
        ) const;

        //- Apply
        void apply
        (
            tensorField& AU,
            const vectorField& U,
            const scalar rho
        ) const;

        //- Disallow default bitwise copy construct
        fixedCoeff(const fixedCoeff&);

        //- Disallow default bitwise assignment
        void operator=(const fixedCoeff&);


public:

    //- Runtime type information
    TypeName("fixedCoeff");

    //- Constructor
    fixedCoeff
    (
        const word& name,
        const word& modelType,
        const fvMesh& mesh,
        const dictionary& dict,
        const word& cellZoneName
    );

    //- Destructor
    virtual ~fixedCoeff();


    // Member Functions

        //- Calculate the porosity force
        virtual void calcForce
        (
            const volVectorField& U,
            const volScalarField& rho,
            const volScalarField& mu,
            vectorField& force
        ) const;

        //- Add resistance
        virtual void correct(fvVectorMatrix& UEqn) const;

        //- Add resistance
        virtual void correct
        (
            fvVectorMatrix& UEqn,
            const volScalarField& rho,
            const volScalarField& mu
        ) const;

        //- Add resistance
        virtual void correct
        (
            const fvVectorMatrix& UEqn,
            volTensorField& AU            
        ) const;


    // I-O

        //- Write
        bool writeData(Ostream& os) const;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace porosityModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
