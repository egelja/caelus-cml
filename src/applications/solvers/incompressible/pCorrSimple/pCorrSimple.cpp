/*---------------------------------------------------------------------------*\
Copyright (C) 2016 Applied CCM
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

    corrSimple

Description

    Steady-state solver for incompressible turbulent flows based on
    SIMPLE algorithm and pressure correction variable

References

    [1] "Numerical Heat Transfer and Fluid Flow", S.V. Patankar, Hemisphere
        Publishing Corporation, 1980, USA

    [2] "Computational Methods for FLuid Dynamics", J.H. Ferziger, M. Peric,
        Springler-Verlag, 2002

    [3] "The Finite Volume Method in Computational Fluid Dynamics",
        F. Moukalled, L. Mangani, M.Darwish, Springer, 2016

Author
    Aleksandar Jemcov
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "singlePhaseTransportModel.hpp"
#include "RASModel.hpp"
#include "simpleControl.hpp"
#include "fvIOoptionList.hpp"
#include "orthogonalSnGrad.hpp"


int main(int argc, char *argv[])
{
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    #include "createMesh.hpp"
    #include "createFields.hpp"
    #include "createFvOptions.hpp"
    #include "initContinuityErrs.hpp"

    simpleControl simple(mesh);

    Info<< "\nStarting time loop\n" << endl;

    while (simple.loop())
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

        phi.storePrevIter();
        Uf.storePrevIter();
        
        {
            #include "UEqn.hpp"
            #include "pEqn.hpp"
        }

        turbulence->correct();

        runTime.write();

        #include "reportTimeStats.hpp"            
    }

    Info<< "End\n" << endl;

    return 0;
}



