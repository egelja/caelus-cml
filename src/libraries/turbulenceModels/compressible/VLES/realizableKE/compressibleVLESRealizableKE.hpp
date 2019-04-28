/*---------------------------------------------------------------------------*\
Copyright (C) 2015 - 2016 Appled CCM
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gmut_.org/licenses/>.

Class
    CML::compressible::VLESModels::realizableVLESKE

SourceFiles
    compressibleVLESRealizableVLESKE.cpp

References:
    [1] D. W. Stephens, C. Sideroff and A. Jemcov, "A Two Equation VLES
        Turbulence Model with Near-Wall Delayed Behavior" 7th Asia-Pacific 
        International Symposium in Aerospace Technology, November 2015, 
        Cairns, Australia

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef compressibleRealizableKEVLES_H
#define compressibleRealizableKEVLES_H

#include "compressibleVLESModel.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace compressible
{
namespace VLESModels
{

class realizableVLESKE : public VLESModel
{

protected:

    Switch delayed_;
    Switch outputFr_;

    dimensionedScalar Cmu_;
    dimensionedScalar A0_;
    dimensionedScalar C2_;
    dimensionedScalar sigmak_;
    dimensionedScalar sigmaEps_;
    dimensionedScalar Prt_;
    dimensionedScalar Cx_;

    volScalarField k_;
    volScalarField epsilon_;
    volScalarField mut_;
    volScalarField muSgs_;
    volScalarField alphat_;
    volScalarField alphaSgs_;
    volScalarField Fr_;

    //- Wall distance field
    //  Note: different to wall distance in parent VLESModel
    wallDist y_;


    tmp<volScalarField> rCmu
    (
        volTensorField const& gradU,
        volScalarField const& S2,
        volScalarField const& magS
    );

    tmp<volScalarField> rCmu(volTensorField const& gradU);

public:

    TypeName("realizableKEVLES");

    realizableVLESKE
    (
        volScalarField const& rho,
        volVectorField const& U,
        surfaceScalarField const& phi,
        fluidThermo const& thermoPhysicalModel,
        word const& turbulenceModelName = turbulenceModel::typeName,
        word const& modelName = typeName
    );

    virtual ~realizableVLESKE()
    {}

    tmp<volScalarField> F1() const;

    //- Return the resolution function
    virtual tmp<volScalarField> Fr() const
    {
        return Fr_;
    }

    virtual tmp<volScalarField> mut() const
    {
        return mut_;
    }

    virtual tmp<volScalarField> alphat() const
    {
        return alphat_;
    }

    tmp<volScalarField> DkEff() const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DkEff", mut_/sigmak_ + mu())
        );
    }

    tmp<volScalarField> DepsilonEff() const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DepsilonEff", mut_/sigmaEps_ + mu())
        );
    }

    virtual tmp<volScalarField> k() const
    {
        return k_;
    }

    virtual tmp<volScalarField> epsilon() const
    {
        return epsilon_;
    }

    virtual void correct();

    virtual bool read();
};


}
}
}

#endif

