/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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

#include "cachedRandom.hpp"
#include "OSspecific.hpp"
#include "PstreamReduceOps.hpp"

// * * * * * * * * * * * * * private Member Functions  * * * * * * * * * * * //

CML::scalar CML::cachedRandom::scalar01()
{
    if (sampleI_ < 0)
    {
        return osRandomDouble();
    }

    if (sampleI_ == samples_.size() - 1)
    {
        scalar s = samples_[sampleI_];
        sampleI_ = 0;
        return s;
    }
    else
    {
        scalar s = samples_[sampleI_];
        sampleI_++;
        return s;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::cachedRandom::cachedRandom(const label seed, const label count)
:
    seed_(1),
    samples_(0),
    sampleI_(-1),
    hasGaussSample_(false),
    gaussSample_(0)
{
    if (seed > 1)
    {
        seed_ = seed;
    }

    // Samples will be cached if count > 0
    if (count > 0)
    {
        samples_.setSize(count);
        sampleI_ = 0;
    }

    // Initialise samples
    osRandomSeed(seed_);
    forAll(samples_, i)
    {
        samples_[i] = osRandomDouble();
    }
}


CML::cachedRandom::cachedRandom(const cachedRandom& cr, const bool reset)
:
    seed_(cr.seed_),
    samples_(cr.samples_),
    sampleI_(cr.sampleI_),
    hasGaussSample_(cr.hasGaussSample_),
    gaussSample_(cr.gaussSample_)
{
    if (reset)
    {
        hasGaussSample_ = false;
        gaussSample_ = 0;
    }
    if (sampleI_ == -1)
    {
        WarningIn
        (
            "CML::cachedRandom::cachedRandom(const cachedRandom& cr)"
        )   << "Copy constructor called, but samples not being cached. "
            << "This may lead to non-repeatable behaviour" << endl;

        osRandomSeed(seed_);
    }

    if (reset && samples_.size())
    {
        sampleI_ = 0;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::cachedRandom::~cachedRandom()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<>
CML::scalar CML::cachedRandom::sample01()
{
    return scalar01();
}


template<>
CML::label CML::cachedRandom::sample01()
{
    return round(scalar01());
}


template<>
CML::scalar CML::cachedRandom::GaussNormal()
{
    if (hasGaussSample_)
    {
        hasGaussSample_ = false;
        return gaussSample_;
    }
    else
    {
        scalar rsq, v1, v2;
        do
        {
            v1 = 2*scalar01() - 1;
            v2 = 2*scalar01() - 1;
            rsq = sqr(v1) + sqr(v2);
        } while (rsq >= 1 || rsq == 0);

        scalar fac = sqrt(-2*log(rsq)/rsq);

        gaussSample_ = v1*fac;
        hasGaussSample_ = true;

        return v2*fac;
    }
}


template<>
CML::label CML::cachedRandom::GaussNormal()
{
    return round(GaussNormal<scalar>());
}


template<>
CML::scalar CML::cachedRandom::position
(
    const scalar& start,
    const scalar& end
)
{
    return start + scalar01()*(end - start);
}


template<>
CML::label CML::cachedRandom::position(const label& start, const label& end)
{
    return start + round(scalar01()*(end - start));
}


template<>
CML::scalar CML::cachedRandom::globalSample01()
{
    scalar value = -GREAT;

    if (Pstream::master())
    {
        value = scalar01();
    }

    reduce(value, maxOp<scalar>());

    return value;
}


template<>
CML::label CML::cachedRandom::globalSample01()
{
    scalar value = -GREAT;

    if (Pstream::master())
    {
        value = scalar01();
    }

    reduce(value, maxOp<scalar>());

    return round(value);
}


template<>
CML::scalar CML::cachedRandom::globalGaussNormal()
{
    scalar value = -GREAT;

    if (Pstream::master())
    {
        value = GaussNormal<scalar>();
    }

    reduce(value, maxOp<scalar>());

    return value;
}


template<>
CML::label CML::cachedRandom::globalGaussNormal()
{
    scalar value = -GREAT;

    if (Pstream::master())
    {
        value = GaussNormal<scalar>();
    }

    reduce(value, maxOp<scalar>());

    return round(value);
}


template<>
CML::scalar CML::cachedRandom::globalPosition
(
    const scalar& start,
    const scalar& end
)
{
    scalar value = -GREAT;

    if (Pstream::master())
    {
        value = scalar01()*(end - start);
    }

    reduce(value, maxOp<scalar>());

    return start + value;
}


template<>
CML::label CML::cachedRandom::globalPosition
(
    const label& start,
    const label& end
)
{
    label value = labelMin;

    if (Pstream::master())
    {
        value = round(scalar01()*(end - start));
    }

    reduce(value, maxOp<label>());

    return start + value;
}


// ************************************************************************* //
