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


// ************************************************************************* //
