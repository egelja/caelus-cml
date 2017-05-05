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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::label CML::tetIndices::cell() const
{
    return cellI_;
}


CML::label CML::tetIndices::face() const
{
    return faceI_;
}


CML::label CML::tetIndices::faceBasePt() const
{
    return faceBasePtI_;
}


CML::label CML::tetIndices::facePtA() const
{
    return facePtAI_;
}


CML::label CML::tetIndices::facePtB() const
{
    return facePtBI_;
}


CML::label CML::tetIndices::tetPt() const
{
    return tetPtI_;
}


CML::tetPointRef CML::tetIndices::tet(const polyMesh& mesh) const
{
    const pointField& pPts = mesh.points();
    const faceList& pFaces = mesh.faces();
    const vectorField& pC = mesh.cellCentres();

    const CML::face& f = pFaces[faceI_];

    return tetPointRef
    (
        pC[cellI_],
        pPts[f[faceBasePtI_]],
        pPts[f[facePtAI_]],
        pPts[f[facePtBI_]]
    );
}


CML::tetPointRef CML::tetIndices::oldTet(const polyMesh& mesh) const
{
    const pointField& oldPPts = mesh.oldPoints();
    const faceList& pFaces = mesh.faces();

    // We need to reconstruct the old Cc from oldPoints (it isn't
    // stored)
    point oldC = mesh.cells()[cellI_].centre
    (
        oldPPts,
        pFaces
    );

    const CML::face& f = pFaces[faceI_];

    return tetPointRef
    (
        oldC,
        oldPPts[f[faceBasePtI_]],
        oldPPts[f[facePtAI_]],
        oldPPts[f[facePtBI_]]
    );
}


CML::triPointRef CML::tetIndices::faceTri(const polyMesh& mesh) const
{
    const pointField& pPts = mesh.points();
    const faceList& pFaces = mesh.faces();

    const CML::face& f = pFaces[faceI_];

    return triPointRef
    (
        pPts[f[faceBasePtI_]],
        pPts[f[facePtAI_]],
        pPts[f[facePtBI_]]
    );
}


CML::triFace CML::tetIndices::faceTriIs(const polyMesh& mesh) const
{
    const faceList& pFaces = mesh.faces();

    const CML::face& f = pFaces[faceI_];

    return triFace
    (
        f[faceBasePtI_],
        f[facePtAI_],
        f[facePtBI_]
    );
}


CML::triPointRef CML::tetIndices::oldFaceTri(const polyMesh& mesh) const
{
    const pointField& oldPPts = mesh.oldPoints();
    const faceList& pFaces = mesh.faces();

    const CML::face& f = pFaces[faceI_];

    return triPointRef
    (
        oldPPts[f[faceBasePtI_]],
        oldPPts[f[facePtAI_]],
        oldPPts[f[facePtBI_]]
    );
}


CML::label& CML::tetIndices::cell()
{
    return cellI_;
}


CML::label& CML::tetIndices::face()
{
    return faceI_;
}


CML::label& CML::tetIndices::faceBasePt()
{
    return faceBasePtI_;
}


CML::label& CML::tetIndices::facePtA()
{
    return facePtAI_;
}


CML::label& CML::tetIndices::facePtB()
{
    return facePtBI_;
}


CML::label& CML::tetIndices::tetPt()
{
    return tetPtI_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool CML::tetIndices::operator==(const CML::tetIndices& rhs) const
{
    return
    (
        cell() == rhs.cell()
     && face() == rhs.face()
     && faceBasePt() == rhs.faceBasePt()
     && facePtA() == rhs.facePtA()
     && facePtB() == rhs.facePtB()
     && tetPt() == rhs.tetPt()
    );
}


inline bool CML::tetIndices::operator!=(const CML::tetIndices& rhs) const
{
    return !(*this == rhs);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// ************************************************************************* //
