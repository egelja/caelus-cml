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

#include "TPS.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(TPS, 0);
    addToRunTimeSelectionTable(RBFFunction, TPS, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::TPS::TPS(const scalar radius)
:
    RBFFunction(),
    radius_(radius)
{}


CML::TPS::TPS(const dictionary& dict)
:
    RBFFunction(),
    radius_(readScalar(dict.lookup("radius")))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::TPS::~TPS()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::scalarField> CML::TPS::weights
(
    const vectorField& points,
    const vector& controlPoint
) const
{
    scalarField dist(mag(points - controlPoint));
    scalarField RBF(dist.size());

    forAll(RBF, i)
    {
        if (dist[i] > SMALL)
        {
            RBF[i] = sqr(dist[i])*log(dist[i]);
        }
        else
        {
            RBF[i] = 0.0;
        }
    }

    return RBF;
}


// ************************************************************************* //
