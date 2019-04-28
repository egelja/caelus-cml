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

InClass
    barotropicCompressibilityModel

\*---------------------------------------------------------------------------*/

#include "barotropicCompressibilityModel.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(barotropicCompressibilityModel, 0);
    defineRunTimeSelectionTable(barotropicCompressibilityModel, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::barotropicCompressibilityModel::barotropicCompressibilityModel
(
    const dictionary& compressibilityProperties,
    const volScalarField& gamma,
    const word& psiName
)
:
    compressibilityProperties_(compressibilityProperties),
    psi_
    (
        IOobject
        (
            psiName,
            gamma.mesh().time().timeName(),
            gamma.mesh()
        ),
        gamma.mesh(),
        dimensionedScalar(psiName, dimensionSet(0, -2, 2, 0, 0), 0)
    ),
    gamma_(gamma)
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool CML::barotropicCompressibilityModel::read
(
    const dictionary& compressibilityProperties
)
{
    compressibilityProperties_ = compressibilityProperties;

    return true;
}
