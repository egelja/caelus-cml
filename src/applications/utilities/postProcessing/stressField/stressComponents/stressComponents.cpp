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
    stressComponents

Description
    Calculates and writes the scalar fields of the six components of the stress
    tensor sigma for each time.

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "incompressible/singlePhaseTransportModel/singlePhaseTransportModel.hpp"
#include "zeroGradientFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();

#   include "setRootCase.hpp"
#   include "createTime.hpp"

    instantList timeDirs = timeSelector::select0(runTime, args);

#   include "createMesh.hpp"

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        Info<< "Time = " << runTime.timeName() << endl;

        IOobject Uheader
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        );

        // Check U exists
        if (Uheader.headerOk())
        {
            mesh.readUpdate();

            Info<< "    Reading U" << endl;
            volVectorField U(Uheader, mesh);

#           include "createPhi.hpp"

            singlePhaseTransportModel laminarTransport(U, phi);

            volSymmTensorField sigma
            (
                IOobject
                (
                    "sigma",
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                laminarTransport.nu()*2*dev(symm(fvc::grad(U)))
            );


            volScalarField sigmaxx
            (
                IOobject
                (
                    "sigmaxx",
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ
                ),
                sigma.component(symmTensor::XX)
            );
            sigmaxx.write();

            volScalarField sigmayy
            (
                IOobject
                (
                    "sigmayy",
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ
                ),
                sigma.component(symmTensor::YY)
            );
            sigmayy.write();

            volScalarField sigmazz
            (
                IOobject
                (
                    "sigmazz",
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ
                ),
                sigma.component(symmTensor::ZZ)
            );
            sigmazz.write();

            volScalarField sigmaxy
            (
                IOobject
                (
                    "sigmaxy",
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ
                ),
                sigma.component(symmTensor::XY)
            );
            sigmaxy.write();

            volScalarField sigmaxz
            (
                IOobject
                (
                    "sigmaxz",
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ
                ),
                sigma.component(symmTensor::XZ)
            );
            sigmaxz.write();

            volScalarField sigmayz
            (
                IOobject
                (
                    "sigmayz",
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ
                ),
                sigma.component(symmTensor::YZ)
            );
            sigmayz.write();

            volVectorField Ub
            (
                IOobject
                (
                    "Ub",
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ
                ),
                U,
                zeroGradientFvPatchVectorField::typeName
            );
            Ub.correctBoundaryConditions();
            Ub.write();

            volScalarField sigmaUn
            (
                IOobject
                (
                    "sigmaUn",
                    runTime.timeName(),
                    mesh,
                    IOobject::NO_READ
                ),
                0.0*sigma.component(symmTensor::YZ)
            );

            forAll(sigmaUn.boundaryField(), patchI)
            {
                sigmaUn.boundaryField()[patchI] =
                (
                    mesh.boundary()[patchI].nf()
                  & sigma.boundaryField()[patchI]
                )().component(vector::X);
            }

            sigmaUn.write();
        }
        else
        {
            Info<< "    No U" << endl;
        }

        Info<< endl;
    }

    Info<< "End" << endl;

    return 0;
}


// ************************************************************************* //
