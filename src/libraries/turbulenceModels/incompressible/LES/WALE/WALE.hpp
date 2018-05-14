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
    WALE.cpp

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef WALE_HPP_
#define WALE_HPP_

#include "GenEddyVisc.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace incompressible
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
        volVectorField const& U,
        surfaceScalarField const& phi,
        transportModel& transport,
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
} // End namespace incompressible
} // End namespace CML

#endif


