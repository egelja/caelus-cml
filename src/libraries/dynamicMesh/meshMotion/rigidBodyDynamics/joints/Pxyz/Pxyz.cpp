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

#include "Pxyz.hpp"
#include "rigidBodyModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace joints
{
    defineTypeNameAndDebug(Pxyz, 0);

    addToRunTimeSelectionTable
    (
        joint,
        Pxyz,
        dictionary
    );
}
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::RBD::joints::Pxyz::Pxyz()
:
    joint(3)
{
    S_[0] = spatialVector(0, 0, 0, 1, 0, 0);
    S_[1] = spatialVector(0, 0, 0, 0, 1, 0);
    S_[2] = spatialVector(0, 0, 0, 0, 0, 1);
}


CML::RBD::joints::Pxyz::Pxyz(const dictionary& dict)
:
    joint(3)
{
    S_[0] = spatialVector(0, 0, 0, 1, 0, 0);
    S_[1] = spatialVector(0, 0, 0, 0, 1, 0);
    S_[2] = spatialVector(0, 0, 0, 0, 0, 1);
}


CML::autoPtr<CML::RBD::joint> CML::RBD::joints::Pxyz::clone() const
{
    return autoPtr<joint>(new Pxyz(*this));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::joints::Pxyz::~Pxyz()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void CML::RBD::joints::Pxyz::jcalc
(
    joint::XSvc& J,
    const scalarField& q,
    const scalarField& qDot
) const
{
    J.X.E() = tensor::I;
    J.X.r() = q.block<vector>(qIndex_);

    J.S = Zero;
    J.S(3,0) = 1;
    J.S(4,1) = 1;
    J.S(5,2) = 1;

    J.v = spatialVector(Zero, qDot.block<vector>(qIndex_));
    J.c = Zero;
}


// ************************************************************************* //
