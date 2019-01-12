/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "nullJoint.hpp"
#include "rigidBodyModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace joints
{
    defineTypeNameAndDebug(null, 0);

    addToRunTimeSelectionTable
    (
        joint,
        null,
        dictionary
    );
}
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::joints::null::null()
:
    joint(0)
{}


CML::RBD::joints::null::null(const dictionary& dict)
:
    joint(0)
{}


CML::autoPtr<CML::RBD::joint> CML::RBD::joints::null::clone() const
{
    return autoPtr<joint>(new null(*this));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::joints::null::~null()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::RBD::joints::null::jcalc
(
    joint::XSvc& J,
    const scalarField& q,
    const scalarField& qDot
) const
{
    FatalErrorInFunction
        << "Cannot calculate the state of a null-joint"
        << abort(FatalError);
}


// ************************************************************************* //
