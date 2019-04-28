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

#include "multiNormal.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace distributionModels
{
    defineTypeNameAndDebug(multiNormal, 0);
    addToRunTimeSelectionTable(distributionModel, multiNormal, dictionary);
}
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::distributionModels::multiNormal::multiNormal
(
    const dictionary& dict,
    Random& rndGen
)
:
    distributionModel(typeName, dict, rndGen),
    minValue_(readScalar(distributionModelDict_.lookup("minValue"))),
    maxValue_(readScalar(distributionModelDict_.lookup("maxValue"))),
    range_(maxValue_ - minValue_),
    expectation_(distributionModelDict_.lookup("expectation")),
    variance_(distributionModelDict_.lookup("variance")),
    strength_(distributionModelDict_.lookup("strength"))
{
    check();

    scalar sMax = 0;
    label n = strength_.size();
    for (label i=0; i<n; i++)
    {
        scalar x = expectation_[i];
        scalar s = strength_[i];
        for (label j=0; j<n; j++)
        {
            if (i!=j)
            {
                scalar x2 = (x - expectation_[j])/variance_[j];
                scalar y = exp(-0.5*x2*x2);
                s += strength_[j]*y;
            }
        }

        sMax = max(sMax, s);
    }

    for (label i=0; i<n; i++)
    {
        strength_[i] /= sMax;
    }
}


CML::distributionModels::multiNormal::multiNormal(const multiNormal& p)
:
    distributionModel(p),
    minValue_(p.minValue_),
    maxValue_(p.maxValue_),
    range_(p.range_),
    expectation_(p.expectation_),
    variance_(p.variance_),
    strength_(p.strength_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::distributionModels::multiNormal::~multiNormal()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::scalar CML::distributionModels::multiNormal::sample() const
{
    scalar y = 0;
    scalar x = 0;
    label n = expectation_.size();
    bool success = false;

    while (!success)
    {
        x = minValue_ + range_*rndGen_.sample01<scalar>();
        y = rndGen_.sample01<scalar>();
        scalar p = 0.0;

        for (label i=0; i<n; i++)
        {
            scalar nu = expectation_[i];
            scalar sigma = variance_[i];
            scalar s = strength_[i];
            scalar v = (x - nu)/sigma;
            p += s*exp(-0.5*v*v);
        }

        if (y<p)
        {
            success = true;
        }
    }

    return x;
}


CML::scalar CML::distributionModels::multiNormal::minValue() const
{
    return minValue_;
}


CML::scalar CML::distributionModels::multiNormal::maxValue() const
{
    return maxValue_;
}


CML::scalar CML::distributionModels::multiNormal::meanValue() const
{
    scalar mean = 0.0;
    forAll(strength_, i)
    {
        mean += strength_[i]*expectation_[i];
    }

    return mean;
}


// ************************************************************************* //
