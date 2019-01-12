/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    CML::compressible::VLESModels::laminar

Description
    Dummy turbulence model for laminar compressible flow.

SourceFiles
    compressibleVLESLaminar.cpp

\*---------------------------------------------------------------------------*/

#ifndef compressibleVLESLaminar_HPP
#define compressibleVLESLaminar_HPP

#include "compressibleVLESModel.hpp"

namespace CML
{
namespace compressible
{
namespace VLESModels
{

class laminar
:
    public VLESModel
{

public:

    TypeName("laminar");

    laminar
    (
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi,
        const fluidThermo& thermoPhysicalModel,
        const word& turbulenceModelName = turbulenceModel::typeName,
        const word& modelName = typeName
    );

    virtual ~laminar()
    {}

    virtual tmp<volScalarField> Fr() const;

    virtual tmp<volScalarField> mut() const;

    virtual tmp<volScalarField> muEff() const
    {
        return tmp<volScalarField>(new volScalarField("muEff", mu()));
    }

    virtual tmp<volScalarField> alphat() const;

    virtual tmp<volScalarField> alphaEff() const
    {
       return tmp<volScalarField>(new volScalarField("alphaEff", alpha()));
    }

    virtual tmp<volScalarField> k() const;

    virtual tmp<volScalarField> epsilon() const;

    virtual tmp<volSymmTensorField> R() const;

    virtual tmp<volSymmTensorField> devRhoReff() const;

    virtual tmp<fvVectorMatrix> divDevRhoReff(volVectorField& U) const;

    virtual void correct();

    virtual bool read();
};

}
}
}

#endif

