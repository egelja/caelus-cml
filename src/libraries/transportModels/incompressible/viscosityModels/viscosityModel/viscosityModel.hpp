/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

Namespace
    CML::viscosityModels

Description
    A namespace for various incompressible viscosityModel implementations.

Class
    CML::viscosityModel

Description
    An abstract base class for incompressible viscosityModels.

    The strain rate is defined by:

        sqrt(2.0)*mag(symm(grad(U)))


SourceFiles
    viscosityModel.cpp
    viscosityModelNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef viscosityModel_H
#define viscosityModel_H

#include "IOdictionary.hpp"
#include "Switch.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"
#include "volFieldsFwd.hpp"
#include "volFields.hpp"
#include "surfaceFieldsFwd.hpp"
#include "dimensionedScalar.hpp"
#include "tmp.hpp"
#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                Class viscosityModel Declaration
\*---------------------------------------------------------------------------*/

class viscosityModel
{
private:

    //- Disallow copy construct
    viscosityModel(const viscosityModel&);

    //- Disallow default bitwise assignment
    void operator=(const viscosityModel&);

protected:

    word name_;

    dictionary viscosityProperties_;

    //- Switch for printing model coefficients
    const Switch printCoeffs_;

    //- Switch for outputing shear strain rate
    const Switch outputShearStrainRate_;

    const volVectorField& U_;
    const surfaceScalarField& phi_;

    //- Shear Strain Rate field 
    volScalarField ssr_;

    //- Print model coefficients
    virtual void printCoeffs();

public:

    //- Runtime type information
    TypeName("viscosityModel");


    // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        viscosityModel,
        dictionary,
        (
            const word& name,
            const dictionary& viscosityProperties,
            const volVectorField& U,
            const surfaceScalarField& phi
        ),
        (name, viscosityProperties, U, phi)
    );


    // Selectors

    //- Return a reference to the selected viscosity model
    static autoPtr<viscosityModel> New
    (
        const word& name,
        const dictionary& viscosityProperties,
        const volVectorField& U,
        const surfaceScalarField& phi
    );


    //- Construct from components
    viscosityModel
    (
        const word& name,
        const dictionary& viscosityProperties,
        const volVectorField& U,
        const surfaceScalarField& phi
    );


    //- Destructor
    virtual ~viscosityModel()
    {}


    // Member Functions

    //- Return the phase transport properties dictionary
    const dictionary& viscosityProperties() const
    {
        return viscosityProperties_;
    }

    //- Return the shear strain rate
    const volScalarField& strainRate() const
    {
        return ssr_;
    }

    //- Return the laminar viscosity
    virtual tmp<volScalarField> nu() const = 0;

    //- Correct the laminar viscosity
    virtual void correct();

    //- Read transportProperties dictionary
    virtual bool read(const dictionary& viscosityProperties) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
