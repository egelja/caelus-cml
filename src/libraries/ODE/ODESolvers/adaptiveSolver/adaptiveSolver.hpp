/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
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
    CML::adaptiveSolver

Description

SourceFiles
    adaptiveSolver.cpp

\*---------------------------------------------------------------------------*/

#ifndef adaptiveSolver_HPP
#define adaptiveSolver_HPP

#include "ODESolver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class adaptiveSolver Declaration
\*---------------------------------------------------------------------------*/

class adaptiveSolver
{

    //- Step-size adjustment controls
    scalar safeScale_, alphaInc_, alphaDec_, minScale_, maxScale_;

    //- Cache for dydx at the initial time
    mutable scalarField dydx0_;

    //- Temprorary for the test-step solution
    mutable scalarField yTemp_;


public:

    //- Construct from ODESystem
    adaptiveSolver(const ODESystem& ode, const dictionary& dict);


    //- Destructor
    virtual ~adaptiveSolver()
    {}


    // Member Functions

    //- Resize the ODE solver
    bool resize(const label n);

    //- Solve a single step dx and return the error
    virtual scalar solve
    (
        const scalar x0,
        const scalarField& y0,
        const scalarField& dydx0,
        const scalar dx,
        scalarField& y
    ) const = 0;

    //- Solve the ODE system and the update the state
    void solve
    (
        const ODESystem& ode,
        scalar& x,
        scalarField& y,
        scalar& dxTry
    ) const;

};


} // End namespace CML


#endif
