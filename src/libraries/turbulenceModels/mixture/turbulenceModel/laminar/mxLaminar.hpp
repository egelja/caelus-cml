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

Class
    CML::mixture::laminar

Description
    Mixture turbulence model for laminar multiphase flow.

SourceFiles
    mxLaminar.cpp

\*---------------------------------------------------------------------------*/

#ifndef mxLaminar_HPP
#define mxLaminar_HPP

#include "mxTurbulenceModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace mixture
{

/*---------------------------------------------------------------------------*\
                           Class laminar Declaration
\*---------------------------------------------------------------------------*/

class laminar
:
    public turbulenceModel
{

public:

    //- Runtime type information
    TypeName("laminar");


    //- Construct from components
    laminar
    (
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi,
        const twoPhaseMixture& transport,
        const word& turbulenceModelName = turbulenceModel::typeName
    );


    //- Return a reference to the selected turbulence model
    static autoPtr<laminar> New
    (
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi,
        const twoPhaseMixture& transport,
        const word& turbulenceModelName = turbulenceModel::typeName
    );


    //- Destructor
    virtual ~laminar()
    {}


    // Member Functions

    //- Const access to the coefficients dictionary
    virtual const dictionary& coeffDict() const;

    //- Return the turbulence viscosity, i.e. 0 for laminar flow
    virtual tmp<volScalarField> mut() const;

    //- Return the effective viscosity, i.e. the laminar viscosity
    virtual tmp<volScalarField> muEff() const;

    //- Return the turbulence kinetic energy, i.e. 0 for laminar flow
    virtual tmp<volScalarField> k() const;

    //- Return the turbulence kinetic energy dissipation rate,
    //  i.e. 0 for laminar flow
    virtual tmp<volScalarField> epsilon() const;

    //- Return the Reynolds stress tensor, i.e. 0 for laminar flow
    virtual tmp<volSymmTensorField> R() const;

    //- Return the effective stress tensor, i.e. the laminar stress
    virtual tmp<volSymmTensorField> devRhoReff() const;

    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevRhoReff(volVectorField& U) const;

    //- Correct the laminar viscosity
    virtual void correct();

    //- Read turbulenceProperties dictionary
    virtual bool read();
};


} // End namespace mixture
} // End namespace CML


#endif
