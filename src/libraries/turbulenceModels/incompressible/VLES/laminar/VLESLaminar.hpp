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
    CML::incompressible::VLESModels::laminar

Description
    Dummy turbulence model for laminar incompressible flow.

SourceFiles
    VLESLaminar.cpp

\*---------------------------------------------------------------------------*/

#ifndef VLESLaminar_HPP
#define VLESLaminar_HPP

#include "VLESModel.hpp"

namespace CML
{
namespace incompressible
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
        volVectorField const& U,
        surfaceScalarField const& phi,
        transportModel& transport,
        word const& turbulenceModelName = turbulenceModel::typeName,
        word const& modelName = typeName
    );

    virtual ~laminar()
    {}

    virtual tmp<volScalarField> Fr() const;

    virtual tmp<volScalarField> nut() const;

    virtual tmp<volScalarField> nuEff() const
    {
        return tmp<volScalarField>(new volScalarField("nuEff", nu()));
    }

    virtual tmp<volScalarField> k() const;

    virtual tmp<volScalarField> epsilon() const;

    virtual tmp<volSymmTensorField> R() const;

    virtual tmp<volSymmTensorField> devReff() const;

    virtual tmp<fvVectorMatrix> divDevReff(volVectorField& U) const;

    virtual tmp<fvVectorMatrix> divDevRhoReff
    (
        volScalarField const& rho,
        volVectorField& U
    ) const;

    virtual void correct();

    virtual bool read();
};

}
}
}

#endif

