/*---------------------------------------------------------------------------*\
Description
    SpalartAllmaras DES turbulence model for incompressible flows

    References:
    1) P.R. Spalart, W-H. Jou, M. Strelets, S. R. Allmaras, "Comments
       on the Feasibility of LES for Wings, and on Hybrid RANS/LES
       Approach", Proceedings of the First AFOSR International Conference
       on DNS/LES, Lousiana Tech University, Ruston Louisiana, USA,
       August 4-8, 1997

    2) Spalart, P. R. and Allmaras, S. R., "A One-Equation Turbulence Model for 
       Aerodynamic Flows," Recherche Aerospatiale, No. 1, 1994, pp. 5-21.

Author: A. Jemcov
Copyright Applied CCM (2014), ESI
\*---------------------------------------------------------------------------*/

#ifndef SpalartAllmarasDES_HPP
#define SpalartAllmarasDES_HPP

#include "LESModel.hpp"
#include "volFields.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace incompressible
{
namespace LESModels
{

class SpalartAllmarasDES
:
    public LESModel
{
    // Private Member Functions

        //- Update sub-grid scale fields
        void updateSubGridScaleFields();

        // Disallow default bitwise copy construct and assignment
        SpalartAllmarasDES(const SpalartAllmarasDES&);
        SpalartAllmarasDES& operator=(const SpalartAllmarasDES&);


protected:

    // Protected data

        dimensionedScalar sigmaNut_;
        dimensionedScalar kappa_;


        // Model constants

            dimensionedScalar Cb1_;
            dimensionedScalar Cb2_;
            dimensionedScalar Cv1_;
            dimensionedScalar Cv2_;
            dimensionedScalar CDES_;
            dimensionedScalar ck_;
            dimensionedScalar Cw1_;
            dimensionedScalar Cw2_;
            dimensionedScalar Cw3_;


        // Fields

            wallDist y_;
            volScalarField nuTilda_;
            volScalarField nuSgs_;


    // Protected Member Functions

        virtual tmp<volScalarField> chi() const;
        virtual tmp<volScalarField> fv1() const;
        virtual tmp<volScalarField> fv2() const;
        virtual tmp<volScalarField> S(const volTensorField& gradU) const;

        virtual tmp<volScalarField> STilda
        (
            const volScalarField& S,
            const volScalarField& dTilda
        ) const;

        virtual tmp<volScalarField> r
        (
            const volScalarField& visc,
            const volScalarField& S,
            const volScalarField& dTilda
        ) const;

        virtual tmp<volScalarField> fw
        (
            const volScalarField& S,
            const volScalarField& dTilda
        ) const;

        //- Length scale
        virtual tmp<volScalarField> dTilda(const volScalarField& S) const;


public:

    //- Runtime type information
    TypeName("SpalartAllmarasDES");


    // Constructors

        //- Construct from components
        SpalartAllmarasDES
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            transportModel& transport,
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    //- Destructor
    virtual ~SpalartAllmarasDES()
    {}


    // Member Functions

        //- Return SGS kinetic energy
        virtual tmp<volScalarField> k() const;

        //- Return sub-grid disipation rate
        virtual tmp<volScalarField> epsilon() const;

        tmp<volScalarField> nuTilda() const
        {
            return nuTilda_;
        }

        //- Return SGS viscosity
        virtual tmp<volScalarField> nuSgs() const
        {
            return nuSgs_;
        }

        //- Return the sub-grid stress tensor.
        virtual tmp<volSymmTensorField> B() const;

        //- Return the effective sub-grid turbulence stress tensor
        //  including the laminar stress
        virtual tmp<volSymmTensorField> devReff() const;

        //- Return the deviatoric part of the divergence of Beff
        //  i.e. the additional term in the filtered NSE.
        virtual tmp<fvVectorMatrix> divDevReff(volVectorField& U) const;

        //- Return the deviatoric part of the effective sub-grid
        //  turbulence stress tensor including the laminar stress
        virtual tmp<fvVectorMatrix> divDevRhoReff
        (
            const volScalarField& rho,
            volVectorField& U
        ) const;

        //- Correct nuTilda and related properties
        virtual void correct(const tmp<volTensorField>& gradU);

        //- Read LESProperties dictionary
        virtual bool read();
};


}
}
}

#endif


