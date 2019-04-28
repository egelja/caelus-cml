/*---------------------------------------------------------------------------*\
Copyright (C) 2015 - 2016 Applied CCM 
--------------------------------------------------------------------------------
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
    Spalart-Allmaras VLES model

Notes

    The default model coefficients correspond to the following:
    \verbatim
        SpalartAllmarasVLESCoeffs
        {
            curvatureCorrection false;
            Cb1                 0.1355;
            Cb2                 0.622;
            Cw2                 0.3;
            Cw3                 2.0;
            Cv1                 7.1;
            Cr1                 1.0;
            Cr2                 12.0;
            Cr3                 1.0;
            sigmaNut            0.66666;
            kappa               0.41;
        }
    \endverbatim

Author(s)
    Aleksandar Jemcov
    
\*---------------------------------------------------------------------------*/
#ifndef compressibleSpalartAllmarasVLES_HPP
#define compressibleSpalartAllmarasVLES_HPP

#include "compressibleVLESModel.hpp"
#include "wallDist.hpp"

namespace CML
{
namespace compressible
{
namespace VLESModels
{

class SpalartAllmarasVLES : public VLESModel
{

protected:

    Switch curvatureCorrection_;

    Switch delayed_;
    Switch outputfr1_;
    Switch outputFr_;
            
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
    dimensionedScalar Cx_;

    volScalarField nuTilda_;
    volScalarField mut_;
    volScalarField muSgs_;
    volScalarField alphat_;
    volScalarField alphaSgs_;
    volScalarField fr1_;
    volScalarField Fr_;
    wallDist d_;

    tmp<volScalarField> chi() const;

    tmp<volScalarField> fv1(const volScalarField& chi) const;

    tmp<volScalarField> fv2
    (
        volScalarField const& chi,
        volScalarField const& fv1
    ) const;

    tmp<volScalarField> fw(volScalarField const& Stilda) const;

public:

    TypeName("SpalartAllmarasVLES");


    SpalartAllmarasVLES
    (
        volScalarField const& rho,
        volVectorField const& U,
        surfaceScalarField const & phi,
        fluidThermo const& thermophysicalModel,
        word const& turbulenceModelName = turbulenceModel::typeName,
        word const& modelName = typeName
    );

    virtual ~SpalartAllmarasVLES() {}

    tmp<volScalarField> F1() const;

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

    tmp<volScalarField> DnuTildaEff() const;

    virtual tmp<volScalarField> k() const;

    virtual tmp<volScalarField> epsilon() const;

    virtual tmp<volSymmTensorField> R() const;

    virtual void correct();

    virtual bool read();
};


}
}
}

#endif

