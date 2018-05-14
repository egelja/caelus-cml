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

    Coherent structure model for VLES based on k-omega SST transport equations

References

    [1] D. W. Stephens, C. Sideroff and A. Jemcov, "A Two Equation VLES
        Turbulence Model with Near-Wall Delayed Behavior" 7th Asia-Pacific 
        International Symposium in Aerospace Technology, November 2015, 
        Cairns, Australia

    [2] H. Kobayashi, F.E. Ham, X. Wu "Application of a local SGS model 
        based on coherent structure to complex geometries", International
        Journal of Heat and Fluid Flow, 29(3) pp. 640-653

Author(s)
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef kOmegaSSTCSModelVLES_HPP
#define kOmegaSSTCSModelVLES_HPP

#include "VLESModel.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace incompressible
{
namespace VLESModels
{

class kOmegaSSTCSModelVLES : public VLESModel
{

protected:

    // Protected
    Switch delayed_;
    Switch damped_;
    Switch curvatureCorrection_;
    Switch outputfr1_;
    Switch outputFr_;
    Switch outputFd_;

    // Model coefficients
    dimensionedScalar alphaK1_;
    dimensionedScalar alphaK2_;

    dimensionedScalar alphaOmega1_;
    dimensionedScalar alphaOmega2_;

    dimensionedScalar gamma1_;
    dimensionedScalar gamma2_;

    dimensionedScalar beta1_;
    dimensionedScalar beta2_;

    dimensionedScalar betaStar_;

    dimensionedScalar a1_;
    dimensionedScalar c1_;

    dimensionedScalar Cr1_;
    dimensionedScalar Cr2_;
    dimensionedScalar Cr3_;
    dimensionedScalar Cscale_;
    dimensionedScalar frMax_;
    dimensionedScalar Cmu_;
    dimensionedScalar cPrime_;
    dimensionedScalar betaPrime_;
    dimensionedScalar nPrime_;

    //- Wall distance field
    //  Note: different to wall distance in parent VLESModel
    wallDist y_;

    // Fields

    volScalarField k_;
    volScalarField omega_;
    volScalarField nut_;
    volScalarField nuSgs_;
    volScalarField yStar_;
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

    //- Runtime type information
    TypeName("kOmegaSSTCSModelVLES");


    // Constructors

    //- Construct from components
    kOmegaSSTCSModelVLES
    (
        volVectorField const& U,
        surfaceScalarField const& phi,
        transportModel& transport,
        word const& turbulenceModelName = turbulenceModel::typeName,
        word const& modelName = typeName
    );

    //- Destructor
    virtual ~kOmegaSSTCSModelVLES()
    {}

    // Member Functions

    //- Return the resolution function
    virtual tmp<volScalarField> Fr() const
    {
        return Fr_;
    }

    //- Return the turbulence viscosity
    virtual tmp<volScalarField> nut() const
    {
        return nut_;
    }

    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff(volScalarField const& F1) const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DkEff", alphaK(F1)*nut_ + nu())
        );
    }

    //- Return the effective diffusivity for omega
    tmp<volScalarField> DomegaEff(volScalarField const& F1) const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DomegaEff", alphaOmega(F1)*nut_ + nu())
        );
    }

    //- Return the turbulence kinetic energy
    virtual tmp<volScalarField> k() const
    {
        return k_;
    }

    //- Return the turbulence specific dissipation rate
    virtual tmp<volScalarField> omega() const
    {
        return omega_;
    }

    //- Return the turbulence kinetic energy dissipation rate
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

    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct();

    //- Read VLESProperties dictionary
    virtual bool read();
};


}
}
}

#endif

