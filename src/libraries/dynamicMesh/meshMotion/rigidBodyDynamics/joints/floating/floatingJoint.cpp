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

#include "floatingJoint.hpp"
#include "rigidBodyModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "Rs.hpp"
#include "Rzyx.hpp"
#include "Pxyz.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace joints
{
    defineTypeNameAndDebug(floating, 0);

    addToRunTimeSelectionTable
    (
        joint,
        floating,
        dictionary
    );
}
}
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

CML::autoPtr<CML::RBD::joints::composite>
CML::RBD::joints::floating::sixDoF()
{
    PtrList<joint> cj(2);
    cj.set(0, new joints::Pxyz());

    // The quaternion-based spherical joint could be used
    // but then w must be set appropriately
    //cj.set(1, new joints::Rs());

    // Alternatively the Euler-angle joint can be used
    cj.set(1, new joints::Rzyx());

    return autoPtr<composite>(new composite(cj));
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::joints::floating::floating()
:
    composite(sixDoF())
{}


CML::RBD::joints::floating::floating(const dictionary& dict)
:
    composite(sixDoF())
{}


CML::autoPtr<CML::RBD::joint> CML::RBD::joints::floating::clone() const
{
    return autoPtr<joint>(new floating(*this));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::joints::floating::~floating()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::RBD::joints::floating::write(Ostream& os) const
{
    joint::write(os);
}


// ************************************************************************* //
