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

#include "relaxationSchemeSpatial.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationSchemes
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(relaxationSchemeSpatial, 0);
addToRunTimeSelectionTable
(
    relaxationScheme,
    relaxationSchemeSpatial,
    dictionary
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

relaxationSchemeSpatial::relaxationSchemeSpatial
(
    const word& subDictName,
    const fvMesh& mesh,
    vectorField& U,
    scalarField& alpha
)
:
    relaxationScheme(subDictName, mesh, U, alpha),
    exponent_(coeffDict_.lookupOrDefault<scalar>("alphaExponent",3.5))
{
    const scalarField sigma(relaxShape_->sigma());

    weight_.setSize(sigma.size());

    forAll (weight_, celli)
    {
        weight_[celli] = 1.0 -
            (CML::exp(CML::pow(sigma[celli],exponent_)) - 1.0)
            /(CML::exp(1.0) - 1.0);
    }
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void relaxationSchemeSpatial::correct()
{
    // Obtain relaxation zone cells and local sigma coordinate
    // The number of cells and the sigma coordinate can have changed
    // for dynamic meshes
    const labelList& cells = relaxShape_->cells();
    const scalarField& sigma = relaxShape_->sigma();

    // Compute the relaxation weights - only changes for moving/changing meshes
    if (weight_.size() != sigma.size())
    {
    	weight_.clear();
        weight_.setSize(sigma.size(), 0.0);
        relaxWeight_->weights(cells, sigma, weight_);

        alphaTarget_.clear();
        alphaTarget_.setSize(cells.size(), 0.0);

        UTarget_.clear();
        UTarget_.setSize(cells.size(), vector::zero);
    }

    // Perform the correction
    const scalarField& V(mesh_.V());
    const vectorField& C(mesh_.C());
    const cellList& cc(mesh_.cells());
    const pointField& pp(mesh_.points());
    const faceList& fL(mesh_.faces());

    forAll (cells, celli)
    {
        const label cellNo = cells[celli];
        const pointField& p = cc[cellNo].points(fL, pp);

        // Evaluate the cell height and the signedDistance to the surface from
        // the cell centre
        scalar cellHeight
            (
                CML::max(p & waveProps_->returnDir())
              - CML::min(p & waveProps_->returnDir())
            );
        scalar sdc(signedPointToSurfaceDistance(C[cellNo]));

        // Target variables
        alphaTarget_[celli]= 0.0;
        UTarget_[celli] = waveProps_->windVelocity(mesh_.time().value());

        // Only do cutting, if surface is close by
        if (CML::mag(sdc) <= 2*cellHeight)
        {
            localCellNeg lc(mesh_, cellNo);

            dividePolyhedral(point::zero, vector::one, lc);

            if (lc.ccNeg().size() >= 4)
            {
                UTarget_[celli] = waveProps_->U(lc.centreNeg(), mesh_.time().value());
                alphaTarget_[celli] = lc.magNeg()/V[cellNo];
            }
        }
        else if (sdc < 0)
        {
            alphaTarget_[celli] = 1.0;
            UTarget_[celli] = waveProps_->U(C[cellNo], mesh_.time().value());
        }
        else
        {
            alphaTarget_[celli] = 0.0;
            UTarget_[celli] = waveProps_->windVelocity(mesh_.time().value());
        }

        U_[cellNo] = (1 - weight_[celli])*UTarget_[celli] + weight_[celli]*U_[cellNo];

        alpha_[cellNo] = (1 - weight_[celli])*alphaTarget_[celli]
            + weight_[celli]*alpha_[cellNo];
    }

    // REMEMBER NOT TO PUT correctBoundaryConditions() HERE BUT ONE LEVEL UP
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationSchemes
} // End namespace CML

// ************************************************************************* //
