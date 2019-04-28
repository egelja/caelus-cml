/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::tetIndices::cell() const
{
    return celli_;
}


inline CML::label& CML::tetIndices::cell()
{
    return celli_;
}


inline CML::label CML::tetIndices::face() const
{
    return facei_;
}


inline CML::label& CML::tetIndices::face()
{
    return facei_;
}


inline CML::label CML::tetIndices::tetPt() const
{
    return tetPti_;
}


inline CML::label& CML::tetIndices::tetPt()
{
    return tetPti_;
}


inline CML::triFace CML::tetIndices::faceTriIs(const polyMesh& mesh) const
{
    const CML::face& f = mesh.faces()[face()];

    label faceBasePtI = mesh.tetBasePtIs()[face()];

    if (faceBasePtI < 0)
    {
        static label badFacei = -1;

        if (badFacei != face())
        {
            WarningInFunction
                << "No base point for face " << face() << ", " << f
                << ", produces a valid tet decomposition." << endl;

            badFacei = face();
        }

        faceBasePtI = 0;
    }

    label facePtI = (tetPt() + faceBasePtI) % f.size();
    label faceOtherPtI = f.fcIndex(facePtI);

    if (mesh.faceOwner()[face()] != cell())
    {
        Swap(facePtI, faceOtherPtI);
    }

    return triFace(f[faceBasePtI], f[facePtI], f[faceOtherPtI]);
}


inline CML::tetPointRef CML::tetIndices::tet(const polyMesh& mesh) const
{
    const pointField& meshPoints = mesh.points();
    const triFace tri = faceTriIs(mesh);

    return tetPointRef
    (
        mesh.cellCentres()[cell()],
        meshPoints[tri[0]],
        meshPoints[tri[1]],
        meshPoints[tri[2]]
    );
}


inline CML::triPointRef CML::tetIndices::faceTri(const polyMesh& mesh) const
{
    const pointField& meshPoints = mesh.points();
    const triFace tri = faceTriIs(mesh);

    return triPointRef
    (
        meshPoints[tri[0]],
        meshPoints[tri[1]],
        meshPoints[tri[2]]
    );
}


inline CML::triPointRef CML::tetIndices::oldFaceTri
(
    const polyMesh& mesh
) const
{
    const pointField& meshOldPoints = mesh.oldPoints();
    const triFace tri = faceTriIs(mesh);

    return triPointRef
    (
        meshOldPoints[tri[0]],
        meshOldPoints[tri[1]],
        meshOldPoints[tri[2]]
    );
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool CML::tetIndices::operator==(const CML::tetIndices& rhs) const
{
    return
        cell() == rhs.cell()
     && face() == rhs.face()
     && tetPt() == rhs.tetPt();
}


inline bool CML::tetIndices::operator!=(const CML::tetIndices& rhs) const
{
    return !(*this == rhs);
}


// ************************************************************************* //
