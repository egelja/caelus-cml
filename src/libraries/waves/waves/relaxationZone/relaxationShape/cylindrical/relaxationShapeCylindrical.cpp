/*---------------------------------------------------------------------------*\
Copyright held by original author
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

#include "relaxationShapeCylindrical.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace relaxationShapes
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(relaxationShapeCylindrical, 0);
addToRunTimeSelectionTable
(
    relaxationShape,
    relaxationShapeCylindrical,
    dictionary
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


relaxationShapeCylindrical::relaxationShapeCylindrical
(
    const word& subDictName,
    const fvMesh& mesh_
)
:
    relaxationShape(subDictName, mesh_),

    centre_(vector(coeffDict_.lookup("centre"))),
    rInner_(readScalar(coeffDict_.lookup("rInner"))),
    rOuter_(readScalar(coeffDict_.lookup("rOuter")))

{
    width_   = CML::mag(rOuter_ - rInner_);
    centre_ -= (centre_ & direction_)*direction_;

    // Find computational cells inside the relaxation-shape
    findComputationalCells();

    // Computate the sigma coordinate
    computeSigmaCoordinate();
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void relaxationShapeCylindrical::findComputationalCells()
{
    const vectorField& cc = mesh_.C();

    cells_.setSize(5000);
    label count(0);

    forAll (cc, celli)
    {
        if (insideZone(celli ))
        {
            cells_[count++] = celli;

            if (count == cells_.size())
            {
                cells_.setSize( static_cast<label>(count*1.1 ));
            }
        }
    }

    cells_.setSize(count);
}


void relaxationShapeCylindrical::computeSigmaCoordinate()
{
    const vectorField& C = mesh_.C();
    sigma_.setSize(cells_.size(), 0);

    forAll (cells_, celli)
    {
        vector cc(C[cells_[celli]]);
        cc -= ((cc & direction_ )*direction_);

        sigma_[celli]  = (CML::mag(cc - centre_) - rInner_)/width_;
    }
}


bool relaxationShapeCylindrical::insideZone
(
    const label& celli
) const
{
    bool inside(false);

    vector cc(mesh_.C()[celli]);
    cc -= (direction_ & cc)*direction_;

    scalar dist(CML::mag(cc - centre_));

    if (dist >= rInner_ && dist <= rOuter_)
    {
        inside = true;
    }

    return inside;
}


const pointField& relaxationShapeCylindrical::pointSet()
{
    NotImplemented;
}


scalar relaxationShapeCylindrical::interpolation
(
    const scalarField& source,
    const point& p0
) const
{
    NotImplemented;
    return 0.0;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationShapes
} // End namespace CML

// ************************************************************************* //
