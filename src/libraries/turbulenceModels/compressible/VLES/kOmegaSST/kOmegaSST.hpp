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

#include "VLESModel.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace compressible
{
namespace VLESModels
{

class VLESKOmegaSST
:
    public VLESModel
{

protected:

    // Protected data

        // Model coefficients
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

        //- Wall distance field
        //  Note: different to wall distance in parent VLESModel
        wallDist y_;

        // Fields

            volScalarField k_;
            volScalarField omega_;
            volScalarField mut_;
            volScalarField alphat_;
  //volScalarField fr1_;


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

        tmp<volScalarField> alphaK(const volScalarField& F1) const
        {
            return blend(F1, alphaK1_, alphaK2_);
        }

        tmp<volScalarField> alphaOmega(const volScalarField& F1) const
        {
            return blend(F1, alphaOmega1_, alphaOmega2_);
        }

        tmp<volScalarField> beta(const volScalarField& F1) const
        {
            return blend(F1, beta1_, beta2_);
        }

        tmp<volScalarField> gamma(const volScalarField& F1) const
        {
            return blend(F1, gamma1_, gamma2_);
        }


public:

    //- Runtime type information
    TypeName("VLESKOmegaSST");


    // Constructors

        //- Construct from components
        VLESKOmegaSST
        (
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const basicThermo& thermoPhysicalModel,
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    //- Destructor
    virtual ~VLESKOmegaSST()
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

        //- Return the Reynolds stress tensor
        virtual tmp<volSymmTensorField> R() const;

        //- Return the effective stress tensor including the laminar stress
        virtual tmp<volSymmTensorField> devRhoReff() const;

        //- Return the source term for the momentum equation
        virtual tmp<fvVectorMatrix> divDevRhoReff(volVectorField& U) const;

        //- Solve the turbulence equations and correct the turbulence viscosity
        virtual void correct();

        //- Read VLESProperties dictionary
        virtual bool read();
};


}
}
}

#endif

