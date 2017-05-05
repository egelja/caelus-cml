/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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
    CML::compressible::VLESModels

Description
    Namespace for compressible VLES turbulence models.

Class
    CML::compressible::VLESModel

Description
    Abstract base class for compressible turbulence models.

SourceFiles
    VLESModel.cpp

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef compressibleVLESModel_H
#define compressibleVLESModel_H

#include "compressible/turbulenceModel/turbulenceModel.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "fvm.hpp"
#include "fvc.hpp"
#include "fvMatrices.hpp"
#include "basicThermo.hpp"
#include "IOdictionary.hpp"
#include "Switch.hpp"
#include "bound.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

namespace CML
{
namespace compressible
{

class VLESModel
:
    public turbulenceModel,
    public IOdictionary
{

protected:

    Switch turbulence_;

    Switch printCoeffs_;

    dictionary coeffDict_;

    dimensionedScalar kMin_;

    dimensionedScalar epsilonMin_;

    dimensionedScalar omegaMin_;

    virtual void printCoeffs();

private:

    VLESModel(const VLESModel&);

    void operator=(const VLESModel&);

public:

    TypeName("VLESModel");

    declareRunTimeSelectionTable
    (
        autoPtr,
        VLESModel,
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

    VLESModel
    (
        const word& type,
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi,
        const basicThermo& thermoPhysicalModel,
        const word& turbulenceModelName = turbulenceModel::typeName
    );

    static autoPtr<VLESModel> New
    (
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi,
        const basicThermo& thermoPhysicalModel,
        const word& turbulenceModelName = turbulenceModel::typeName
    );

    virtual ~VLESModel()
    {}

    const dimensionedScalar& kMin() const
    {
        return kMin_;
    }

    const dimensionedScalar& epsilonMin() const
    {
        return epsilonMin_;
    }

    const dimensionedScalar& omegaMin() const
    {
        return omegaMin_;
    }

    dimensionedScalar& kMin()
    {
        return kMin_;
    }

    dimensionedScalar& epsilonMin()
    {
        return epsilonMin_;
    }

    dimensionedScalar& omegaMin()
    {
        return omegaMin_;
    }

    virtual const dictionary& coeffDict() const
    {
        return coeffDict_;
    }

    virtual tmp<volScalarField> muEff() const
    {
        return tmp<volScalarField>
        (
            new volScalarField("muEff", mut() + mu())
        );
    }

    virtual tmp<volScalarField> alphaEff() const
    {
        return tmp<volScalarField>
        (
            new volScalarField("alphaEff", alphat() + alpha())
        );
    }

    //- Return the effective turbulent thermal diffusivity for a patch
    virtual tmp<scalarField> alphaEff(const label patchI) const
    {
       return
             alphat()().boundaryField()[patchI]
          + alpha().boundaryField()[patchI];
    }

    virtual void correct();

    virtual bool read();
};

}
}

#endif


