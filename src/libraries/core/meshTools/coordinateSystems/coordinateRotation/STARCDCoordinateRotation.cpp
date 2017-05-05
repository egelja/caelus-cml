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

#include "STARCDCoordinateRotation.hpp"

#include "mathematicalConstants.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(STARCDCoordinateRotation, 0);
    addToRunTimeSelectionTable
    (
        coordinateRotation,
        STARCDCoordinateRotation,
        dictionary
    );
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::STARCDCoordinateRotation::calcTransform
(
    const scalar rotZ,
    const scalar rotX,
    const scalar rotY,
    const bool inDegrees
)
{
    scalar x = rotX;
    scalar y = rotY;
    scalar z = rotZ;

    if (inDegrees)
    {
        x *= constant::mathematical::pi/180.0;
        y *= constant::mathematical::pi/180.0;
        z *= constant::mathematical::pi/180.0;
    }

    tensor::operator=
    (
        tensor
        (
            cos(y)*cos(z) - sin(x)*sin(y)*sin(z),
            -cos(x)*sin(z),
            sin(x)*cos(y)*sin(z) + sin(y)*cos(z),

            cos(y)*sin(z) + sin(x)*sin(y)*cos(z),
            cos(x)*cos(z),
            sin(y)*sin(z) - sin(x)*cos(y)*cos(z),

            -cos(x)*sin(y),
            sin(x),
            cos(x)*cos(y)
        )
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::STARCDCoordinateRotation::STARCDCoordinateRotation()
:
    coordinateRotation()
{}


CML::STARCDCoordinateRotation::STARCDCoordinateRotation
(
    const vector& rotZrotXrotY,
    const bool inDegrees
)
:
    coordinateRotation()
{
    calcTransform
    (
        rotZrotXrotY.component(vector::X),
        rotZrotXrotY.component(vector::Y),
        rotZrotXrotY.component(vector::Z),
        inDegrees
    );
}


CML::STARCDCoordinateRotation::STARCDCoordinateRotation
(
    const scalar rotZ,
    const scalar rotX,
    const scalar rotY,
    const bool inDegrees
)
:
    coordinateRotation()
{
    calcTransform(rotZ, rotX, rotY, inDegrees);
}


CML::STARCDCoordinateRotation::STARCDCoordinateRotation
(
    const dictionary& dict
)
:
    coordinateRotation()
{
    vector rotation(dict.lookup("rotation"));

    calcTransform
    (
        rotation.component(vector::X),
        rotation.component(vector::Y),
        rotation.component(vector::Z),
        dict.lookupOrDefault("degrees", true)
    );
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
