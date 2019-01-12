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
    CML::Rosenbrock23

Description
    L-stable embedded Rosenbrock ODE solver of order (2)3.

    References:
    \verbatim
        Sandu, A., Verwer, J. G., Blom, J. G., Spee, E. J., Carmichael, G. R.,
        & Potra, F. A. (1997).
        Benchmarking stiff ODE solvers for atmospheric chemistry problems II:
        Rosenbrock solvers.
        Atmospheric environment, 31(20), 3459-3472.
    \endverbatim

SourceFiles
    Rosenbrock23.cpp

\*---------------------------------------------------------------------------*/

#ifndef Rosenbrock23_HPP
#define Rosenbrock23_HPP

#include "ODESolver.hpp"
#include "adaptiveSolver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class Rosenbrock23 Declaration
\*---------------------------------------------------------------------------*/

class Rosenbrock23
:
    public ODESolver,
    public adaptiveSolver
{

    mutable scalarField k1_;
    mutable scalarField k2_;
    mutable scalarField k3_;
    mutable scalarField err_;
    mutable scalarField dydx_;
    mutable scalarField dfdx_;
    mutable scalarSquareMatrix dfdy_;
    mutable scalarSquareMatrix a_;
    mutable labelList pivotIndices_;

    static const scalar
        a21, a31, a32,
        c21, c31, c32,
        b1, b2, b3,
        e1, e2, e3,
        gamma,
        c2, c3,
        d1, d2, d3;


public:

    //- Runtime type information
    TypeName("Rosenbrock23");


    //- Construct from ODESystem
    Rosenbrock23(const ODESystem& ode, const dictionary& dict);


    //- Destructor
    virtual ~Rosenbrock23()
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
