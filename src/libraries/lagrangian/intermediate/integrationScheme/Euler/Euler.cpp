/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2017 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "Euler.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace integrationSchemes
{
    defineTypeNameAndDebug(Euler, 0);
    addToRunTimeSelectionTable(integrationScheme, Euler, dictionary);
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::integrationSchemes::Euler::Euler()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::integrationSchemes::Euler::~Euler()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::scalar CML::integrationSchemes::Euler::dtEff
(
    const scalar dt,
    const scalar Beta
) const
{
    return dt/(1 + Beta*dt);
}


CML::scalar CML::integrationSchemes::Euler::sumDtEff
(
    const scalar dt,
    const scalar Beta
) const
{
    return sqr(dt)/(1 + Beta*dt);
}


// ************************************************************************* //
