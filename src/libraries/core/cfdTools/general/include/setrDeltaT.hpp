{
    const dictionary& pimpleDict = pimple.dict();

    scalar maxCo
    (
        pimpleDict.lookupOrDefault<scalar>("maxCo", 0.8)
    );

    scalar rDeltaTSmoothingCoeff
    (
        pimpleDict.lookupOrDefault<scalar>("rDeltaTSmoothingCoeff", 0.02)
    );

    scalar rDeltaTDampingCoeff
    (
        pimpleDict.lookupOrDefault<scalar>("rDeltaTDampingCoeff", 1.0)
    );

    scalar maxDeltaT
    (
        pimpleDict.lookupOrDefault<scalar>("maxDeltaT", GREAT)
    );

    volScalarField rDeltaT0("rDeltaT0", rDeltaT);

    // Set the reciprocal time-step from the local Courant number
    rDeltaT.dimensionedInternalField() = max
    (
        1/dimensionedScalar("maxDeltaT", dimTime, maxDeltaT),
        fvc::surfaceSum(mag(phi))().dimensionedInternalField()
       /((2*maxCo)*mesh.V()*rho.dimensionedInternalField())
    );

    if (pimple.transonic())
    {
        surfaceScalarField phid
        (
            "phid",
            fvc::interpolate(psi)*(fvc::interpolate(U) & mesh.Sf())
        );

        rDeltaT.dimensionedInternalField() = max
        (
            rDeltaT.dimensionedInternalField(),
            fvc::surfaceSum(mag(phid))().dimensionedInternalField()
            /((2*maxCo)*mesh.V()*psi.dimensionedInternalField())
        );
    }

    // Update tho boundary values of the reciprocal time-step
/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
Copyright (C) 2011 OpenCFD
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
    rDeltaT.correctBoundaryConditions();

    Info<< "Flow time scale min/max = "
        << gMin(1/rDeltaT.internalField())
        << ", " << gMax(1/rDeltaT.internalField()) << endl;

    if (rDeltaTSmoothingCoeff < 1.0)
    {
        for (label i = 0; i < 1; ++i)
	{
            fvc::smooth(rDeltaT, rDeltaTSmoothingCoeff);
	}
    }

    Info<< "Smoothed flow time scale min/max = "
        << gMin(1/rDeltaT.internalField())
        << ", " << gMax(1/rDeltaT.internalField()) << endl;

    // Limit rate of change of time scale
    // - reduce as much as required
    // - only increase at a fraction of old time scale
    if
    (
        rDeltaTDampingCoeff < 1.0
     && runTime.timeIndex() > runTime.startTimeIndex() + 1
    )
    {
        rDeltaT =
            rDeltaT0
           *max(rDeltaT/rDeltaT0, scalar(1) - rDeltaTDampingCoeff);

        Info<< "Damped flow time scale min/max = "
            << gMin(1/rDeltaT.internalField())
            << ", " << gMax(1/rDeltaT.internalField()) << endl;
    }
}
