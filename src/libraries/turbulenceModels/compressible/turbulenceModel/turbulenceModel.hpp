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

Namespace
    CML::compressible::turbulenceModels

Description
    Namespace for compressible turbulence turbulence models.


Class
    CML::compressible::turbulenceModel

Description
    Abstract base class for compressible turbulence models
    (RAS, LES and laminar).


SourceFiles
    turbulenceModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef compressibleturbulenceModel_H
#define compressibleturbulenceModel_H

#include "primitiveFieldsFwd.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "fvMatricesFwd.hpp"
#include "basicThermo.hpp"
#include "nearWallDist.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declarations
class fvMesh;

namespace compressible
{

/*---------------------------------------------------------------------------*\
                       Class turbulenceModel Declaration
\*---------------------------------------------------------------------------*/

class turbulenceModel
:
    public regIOobject
{

protected:

    // Protected data

        const Time& runTime_;
        const fvMesh& mesh_;

        const volScalarField& rho_;
        const volVectorField& U_;
        const surfaceScalarField& phi_;

        const basicThermo& thermophysicalModel_;

        //- Near wall distance boundary field
        nearWallDist y_;


private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        turbulenceModel(const turbulenceModel&);

        //- Disallow default bitwise assignment
        void operator=(const turbulenceModel&);


public:

    //- Runtime type information
    TypeName("turbulenceModel");


    // Declare run-time constructor selection table

        declareRunTimeNewSelectionTable
        (
            autoPtr,
            turbulenceModel,
            turbulenceModel,
            (
                const volScalarField& rho,
                const volVectorField& U,
                const surfaceScalarField& phi,
                const basicThermo& thermoPhysicalModel,
                const word& turbulenceModelName
            ),
            (rho, U, phi, thermoPhysicalModel, turbulenceModelName)
        );


    // Constructors

        //- Construct from components
        turbulenceModel
        (
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const basicThermo& thermoPhysicalModel,
            const word& turbulenceModelName = typeName
        );


    // Selectors

        //- Return a reference to the selected turbulence model
        static autoPtr<turbulenceModel> New
        (
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const basicThermo& thermoPhysicalModel,
            const word& turbulenceModelName = typeName
        );


    //- Destructor
    virtual ~turbulenceModel()
    {}


    // Member Functions

        //- Const access to the coefficients dictionary
        virtual const dictionary& coeffDict() const = 0;

        //- Helper function to return the name of the turbulence G field
        inline word GName() const
        {
            return word(type() + ":G");
        }

        //- Access function to density field
        const volScalarField& rho() const
        {
            return rho_;
        }

        //- Access function to velocity field
        const volVectorField& U() const
        {
            return U_;
        }

        //- Access function to flux field
        const surfaceScalarField& phi() const
        {
            return phi_;
        }

        //- Access function to thermophysical model
        const basicThermo& thermo() const
        {
             return thermophysicalModel_;
        }

        //- Return the near wall distances
        const nearWallDist& y() const
        {
            return y_;
        }

        //- Return the laminar viscosity
        const volScalarField& mu() const
        {
            return thermophysicalModel_.mu();
        }

        //- Return the laminar thermal conductivity
        const volScalarField& alpha() const
        {
            return thermophysicalModel_.alpha();
        }

        //- Return the turbulence viscosity
        virtual tmp<volScalarField> mut() const = 0;

        //- Return the effective viscosity
        virtual tmp<volScalarField> muEff() const = 0;

        //- Return the turbulence thermal diffusivity
        virtual tmp<volScalarField> alphat() const = 0;

        //- Return the effective turbulence thermal diffusivity
        virtual tmp<volScalarField> alphaEff() const = 0;

        //- Return the effective turbulence thermal diffusivity for a patch
        virtual tmp<scalarField> alphaEff(const label patchI) const = 0;

        //- Return the turbulence kinetic energy
        virtual tmp<volScalarField> k() const = 0;

        //- Return the turbulence kinetic energy dissipation rate
        virtual tmp<volScalarField> epsilon() const = 0;

        //- Return the laminar+turbulence kinetic energy dissipation rate
        //  Used as the viscous contribution to the energy equations
        virtual tmp<volScalarField> rhoEpsilonEff() const;

        //- Return the Reynolds stress tensor
        virtual tmp<volSymmTensorField> R() const = 0;

        //- Return the effective stress tensor including the laminar stress
        virtual tmp<volSymmTensorField> devRhoReff() const = 0;

        //- Return the source term for the momentum equation
        virtual tmp<fvVectorMatrix> divDevRhoReff(volVectorField& U) const = 0;

        //- Solve the turbulence equations and correct the turbulence viscosity
        virtual void correct() = 0;

        //- Read LESProperties or RASProperties dictionary
        virtual bool read() = 0;

        //- Default dummy write function
        virtual bool writeData(Ostream&) const
        {
            return true;
        }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace compressible
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
