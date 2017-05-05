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

#include "Ra.hpp"
#include "rigidBodyModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace joints
{
    defineTypeNameAndDebug(Ra, 0);

    addToRunTimeSelectionTable
    (
        joint,
        Ra,
        dictionary
    );
}
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::joints::Ra::Ra(const vector& axis)
:
    joint(1)
{
    S_[0] = spatialVector(axis/mag(axis), Zero);
}


CML::RBD::joints::Ra::Ra(const dictionary& dict)
:
    joint(1)
{
    vector axis(dict.lookup("axis"));
    S_[0] = spatialVector(axis/mag(axis), Zero);
}


CML::autoPtr<CML::RBD::joint> CML::RBD::joints::Ra::clone() const
{
    return autoPtr<joint>(new Ra(*this));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::joints::Ra::~Ra()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::RBD::joints::Ra::jcalc
(
    joint::XSvc& J,
    const scalarField& q,
    const scalarField& qDot
) const
{
    J.X = Xr(S_[0].w(), q[qIndex_]);
    J.S1 = S_[0];
    J.v = S_[0]*qDot[qIndex_];
    J.c = Zero;
}


void CML::RBD::joints::Ra::write(Ostream& os) const
{
    joint::write(os);
    os.writeKeyword("axis")
        << S_[0].w() << token::END_STATEMENT << nl;
}


// ************************************************************************* //
