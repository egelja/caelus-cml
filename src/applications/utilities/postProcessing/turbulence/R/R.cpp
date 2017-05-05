/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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

Application
    R

Description
    Calculates and writes the Reynolds stress R for the current time step.

\*---------------------------------------------------------------------------*/

#include "calc.hpp"
#include "fvCFD.hpp"
#include "incompressible/singlePhaseTransportModel/singlePhaseTransportModel.hpp"
#include "RASModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void CML::calc(const argList& args, const Time& runTime, const fvMesh& mesh)
{
    #include "createFields.hpp"

    Info<< "\nCalculating the Reynolds Stress R\n" << endl;

    volSymmTensorField R
    (
        IOobject
        (
            "R",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        RASModel->R()
    );

    R.write();

    Info<< "End" << endl;
}


// ************************************************************************* //
