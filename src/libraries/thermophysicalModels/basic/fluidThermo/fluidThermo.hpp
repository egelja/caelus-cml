/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::fluidThermo

Description
    Fundamental fluid thermodynamic properties

SourceFiles
    fluidThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef fluidThermo_HPP
#define fluidThermo_HPP

#include "basicThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class fluidThermo Declaration
\*---------------------------------------------------------------------------*/

class fluidThermo
:
    public basicThermo
{

public:

    //- Runtime type information
    TypeName("fluidThermo");


    //- Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        fluidThermo,
        fvMesh,
        (const fvMesh& mesh, const word& phaseName),
        (mesh, phaseName)
    );


    //- Construct from mesh and phase name
    fluidThermo
    (
        const fvMesh&,
        const word& phaseName
    );

    //- Construct from mesh and phase name
    fluidThermo
    (
        const fvMesh&,
        const dictionary&,
        const word& phaseName
    );


    //- Selector
    static autoPtr<fluidThermo> New
    (
        const fvMesh&,
        const word& phaseName=word::null
    );


    //- Destructor
    virtual ~fluidThermo()
    {}


    // Member functions

    // Access to thermodynamic state variables

    //- Add the given density correction to the density field.
    //  Used to update the density field following pressure solution
    virtual void correctRho(const volScalarField& deltaRho) = 0;

    //- Compressibility [s^2/m^2]
    virtual const volScalarField& psi() const = 0;


    // Access to transport state variables

    //- Dynamic viscosity of mixture [kg/m/s]
    virtual const volScalarField& mu() const = 0;

    //- Dynamic viscosity of mixture for patch [kg/m/s]
    virtual const scalarField& mu(const label patchi) const = 0;

    //- Kinematic viscosity of mixture [m^2/s]
    virtual tmp<volScalarField> nu() const;

    //- Kinematic viscosity of mixture for patch [m^2/s]
    virtual tmp<scalarField> nu(const label patchi) const;

};


} // End namespace CML


#endif
