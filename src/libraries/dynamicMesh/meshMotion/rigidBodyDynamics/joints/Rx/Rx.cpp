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

#include "Rx.hpp"
#include "rigidBodyModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace joints
{
    defineTypeNameAndDebug(Rx, 0);

    addToRunTimeSelectionTable
    (
        joint,
        Rx,
        dictionary
    );
}
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::joints::Rx::Rx()
:
    joint(1)
{
    S_[0] = spatialVector(1, 0, 0, 0, 0, 0);
}


CML::RBD::joints::Rx::Rx(const dictionary& dict)
:
    joint(1)
{
    S_[0] = spatialVector(1, 0, 0, 0, 0, 0);
}


CML::autoPtr<CML::RBD::joint> CML::RBD::joints::Rx::clone() const
{
    return autoPtr<joint>(new Rx(*this));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::joints::Rx::~Rx()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::RBD::joints::Rx::jcalc
(
    joint::XSvc& J,
    const scalarField& q,
    const scalarField& qDot
) const
{
    J.X = Xrx(q[qIndex_]);
    J.S1 = S_[0];
    J.v = Zero;
    J.v.wx() = qDot[qIndex_];
    J.c = Zero;
}


// ************************************************************************* //
