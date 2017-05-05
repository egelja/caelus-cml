/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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

#include "degenerateMatcher.hpp"
#include "ListOps.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

CML::hexMatcher CML::degenerateMatcher::hex;
CML::wedgeMatcher CML::degenerateMatcher::wedge;
CML::prismMatcher CML::degenerateMatcher::prism;
CML::tetWedgeMatcher CML::degenerateMatcher::tetWedge;
CML::pyrMatcher CML::degenerateMatcher::pyr;
CML::tetMatcher CML::degenerateMatcher::tet;


CML::cellShape CML::degenerateMatcher::match
(
    const faceList& faces,
    const labelList& owner,
    const label cellI,
    const labelList& cellFaces
)
{
    // Recognize in order of assumed occurrence.

    if (hex.matchShape(false, faces, owner, cellI, cellFaces))
    {
        return cellShape(hex.model(), hex.vertLabels());
    }
    else if (tet.matchShape(false, faces, owner, cellI, cellFaces))
    {
        return cellShape(tet.model(), tet.vertLabels());
    }
    else if (prism.matchShape(false, faces, owner, cellI, cellFaces))
    {
        return cellShape(prism.model(), prism.vertLabels());
    }
    else if (pyr.matchShape(false, faces, owner, cellI, cellFaces))
    {
        return cellShape(pyr.model(), pyr.vertLabels());
    }
    else if (wedge.matchShape(false, faces, owner, cellI, cellFaces))
    {
        return cellShape(wedge.model(), wedge.vertLabels());
    }
    else if (tetWedge.matchShape(false, faces, owner, cellI, cellFaces))
    {
        return cellShape(tetWedge.model(), tetWedge.vertLabels());
    }
    else
    {
        return cellShape(*(cellModeller::lookup(0)), labelList(0));
    }
}


CML::cellShape CML::degenerateMatcher::match(const faceList& faces)
{
    // Do as if single cell mesh; all faces are referenced by a single cell

    return match
    (
        faces,
        labelList(faces.size(), 0),    // cell 0 is owner of all faces
        0,                             // cell 0
        identity(faces.size())         // cell 0 consists of all faces
    );
}


CML::cellShape CML::degenerateMatcher::match(const cellShape& shape)
{
    return match(shape.collapsedFaces());
}


CML::cellShape CML::degenerateMatcher::match
(
    const primitiveMesh& mesh,
    const label cellI
)
{
    return match
    (
        mesh.faces(),
        mesh.faceOwner(),
        cellI,
        mesh.cells()[cellI]
    );
}


// ************************************************************************* //
