/*---------------------------------------------------------------------------*\
Copyright (C) 2017 OpenFOAM Foundation
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

#include "quarterCosineRamp.hpp"
#include "mathematicalConstants.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace DataEntryTypes
{
    makeScalarDataEntry(quarterCosineRamp);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::DataEntryTypes::quarterCosineRamp::quarterCosineRamp
(
    const word& entryName,
    const dictionary& dict
)
:
    ramp(entryName, dict)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::DataEntryTypes::quarterCosineRamp::~quarterCosineRamp()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::scalar CML::DataEntryTypes::quarterCosineRamp::value
(
    const scalar t
) const
{
    return 1 - cos(0.5*constant::mathematical::pi*linearRamp(t));
}


// ************************************************************************* //
