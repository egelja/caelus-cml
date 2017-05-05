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

#include "edgeVertex.hpp"
#include "meshTools.hpp"
#include "refineCell.hpp"


// * * * * * * * * * * * * * * * Static Functions  * * * * * * * * * * * * * //


// Update stored refine list using map
void CML::edgeVertex::updateLabels
(
    const labelList& map,
    List<refineCell>& refCells
)
{
    label newRefI = 0;

    forAll(refCells, refI)
    {
        const refineCell& refCell = refCells[refI];

        label oldCellI = refCell.cellNo();

        label newCellI = map[oldCellI];

        if (newCellI != -1)
        {
            refCells[newRefI++] = refineCell(newCellI, refCell.direction());
        }
    }
    refCells.setSize(newRefI);
}


// Update stored cell numbers using map.
// Do in two passes to prevent allocation if nothing changed.
void CML::edgeVertex::updateLabels
(
    const labelList& map,
    Map<label>& cellPairs
)
{
    // Iterate over map to see if anything changed
    bool changed = false;

    forAllConstIter(Map<label>, cellPairs, iter)
    {
        label newMaster = map[iter.key()];

        label newSlave = -1;

        if (iter() != -1)
        {
            newSlave = map[iter()];
        }

        if ((newMaster != iter.key()) || (newSlave != iter()))
        {
            changed = true;

            break;
        }
    }

    // Relabel (use second Map to prevent overlapping)
    if (changed)
    {
        Map<label> newCellPairs(2*cellPairs.size());

        forAllConstIter(Map<label>, cellPairs, iter)
        {
            label newMaster = map[iter.key()];

            label newSlave = -1;

            if (iter() != -1)
            {
                newSlave = map[iter()];
            }

            if (newMaster == -1)
            {
                WarningIn
                (
                    "edgeVertex::updateLabels(const labelList&, "
                    "Map<label>&)"
                )   << "master cell:" << iter.key()
                    << " has disappeared" << endl;
            }
            else
            {
                newCellPairs.insert(newMaster, newSlave);
            }
        }

        cellPairs = newCellPairs;
    }
}


// Update stored cell numbers using map.
// Do in two passes to prevent allocation if nothing changed.
void CML::edgeVertex::updateLabels
(
    const labelList& map,
    labelHashSet& cells
)
{
    // Iterate over map to see if anything changed
    bool changed = false;

    forAllConstIter(labelHashSet, cells, iter)
    {
        const label newCellI = map[iter.key()];

        if (newCellI != iter.key())
        {
            changed = true;

            break;
        }
    }

    // Relabel (use second Map to prevent overlapping)
    if (changed)
    {
        labelHashSet newCells(2*cells.size());

        forAllConstIter(labelHashSet, cells, iter)
        {
            const label newCellI = map[iter.key()];

            if (newCellI != -1)
            {
                newCells.insert(newCellI);
            }
        }

        cells = newCells;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::point CML::edgeVertex::coord
(
    const primitiveMesh& mesh,
    const label cut,
    const scalar weight
)
{
    const pointField& pts = mesh.points();

    if (isEdge(mesh, cut))
    {
        const edge& e = mesh.edges()[getEdge(mesh, cut)];

        return weight*pts[e.end()] + (1-weight)*pts[e.start()];
    }
    else
    {
        return pts[getVertex(mesh, cut)];
    }
}


CML::label CML::edgeVertex::cutPairToEdge
(
    const primitiveMesh& mesh,
    const label cut0,
    const label cut1
)
{
    if (!isEdge(mesh, cut0) && !isEdge(mesh, cut1))
    {
        return meshTools::findEdge
        (
            mesh,
            getVertex(mesh, cut0),
            getVertex(mesh, cut1)
        );
    }
    else
    {
        return -1;
    }
}


CML::Ostream& CML::edgeVertex::writeCut
(
    Ostream& os,
    const label cut,
    const scalar weight
) const
{
    if (isEdge(cut))
    {
        label edgeI = getEdge(cut);

        const edge& e = mesh().edges()[edgeI];

        os  << "edge:" << edgeI << e << ' ' << coord(cut, weight);
    }
    else
    {
        label vertI = getVertex(cut);

        os << "vertex:" << vertI << ' ' << coord(cut, weight);
    }
    return os;
}


CML::Ostream& CML::edgeVertex::writeCuts
(
    Ostream& os,
    const labelList& cuts,
    const scalarField& weights
) const
{
    forAll(cuts, cutI)
    {
        if (cutI > 0)
        {
            os << ' ';
        }
        writeCut(os, cuts[cutI], weights[cutI]);
    }
    return os;
}


// ************************************************************************* //
