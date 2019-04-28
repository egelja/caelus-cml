/*---------------------------------------------------------------------------*\
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

#include "edgeMesh.hpp"
#include "boundBox.hpp"
#include "EMESHedgeFormat.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::edgeMesh::edgeMesh
(
    const fileName& name,
    const word& ext
)
:
    points_(0),
    edges_(0),
    pointEdgesPtr_(nullptr)
{
    read(name, ext);
}


CML::edgeMesh::edgeMesh(const fileName& name)
:
    points_(0),
    edges_(0),
    pointEdgesPtr_(nullptr)
{
    read(name);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::edgeMesh::read(const fileName& name)
{
    word ext = name.ext();
    if (ext == "gz")
    {
        fileName unzipName = name.lessExt();
        return read(unzipName, unzipName.ext());
    }
    else
    {
        return read(name, ext);
    }
}


// Read from file in given format
bool CML::edgeMesh::read
(
    const fileName& name,
    const word& ext
)
{
    // read via selector mechanism
    transfer(New(name, ext)());
    return true;
}


void CML::edgeMesh::write
(
    const fileName& name,
    const edgeMesh& mesh
)
{
    if (debug)
    {
        Info<< "edgeMesh::write"
            "(const fileName&, const edgeMesh&) : "
            "writing to " << name
            << endl;
    }

    const word ext = name.ext();

    writefileExtensionMemberFunctionTable::iterator mfIter =
        writefileExtensionMemberFunctionTablePtr_->find(ext);

    if (mfIter == writefileExtensionMemberFunctionTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown file extension " << ext << nl << nl
            << "Valid types are :" << endl
            << writefileExtensionMemberFunctionTablePtr_->sortedToc()
            << exit(FatalError);
    }
    else
    {
        mfIter()(name, mesh);
    }
}


void CML::edgeMesh::writeStats(Ostream& os) const
{
    os  << "points      : " << points().size() << nl;
    os  << "edges       : " << edges().size() << nl;
    os  << "boundingBox : " << boundBox(this->points()) << endl;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const edgeMesh& em)
{
    fileFormats::EMESHedgeFormat::write(os, em.points_, em.edges_);

    // Check state of Ostream
    os.check("Ostream& operator<<(Ostream&, const edgeMesh&)");

    return os;
}


CML::Istream& CML::operator>>(Istream& is, edgeMesh& em)
{
    fileFormats::EMESHedgeFormat::read(is, em.points_, em.edges_);

    em.pointEdgesPtr_.clear();

    // Check state of Istream
    is.check("Istream& operator>>(Istream&, edgeMesh&)");

    return is;
}


// ************************************************************************* //
