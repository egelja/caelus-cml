/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2016 Applied CCM 
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
    DES implementation of the k-omega-SST turbulence model for incompressible
    flows (SST-2003).
    
Author(s)
    Chris Sideroff
    Aleksandar Jemcov
    Darrin Stephens
    
Contact
    Aleks Jemcov: a.jemcov@appliedccm.com
    Chris Sideroff: c.sideroff@appliedccm.ca
    Darrin Stephens: d.stephens@appliedccm.com.au
\*---------------------------------------------------------------------------*/

#ifndef kOmegaSSTDES_HPP
#define kOmegaSSTDES_HPP

#include "LESModel.hpp"
#include "volFields.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace incompressible
{
namespace LESModels
{

class kOmegaSSTDES : public LESModel
{

    // Disallow default bitwise copy construct and assignment
    kOmegaSSTDES(const kOmegaSSTDES&);
    kOmegaSSTDES& operator=(kOmegaSSTDES const&);
        
protected:

    // Protected data

    //- Curvature correction on/off flag
    Switch curvatureCorrection_;

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

    dimensionedScalar kappa_;

    dimensionedScalar CDES_;

    dimensionedScalar Cr1_;
    dimensionedScalar Cr2_;
    dimensionedScalar Cr3_;
    dimensionedScalar Cscale_;
    dimensionedScalar frMax_;

    //- Wall distance field
    wallDist y_;

    // Fields
    volScalarField k_;
    volScalarField omega_;
    volScalarField nuSgs_;
    volScalarField fr1_;

    tmp<volScalarField> CDkOmega() const;
    tmp<volScalarField> F1() const;
    tmp<volScalarField> F2() const;

    tmp<volScalarField> Lt() const;
    virtual tmp<volScalarField> FDES() const;

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
    TypeName("kOmegaSSTDES");

    // Constructors

    //- Construct from components
    kOmegaSSTDES
    (
        volVectorField const& U,
        surfaceScalarField const& phi,
        transportModel& transport,
        word const& turbulenceModelName = turbulenceModel::typeName,
        word const& modelName = typeName
    );

    //- Destructor
    virtual ~kOmegaSSTDES()
    {}

    // Member Functions

    //- Return the SGS viscosity
    virtual tmp<volScalarField> nuSgs() const
    {
        return nuSgs_;
    }

    //- Return the effective diffusivity for k
    tmp<volScalarField> DkEff(volScalarField const& F1) const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DkEff", alphaK(F1)*nuSgs_ + nu())
        );
    }

    //- Return the effective diffusivity for omega
    tmp<volScalarField> DomegaEff(volScalarField const& F1) const
    {
        return tmp<volScalarField>
        (
            new volScalarField("DomegaEff", alphaOmega(F1)*nuSgs_ + nu())
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

    //- Return the sub-grid stress tensor
    virtual tmp<volSymmTensorField> B() const;

    //- Return the effective stress tensor including the laminar stress
    virtual tmp<volSymmTensorField> devReff() const;

    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevReff(volVectorField& U) const;

    //- Return the source term for the momentum equation
    virtual tmp<fvVectorMatrix> divDevRhoReff
    (
        volScalarField const& rho,
        volVectorField& U
    ) const;

    //- Solve the turbulence equations and correct the turbulence viscosity
    virtual void correct(tmp<volTensorField> const& gradU);

    //- Read RASProperties dictionary
    virtual bool read();
};


}
}
}

#endif

