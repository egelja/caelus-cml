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

#include "rpm.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "mathematicalConstants.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    namespace SRF
    {
        defineTypeNameAndDebug(rpm, 0);

        addToRunTimeSelectionTable
        (
            SRFModel,
            rpm,
            dictionary
        );
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::SRF::rpm::rpm
(
    const volVectorField& U
)
:
    SRFModel(typeName, U),
    rpm_(readScalar(SRFModelCoeffs_.lookup("rpm")))
{
    // Initialise the angular velocity
    omega_.value() = axis_*rpm_*constant::mathematical::twoPi/60.0;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::SRF::rpm::~rpm()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::SRF::rpm::read()
{
    if (SRFModel::read())
    {
        // Re-read rpm
        SRFModelCoeffs_.lookup("rpm") >> rpm_;

        // Update angular velocity
        omega_.value() = axis_*rpm_*(constant::mathematical::twoPi/60.0);

        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
