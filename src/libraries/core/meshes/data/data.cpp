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

#include "data.hpp"
#include "Time.hpp"
#include "lduMatrix.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

int CML::data::debug(CML::debug::debugSwitch("data", false));

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::data::data(const objectRegistry& obr)
:
    IOdictionary
    (
        IOobject
        (
            "data",
            obr.time().system(),
            obr,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        )
    ),
    prevTimeIndex_(0)
{
    set("solverPerformance", dictionary());
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::dictionary& CML::data::solverPerformanceDict() const
{
    return subDict("solverPerformance");
}


void CML::data::setSolverPerformance
(
    const word& name,
    const solverPerformance& sp
) const
{
    dictionary& dict = const_cast<dictionary&>(solverPerformanceDict());

    List<solverPerformance> perfs;

    if (prevTimeIndex_ != this->time().timeIndex())
    {
        // reset solver performance between iterations
        prevTimeIndex_ = this->time().timeIndex();
        dict.clear();
    }
    else
    {
        dict.readIfPresent(name, perfs);
    }

    // append to list
    perfs.setSize(perfs.size()+1, sp);

    dict.set(name, perfs);
}


void CML::data::setSolverPerformance
(
    const solverPerformance& sp
) const
{
    setSolverPerformance(sp.fieldName(), sp);
}


// ************************************************************************* //
