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

#include "outputFilterOutputControl.hpp"

// * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * * //

namespace CML
{
    template<>
    const char* CML::NamedEnum
    <
        CML::outputFilterOutputControl::outputControls,
        2
    >::names[] =
    {
        "timeStep",
        "outputTime"
    };
}

const CML::NamedEnum<CML::outputFilterOutputControl::outputControls, 2>
    CML::outputFilterOutputControl::outputControlNames_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::outputFilterOutputControl::outputFilterOutputControl
(
    const Time& t,
    const dictionary& dict
)
:
    time_(t),
    outputControl_(ocTimeStep),
    outputInterval_(0)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::outputFilterOutputControl::~outputFilterOutputControl()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::outputFilterOutputControl::read(const dictionary& dict)
{
    if (dict.found("outputControl"))
    {
        outputControl_ = outputControlNames_.read(dict.lookup("outputControl"));
    }
    else
    {
        outputControl_ = ocTimeStep;
    }

    switch (outputControl_)
    {
        case ocTimeStep:
        {
            outputInterval_ = dict.lookupOrDefault<label>("outputInterval", 0);
            break;
        }

        default:
        {
            // do nothing
            break;
        }
    }
}


bool CML::outputFilterOutputControl::output() const
{
    switch (outputControl_)
    {
        case ocTimeStep:
        {
            return
            (
                (outputInterval_ <= 1)
             || !(time_.timeIndex() % outputInterval_)
            );
            break;
        }

        case ocOutputTime:
        {
            return time_.outputTime();
            break;
        }

        default:
        {
            // this error should not actually be possible
            FatalErrorIn("bool CML::outputFilterOutputControl::output()")
                << "Undefined output control: "
                << outputControlNames_[outputControl_] << nl
                << abort(FatalError);
            break;
        }
    }

    return false;
}


// ************************************************************************* //
