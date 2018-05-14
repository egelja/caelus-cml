/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 - 2018 Applied CCM
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

    simpleFoam

Description

    Steady-state solver for incompressible, turbulent flow with option to use 
    either SIMPLE in primitive or correction pressure form or SIMPLEC 
    algorithm in primitive form.

References:

    [1] "Numerical Heat Transfer and Fluid Flow", S.V. Patankar, Hemisphere
        Publishing Corporation, 1980, USA

    [2] "Computational Methods for FLuid Dynamics", J.H. Ferziger, M. Peric,
        Springler-Verlag, 2002


    [3] "Enhancements of the SIMPLE Method for Predicting Incompressible Fluid 
        Flows", Van Doormal, J. P. and Raithby, G. D., Numerical Heat Transfer 
        Vol. 7, Iss. 2, p. 147-163, 1984

Author(s)

    Aleksandar Jemcov
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#include "fvCFD.hpp"
#include "singlePhaseTransportModel.hpp"
#include "RASModel.hpp"
#include "simpleControl.hpp"
#include "fvIOoptionList.hpp"
#include "orthogonalSnGrad.hpp"
#include "profiling.hpp"

int main(int argc, char *argv[])
{
    #include "setRootCase.hpp"
    #include "createTime.hpp"
    addProfiling(init, "Initialisation");
    #include "createMesh.hpp"

    simpleControl simple(mesh);    
    
    #include "createFields.hpp"
    #include "createMRF.hpp"
    #include "createFvOptions.hpp"
    #include "initContinuityErrs.hpp"

    endProfiling(init);

    Info<< "Solver variable formulation: ";
    simple.correctionForm() ?
        Info<< "correction form\n"
        :
        Info<< "primitive form\n"
            << endl;
    Info<< "Algorithm formulation: ";
    simple.consistent() ?
        Info<< "SIMPLEC\n"
        :
        Info<< "SIMPLE\n"
            << endl;

    Info<< "\nStarting time loop\n" << endl;

    while (simple.loop())
    {
        addProfiling(timeLoop, "Iteration");
        Info<< "Time = " << runTime.timeName() << nl << endl;

        if (simple.correctionForm())
        {
            phi.storePrevIter();
            Uf.storePrevIter();
        }

        // --- Pressure-velocity SIMPLE corrector
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



