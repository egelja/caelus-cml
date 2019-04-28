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

Class
    CML::compressible::LESModels::CoherentStructureModel

Description
    Coherent Structure LES model

Author
   Aleksandar Jemcov

SourceFiles
    compressibleCoherentStructureModel.cpp

References
    [1] "Application of a local SGS model based on coherent structure to
         complex geometries", H. Kobayashi, F.E. Ham, X. Wu, International
         Journal of Heat and Fluid Flow, 29(3) pp. 640-653


\*---------------------------------------------------------------------------*/

#ifndef compressibleCoherentStructureModel_HPP
#define compressibleCoherentStructureModel_HPP

#include "compressibleGenEddyVisc.hpp"

namespace CML
{
namespace compressible
{
namespace LESModels
{

class CoherentStructureModel : public GenEddyVisc
{

    dimensionedScalar cPrime_;

    //- Update sub-grid scale fields
    void updateSubGridScaleFields(volTensorField const& gradU);

    //- Calculate C
    volScalarField C(volTensorField const& gradU) const;

    // Disallow default bitwise copy construct and assignment
    CoherentStructureModel(CoherentStructureModel const&);
    CoherentStructureModel& operator=(CoherentStructureModel const&);

public:

    //- Runtime type information
    TypeName("CoherentStructureModel");

    //- Construct from components
    CoherentStructureModel
    (
        const volScalarField& rho,
        volVectorField const& U,
        surfaceScalarField const& phi,
        const fluidThermo& thermoPhysicalModel,
        word const& turbulenceModelName = turbulenceModel::typeName,
        word const& modelName = typeName
    );

    //- Destructor
    virtual ~CoherentStructureModel()
    {}

    //- Return SGS kinetic energy
    virtual tmp<volScalarField> k() const
    {
        volSymmTensorField D(symm(fvc::grad(U())));
        scalar const ce = 1.048;
        volScalarField const ck(C(fvc::grad(U()))*ce);
        volScalarField const a(ce/delta());
        volScalarField const b((2.0/3.0)*tr(D));
        volScalarField const c(2*ck*delta()*(dev(D) && D));

        return sqr((-b + sqrt(sqr(b) + 4*a*c))/(2*a));
    }

    //- Correct Eddy-Viscosity and related properties
    virtual void correct(tmp<volTensorField> const& gradU);

    //- Read LESProperties dictionary
    virtual bool read();
};


}
}
}
#endif

