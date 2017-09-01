/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

#include "relaxationScheme.hpp"
#include "wallFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationSchemes
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(relaxationScheme, 0);
defineRunTimeSelectionTable(relaxationScheme, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

relaxationScheme::relaxationScheme
(
    const word& subDictName,
    const fvMesh& mesh,
    vectorField& U,
    scalarField& alpha,
    const bool empty
)
:
    IOdictionary
    (
        mesh.thisDb().lookupObject<IOobject>("waveProperties")
    ),
    convexPolyhedral(mesh, true),
    mesh_(mesh),
    U_(U),
    alpha_(alpha),
    alphaTarget_(0),
    UTarget_(0),
    coeffDict_(subDict(subDictName + "Coeffs").subDict("relaxationZone"))
{
    if (!empty)
    {
        relaxShape_  = relaxationShapes::relaxationShape::New(subDictName, mesh_);
        relaxWeight_ = relaxationWeights::relaxationWeight::New(subDictName, mesh_);
        waveProps_   = waveTheories::waveTheory::New(subDictName, mesh_);
        numBeach_    = numericalBeaches::numericalBeach::New(subDictName, mesh_ );

        // Update size of target fields and zero
        alphaTarget_.setSize(relaxShape_->cells().size(), 0.0);
        UTarget_.setSize(relaxShape_->cells().size(), vector::zero);
    }
}


relaxationScheme::~relaxationScheme()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void relaxationScheme::signedPointToSurfaceDistance
(
    const pointField& pp,
    scalarField& sd
)
{
    forAll (pp, pointi)
    {
        sd[pointi] = signedPointToSurfaceDistance(pp[pointi]);
    }
}


scalar relaxationScheme::signedPointToSurfaceDistance
(
    const point& pp
) const
{
    scalar temp = waveProps_->eta(pp, db().time().value());
    temp += (waveProps_->returnDir() & pp);
    temp *= -1.0;

    return temp;
}


void relaxationScheme::targetAlphaField
(
    volScalarField& targetAlphaField
)
{
    const labelList& cc = cells();

    forAll (cc, celli)
    {
        const label cellNo = cc[celli];

        targetAlphaField[cellNo] = alphaTarget_[celli];
    }
}


void relaxationScheme::targetVelocityField
(
    volVectorField& targetVelocityField
)
{
    const labelList& cc = cells();

    forAll (cc, celli)
    {
        const label cellNo = cc[celli];

        targetVelocityField[cellNo] = UTarget_[celli];
    }
}


void relaxationScheme::numericalBeach
(
    volScalarField& artVisc
)
{
    const labelList& cc(cells());
    const scalarField& ss(sigma());

    numBeach_->correct(cc, ss, artVisc);

}


void relaxationScheme::addSup(fvMatrix<vector>& eqn)
{
   // Do nothing
}


void relaxationScheme::constrain(fvMatrix<vector>& eqn)
{
   // Do nothing
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationSchemes
} // End namespace CML

// ************************************************************************* //
