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

\*----------------------------------------------------------------------------*/

#include "ensightPartFaces.hpp"
#include "IOstreams.hpp"
#include "IStringStream.hpp"
#include "dictionary.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(ensightPartFaces, 0);
    addToRunTimeSelectionTable(ensightPart, ensightPartFaces, istream);
}


const CML::List<CML::word> CML::ensightPartFaces::elemTypes_
(
    IStringStream
    (
        "(tria3 quad4 nsided)"
    )()
);


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void CML::ensightPartFaces::classify(const faceList& faces)
{
    // count the shapes
    label nTri  = 0;
    label nQuad = 0;
    label nPoly = 0;

    forAll(faces, faceI)
    {
        const face& f = faces[faceI];

        if (f.size() == 3)
        {
            nTri++;
        }
        else if (f.size() == 4)
        {
            nQuad++;
        }
        else
        {
            nPoly++;
        }
    }

    // we can avoid double looping, but at the cost of allocation

    labelList triCells(nTri);
    labelList quadCells(nQuad);
    labelList polygonCells(nPoly);

    nTri  = 0;
    nQuad = 0;
    nPoly = 0;

    // classify the shapes
    forAll(faces, faceI)
    {
        const face& f = faces[faceI];

        if (f.size() == 3)
        {
            triCells[nTri++] = faceI;
        }
        else if (f.size() == 4)
        {
            quadCells[nQuad++] = faceI;
        }
        else
        {
            polygonCells[nPoly++] = faceI;
        }
    }


    // MUST match with elementTypes
    elemLists_.setSize(elementTypes().size());

    elemLists_[tria3Elements].transfer(triCells);
    elemLists_[quad4Elements].transfer(quadCells);
    elemLists_[nsidedElements].transfer(polygonCells);

    size_ = faces.size();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::ensightPartFaces::ensightPartFaces
(
    label partNumber,
    const string& partDescription
)
:
    ensightPart(partNumber, partDescription),
    faces_(faceList::null()),
    contiguousPoints_(false)
{
    isCellData_ = false;
    offset_ = 0;
    size_ = 0;
}


CML::ensightPartFaces::ensightPartFaces
(
    label partNumber,
    const string& partDescription,
    const pointField& points,
    const faceList& faces,
    const bool contiguousPoints
)
:
    ensightPart(partNumber, partDescription, points),
    faces_(faces),
    contiguousPoints_(contiguousPoints)
{
    isCellData_ = false;
    offset_ = 0;
    size_ = 0;

    // classify the face shapes
    classify(faces);
}


CML::ensightPartFaces::ensightPartFaces
(
    label partNumber,
    const polyMesh& mesh,
    const polyPatch& patch
)
:
    ensightPart(partNumber, patch.name(), mesh.points()),
    faces_(mesh.faces()),
    contiguousPoints_(false)
{
    isCellData_ = false;
    offset_ = patch.start();

    // classify the face shapes
    classify(patch);
}


CML::ensightPartFaces::ensightPartFaces(const ensightPartFaces& part)
:
    ensightPart(part),
    faces_(part.faces_),
    contiguousPoints_(part.contiguousPoints_)
{}


CML::ensightPartFaces::ensightPartFaces(Istream& is)
:
    ensightPart(),
    faces_(faceList::null()),
    contiguousPoints_(false)
{
    isCellData_ = false;
    reconstruct(is);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::ensightPartFaces::~ensightPartFaces()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::ensightPart::localPoints CML::ensightPartFaces::calcLocalPoints() const
{
    if (contiguousPoints_)
    {
        localPoints ptList;
        ptList.list = identity(points_.size());
        ptList.nPoints = points_.size();
        return ptList;
    }

    localPoints ptList(points_);
    labelList& usedPoints = ptList.list;
    label nPoints = 0;

    forAll(elemLists_, typeI)
    {
        const labelUList& idList = elemLists_[typeI];

        // add all points from faces
        forAll(idList, i)
        {
            const label id = idList[i] + offset_;
            const face& f = faces_[id];

            forAll(f, fp)
            {
                if (usedPoints[f[fp]] == -1)
                {
                    usedPoints[f[fp]] = nPoints++;
                }
            }
        }
    }

    // this is not absolutely necessary, but renumber anyhow
    nPoints = 0;
    forAll(usedPoints, ptI)
    {
        if (usedPoints[ptI] > -1)
        {
            usedPoints[ptI] = nPoints++;
        }
    }

    ptList.nPoints = nPoints;
    return ptList;
}


void CML::ensightPartFaces::writeConnectivity
(
    ensightGeoFile& os,
    const word& key,
    const faceList& faces,
    const labelUList& idList,
    const labelUList& pointMap
) const
{
    os.writeKeyword(key);
    os.write(idList.size());
    os.newline();

    // write (polygon) face sizes
    if (key == "nsided")
    {
        // write the number of points per face
        forAll(idList, i)
        {
            const label id = idList[i] + offset_;
            const face& f = faces[id];

            os.write(f.size());
            os.newline();
        }
    }

    // write the points describing the face
    forAll(idList, i)
    {
        const label id = idList[i] + offset_;
        const face& f = faces[id];

        // convert global -> local index
        // (note: Ensight indices start with 1)
        forAll(f, fp)
        {
            os.write(pointMap[f[fp]] + 1);
        }
        os.newline();
    }
}


void CML::ensightPartFaces::writeConnectivity
(
    ensightGeoFile& os,
    const word& key,
    const labelUList& idList,
    const labelUList& pointMap
) const
{
    writeConnectivity
    (
        os,
        key,
        faces_,
        idList,
        pointMap
    );
}


void CML::ensightPartFaces::writeGeometry(ensightGeoFile& os) const
{
    ensightPart::writeGeometry(os, points_);
}


// ************************************************************************* //
