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
    CoherentStructureModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef CoherentStructureModel_HPP
#define CoherentStructureModel_HPP

#include "GenEddyVisc.hpp"

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
            const basicThermo& thermoPhysicalModel,
        word const& turbulenceModelName = turbulenceModel::typeName,
        word const& modelName = typeName
    );

    //- Destructor
    virtual ~CoherentStructureModel()
    {}

    //- Return SGS kinetic energy
    virtual tmp<volScalarField> k() const;

    //- Correct Eddy-Viscosity and related properties
    virtual void correct(tmp<volTensorField> const& gradU);

    //- Read LESProperties dictionary
    virtual bool read();
};


}
}
}
#endif

