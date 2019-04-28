/*---------------------------------------------------------------------------*\
Copyright (C) 2018 OpenFOAM Foundation
Copyright (C) Applied CCM
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
    CML::incompressible::LESModels::WALE

Description
    WALE model 

SourceFiles
    compressibleWALE.cpp

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef COMPRESSIBLE_WALE_HPP_
#define COMPRESSIBLE_WALE_HPP_

#include "compressibleGenEddyVisc.hpp"

namespace CML
{
namespace compressible
{
namespace LESModels
{

class WALE : public GenEddyVisc
{
    // Private data

    dimensionedScalar ck_;

    dimensionedScalar cw_;


    // Private Member Functions

    //- Update sub-grid scale fields
    void updateSubGridScaleFields(volTensorField const& gradU);

    // Disallow default bitwise copy construct and assignment
    WALE(WALE const&);
    WALE& operator=(WALE const&);


public:

    //- Runtime type information
    TypeName("WALE");

    // Constructors

    //- Construct from components
    WALE
    (
        volScalarField const& rho,
        volVectorField const& U,
        surfaceScalarField const& phi,
        fluidThermo const& thermoPhysicalModel,
        word const& turbulenceModelName = turbulenceModel::typeName,
        word const& modelName = typeName
    );


    //- Destructor
    virtual ~WALE()
    {}


    // Member Functions

    //- Compute SGS kinetic energy
    tmp<volScalarField> k(tmp<volTensorField> const& gradU) const
    {
        volScalarField magSqrSd(magSqr(dev(symm(gradU&gradU))));
        return tmp<volScalarField>
        (
            new volScalarField
            (
                IOobject
                (
                    "k",
                    this->runTime_.timeName(),
                    this->mesh_,
		    IOobject::NO_READ,
		    IOobject::NO_WRITE
                ),
                sqr(sqr(cw_)*this->delta()/ck_)*
                (
                    pow3(magSqrSd)
                   /(
                       sqr
                        (
                            pow(magSqr(symm(gradU)), 5.0/2.0)
                          + pow(magSqrSd, 5.0/4.0)
                        )
                     +  dimensionedScalar
                        (
                           "small",
                           dimensionSet(0, 0, -10, 0, 0),
                           SMALL
                        )
                    )
                )
            )
        );
    }
  

    //- Return SGS kinetic energy
    virtual tmp<volScalarField> k() const
    {
        return k(fvc::grad(U()));
    }


    //- Correct Eddy-Viscosity and related properties
    virtual void correct(tmp<volTensorField> const& gradU);

    //- Read LESProperties dictionary
    virtual bool read();
};

} // End namespace LESModels
} // End namespace compressible
} // End namespace CML

#endif


