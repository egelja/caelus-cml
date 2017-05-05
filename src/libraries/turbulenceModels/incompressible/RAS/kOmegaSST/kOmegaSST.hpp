/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM 

Description
    Implementation of the k-omega-SST turbulence model for incompressible
    flows (SST-2003).
    
    This form of the SST model have several relatively minor variations from
    the orginal SST version.

    References:
    \verbatim
        "Ten Tears of Industrial Experience with the SST Turbulence Model"
        F.R. Menter,
        M.   Kuntz,
        R.   Langtry
        Turbulence, Heat and Mass Transfer4, ed: K. Hanjalic, Y. Nagano,
        and M. Tummers, Begell House Inc., 2003, pp 625-632.

    \endverbatim

    Note, a typographical error existed i nthis paper that was subsequently
    corrected but the authors. In the omega equation (2nd part of eqn(1)
    in the paper), the production term was incorrectly given as 
    \alpha\rho S^{2} when it should have read 
    \frac{\alpha\widetilde{P_{k}}}{\nu_{t}}. In this expression, the Pk term 
    with the tidle refers to the limited value of the production term. The main
    change in this model is the definition of eddy viscosity, which uses the 
    strain invariant rather than magnitude of vorticity in its definition.
 
    The default model coefficients correspond to the following:
    \verbatim
        kOmegaSSTCoeffs
        {
            alphaK1     0.85034;
            alphaK2     1.0;
            alphaOmega1 0.5;
            alphaOmega2 0.85616;
            beta1       0.075;
            beta2       0.0828;
            betaStar    0.09;
            gamma1      0.5532;
            gamma2      0.4403;
            a1          0.31;
            b1          1.0;
            c1          10.0;
        }
    \endverbatim

Author(s)
    Aleksandar Jemcov
    Darrin Stephens
    
Contact
    Aleks Jemcov: a.jemcov@appliedccm.com
    Chris Sideroff: c.sideroff@appliedccm.ca
    Darrin Stephens: d.stephens@appliedccm.com.au
\*---------------------------------------------------------------------------*/

#ifndef kOmegaSST_HPP
#define kOmegaSST_HPP

#include "RASModel.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace incompressible
{
namespace RASModels
{

class kOmegaSST
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

            dimensionedScalar gamma1_;
            dimensionedScalar gamma2_;

            dimensionedScalar beta1_;
            dimensionedScalar beta2_;

            dimensionedScalar betaStar_;

            dimensionedScalar a1_;
            dimensionedScalar c1_;

        //- Wall distance field
        //  Note: different to wall distance in parent RASModel
        wallDist y_;

        // Fields

            volScalarField k_;
            volScalarField omega_;
            volScalarField nut_;


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
    TypeName("kOmegaSST");


    // Constructors

        //- Construct from components
        kOmegaSST
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            transportModel& transport,
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    //- Destructor
    virtual ~kOmegaSST()
    {}


    // Member Functions

        //- Return the turbulence viscosity
        virtual tmp<volScalarField> nut() const
        {
            return nut_;
        }

        //- Return the effective diffusivity for k
        tmp<volScalarField> DkEff(const volScalarField& F1) const
        {
            return tmp<volScalarField>
            (
                new volScalarField("DkEff", alphaK(F1)*nut_ + nu())
            );
        }

        //- Return the effective diffusivity for omega
        tmp<volScalarField> DomegaEff(const volScalarField& F1) const
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

        //- Return the Reynolds stress tensor
        virtual tmp<volSymmTensorField> R() const;

        //- Return the effective stress tensor including the laminar stress
        virtual tmp<volSymmTensorField> devReff() const;

        //- Return the source term for the momentum equation
        virtual tmp<fvVectorMatrix> divDevReff(volVectorField& U) const;

        //- Return the source term for the momentum equation
        virtual tmp<fvVectorMatrix> divDevRhoReff
        (
            const volScalarField& rho,
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

