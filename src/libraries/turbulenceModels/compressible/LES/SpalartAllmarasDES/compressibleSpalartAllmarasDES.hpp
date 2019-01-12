/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    SpalartAllmaras DES turbulence model for compressible flows

    References:
    1) P.R. Spalart, W-H. Jou, M. Strelets, S. R. Allmaras, "Comments
       on the Feasibility of LES for Wings, and on Hybrid RANS/LES
       Approach", Proceedings of the First AFOSR International Conference
       on DNS/LES, Lousiana Tech University, Ruston Louisiana, USA,
       August 4-8, 1997

    2) Spalart, P. R. and Allmaras, S. R., "A One-Equation Turbulence Model for 
       Aerodynamic Flows," Recherche Aerospatiale, No. 1, 1994, pp. 5-21.

Author(s)
    Aleksander Jemcov
    Chris Sideroff

Contact
    Aleks Jemcov: a.jemcov@appliedccm.com
    Chris Sideroff: c.sideroff@appliedccm.ca
    Darrin Stephens: d.stephens@appliedccm.com.au
\*---------------------------------------------------------------------------*/

#ifndef compressibleSpalartAllmarasDES_HPP
#define compressibleSpalartAllmarasDES_HPP

#include "compressibleLESModel.hpp"
#include "volFields.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace compressible
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

        // Model constants

            dimensionedScalar sigmaNut_;
            dimensionedScalar kappa_;
            dimensionedScalar Prt_;

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
            volScalarField muSgs_;
            volScalarField alphaSgs_;


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
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const fluidThermo& thermophysicalModel,
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
        virtual tmp<volScalarField> muSgs() const
        {
            return muSgs_;
        }

        //- Return SGS thermal diffusivity
        virtual tmp<volScalarField> alphaSgs() const
        {
            return alphaSgs_;
        }

        //- Return the sub-grid stress tensor.
        virtual tmp<volSymmTensorField> B() const;

        //- Return the effective sub-grid turbulence stress tensor
        //  including the laminar stress
        virtual tmp<volSymmTensorField> devRhoBeff() const;

        //- Return the deviatoric part of the divergence of Beff
        //  i.e. the additional term in the filtered NSE.
        virtual tmp<fvVectorMatrix> divDevRhoBeff(volVectorField& U) const;

        //- Correct nuTilda and related properties
        virtual void correct(const tmp<volTensorField>& gradU);

        //- Read LESProperties dictionary
        virtual bool read();
};


}
}
}

#endif


