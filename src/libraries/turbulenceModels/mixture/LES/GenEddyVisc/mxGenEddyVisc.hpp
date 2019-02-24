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
    CML::mixture::LESModels::GenEddyVisc

Description
    General base class for all mixture models that can be implemented as
    an eddy viscosity, i.e. algebraic and one-equation models.

    Contains fields for k (SGS turbulent kinetic energy), gamma
    (modelled viscosity) and epsilon (SGS dissipation).

SourceFiles
    mxGenEddyVisc.cpp

\*---------------------------------------------------------------------------*/

#ifndef mxGenEddyVisc_HPP
#define mxGenEddyVisc_HPP

#include "mxLESModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace mixture
{
namespace LESModels
{

/*---------------------------------------------------------------------------*\
                           Class GenEddyVisc Declaration
\*---------------------------------------------------------------------------*/

class GenEddyVisc
:
    virtual public LESModel
{
private:

    // Disallow default bitwise copy construct and assignment
    GenEddyVisc(const GenEddyVisc&);
    GenEddyVisc& operator=(const GenEddyVisc&);


protected:

    // Model coefficients
    dimensionedScalar ce_;

    // Fields
    volScalarField muSgs_;


public:

    //- Partial Runtime type information
    static const word typeName;


    //- Construct from components
    GenEddyVisc
    (
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi,
        const twoPhaseMixture& transport,
        const word& turbulenceModelName = turbulenceModel::typeName,
        const word& modelName = typeName
    );


    //- Destructor
    virtual ~GenEddyVisc()
    {}


    // Member Functions

    //- Return sub-grid disipation rate
    virtual tmp<volScalarField> epsilon() const
    {
        return tmp<volScalarField>
        (
            new volScalarField
            (
                IOobject
                (   
                    "epsilon",
                    runTime_.timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                ce_*k()*sqrt(k())/delta()
            )
        );
    }

    //- Return the SGS viscosity.
    virtual tmp<volScalarField> muSgs() const
    {
        return muSgs_;
    }

    //- Return the sub-grid stress tensor.
    virtual tmp<volSymmTensorField> B() const;

    //- Return the deviatoric part of the effective sub-grid
    //  turbulence stress tensor including the laminar stress
    virtual tmp<volSymmTensorField> devRhoBeff() const;

    //- Returns div(rho*dev(B)).
    // This is the additional term due to the filtering of the NSE.
    virtual tmp<fvVectorMatrix> divDevRhoBeff(volVectorField& U) const;

    //- Correct Eddy-Viscosity and related properties
    virtual void correct(const tmp<volTensorField>& gradU);

    //- Read LESProperties dictionary
    virtual bool read();
};


} // End namespace LESModels
} // End namespace mixture
} // End namespace CML


#endif
