/*---------------------------------------------------------------------------*\
Copyright (C) 2020 Applied CCM Pty Ltd
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

Application
    Test-rhoReactionThermo

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "rhoReactionThermo.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    #include "createMesh.hpp"

    Info<< "Reading thermophysical properties\n" << endl;
    autoPtr<rhoReactionThermo> pThermo(rhoReactionThermo::New(mesh));
    rhoReactionThermo& thermo = pThermo();

    volScalarField rho
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        thermo.rho()
    );

    Info<< "Mixture density = "
        << rho.weightedAverage(mesh.Vsc()).value()
        << "  Min(" << rho.name() << ") = " << min(rho).value()
        << "  Max(" << rho.name() << ") = " << max(rho).value()
        << endl;

    runTime++;
    Info<< "Time = " << runTime.timeName() << nl << endl;
    
    
    runTime.write();
    Info<< "End\n" << endl;

    return 0;
}
// ************************************************************************* //
