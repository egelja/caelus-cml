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
    CML::Trapezoid

Description
    Trapezoidal ODE solver of order (1)2.

SourceFiles
    Trapezoid.cpp

\*---------------------------------------------------------------------------*/

#ifndef Trapezoid_HPP
#define Trapezoid_HPP

#include "ODESolver.hpp"
#include "adaptiveSolver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class Trapezoid Declaration
\*---------------------------------------------------------------------------*/

class Trapezoid
:
    public ODESolver,
    public adaptiveSolver
{

    mutable scalarField err_;


public:

    //- Runtime type information
    TypeName("Trapezoid");


    //- Construct from ODESystem
    Trapezoid(const ODESystem& ode, const dictionary& dict);


    //- Destructor
    virtual ~Trapezoid()
    {}


    // Member Functions

    //- Inherit solve from ODESolver
    using ODESolver::solve;

    //- Resize the ODE solver
    virtual bool resize();

    //- Solve a single step dx and return the error
    virtual scalar solve
    (
        const scalar x0,
        const scalarField& y0,
        const scalarField& dydx0,
        const scalar dx,
        scalarField& y
    ) const;

    //- Solve the ODE system and the update the state
    virtual void solve
    (
        scalar& x,
        scalarField& y,
        scalar& dxTry
    ) const;

};


} // End namespace CML


#endif
