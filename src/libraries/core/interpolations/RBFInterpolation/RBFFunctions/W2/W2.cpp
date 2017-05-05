/*---------------------------------------------------------------------------*\
Copyright 2009 TU Delft
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "W2.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(W2, 0);
    addToRunTimeSelectionTable(RBFFunction, W2, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::W2::W2(const scalar radius)
:
    RBFFunction(),
    radius_(radius)
{}


CML::W2::W2(const dictionary& dict)
:
    RBFFunction(),
    radius_(readScalar(dict.lookup("radius")))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::W2::~W2()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::scalarField> CML::W2::weights
(
    const vectorField& points,
    const vector& controlPoint
) const
{
    scalarField dist(mag(points - controlPoint));

    scalarField RBF(dist.size());

    RBF = neg(dist - radius_)*
        CML::max(pow4(1 - (dist/radius_)), scalar(0))*(1 + 4*(dist/radius_));

    return RBF;
}

// ************************************************************************* //
