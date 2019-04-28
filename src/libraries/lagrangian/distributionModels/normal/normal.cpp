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

#include "normal.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "mathematicalConstants.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace distributionModels
{
    defineTypeNameAndDebug(normal, 0);
    addToRunTimeSelectionTable(distributionModel, normal, dictionary);
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::distributionModels::normal::normal
(
    const dictionary& dict,
    Random& rndGen
)
:
    distributionModel(typeName, dict, rndGen),
    minValue_(readScalar(distributionModelDict_.lookup("minValue"))),
    maxValue_(readScalar(distributionModelDict_.lookup("maxValue"))),
    expectation_(readScalar(distributionModelDict_.lookup("expectation"))),
    variance_(readScalar(distributionModelDict_.lookup("variance"))),
    a_(0.147)
{
    if (minValue_ < 0)
    {
        FatalErrorInFunction
            << "Minimum value must be greater than zero. "
            << "Supplied minValue = " << minValue_
            << abort(FatalError);
    }

    if (maxValue_ < minValue_)
    {
        FatalErrorInFunction
            << "Maximum value is smaller than the minimum value:"
            << "    maxValue = " << maxValue_ << ", minValue = " << minValue_
            << abort(FatalError);
    }
}


CML::distributionModels::normal::normal(const normal& p)
:
    distributionModel(p),
    minValue_(p.minValue_),
    maxValue_(p.maxValue_),
    expectation_(p.expectation_),
    variance_(p.variance_),
    a_(p.a_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::distributionModels::normal::~normal()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::scalar CML::distributionModels::normal::sample() const
{

    scalar a = erf((minValue_ - expectation_)/variance_);
    scalar b = erf((maxValue_ - expectation_)/variance_);

    scalar y = rndGen_.sample01<scalar>();
    scalar x = erfInv(y*(b - a) + a)*variance_ + expectation_;

    // Note: numerical approximation of the inverse function yields slight
    //       inaccuracies

    x = min(max(x, minValue_), maxValue_);

    return x;
}


CML::scalar CML::distributionModels::normal::minValue() const
{
    return minValue_;
}


CML::scalar CML::distributionModels::normal::maxValue() const
{
    return maxValue_;
}


CML::scalar CML::distributionModels::normal::meanValue() const
{
    return expectation_;
}


CML::scalar CML::distributionModels::normal::erfInv(const scalar y) const
{
    scalar k = 2.0/(constant::mathematical::pi*a_) +  0.5*log(1.0 - y*y);
    scalar h = log(1.0 - y*y)/a_;
    scalar x = sqrt(-k + sqrt(k*k - h));
    if (y < 0.0)
    {
        x *= -1.0;
    }
    return x;
}


// ************************************************************************* //
