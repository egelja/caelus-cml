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

Global
    cfl.hpp

Description
    Compute the compressible CFL number

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

scalar CoNum = 0.0;
scalar meanCoNum = 0.0;

surfaceScalarField cSf
(
    linearInterpolate
    (
        sqrt(1/thermo.psi())
    )
    *
    mesh.magSf()
);

if (mesh.nInternalFaces())
{
    surfaceScalarField cSfbyDelta
    (
        mesh.surfaceInterpolation::deltaCoeffs()*cSf
    );

    scalarField sumPhi
    (
        fvc::surfaceSum(mag(phi))().internalField()
    );


    CoNum = max(cSfbyDelta/mesh.magSf()).value()*runTime.deltaTValue();

    CoNum += 0.5*gMax(sumPhi/(mesh.V().field()*thermo.rho()))
        *runTime.deltaTValue();

    meanCoNum =
        (sum(cSfbyDelta)/sum(mesh.magSf())).value()
       *runTime.deltaTValue();
    meanCoNum += 
        0.5*(gSum(sumPhi)/gSum(mesh.V().field()))*runTime.deltaTValue();

}

Info<< "Mean and max Courant Numbers = "
    << meanCoNum << " " << CoNum << endl;


