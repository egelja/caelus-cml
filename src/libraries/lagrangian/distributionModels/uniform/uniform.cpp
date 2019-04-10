/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

#include "uniform.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace distributionModels
{
    defineTypeNameAndDebug(uniform, 0);
    addToRunTimeSelectionTable(distributionModel, uniform, dictionary);
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::distributionModels::uniform::uniform
(
    const dictionary& dict,
    Random& rndGen
)
:
    distributionModel(typeName, dict, rndGen),
    minValue_(readScalar(distributionModelDict_.lookup("minValue"))),
    maxValue_(readScalar(distributionModelDict_.lookup("maxValue")))
{
    check();
}


CML::distributionModels::uniform::uniform(const uniform& p)
:
    distributionModel(p),
    minValue_(p.minValue_),
    maxValue_(p.maxValue_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::distributionModels::uniform::~uniform()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::scalar CML::distributionModels::uniform::sample() const
{
    return rndGen_.scalarAB(minValue_, maxValue_);
}


CML::scalar CML::distributionModels::uniform::minValue() const
{
    return minValue_;
}


CML::scalar CML::distributionModels::uniform::maxValue() const
{
    return maxValue_;
}


CML::scalar CML::distributionModels::uniform::meanValue() const
{
    return 0.5*(minValue_ + maxValue_);
}


// ************************************************************************* //
