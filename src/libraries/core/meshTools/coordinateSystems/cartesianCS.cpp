/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2014 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "cartesianCS.hpp"

#include "one.hpp"
#include "mathematicalConstants.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(cartesianCS, 0);
    addToRunTimeSelectionTable(coordinateSystem, cartesianCS, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::cartesianCS::cartesianCS()
:
    coordinateSystem()
{}


CML::cartesianCS::cartesianCS
(
    const coordinateSystem& cs
)
:
    coordinateSystem(cs)
{}


CML::cartesianCS::cartesianCS
(
    const word& name,
    const coordinateSystem& cs
)
:
    coordinateSystem(name, cs)
{}


CML::cartesianCS::cartesianCS
(
    const word& name,
    const point& origin,
    const coordinateRotation& cr
)
:
    coordinateSystem(name, origin, cr)
{}


CML::cartesianCS::cartesianCS
(
    const word& name,
    const point& origin,
    const vector& axis,
    const vector& dirn
)
:
    coordinateSystem(name, origin, axis, dirn)
{}


CML::cartesianCS::cartesianCS
(
    const word& name,
    const dictionary& dict
)
:
    coordinateSystem(name, dict)
{}


CML::cartesianCS::cartesianCS
(
    const objectRegistry& obr,
    const dictionary& dict
)
:
    coordinateSystem(obr, dict)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::cartesianCS::~cartesianCS()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


CML::vector CML::cartesianCS::localToGlobal
(
    const vector& local,
    bool translate
) const
{
    return coordinateSystem::localToGlobal(local, translate);
}


CML::tmp<CML::vectorField> CML::cartesianCS::localToGlobal
(
    const vectorField& local,
    bool translate
) const
{
    return coordinateSystem::localToGlobal(local, translate);
}


CML::vector CML::cartesianCS::globalToLocal
(
    const vector& global,
    bool translate
) const
{
    return coordinateSystem::globalToLocal(global, translate);
}


CML::tmp<CML::vectorField> CML::cartesianCS::globalToLocal
(
    const vectorField& global,
    bool translate
) const
{
    return coordinateSystem::globalToLocal(global, translate);
}


// ************************************************************************* //
