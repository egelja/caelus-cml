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

#include "exponential.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace distributionModels
{
    defineTypeNameAndDebug(exponential, 0);
    addToRunTimeSelectionTable(distributionModel, exponential, dictionary);
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::distributionModels::exponential::exponential
(
    const dictionary& dict,
    Random& rndGen
)
:
    distributionModel(typeName, dict, rndGen),
    minValue_(readScalar(distributionModelDict_.lookup("minValue"))),
    maxValue_(readScalar(distributionModelDict_.lookup("maxValue"))),
    lambda_(readScalar(distributionModelDict_.lookup("lambda")))
{
    check();
}


CML::distributionModels::exponential::exponential(const exponential& p)
:
    distributionModel(p),
    minValue_(p.minValue_),
    maxValue_(p.maxValue_),
    lambda_(p.lambda_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::distributionModels::exponential::~exponential()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::scalar CML::distributionModels::exponential::sample() const
{
    scalar y = rndGen_.sample01<scalar>();
    scalar K = exp(-lambda_*maxValue_) - exp(-lambda_*minValue_);
    return -(1.0/lambda_)*log(exp(-lambda_*minValue_) + y*K);
}


CML::scalar CML::distributionModels::exponential::minValue() const
{
    return minValue_;
}


CML::scalar CML::distributionModels::exponential::maxValue() const
{
    return maxValue_;
}


CML::scalar CML::distributionModels::exponential::meanValue() const
{
    return 1.0/lambda_;
}


// ************************************************************************* //
