/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "linearNormal.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace extrudeModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(linearNormal, 0);

addToRunTimeSelectionTable(extrudeModel, linearNormal, dictionary);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

linearNormal::linearNormal(const dictionary& dict)
:
    extrudeModel(typeName, dict),
    thickness_(readScalar(coeffDict_.lookup("thickness"))),
    firstCellThickness_(0),
    layerPoints_(nLayers_)
{
    if (thickness_ <= 0)
    {
        FatalErrorInFunction
            << "thickness should be positive : " << thickness_
            << exit(FatalError);
    }

    coeffDict_.readIfPresent("firstCellThickness", firstCellThickness_);

    if (firstCellThickness_ >= thickness_)
    {
        FatalErrorInFunction
            << "firstCellThickness is larger than thickness"
            << exit(FatalError);
    }

    if (firstCellThickness_ > 0)
    {
        layerPoints_[0] = firstCellThickness_;

        for (label layerI = 1; layerI < nLayers_; layerI++)
        {
            layerPoints_[layerI] =
                (thickness_ - layerPoints_[0])
                *sumThickness(layerI) + layerPoints_[0];
        }
    }
    else
    {
        for (label layerI = 0; layerI < nLayers_; layerI++)
        {
            layerPoints_[layerI] = thickness_*sumThickness(layerI + 1);
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

linearNormal::~linearNormal()
{}


// * * * * * * * * * * * * * * * * Operators * * * * * * * * * * * * * * * * //

point linearNormal::operator()
(
    const point& surfacePoint,
    const vector& surfaceNormal,
    const label layer
) const
{
    if (layer == 0)
    {
        return surfacePoint;
    }
    else
    {
        return surfacePoint + layerPoints_[layer - 1]*surfaceNormal;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace extrudeModels
} // End namespace CML

// ************************************************************************* //
