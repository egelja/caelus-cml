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

Application
    wallHeatFlux

Description
    Calculates and writes the heat flux for all patches as the boundary field
    of a volScalarField and also prints the integrated flux for all wall
    patches.

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "solidThermo.hpp"
#include "compressibleRASModel.hpp"
#include "wallFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();
#   include "addRegionOption.hpp"
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    instantList timeDirs = timeSelector::select0(runTime, args);
    #include "createNamedMesh.hpp"

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);
        Info<< "Time = " << runTime.timeName() << endl;
        mesh.readUpdate();

        #include "createFields.hpp"

        surfaceScalarField heatFlux
        (
            fvc::interpolate(RASModel->alphaEff())*fvc::snGrad(h)
        );

        const surfaceScalarField::GeometricBoundaryField& patchHeatFlux =
            heatFlux.boundaryField();

        Info<< "\nWall heat fluxes [W]" << endl;
        forAll(patchHeatFlux, patchi)
        {
            if (isA<wallFvPatch>(mesh.boundary()[patchi]))
            {
                Info<< mesh.boundary()[patchi].name()
                    << " "
                    << gSum
                       (
                           mesh.magSf().boundaryField()[patchi]
                          *patchHeatFlux[patchi]
                       )
                    << endl;
            }
        }
        Info<< endl;

        volScalarField wallHeatFlux
        (
            IOobject
            (
                "wallHeatFlux",
                runTime.timeName(),
                mesh
            ),
            mesh,
            dimensionedScalar("wallHeatFlux", heatFlux.dimensions(), 0.0)
        );

        forAll(wallHeatFlux.boundaryField(), patchi)
        {
            wallHeatFlux.boundaryField()[patchi] = patchHeatFlux[patchi];
        }

        wallHeatFlux.write();
    }

    Info<< "End" << endl;

    return 0;
}

// ************************************************************************* //
