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
    CML::compressible::LESModels

Description
    Namespace for compressible LES models.


Class
    CML::compressible::LESModel

Description
    Base class for all compressible flow LES SGS models.

    This class defines the basic interface for a compressible flow SGS
    model, and encapsulates data of value to all possible models.
    In particular this includes references to all the dependent fields
    (rho, U, phi), the physical viscosity mu, and the LESProperties
    dictionary, which contains the model selection and model coefficients.

SourceFiles
    LESModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef compressibleLESModel_H
#define compressibleLESModel_H

#include "compressible/turbulenceModel/turbulenceModel.hpp"
#include "compressible/LES/compressibleLESdelta/compressibleLESdelta.hpp"
#include "fvm.hpp"
#include "fvc.hpp"
#include "fvMatrices.hpp"
#include "basicThermo.hpp"
#include "bound.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{

/*---------------------------------------------------------------------------*\
                           Class LESModel Declaration
\*---------------------------------------------------------------------------*/

class LESModel
:
    public turbulenceModel,
    public IOdictionary
{

protected:

    // Protected data

        Switch printCoeffs_;
        dictionary coeffDict_;

        dimensionedScalar kMin_;

        autoPtr<CML::LESdelta> delta_;


    // Protected Member Functions

        //- Print model coefficients
        virtual void printCoeffs();


private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        LESModel(const LESModel&);

        //- Disallow default bitwise assignment
        LESModel& operator=(const LESModel&);


public:

    //- Runtime type information
    TypeName("LESModel");


    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            LESModel,
            dictionary,
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
        LESModel
        (
            const word& type,
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const basicThermo& thermoPhysicalModel,
            const word& turbulenceModelName = turbulenceModel::typeName
        );


    // Selectors

        //- Return a reference to the selected LES model
        static autoPtr<LESModel> New
        (
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const basicThermo& thermoPhysicalModel,
            const word& turbulenceModelName = turbulenceModel::typeName
        );


    //- Destructor
    virtual ~LESModel()
    {}


    // Member Functions

        // Access

            //- Const access to the coefficients dictionary,
            //  which provides info. about choice of models,
            //  and all related data (particularly model coefficients).
            virtual const dictionary& coeffDict() const
            {
                return coeffDict_;
            }

            //- Return the lower allowable limit for k (default: SMALL)
            const dimensionedScalar& kMin() const
            {
                return kMin_;
            }

            //- Allow kMin to be changed
            dimensionedScalar& kMin()
            {
                return kMin_;
            }

            //- Access function to filter width
            inline const volScalarField& delta() const
            {
                return delta_();
            }


        //- Return the SGS turbulent viscosity
        virtual tmp<volScalarField> muSgs() const = 0;

        //- Return the effective viscosity
        virtual tmp<volScalarField> muEff() const
        {
            return tmp<volScalarField>
            (
                new volScalarField("muEff", muSgs() + mu())
            );
        }

        //- Return the SGS turbulent thermal diffusivity
        virtual tmp<volScalarField> alphaSgs() const = 0;

        //- Return the effective thermal diffusivity
        virtual tmp<volScalarField> alphaEff() const
        {
            return tmp<volScalarField>
            (
                new volScalarField("alphaEff", alphaSgs() + alpha())
            );
        }

        //- Return the effective turbulence thermal diffusivity for a patch
        virtual tmp<scalarField> alphaEff(const label patchI) const
        {
            return
                alphaSgs()().boundaryField()[patchI]
              + alpha().boundaryField()[patchI];
        }

        //- Return the sub-grid stress tensor.
        virtual tmp<volSymmTensorField> B() const = 0;

        //- Return the deviatoric part of the effective sub-grid
        //  turbulence stress tensor including the laminar stress
        virtual tmp<volSymmTensorField> devRhoBeff() const = 0;

        //- Returns div(rho*dev(B)).
        // This is the additional term due to the filtering of the NSE.
        virtual tmp<fvVectorMatrix> divDevRhoBeff(volVectorField& U) const = 0;


        // RAS compatibility functions for the turbulenceModel base class

            //- Return the turbulence viscosity
            virtual tmp<volScalarField> mut() const
            {
                return muSgs();
            }

            //- Return the turbulence thermal diffusivity
            virtual tmp<volScalarField> alphat() const
            {
                return alphaSgs();
            }

            //- Return the Reynolds stress tensor
            virtual tmp<volSymmTensorField> R() const
            {
                return B();
            }

            //- Return the effective stress tensor including the laminar stress
            virtual tmp<volSymmTensorField> devRhoReff() const
            {
                return devRhoBeff();
            }

            //- Return the source term for the momentum equation
            virtual tmp<fvVectorMatrix> divDevRhoReff(volVectorField& U) const
            {
                return divDevRhoBeff(U);
            }


        //- Correct Eddy-Viscosity and related properties.
        //  This calls correct(const tmp<volTensorField>& gradU) by supplying
        //  gradU calculated locally.
        virtual void correct();

        //- Correct Eddy-Viscosity and related properties
        virtual void correct(const tmp<volTensorField>& gradU);

        //- Read LESProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace compressible
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
