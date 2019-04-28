/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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

#include "simplecControl.hpp"
#include "Time.hpp"

namespace CML
{
    defineTypeNameAndDebug(simplecControl, 0);
}

void CML::simplecControl::read()
{
    solutionControl::read(true);
}

bool CML::simplecControl::criteriaSatisfied()
{
    if (residualControl_.empty())
    {
        return false;
    }

    bool achieved = true;
    bool checked = false;

    const dictionary& solverDict = mesh_.solverPerformanceDict();
    forAllConstIter(dictionary, solverDict, iter)
    {
        const word& variableName = iter().keyword();
        const label fieldI = applyToField(variableName);
        if (fieldI != -1)
        {
            const List<solverPerformance> sp(iter().stream());
            const scalar residual = sp.first().initialResidual();

            checked = true;

            bool absCheck = residual < residualControl_[fieldI].absTol;
            achieved = achieved && absCheck;

            if (debug)
            {
                Info<< algorithmName_ << " solution statistics:" << endl;

                Info<< "    " << variableName << ": tolerance = " << residual
                    << " (" << residualControl_[fieldI].absTol << ")"
                    << endl;
            }
        }
    }

    return checked && achieved;
}

CML::simplecControl::simplecControl(fvMesh& mesh)
:
    solutionControl(mesh, "SIMPLEC"),
    initialised_(false)
{
    read();

    Info<< nl;

    if (residualControl_.empty())
    {
        Info<< algorithmName_ << ": no convergence criteria found. "
            << "Calculations will run for " << mesh_.time().endTime().value()
            << " steps." << nl << endl;
    }
    else
    {
        Info<< algorithmName_ << ": convergence criteria" << nl;
        forAll(residualControl_, i)
        {
            Info<< "    field " << residualControl_[i].name << token::TAB
                << " tolerance " << residualControl_[i].absTol
                << nl;
        }
        Info<< endl;
    }
}

CML::simplecControl::~simplecControl()
{}

bool CML::simplecControl::loop()
{
    read();

    Time& time = const_cast<Time&>(mesh_.time());

    if (initialised_)
    {
        if (criteriaSatisfied())
        {
            Info<< nl << algorithmName_ << " solution converged in "
                << time.timeName() << " iterations" << nl << endl;

            // Set to finalise calculation
            time.writeAndEnd();
        }
        else
        {
            storePrevIterFields();
        }
    }
    else
    {
        initialised_ = true;
        storePrevIterFields();
    }

    return time.loop();
}

