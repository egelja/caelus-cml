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

\*---------------------------------------------------------------------------*/

#include "sphericalCS.hpp"

#include "one.hpp"
#include "mathematicalConstants.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(sphericalCS, 0);
    addToRunTimeSelectionTable(coordinateSystem, sphericalCS, dictionary);
    addToRunTimeSelectionTable(coordinateSystem, sphericalCS, origRotation);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sphericalCS::sphericalCS(const bool inDegrees)
:
    coordinateSystem(),
    inDegrees_(inDegrees)
{}


CML::sphericalCS::sphericalCS
(
    const coordinateSystem& cs,
    const bool inDegrees
)
:
    coordinateSystem(cs),
    inDegrees_(inDegrees)
{}


CML::sphericalCS::sphericalCS
(
    const word& name,
    const coordinateSystem& cs,
    const bool inDegrees
)
:
    coordinateSystem(name, cs),
    inDegrees_(inDegrees)
{}


CML::sphericalCS::sphericalCS
(
    const word& name,
    const point& origin,
    const coordinateRotation& cr,
    const bool inDegrees
)
:
    coordinateSystem(name, origin, cr),
    inDegrees_(inDegrees)
{}


CML::sphericalCS::sphericalCS
(
    const word& name,
    const point& origin,
    const vector& axis,
    const vector& dirn,
    const bool inDegrees
)
:
    coordinateSystem(name, origin, axis, dirn),
    inDegrees_(inDegrees)
{}


CML::sphericalCS::sphericalCS
(
    const word& name,
    const dictionary& dict
)
:
    coordinateSystem(name, dict),
    inDegrees_(dict.lookupOrDefault("degrees", true))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::sphericalCS::inDegrees() const
{
    return inDegrees_;
}


bool& CML::sphericalCS::inDegrees()
{
    return inDegrees_;
}


CML::vector CML::sphericalCS::localToGlobal
(
    const vector& local,
    bool translate
) const
{
    scalar r = local.x();
    const scalar theta
    (
        local.y()
       *(inDegrees_ ? constant::mathematical::pi/180.0 : 1.0)
    );
    const scalar phi
    (
        local.z()
       *(inDegrees_ ? constant::mathematical::pi/180.0 : 1.0)
    );

    return coordinateSystem::localToGlobal
    (
        vector(r*cos(theta)*sin(phi), r*sin(theta)*sin(phi), r*cos(phi)),
        translate
    );
}


CML::tmp<CML::vectorField> CML::sphericalCS::localToGlobal
(
    const vectorField& local,
    bool translate
) const
{
    const scalarField r(local.component(vector::X));
    const scalarField theta
    (
        local.component(vector::Y)
       *(inDegrees_ ? constant::mathematical::pi/180.0 : 1.0)
    );
    const scalarField phi
    (
        local.component(vector::Z)
       *(inDegrees_ ? constant::mathematical::pi/180.0 : 1.0)
    );

    vectorField lc(local.size());
    lc.replace(vector::X, r*cos(theta)*sin(phi));
    lc.replace(vector::Y, r*sin(theta)*sin(phi));
    lc.replace(vector::Z, r*cos(phi));

    return coordinateSystem::localToGlobal(lc, translate);
}


CML::vector CML::sphericalCS::globalToLocal
(
    const vector& global,
    bool translate
) const
{
    const vector lc = coordinateSystem::globalToLocal(global, translate);
    const scalar r = mag(lc);

    return vector
    (
        r,
        atan2
        (
            lc.y(), lc.x()
        )*(inDegrees_ ? 180.0/constant::mathematical::pi : 1.0),
        acos
        (
            lc.z()/(r + SMALL)
        )*(inDegrees_ ? 180.0/constant::mathematical::pi : 1.0)
    );
}


CML::tmp<CML::vectorField> CML::sphericalCS::globalToLocal
(
    const vectorField& global,
    bool translate
) const
{
    const vectorField lc(coordinateSystem::globalToLocal(global, translate));
    const scalarField r(mag(lc));

    tmp<vectorField> tresult(new vectorField(lc.size()));
    vectorField& result = tresult();

    result.replace
    (
        vector::X, r

    );

    result.replace
    (
        vector::Y,
        atan2
        (
            lc.component(vector::Y),
            lc.component(vector::X)
        )*(inDegrees_ ? 180.0/constant::mathematical::pi : 1.0)
    );

    result.replace
    (
        vector::Z,
        acos
        (
            lc.component(vector::Z)/(r + SMALL)
        )*(inDegrees_ ? 180.0/constant::mathematical::pi : 1.0)
    );

    return tresult;
}


// ************************************************************************* //
