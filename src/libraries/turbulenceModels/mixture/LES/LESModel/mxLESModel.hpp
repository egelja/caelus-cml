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

Namespace
    CML::mixture::LESModels

Description
    Namespace for mixture LES models.


Class
    CML::mixture::LESModel

Description
    Base class for all mixture multiphase flow LES SGS models.

    This class defines the basic interface for a multiphase mixture flow SGS
    model, and encapsulates data of value to all possible models.
    In particular this includes references to all the dependent fields
    (rho, U, phi), the physical viscosity mu, and the LESProperties
    dictionary, which contains the model selection and model coefficients.

SourceFiles
    mxLESModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef mxLESModel_HPP
#define mxLESModel_HPP

#include "mixture/turbulenceModel/mxTurbulenceModel.hpp"
#include "mixture/LES/mxLESdelta/mxLESdelta.hpp"
#include "fvm.hpp"
#include "fvc.hpp"
#include "fvMatrices.hpp"
#include "bound.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace mixture
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

    Switch printCoeffs_;
    dictionary coeffDict_;

    dimensionedScalar kMin_;

    autoPtr<CML::LESdelta> delta_;


    //- Print model coefficients
    virtual void printCoeffs();


private:

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
            const twoPhaseMixture& transport,
            const word& turbulenceModelName
        ),
        (rho, U, phi, transport, turbulenceModelName)
    );


    //- Construct from components
    LESModel
    (
        const word& type,
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi,
        const twoPhaseMixture& transport,
        const word& turbulenceModelName = turbulenceModel::typeName
    );


    //- Return a reference to the selected LES model
    static autoPtr<LESModel> New
    (
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi,
        const twoPhaseMixture& transport,
        const word& turbulenceModelName = turbulenceModel::typeName
    );


    //- Destructor
    virtual ~LESModel()
    {}


    // Member Functions

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
    virtual const volScalarField& delta() const
    {
        return delta_();
    }

    //- Return the SGS turbulent viscosity.
    virtual tmp<volScalarField> muSgs() const = 0;

    //- Return the effective viscosity
    virtual tmp<volScalarField> muEff() const
    {
        return tmp<volScalarField>
        (
            new volScalarField("muEff", muSgs() + mu())
        );
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


} // End namespace mixture
} // End namespace CML


#endif
