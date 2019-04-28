/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM 
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

    Scale Adapted Simulation model based on k-omega SST two equation model
    for compressible flows

    References:
    \verbatim
        "Ten Tears of Industrial Experience with the SSTSAS Turbulence Model"
        F.R. Menter,
        M.   Kuntz,
        R.   Langtry
        Turbulence, Heat and Mass Transfer4, ed: K. Hanjalic, Y. Nagano,
        and M. Tummers, Begell House Inc., 2003, pp 625-632.

        "Scale-Adaptive Simulation Theory", ANSYS CFX 14.5 documentation

        "The SAS Model : A Turbulence Model with Controlled Modelled
         Dissipation", L. Davidson, 20th Nordic Seminar on Computational 
         Mechanics, 20-23 Nov. 2007, Goteborg

    \endverbatim

    The default model coefficients correspond to the following:
    \verbatim
        kOmegaSSTSASCoeffs
        {
            alphaK1     0.85034;
            alphaK2     1.0;
            alphaOmega1 0.5;
            alphaOmega2 0.85616;
            Prt         0.9;
            beta1       0.075;
            beta2       0.0828;
            betaStar    0.09;
            gamma1      0.5532;
            gamma2      0.4403;
            a1          0.31;
            b1          1.0;
            c1          10.0;
            C           2.0;
            Cs_         0.11;
            zeta2       3.51;
            sigmaPhi    2.0/3.0;
            kappa       0.41
            Cmu         0.41
        }
    \endverbatim

Author(s)
    Chris Sideroff
    Aleksandar Jemcov
    
Contact
    Aleks Jemcov: a.jemcov@appliedccm.com
    Chris Sideroff: c.sideroff@appliedccm.ca
    Darrin Stephens: d.stephens@appliedccm.com.au
\*---------------------------------------------------------------------------*/

#ifndef compressiblekOmegaSSTSAS_HPP
#define compressiblekOmegaSSTSAS_HPP

#include "compressibleRASModel.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace compressible
{
namespace RASModels
{

class kOmegaSSTSAS
:
    public RASModel
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

        // SAS related constants

            dimensionedScalar C_;
            dimensionedScalar Cs_;
            dimensionedScalar zeta2_;
            dimensionedScalar sigmaPhi_;
            dimensionedScalar kappa_;
            dimensionedScalar Cmu_;

        //- Wall distance field
        //  Note: different to wall distance in parent RASModel
        wallDist y_;

        // Fields

            volScalarField k_;
            volScalarField omega_;
            volScalarField mut_;
            volScalarField alphat_;


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
    TypeName("kOmegaSSTSAS");


    // Constructors

        //- Construct from components
        kOmegaSSTSAS
        (
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const fluidThermo& thermophysicalModel, 
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    //- Destructor
    virtual ~kOmegaSSTSAS()
    {}


    // Member Functions

        //- Return the turbulence viscosity
        virtual tmp<volScalarField> mut() const
        {
            return mut_;
        }

        //- Return the thermal diffusivity 
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

        //- Read RASProperties dictionary
        virtual bool read();
};


}
}
}

#endif

