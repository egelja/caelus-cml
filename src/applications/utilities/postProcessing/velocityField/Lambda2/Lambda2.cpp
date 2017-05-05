/*---------------------------------------------------------------------------* \
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
    Lambda2

Description
    Calculates and writes the second largest eigenvalue of the sum of the
    square of the symmetrical and anti-symmetrical parts of the velocity
    gradient tensor.

    The -noWrite option has no meaning.

\*---------------------------------------------------------------------------*/

#include "calc.hpp"
#include "fvc.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void CML::calc(const argList& args, const Time& runTime, const fvMesh& mesh)
{
    IOobject Uheader
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
    );

    if (Uheader.headerOk())
    {
        Info<< "    Reading U" << endl;
        volVectorField U(Uheader, mesh);

        const volTensorField gradU(fvc::grad(U));

        volTensorField SSplusWW
        (
            (symm(gradU) & symm(gradU)) + (skew(gradU) & skew(gradU))
        );

        volScalarField Lambda2
        (
            IOobject
            (
                "Lambda2",
                runTime.timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            -eigenValues(SSplusWW)().component(vector::Y)
        );

        Info<< "    Writing -Lambda2" << endl;
        Lambda2.write();
    }
    else
    {
        Info<< "    No U" << endl;
    }

    Info<< "\nEnd\n" << endl;
}


// ************************************************************************* //
