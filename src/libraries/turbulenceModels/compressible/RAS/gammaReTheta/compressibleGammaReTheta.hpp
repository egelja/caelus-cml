/*---------------------------------------------------------------------------*\
Copyright (C) 2016 Applied CCM 
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

    gamma-ReTheta transition model model for incompressible flows

References:

    [1] "A Correlation-Based Transition Model using Local Variables for
         for Unstructured Parallelized CFD Code", R.B. Langtry, PhD
         Thesis, May 31, 2006

    [2] "Calibrating the gamma-ReTheta Transition Model for Commercial
         CFD", P. Malan, K. Suluksna, W. Juntasaro, 47th AIAA Aerospace
         Sciences Meeting, Jan 2009

Author:
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/


#ifndef compressibleGammaReTheta_H
#define compressibleGammaReTheta_H

#include "compressibleRASModel.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace compressible
{
namespace RASModels
{
class gammaReTheta : public RASModel
{
    // Private data
    
    static const scalar tol_;
    static const int maxIter_;
                    
    dimensionedScalar ca1_;
    dimensionedScalar ce1_;

    dimensionedScalar ca2_;
    dimensionedScalar ce2_;

    dimensionedScalar cThetat_;

    dimensionedScalar sigmaf_;
    dimensionedScalar sigmaThetat_;

    dimensionedScalar s1_;

    dimensionedScalar alphaK1_;
    dimensionedScalar alphaK2_;

    dimensionedScalar alphaOmega1_;
    dimensionedScalar alphaOmega2_;

    dimensionedScalar intermittency1_;
    dimensionedScalar intermittency2_;

    dimensionedScalar beta1_;
    dimensionedScalar beta2_;

    dimensionedScalar betaStar_;

    dimensionedScalar a1_;
    dimensionedScalar c1_;

    wallDist y_;

    // Fields

    volScalarField intermittency_;
    volScalarField ReThetaTilda_;
    volScalarField k_;
    volScalarField omega_;
    volScalarField mut_;
    volScalarField alphat_;


    // Private member functions

    volScalarField Flength() const;
    volScalarField ReThetac() const;

    tmp<volScalarField> Fonset() const;
    tmp<volScalarField> Fonset1() const;
    tmp<volScalarField> Fturb() const;
    tmp<volScalarField> Freattach() const;
    tmp<volScalarField> Fwake() const;
    tmp<volScalarField> FThetat() const;

    void ReTheta(volScalarField& ReThetaField) const;
    scalar ReThetaEq(scalar Tu, scalar lambda, scalar const K) const;

    tmp<volScalarField> intermittencySep() const;


    tmp<volScalarField> F1(const volScalarField& CDkOmega) const;
    tmp<volScalarField> F2() const;

    tmp<volScalarField> blend
    (
        const volScalarField& F1,
        const dimensionedScalar& psi1,
        const dimensionedScalar& psi2
    ) const
    {
        return F1*(psi1 - psi2) + psi2;
    }

    tmp<volScalarField> alphaK
    (
        const volScalarField& F1
    ) const
    {
        return blend(F1, alphaK1_, alphaK2_);
    }

    tmp<volScalarField> alphaOmega
    (
        const volScalarField& F1
    ) const
    {
        return blend(F1, alphaOmega1_, alphaOmega2_);
    }

    tmp<volScalarField> beta
    (
        const volScalarField& F1
    ) const
    {
        return blend(F1, beta1_, beta2_);
    }

    tmp<volScalarField> intermittency
    (
        const volScalarField& F1
    ) const
    {
        return blend(F1, intermittency1_, intermittency2_);
    }

public:

    //- Runtime type information
    TypeName("gammaReTheta");


    // Constructors

    gammaReTheta
    (
        volScalarField const& rho,
        volVectorField const& U,
        surfaceScalarField const& phi,
        fluidThermo const& thermophysicalModel,
        word const& turbulenceModelName = turbulenceModel::typeName,
        word const& modelName = typeName
    );


    //- Destructor
    virtual ~gammaReTheta()
    {}


    // Member Functions

    //- Return the turbulence viscosity
    virtual tmp<volScalarField> mut() const
    {
        return mut_;
    }

    //- Return the turbulent thermal diffusivity
    virtual tmp<volScalarField> alphat() const
    {
        return alphat_;
    }

    //- Return the effective diffusivity for intermittency
    tmp<volScalarField> DintermittencyEff() const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DintermittencyEff", (mut_/sigmaf_) + mu())
        );
    }

    //- Return the effective diffusivity for ReThetaTilda
    tmp<volScalarField> DReThetaTildaEff() const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DReThetaTildaEff", sigmaThetat_*(mut_ + mu()))
        );
    }

    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff(const volScalarField& F1) const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DkEff", alphaK(F1)*mut_ + mu())
        );
    }

    //- Return the effective diffusivity for omega
    tmp<volScalarField> DomegaEff(const volScalarField& F1) const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DomegaEff", alphaOmega(F1)*mut_ + mu())
        );
    }

    //- Return the intermittency
    virtual tmp<volScalarField> intermittency() const
    {
        return intermittency_;
    }

    //- Return the local transition momentum thickness Reynolds number
    virtual tmp<volScalarField> ReThetaTilda() const
    {
        return ReThetaTilda_;
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

    //- Return the viscosity ratio
    tmp<volScalarField> Rt() const
    {
        return tmp<volScalarField>
        (
            new volScalarField("Rt", k_/(mu()/rho_*omega_))
        );
    }

    //- Return the Reynolds stress tensor
    virtual tmp<volSymmTensorField> R() const;

    //- Return the effective stress tensor including the laminar stress
    virtual tmp<volSymmTensorField> devRhoReff() const;

    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevRhoReff
    (
        volVectorField& U
    ) const;

    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct();

    //- Read RASProperties dictionary
    virtual bool read();
};


} 
} 
} 

#endif
