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

#include "multiMotion.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace solidBodyMotionFunctions
{
    defineTypeNameAndDebug(multiMotion, 0);
    addToRunTimeSelectionTable
    (
        solidBodyMotionFunction,
        multiMotion,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidBodyMotionFunctions::multiMotion::multiMotion
(
    const dictionary& SBMFCoeffs,
    const Time& runTime
)
:
    solidBodyMotionFunction(SBMFCoeffs, runTime)
{
    read(SBMFCoeffs);
}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

CML::solidBodyMotionFunctions::multiMotion::~multiMotion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::septernion
CML::solidBodyMotionFunctions::multiMotion::transformation() const
{
    scalar t = time_.value();

    septernion TR = SBMFs_[0].transformation();

    for (label i = 1; i < SBMFs_.size(); i++)
    {
        TR *= SBMFs_[i].transformation();
    }

    Info<< "solidBodyMotionFunctions::multiMotion::transformation(): "
        << "Time = " << t << " transformation: " << TR << endl;

    return TR;
}


bool CML::solidBodyMotionFunctions::multiMotion::read
(
    const dictionary& SBMFCoeffs
)
{
    solidBodyMotionFunction::read(SBMFCoeffs);

    label i = 0;
    SBMFs_.setSize(SBMFCoeffs_.size());

    forAllConstIter(IDLList<entry>, SBMFCoeffs_, iter)
    {
        if (iter().isDict())
        {
            SBMFs_.set
            (
                i,
                solidBodyMotionFunction::New(iter().dict(), time_)
            );

            Info<< "Constructed SBMF " << i << " : "
                << iter().keyword() << " of type "
                << SBMFs_[i].type() << endl;

            i++;
        }
    }
    SBMFs_.setSize(i);

    return true;
}

// ************************************************************************* //
