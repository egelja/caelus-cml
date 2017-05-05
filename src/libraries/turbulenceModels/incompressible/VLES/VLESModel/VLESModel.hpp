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
    CML::incompressible::VLESModels

Description
    Namespace for incompressible VLES turbulence models.

Class
    CML::incompressible::VLESModel

Description
    Abstract base class for incompressible turbulence models.

SourceFiles
    VLESModel.C

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef VLESModel_H
#define VLESModel_H

#include "incompressible/turbulenceModel/turbulenceModel.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "fvm.hpp"
#include "fvc.hpp"
#include "fvMatrices.hpp"
#include "incompressible/transportModel/transportModel.hpp"
#include "IOdictionary.hpp"
#include "Switch.hpp"
#include "bound.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

namespace CML
{
namespace incompressible
{

class VLESModel : public turbulenceModel, public IOdictionary
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
                volVectorField const& U,
                surfaceScalarField const& phi,
                transportModel& transport,
                word const& turbulenceModelName
        ),
        (U, phi, transport, turbulenceModelName)
    );

    VLESModel
    (
        word const& type,
        volVectorField const& U,
        surfaceScalarField const& phi,
        transportModel& transport,
        word const& turbulenceModelName = turbulenceModel::typeName
    );

    static autoPtr<VLESModel> New
    (
        const volVectorField& U,
        const surfaceScalarField& phi,
        transportModel& transport,
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

    virtual tmp<volScalarField> nuEff() const
    {
        return tmp<volScalarField>
        (
            new volScalarField("nuEff", nut() + nu())
        );
    }

    virtual void correct();

    virtual bool read();
};

}
}

#endif


