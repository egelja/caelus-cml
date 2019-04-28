/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
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

Description
    SpalartAllmaras DDES LES turbulence model for incompressible flows

    References:
    1) P.R. Spalart, S. Deck, S., M.L.Shur, K.D. Squires, M.Kh Strelets, and
       A. Travin. `A new version of detached-eddy simulation, resistant to
       ambiguous grid densities'. Theor. Comp. Fluid Dyn., 20:181-195, 2006.

    2) Spalart, P. R. and Allmaras, S. R., "A One-Equation Turbulence Model for 
       Aerodynamic Flows," Recherche Aerospatiale, No. 1, 1994, pp. 5-21.

Author: A. Jemcov

\*---------------------------------------------------------------------------*/

#ifndef compressibleSpalartAllmarasDDES_HPP
#define compressibleSpalartAllmarasDDES_HPP

#include "compressibleSpalartAllmarasDES.hpp"

namespace CML
{
namespace compressible
{
namespace LESModels
{

class SpalartAllmarasDDES
:
    public SpalartAllmarasDES
{
    // Private Member Functions

    tmp<volScalarField> fd(volScalarField const& S) const;

    tmp<volScalarField> rd
    (
        volScalarField const& visc,
        volScalarField const& S
    ) const;

    // Disallow default bitwise copy construct and assignment
    SpalartAllmarasDDES(SpalartAllmarasDDES const&);
    SpalartAllmarasDDES& operator=(SpalartAllmarasDDES const&);

protected:

    // Protected Member Functions

    //- Length scale
    virtual tmp<volScalarField> dTilda(volScalarField const& S) const;

    virtual tmp<volScalarField> S(volTensorField const& gradU) const;

public:

    //- Runtime type information
    TypeName("SpalartAllmarasDDES");

    // Constructors

    //- Construct from components
    SpalartAllmarasDDES
    (
        volScalarField const& rho,
        volVectorField const& U,
        surfaceScalarField const& phi,
        fluidThermo const& thermophysicalModel,
        word const& turbulenceModelName = turbulenceModel::typeName,
        word const& modelName = typeName
    );

    //- Destructor
    virtual ~SpalartAllmarasDDES() {}
};

} 
} 
}

#endif


