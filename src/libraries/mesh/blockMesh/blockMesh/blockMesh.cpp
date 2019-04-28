/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "blockMesh.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

bool CML::blockMesh::blockMesh::verboseOutput(false);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::blockMesh::blockMesh(const IOdictionary& dict, const word& regionName)
:
    blockPointField_(dict.lookup("vertices")),
    scaleFactor_(1.0),
    topologyPtr_(createTopology(dict, regionName))
{
    calcMergeInfo();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::blockMesh::~blockMesh()
{
    delete topologyPtr_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::blockMesh::verbose(const bool on)
{
    verboseOutput = on;
}


const CML::pointField& CML::blockMesh::blockPointField() const
{
    return blockPointField_;
}


const CML::polyMesh& CML::blockMesh::topology() const
{
    if (!topologyPtr_)
    {
        FatalErrorInFunction
            << "topologyPtr_ not allocated"
            << exit(FatalError);
    }

    return *topologyPtr_;
}


CML::PtrList<CML::dictionary> CML::blockMesh::patchDicts() const
{
    const polyPatchList& patchTopologies = topology().boundaryMesh();

    PtrList<dictionary> patchDicts(patchTopologies.size());

    forAll(patchTopologies, patchI)
    {
        OStringStream os;
        patchTopologies[patchI].write(os);
        IStringStream is(os.str());
        patchDicts.set(patchI, new dictionary(is));
    }
    return patchDicts;
}


CML::scalar CML::blockMesh::scaleFactor() const
{
    return scaleFactor_;
}


const CML::pointField& CML::blockMesh::points() const
{
    if (points_.empty())
    {
        createPoints();
    }

    return points_;
}


const CML::cellShapeList& CML::blockMesh::cells() const
{
    if (cells_.empty())
    {
        createCells();
    }

    return cells_;
}


const CML::faceListList& CML::blockMesh::patches() const
{
    if (patches_.empty())
    {
        createPatches();
    }

    return patches_;
}


CML::wordList CML::blockMesh::patchNames() const
{
    return topology().boundaryMesh().names();
}


//CML::wordList CML::blockMesh::patchTypes() const
//{
//    return topology().boundaryMesh().types();
//}
//
//
//CML::wordList CML::blockMesh::patchPhysicalTypes() const
//{
//    return topology().boundaryMesh().physicalTypes();
//}


CML::label CML::blockMesh::numZonedBlocks() const
{
    label num = 0;

    forAll(*this, blockI)
    {
        if (operator[](blockI).zoneName().size())
        {
            num++;
        }
    }

    return num;
}


void CML::blockMesh::writeTopology(Ostream& os) const
{
    const pointField& pts = topology().points();

    forAll(pts, pI)
    {
        const point& pt = pts[pI];

        os << "v " << pt.x() << ' ' << pt.y() << ' ' << pt.z() << endl;
    }

    const edgeList& edges = topology().edges();

    forAll(edges, eI)
    {
        const edge& e = edges[eI];

        os << "l " << e.start() + 1 << ' ' << e.end() + 1 << endl;
    }
}

// ************************************************************************* //
