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

#include "subCycleTime.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::subCycleTime::subCycleTime(Time& t, const label nSubCycles)
:
    time_(t),
    nSubCycles_(nSubCycles),
    subCycleIndex_(0)
{
    time_.subCycle(nSubCycles_);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::subCycleTime::~subCycleTime()
{
    endSubCycle();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::subCycleTime::end() const
{
    return subCycleIndex_ > nSubCycles_;
}


void CML::subCycleTime::endSubCycle()
{
    time_.endSubCycle();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

CML::subCycleTime& CML::subCycleTime::operator++()
{
    time_++;
    subCycleIndex_++;
    return *this;
}


CML::subCycleTime& CML::subCycleTime::operator++(int)
{
    return operator++();
}


// ************************************************************************* //
