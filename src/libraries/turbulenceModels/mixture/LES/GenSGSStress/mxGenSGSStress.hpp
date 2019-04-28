/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

Class
    CML::mixture::LESModels::GenSGSStress

Description
    General base class for all mixture models that directly
    solve for the SGS stress tensor B.

    Contains tensor fields B (the SGS stress tensor) as well as scalar
    fields for k (SGS turbulent energy) gamma (SGS viscosity) and epsilon
    (SGS dissipation).

SourceFiles
    mxGenSGSStress.cpp

\*---------------------------------------------------------------------------*/

#ifndef mxGenSGSStress_HPP
#define mxGenSGSStress_HPP

#include "mxLESModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace mixture
{
namespace LESModels
{

/*---------------------------------------------------------------------------*\
                           Class GenSGSStress Declaration
\*---------------------------------------------------------------------------*/

class GenSGSStress
:
    virtual public LESModel
{
private:

    // Disallow default bitwise copy construct and assignment
    GenSGSStress(const GenSGSStress&);
    GenSGSStress& operator=(const GenSGSStress&);


protected:

    dimensionedScalar ce_;

    volSymmTensorField B_;
    volScalarField muSgs_;


public:

    //- Partial Runtime type information
    static const word typeName;


    //- Construct from components
    GenSGSStress
    (
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi,
        const twoPhaseMixture& transport,
        const word& turbulenceModelName = turbulenceModel::typeName,
        const word& modelName = typeName
    );


    //- Destructor
    virtual ~GenSGSStress()
    {}


    // Member Functions

    //- Return the SGS turbulent kinetic energy.
    virtual tmp<volScalarField> k() const
    {
        return tmp<volScalarField>
        (
            new volScalarField
            (
                IOobject
                (   
                    "k",
                    runTime_.timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                0.5*tr(B_)
            )
        );
    }

    //- Return the SGS turbulent dissipation.
    virtual tmp<volScalarField> epsilon() const
    {
        const volScalarField K(k());
            
        return tmp<volScalarField>
        (
            new volScalarField
            (
                IOobject
                (   
                    "epsilon",
                    runTime_.timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                ce_*K*sqrt(K)/delta()
            )
        );
    }

    //- Return the SGS viscosity.
    virtual tmp<volScalarField> muSgs() const
    {
        return muSgs_;
    }

    //- Return the sub-grid stress tensor.
    virtual tmp<volSymmTensorField> B() const
    {
        return B_;
    }

    //- Return the deviatoric part of the effective sub-grid
    //  turbulence stress tensor including the laminar stress
    virtual tmp<volSymmTensorField> devRhoBeff() const;

    //- Returns divergence of B : i.e. the additional term in the
    //  filtered NSE
    virtual tmp<fvVectorMatrix> divDevRhoBeff(volVectorField& U) const;

    //- Correct Eddy-Viscosity and related properties
    virtual void correct(const tmp<volTensorField>& gradU);

    //- Read LESProperties dictionary
    virtual bool read();
};


} // End namespace LESModels
} // End namespace mixture
} // End namespace CML


#endif
