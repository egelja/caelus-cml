/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::psiThermo

Description
    Basic thermodynamic properties based on compressibility

SourceFiles
    psiThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef psiThermo_HPP
#define psiThermo_HPP

#include "fluidThermo.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class psiThermo Declaration
\*---------------------------------------------------------------------------*/

class psiThermo
:
    public fluidThermo
{

protected:

    //- Compressibility [s^2/m^2]
    volScalarField psi_;

    //- Dynamic viscosity [kg/m/s]
    volScalarField mu_;

    //- Construct as copy (not implemented)
    psiThermo(const psiThermo&);


public:

    //- Runtime type information
    TypeName("psiThermo");


    //- Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        psiThermo,
        fvMesh,
        (const fvMesh& mesh, const word& phaseName),
        (mesh, phaseName)
    );


    //- Construct from mesh and phase name
    psiThermo
    (
        const fvMesh&,
        const word& phaseName
    );


    //- Selector
    static autoPtr<psiThermo> New
    (
        const fvMesh& mesh,
        const word& phaseName=word::null
    );


    //- Destructor
    virtual ~psiThermo()
    {}


    // Member functions

    // Fields derived from thermodynamic state variables

    //- Add the given density correction to the density field.
    //  Used to update the density field following pressure solution.
    //  For psiThermo does nothing.
    virtual void correctRho(const volScalarField& deltaRho);

    //- Density [kg/m^3] - uses current value of pressure
    virtual tmp<volScalarField> rho() const;

    //- Density for patch [kg/m^3]
    virtual tmp<scalarField> rho(const label patchi) const;

    //- Compressibility [s^2/m^2]
    virtual const volScalarField& psi() const;


    // Access to transport state variables

    //- Dynamic viscosity of mixture [kg/m/s]
    virtual const volScalarField& mu() const;

    //- Dynamic viscosity of mixture for patch [kg/m/s]
    virtual const scalarField& mu(const label patchi) const;

};


} // End namespace CML


#endif
