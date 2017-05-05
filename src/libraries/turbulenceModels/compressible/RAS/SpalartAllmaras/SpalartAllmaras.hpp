/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM 

Description
    Spalart-Allmaras 1-eqn mixing-length model for compressible external
    flows with optional curvature correction (SA-RC).

    References:
    \verbatim
        "A One-Equation Turbulence Model for 
       Aerodynamic Flows"
        P.R. Spalart
        S.R. Allmaras
        Recherche Aerospatiale, No. 1, 1994, pp. 5-21.

    \endverbatim

    \verbatim
        "Turbulence Modeling in Rotating and Curved Channels: Assessing the 
        Spalart-Shur Correction"
        M.L. Shur,
        M.K. Strelets,
        A.K. Travin,
        P.R. Spalart
        AIAA Journal, Vol. 38, No. 5, 2000, pp. 784-792.

    \endverbatim

Notes
    The journal reference (1) had a small typo (appendix only) in its definitio
    of the constant Cw1 (missing square). The typo is corrected below. The 
    original reference made use of a trip term that most people do not include,
    because the model is most often employed for fully turbulent applications.
    Therefore, in this "standard" representation the trip term is being left 
    out.

    The default model coefficients correspond to the following:
    \verbatim
        SpalartAllmarasCoeffs
        {
            curvatureCorrection false;
            Cb1         0.1355;
            Cb2         0.622;
            Cw2         0.3;
            Cw3         2.0;
            Cv1         7.1;
            Cr1         1.0;
            Cr2         12.0;
            Cr3         1.0;
            sigmaNut    0.66666;
            kappa       0.41;
            Prt         0.9;
        }
    \endverbatim

Author(s)
    Aleksandar Jemcov
    Chris Sideroff
    Darrin Stephens
    
Contact
    Aleks Jemcov: a.jemcov@appliedccm.com
    Chris Sideroff: c.sideroff@appliedccm.ca
    Darrin Stephens: d.stephens@appliedccm.com.au
\*---------------------------------------------------------------------------*/
#ifndef compressibleSpalartAllmaras_HPP
#define compressibleSpalartAllmaras_HPP

#include "RASModel.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace compressible
{
namespace RASModels
{

class SpalartAllmaras
:
    public RASModel
{

protected:

    // Protected data

        //- Curvature correction on/off flag
            Switch curvatureCorrection_;

        // Model coefficients

            dimensionedScalar sigmaNut_;
            dimensionedScalar kappa_;
            dimensionedScalar Prt_;

            dimensionedScalar Cb1_;
            dimensionedScalar Cb2_;
            dimensionedScalar Cw1_;
            dimensionedScalar Cw2_;
            dimensionedScalar Cw3_;
            dimensionedScalar Cv1_;
            dimensionedScalar Cr1_;
            dimensionedScalar Cr2_;
            dimensionedScalar Cr3_;


        // Fields
            volScalarField nuTilda_;
            volScalarField mut_;
            volScalarField alphat_;
            volScalarField fr1_;
            wallDist d_;


    // Protected Member Functions

        tmp<volScalarField> chi() const;

        tmp<volScalarField> fv1(const volScalarField& chi) const;

        tmp<volScalarField> fv2
        (
            const volScalarField& chi,
            const volScalarField& fv1
        ) const;

        tmp<volScalarField> fw(const volScalarField& Stilda) const;


public:

    //- Runtime type information
    TypeName("SpalartAllmaras");


    // Constructors

        //- Construct from components
        SpalartAllmaras
        (
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const basicThermo& thermophysicalModel,
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    //- Destructor
    virtual ~SpalartAllmaras()
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

        //- Return the effective diffusivity for nuTilda
        tmp<volScalarField> DnuTildaEff() const;

        //- Return the turbulence kinetic energy
        virtual tmp<volScalarField> k() const;

        //- Return the turbulence kinetic energy dissipation rate
        virtual tmp<volScalarField> epsilon() const;

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

