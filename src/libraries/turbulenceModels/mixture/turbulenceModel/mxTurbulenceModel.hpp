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
    CML::mixture::turbulenceModels

Description
    Namespace for mixture turbulence turbulence models.


Class
    CML::mixture::turbulenceModel

Description
    Abstract base class for mixture turbulence models
    (RAS, LES and laminar).

SourceFiles
    mxTurbulenceModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef mxTurbulenceModel_HPP
#define mxTurbulenceModel_HPP

#include "primitiveFieldsFwd.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "fvMatricesFwd.hpp"
#include "twoPhaseMixture.hpp"
#include "nearWallDist.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declarations
class fvMesh;

namespace mixture
{

/*---------------------------------------------------------------------------*\
                           Class turbulenceModel Declaration
\*---------------------------------------------------------------------------*/

class turbulenceModel
:
    public regIOobject
{

protected:

    const Time& runTime_;
    const fvMesh& mesh_;

    const volScalarField& rho_;
    const volVectorField& U_;
    const surfaceScalarField& phi_;

    const twoPhaseMixture& transportModel_;

    //- Near wall distance boundary field
    nearWallDist y_;


private:

    //- Disallow default bitwise copy construct
    turbulenceModel(const turbulenceModel&);

    //- Disallow default bitwise assignment
    void operator=(const turbulenceModel&);


public:

    //- Runtime type information
    TypeName("turbulenceModel");


    // Declare run-time New selection table
    declareRunTimeNewSelectionTable
    (
        autoPtr,
        turbulenceModel,
        turbulenceModel,
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
    turbulenceModel
    (
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi,
        const twoPhaseMixture& transport,
        const word& turbulenceModelName = typeName
    );


    //- Return a reference to the selected turbulence model
    static autoPtr<turbulenceModel> New
    (
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi,
        const twoPhaseMixture& transport,
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
    inline const volVectorField& U() const
    {
        return U_;
    }

    //- Access function to flux field
    inline const surfaceScalarField& phi() const
    {
        return phi_;
    }

    //- Access function to incompressible transport model
    inline const twoPhaseMixture& transport() const
    {
        return transportModel_;
    }

    //- Return the near wall distances
    const nearWallDist& y() const
    {
        return y_;
    }

    //- Return the laminar viscosity
    inline tmp<volScalarField> mu() const
    {
        return transportModel_.mu();
    }

    //- Return the turbulence viscosity
    virtual tmp<volScalarField> mut() const = 0;

    //- Return the effective viscosity
    virtual tmp<volScalarField> muEff() const = 0;

    //- Return the turbulence kinetic energy
    virtual tmp<volScalarField> k() const = 0;

    //- Return the turbulence kinetic energy dissipation rate
    virtual tmp<volScalarField> epsilon() const = 0;

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


} // End namespace mixture
} // End namespace CML


#endif
