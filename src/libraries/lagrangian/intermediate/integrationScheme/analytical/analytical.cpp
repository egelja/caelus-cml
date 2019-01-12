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

#include "analytical.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace integrationSchemes
{
    defineTypeNameAndDebug(analytical, 0);
    addToRunTimeSelectionTable(integrationScheme, analytical, dictionary);
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::integrationSchemes::analytical::analytical()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::integrationSchemes::analytical::~analytical()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::scalar CML::integrationSchemes::analytical::dtEff
(
    const scalar dt,
    const scalar Beta
) const
{
    return
        mag(Beta*dt) > SMALL
      ? (1 - exp(- Beta*dt))/Beta
      : dt;
}


CML::scalar CML::integrationSchemes::analytical::sumDtEff
(
    const scalar dt,
    const scalar Beta
) const
{
    return
        mag(Beta*dt) > SMALL
      ? dt/Beta - (1 - exp(- Beta*dt))/sqr(Beta)
      : sqr(dt);
}


// ************************************************************************* //
