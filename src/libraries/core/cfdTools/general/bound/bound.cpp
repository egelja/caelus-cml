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

\*---------------------------------------------------------------------------*/

#include "bound.hpp"
#include "volFields.hpp"
#include "fvc.hpp"

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

CML::volScalarField&
CML::bound(volScalarField& vsf, const dimensionedScalar& lowerBound)
{
    const scalar minVsf = min(vsf).value();

    if (minVsf < lowerBound.value())
    {
        Info<< "bounding " << vsf.name()
            << ", min: " << minVsf
            << " max: " << max(vsf).value()
            << " average: " << gAverage(vsf.internalField())
            << endl;

        vsf.internalField() = max
        (
            max
            (
                vsf.internalField(),
                fvc::average(max(vsf, lowerBound))().internalField()
              * pos(-vsf.internalField())
            ),
            lowerBound.value()
        );

        vsf.boundaryField() = max(vsf.boundaryField(), lowerBound.value());
    }

    return vsf;
}


void CML::boundMinMax
(
    volScalarField& vsf,
    const dimensionedScalar& vsf0,
    const dimensionedScalar& vsf1
)
{
    scalar minVsf = min(vsf).value();
    scalar maxVsf = max(vsf).value();

    if (minVsf < vsf0.value() || maxVsf > vsf1.value())
    {
        Info<< "bounding " << vsf.name()
            << ", min: " << gMin(vsf.internalField())
            << " max: " << gMax(vsf.internalField())
            << " average: " << gAverage(vsf.internalField())
            << endl;
    }

    if (minVsf < vsf0.value())
    {
        vsf.internalField() = max
        (
            max
            (
                vsf.internalField(),
                fvc::average(max(vsf, vsf0))().internalField()
                *pos(vsf0.value() - vsf.internalField())
            ),
            vsf0.value()
        );

        vsf.correctBoundaryConditions();
        vsf.boundaryField() = max(vsf.boundaryField(), vsf0.value());
    }

    if (maxVsf > vsf1.value())
    {
        vsf.internalField() = min
        (
            min
            (
                vsf.internalField(),
                fvc::average(min(vsf, vsf1))().internalField()
                *neg(vsf1.value() - vsf.internalField())
                // This is needed when all values are above max
              + pos(vsf1.value() - vsf.internalField())*vsf1.value()
            ),
            vsf1.value()
        );

        vsf.correctBoundaryConditions();
        vsf.boundaryField() = min(vsf.boundaryField(), vsf1.value());
    }
}

// ************************************************************************* //
