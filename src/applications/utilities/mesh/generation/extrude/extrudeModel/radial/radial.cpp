/*---------------------------------------------------------------------------*\
Copyright (C) 2013 OpenFOAM Foundation
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

#include "radial.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace extrudeModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(radial, 0);

addToRunTimeSelectionTable(extrudeModel, radial, dictionary);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

radial::radial(const dictionary& dict)
:
    extrudeModel(typeName, dict),
    R_(DataEntry<scalar>::New("R", coeffDict_))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

radial::~radial()
{}


// * * * * * * * * * * * * * * * * Operators * * * * * * * * * * * * * * * * //

point radial::operator()
(
    const point& surfacePoint,
    const vector& surfaceNormal,
    const label layer
) const
{
    // radius of the surface
    scalar rs = mag(surfacePoint);
    vector rsHat = surfacePoint/rs;

    scalar r = R_->value(layer);

Pout<< "** for layer " << layer << " r:" << r << endl;

    return r*rsHat;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace extrudeModels
} // End namespace CML

// ************************************************************************* //
