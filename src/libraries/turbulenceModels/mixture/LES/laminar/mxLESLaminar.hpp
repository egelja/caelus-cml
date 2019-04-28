/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
Copyright (C) 2019 Applied CCM Pty Ltd
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
    CML::mixture::LESModels::laminar

Description
    LES model for laminar mixture multiphase flow.
    It simply returns laminar properties.

SourceFiles
    mixtureLESLaminar.cpp

\*---------------------------------------------------------------------------*/

#ifndef mxLESLaminar_HPP
#define mxLESLaminar_HPP

#include "mxLESModel.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace mixture
{
namespace LESModels
{

/*---------------------------------------------------------------------------*\
                           Class laminar Declaration
\*---------------------------------------------------------------------------*/

class laminar
:
    public LESModel
{
private:

    // Disallow default bitwise copy construct and assignment
    laminar(const laminar&);
    laminar& operator=(const laminar&);


public:

    //- Runtime type information
    TypeName("laminar");


    //- Construct from components
    laminar
    (
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi,
        const twoPhaseMixture& transport,
        const word& turbulenceModelName = turbulenceModel::typeName,
        const word& modelName = typeName
    );


    //- Destructor
    virtual ~laminar()
    {}


    // Member Functions

    //- Return SGS kinetic energy
    virtual tmp<volScalarField> k() const;

    //- Return sub-grid disipation rate
    virtual tmp<volScalarField> epsilon() const;

    //- Return SGS viscosity
    virtual tmp<volScalarField> muSgs() const;

    //- Return the effective viscosity
    virtual tmp<volScalarField> muEff() const;

    //- Return the sub-grid stress tensor B.
    virtual tmp<volSymmTensorField> B() const;

    //- Return the effective sub-grid turbulence stress tensor
    //  including the laminar stress
    virtual tmp<volSymmTensorField> devRhoBeff() const;

    //- Return the deviatoric part of the effective sub-grid
    //  turbulence stress tensor including the laminar stress
    virtual tmp<fvVectorMatrix> divDevRhoBeff(volVectorField& U) const;

    //- Read LESProperties dictionary
    bool read();
};


} // End namespace LESModels
} // End namespace mixture
} // End namespace CML


#endif
