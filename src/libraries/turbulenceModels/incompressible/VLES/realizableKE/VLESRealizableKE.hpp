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
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::incompressible::VLESModels::realizableVLESKE

Description
    Realizable VLES k-epsilon turbulence model for incompressible flows.

References:
    [1] D. W. Stephens, C. Sideroff and A. Jemcov, "A Two Equation VLES
        Turbulence Model with Near-Wall Delayed Behavior" 7th Asia-Pacific 
        International Symposium in Aerospace Technology, November 2015, 
        Cairns, Australia

SourceFiles
    VLESRealizableVLESKE.cpp

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef realizableKEVLES_H
#define realizableKEVLES_H

#include "VLESModel.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace incompressible
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
    dimensionedScalar Cx_;

    volScalarField k_;
    volScalarField epsilon_;
    volScalarField nut_;
    volScalarField nuSgs_;
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
        volVectorField const& U,
        surfaceScalarField const& phi,
        transportModel& transport,
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

    virtual tmp<volScalarField> nut() const
    {
        return nut_;
    }

    tmp<volScalarField> DkEff() const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DkEff", nut_/sigmak_ + nu())
        );
    }

    tmp<volScalarField> DepsilonEff() const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DepsilonEff", nut_/sigmaEps_ + nu())
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


