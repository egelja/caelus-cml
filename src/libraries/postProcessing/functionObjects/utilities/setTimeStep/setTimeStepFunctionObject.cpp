/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2015 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "setTimeStepFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(setTimeStepFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        setTimeStepFunctionObject,
        dictionary
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::setTimeStepFunctionObject::setTimeStepFunctionObject
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    functionObject(name),
    time_(runTime),
    enabled_(true)
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::setTimeStepFunctionObject::on()
{
    enabled_ = true;
}


void CML::setTimeStepFunctionObject::off()
{
    enabled_ = false;
}


bool CML::setTimeStepFunctionObject::start()
{
    return true;
}


bool CML::setTimeStepFunctionObject::execute(const bool forceWrite)
{
    return true;
}


bool CML::setTimeStepFunctionObject::end()
{
    return true;
}


bool CML::setTimeStepFunctionObject::timeSet()
{
    return true;
}


bool CML::setTimeStepFunctionObject::adjustTimeStep()
{
    if (enabled())
    {
        // Wanted timestep
        scalar newDeltaT = timeStepPtr_().value(time_.timeOutputValue());

        const_cast<Time&>(time()).setDeltaT(newDeltaT, false);

        return true;
    }
    else
    {
        return false;
    }
}


bool CML::setTimeStepFunctionObject::read(const dictionary& dict)
{
    enabled_ = dict.lookupOrDefault("enabled", true);

    if (enabled_)
    {
        timeStepPtr_ = DataEntry<scalar>::New("deltaT", dict);

        // Check that time has adjustTimeStep
        const dictionary& controlDict = time_.controlDict();

        Switch adjust;
        if
        (
            !controlDict.readIfPresent<Switch>("adjustTimeStep", adjust)
         || !adjust
        )
        {
            FatalIOErrorInFunction(dict)
                << "Need to have 'adjustTimeStep' true to enable external"
                << " timestep control" << exit(FatalIOError);
        }
    }
    return true;
}


void CML::setTimeStepFunctionObject::updateMesh(const mapPolyMesh& mpm)
{}


void CML::setTimeStepFunctionObject::movePoints(const pointField& mesh)
{}


// ************************************************************************* //
