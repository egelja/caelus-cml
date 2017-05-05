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

#include "tetIndices.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::tetIndices::tetIndices()
:
    cellI_(-1),
    faceI_(-1),
    faceBasePtI_(-1),
    facePtAI_(-1),
    facePtBI_(-1),
    tetPtI_(-1)
{}


CML::tetIndices::tetIndices
(
    label cellI,
    label faceI,
    label faceBasePtI,
    label facePtAI,
    label facePtBI,
    label tetPtI
)
:
    cellI_(cellI),
    faceI_(faceI),
    faceBasePtI_(faceBasePtI),
    facePtAI_(facePtAI),
    facePtBI_(facePtBI),
    tetPtI_(tetPtI)
{}


CML::tetIndices::tetIndices
(
    label cellI,
    label faceI,
    label tetPtI,
    const polyMesh& mesh
)
:
    cellI_(cellI),
    faceI_(faceI),
    faceBasePtI_(-1),
    facePtAI_(-1),
    facePtBI_(-1),
    tetPtI_(tetPtI)
{
    const faceList& pFaces = mesh.faces();
    const labelList& pOwner = mesh.faceOwner();

    const CML::face& f = pFaces[faceI_];

    bool own = (pOwner[faceI_] == cellI_);

    faceBasePtI_ = mesh.tetBasePtIs()[faceI_];

    label facePtI = (tetPtI_ + faceBasePtI_) % f.size();
    label otherFacePtI = f.fcIndex(facePtI);

    if (own)
    {
        facePtAI_ = facePtI;
        facePtBI_ = otherFacePtI;
    }
    else
    {
        facePtAI_ = otherFacePtI;
        facePtBI_ = facePtI;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::tetIndices::~tetIndices()
{}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, tetIndices& tI)
{
    is  >> tI.cell()
        >> tI.face()
        >> tI.faceBasePt()
        >> tI.facePtA()
        >> tI.facePtB()
        >> tI.tetPt();

    // Check state of Istream
    is.check
    (
        "CML::Istream& CML::operator>>(CML::Istream&, CML::tetIndices&)"
    );

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const tetIndices& tI)
{
    os  << tI.cell() << token::SPACE
        << tI.face() << token::SPACE
        << tI.faceBasePt() << token::SPACE
        << tI.facePtA() << token::SPACE
        << tI.facePtB() << token::SPACE
        << tI.tetPt() << token::SPACE
        << endl;

    // Check state of Ostream
    os.check
    (
        "CML::Ostream& CML::operator<<(CML::Ostream&, "
        "const CML::tetIndices&)"
    );

    return os;
}


// ************************************************************************* //
