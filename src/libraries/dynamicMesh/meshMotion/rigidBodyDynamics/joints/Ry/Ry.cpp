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

#include "Ry.hpp"
#include "rigidBodyModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace joints
{
    defineTypeNameAndDebug(Ry, 0);

    addToRunTimeSelectionTable
    (
        joint,
        Ry,
        dictionary
    );
}
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::joints::Ry::Ry()
:
    joint(1)
{
    S_[0] = spatialVector(0, 1, 0, 0, 0, 0);
}


CML::RBD::joints::Ry::Ry(const dictionary& dict)
:
    joint(1)
{
    S_[0] = spatialVector(0, 1, 0, 0, 0, 0);
}


CML::autoPtr<CML::RBD::joint> CML::RBD::joints::Ry::clone() const
{
    return autoPtr<joint>(new Ry(*this));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::joints::Ry::~Ry()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::RBD::joints::Ry::jcalc
(
    joint::XSvc& J,
    const scalarField& q,
    const scalarField& qDot
) const
{
    J.X = Xry(q[qIndex_]);
    J.S1 = S_[0];
    J.v = Zero;
    J.v.wy() = qDot[qIndex_];
    J.c = Zero;
}


// ************************************************************************* //
