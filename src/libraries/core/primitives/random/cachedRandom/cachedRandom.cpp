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

#include "cachedRandom.hpp"
#include "OSspecific.hpp"

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
    sampleI_(-1)
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
    sampleI_(cr.sampleI_)
{
    if (sampleI_ == -1)
    {
        WarningIn
        (
            "CML::cachedRandom::cachedRandom(const cachedRandom& cr)"
        )   << "Copy constructor called, but samples not being cached. "
            << "This may lead to non-repeatable behaviour" << endl;

        osRandomSeed(seed_);
    }
    else if (reset)
    {
        sampleI_ = 0;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::cachedRandom::~cachedRandom()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<>
CML::label CML::cachedRandom::sample01()
{
    return round(scalar01());
}


template<>
CML::scalar CML::cachedRandom::sample01()
{
    return scalar01();
}


template<>
CML::label CML::cachedRandom::position(const label& start, const label& end)
{
    return start + round(scalar01()*(end - start));
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


void CML::cachedRandom::operator=(const cachedRandom& cr)
{
    seed_ = cr.seed_;
    samples_ = cr.samples_;
    sampleI_ = cr.sampleI_;
}


// ************************************************************************* //
