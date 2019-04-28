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

#include "Istream.hpp"
#include "cell.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::cellShape::cellShape()
:
    m(nullptr)
{}


inline CML::cellShape::cellShape
(
    const cellModel& M,
    const labelList& l,
    const bool doCollapse
)
:
    labelList(l),
    m(&M)
{
    if (doCollapse)
    {
        collapse();
    }
}


inline CML::cellShape::cellShape(Istream& is)
{
    is >> *this;
}


inline CML::autoPtr<CML::cellShape> CML::cellShape::clone() const
{
    return autoPtr<cellShape>(new cellShape(*this));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::pointField CML::cellShape::points
(
    const pointField& meshPoints
) const
{
    // There are as many points as there labels for them
    pointField p(size());

    // For each point in list, set it to the point in 'pnts' addressed
    // by 'labs'
    forAll(p, i)
    {
        p[i] = meshPoints[operator[](i)];
    }

    // Return list
    return p;
}


inline const CML::cellModel& CML::cellShape::model() const
{
    return *m;
}


inline CML::labelList CML::cellShape::meshFaces
(
    const faceList& allFaces,
    const cell& cFaces
) const
{
    // Faces in model order
    faceList localFaces(faces());

    // Do linear match (usually cell shape is low complexity)

    labelList modelToMesh(localFaces.size(), -1);

    forAll(localFaces, i)
    {
        const face& localF = localFaces[i];

        forAll(cFaces, j)
        {
            label meshFaceI = cFaces[j];

            if (allFaces[meshFaceI] == localF)
            {
                modelToMesh[i] = meshFaceI;

                break;
            }
        }
    }

    return modelToMesh;
}


inline CML::labelList CML::cellShape::meshEdges
(
    const edgeList& allEdges,
    const labelList& cEdges
) const
{
    // Edges in model order
    edgeList localEdges(edges());

    // Do linear match (usually cell shape is low complexity)

    labelList modelToMesh(localEdges.size(), -1);

    forAll(localEdges, i)
    {
        const edge& e = localEdges[i];

        forAll(cEdges, j)
        {
            label edgeI = cEdges[j];

            if (allEdges[edgeI] == e)
            {
                modelToMesh[i] = edgeI;

                break;
            }
        }
    }

    return modelToMesh;
}


inline CML::faceList CML::cellShape::faces() const
{
    return m->faces(*this);
}


inline CML::faceList CML::cellShape::collapsedFaces() const
{
    faceList oldFaces(faces());

    faceList newFaces(oldFaces.size());
    label newFaceI = 0;

    forAll(oldFaces, oldFaceI)
    {
        const face& f = oldFaces[oldFaceI];

        face& newF = newFaces[newFaceI];

        newF.setSize(f.size());

        label newFp = 0;
        label prevVertI = -1;

        forAll(f, fp)
        {
            label vertI = f[fp];

            if (vertI != prevVertI)
            {
                newF[newFp++] = vertI;

                prevVertI = vertI;
            }
        }

        if ((newFp > 1) && (newF[newFp-1] == newF[0]))
        {
            --newFp;
        }

        if (newFp > 2)
        {
            // Size face and go to next one
            newF.setSize(newFp);

            newFaceI++;
        }
    }
    newFaces.setSize(newFaceI);

    return newFaces;
}


inline CML::label CML::cellShape::nFaces() const
{
    return m->nFaces();
}


inline CML::edgeList CML::cellShape::edges() const
{
    return m->edges(*this);
}


inline CML::label CML::cellShape::nEdges() const
{
    return m->nEdges();
}


inline CML::label CML::cellShape::nPoints() const
{
    return size();
}


inline CML::point CML::cellShape::centre(const pointField& points) const
{
    return m->centre(*this, points);
}


inline CML::scalar CML::cellShape::mag(const pointField& points) const
{
    return m->mag(*this, points);
}


// ************************************************************************* //
