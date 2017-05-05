/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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

#include "fixedTrim.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "unitConversion.hpp"
#include "mathematicalConstants.hpp"

using namespace CML::constant;

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(fixedTrim, 0);

    addToRunTimeSelectionTable(trimModel, fixedTrim, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fixedTrim::fixedTrim
(
    const fv::rotorDiskSource& rotor,
    const dictionary& dict
)
:
    trimModel(rotor, dict, typeName),
    thetag_(rotor.cells().size(), 0.0)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

CML::fixedTrim::~fixedTrim()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::fixedTrim::read(const dictionary& dict)
{
    trimModel::read(dict);

    scalar theta0 = degToRad(readScalar(coeffs_.lookup("theta0")));
    scalar theta1c = degToRad(readScalar(coeffs_.lookup("theta1c")));
    scalar theta1s = degToRad(readScalar(coeffs_.lookup("theta1s")));

    const List<point>& x = rotor_.x();
    forAll(thetag_, i)
    {
        scalar psi = x[i].y();
        thetag_[i] = theta0 + theta1c*cos(psi) + theta1s*sin(psi);
    }
}


CML::tmp<CML::scalarField> CML::fixedTrim::thetag() const
{
    return tmp<scalarField>(thetag_);
}


void CML::fixedTrim::correct
(
    const vectorField& U,
    vectorField& force
)
{
    // do nothing
}


void CML::fixedTrim::correct
(
    const volScalarField rho,
    const vectorField& U,
    vectorField& force)
{
    // do nothing
}


// ************************************************************************* //
