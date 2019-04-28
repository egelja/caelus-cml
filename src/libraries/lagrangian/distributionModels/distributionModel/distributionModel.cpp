/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
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

#include "distributionModel.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

#ifdef windows
// Symbol to force loading at runtime
extern "C"
void distributionModelsLoad()
{}
#endif

namespace CML
{
    defineTypeNameAndDebug(distributionModel, 0);
    defineRunTimeSelectionTable(distributionModel, dictionary);
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

void CML::distributionModel::check() const
{
    if (minValue() < 0)
    {
        FatalErrorInFunction
            << type() << "distribution: Minimum value must be greater than "
            << "zero." << nl << "Supplied minValue = " << minValue()
            << abort(FatalError);
    }

    if (maxValue() < minValue())
    {
        FatalErrorInFunction
            << type() << "distribution: Maximum value is smaller than the "
            << "minimum value:" << nl << "    maxValue = " << maxValue()
            << ", minValue = " << minValue()
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::distributionModel::distributionModel
(
    const word& name,
    const dictionary& dict,
    Random& rndGen
)
:
    distributionModelDict_(dict.subDict(name + "Distribution")),
    rndGen_(rndGen)
{}


CML::distributionModel::distributionModel
(
    const distributionModel& p
)
:
    distributionModelDict_(p.distributionModelDict_),
    rndGen_(p.rndGen_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::distributionModel::~distributionModel()
{}


// ************************************************************************* //
