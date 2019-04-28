/*---------------------------------------------------------------------------*\
Copyright (C) 2015 - 2016 Applied CCM 
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

Description

    Implementation of the VLES k-omega SST model

References:

    [1] D. W. Stephens, C. Sideroff and A. Jemcov, "A Two Equation VLES
        Turbulence Model with Near-Wall Delayed Behavior" 7th Asia-Pacific 
        International Symposium in Aerospace Technology, November 2015, 
        Cairns, Australia
    
Author(s)

    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef compressibleKOmegaSSTVLES_HPP
#define compressibleKOmegaSSTVLES_HPP

#include "compressibleVLESModel.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace compressible
{
namespace VLESModels
{

class VLESKOmegaSST : public VLESModel
{

protected:

    Switch delayed_;
    Switch damped_;
    Switch curvatureCorrection_;
    Switch outputfr1_;
    Switch outputFr_;
    Switch outputFd_;

    dimensionedScalar alphaK1_;
    dimensionedScalar alphaK2_;

    dimensionedScalar alphaOmega1_;
    dimensionedScalar alphaOmega2_;

    dimensionedScalar Prt_;

    dimensionedScalar gamma1_;
    dimensionedScalar gamma2_;

    dimensionedScalar beta1_;
    dimensionedScalar beta2_;

    dimensionedScalar betaStar_;

    dimensionedScalar a1_;
    dimensionedScalar c1_;

    dimensionedScalar Cx_;

    dimensionedScalar Cr1_;
    dimensionedScalar Cr2_;
    dimensionedScalar Cr3_;
    dimensionedScalar Cscale_;
    dimensionedScalar frMax_;

    wallDist y_;

    volScalarField k_;
    volScalarField omega_;
    volScalarField mut_;
    volScalarField muSgs_;
    volScalarField yStar_;
    volScalarField alphat_;
    volScalarField alphaSgs_;
    volScalarField fr1_;
    volScalarField Fr_;
    volScalarField Fd_;


    tmp<volScalarField> F1(volScalarField const& CDkOmega) const;
    tmp<volScalarField> F2() const;

    tmp<volScalarField> blend
    (
        volScalarField const& F1,
        dimensionedScalar const& psi1,
        dimensionedScalar const& psi2
    ) const
    {
        return F1*(psi1 - psi2) + psi2;
    }

    tmp<volScalarField> alphaK(volScalarField const& F1) const
    {
        return blend(F1, alphaK1_, alphaK2_);
    }

    tmp<volScalarField> alphaOmega(volScalarField const& F1) const
    {
        return blend(F1, alphaOmega1_, alphaOmega2_);
    }

    tmp<volScalarField> beta(volScalarField const& F1) const
    {
        return blend(F1, beta1_, beta2_);
    }

    tmp<volScalarField> gamma(volScalarField const& F1) const
    {
        return blend(F1, gamma1_, gamma2_);
    }

public:

    TypeName("kOmegaSSTVLES");

    VLESKOmegaSST
    (
        volScalarField const& rho,
        volVectorField const& U,
        surfaceScalarField const& phi,
        fluidThermo const& thermoPhysicalModel,
        word const& turbulenceModelName = turbulenceModel::typeName,
        word const& modelName = typeName
    );

    virtual ~VLESKOmegaSST() {}

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

    tmp<volScalarField> DkEff(volScalarField const& F1) const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DkEff", alphaK(F1)*mut_ + mu())
        );
    }

    tmp<volScalarField> DomegaEff(volScalarField const& F1) const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DomegaEff", alphaOmega(F1)*mut_ + mu())
        );
    }

    virtual tmp<volScalarField> k() const
    {
        return k_;
    }

    virtual tmp<volScalarField> omega() const
    {
        return omega_;
    }

    virtual tmp<volScalarField> epsilon() const
    {
        return tmp<volScalarField>
        (
            new volScalarField
            (
                IOobject
                (
                    "epsilon",
                    mesh_.time().timeName(),
                    mesh_
                ),
                betaStar_*k_*omega_,
                omega_.boundaryField().types()
            )
        );
    }

    virtual void correct();

    virtual bool read();
};


}
}
}

#endif

