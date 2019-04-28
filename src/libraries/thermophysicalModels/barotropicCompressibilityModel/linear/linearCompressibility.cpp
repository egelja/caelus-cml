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

#include "linearCompressibility.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    namespace compressibilityModels
    {
        defineTypeNameAndDebug(linear, 0);
        addToRunTimeSelectionTable
        (
            barotropicCompressibilityModel,
            linear,
            dictionary
        );
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::compressibilityModels::linear::linear
(
    const dictionary& compressibilityProperties,
    const volScalarField& gamma,
    const word& psiName
)
:
    barotropicCompressibilityModel(compressibilityProperties, gamma, psiName),
    psiv_
    (
        "psiv",
        dimCompressibility,
        compressibilityProperties_.lookup("psiv")
    ),
    psil_
    (
        "psil",
        dimCompressibility,
        compressibilityProperties_.lookup("psil")
    )
{
    correct();
    psi_.oldTime();
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::compressibilityModels::linear::correct()
{
    psi_ = gamma_*psiv_ + (scalar(1) - gamma_)*psil_;
}


bool CML::compressibilityModels::linear::read
(
    const dictionary& compressibilityProperties
)
{
    barotropicCompressibilityModel::read(compressibilityProperties);

    compressibilityProperties_.lookup("psiv") >> psiv_;
    compressibilityProperties_.lookup("psil") >> psil_;

    return true;
}
