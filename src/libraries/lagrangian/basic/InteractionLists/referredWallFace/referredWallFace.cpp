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

#include "referredWallFace.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::referredWallFace::referredWallFace()
:
    face(),
    pts_(),
    patchI_()
{}


CML::referredWallFace::referredWallFace
(
    const face& f,
    const pointField& pts,
    label patchI
)
:
    face(f),
    pts_(pts),
    patchI_(patchI)
{
    if (this->size() != pts_.size())
    {
        FatalErrorIn
        (
            "CML::referredWallFace::referredWallFace"
            "("
                "const face& f, "
                "const pointField& pts, "
                "label patchI"
            ")"
        )   << "Face and pointField are not the same size. " << nl << (*this)
            << abort(FatalError);
    }
}


CML::referredWallFace::referredWallFace(const referredWallFace& rWF)
:
    face(rWF),
    pts_(rWF.pts_),
    patchI_(rWF.patchI_)
{
    if (this->size() != pts_.size())
    {
        FatalErrorIn
        (
            "CML::referredWallFace::referredWallFace"
            "("
                "const referredWallFace& rWF"
            ")"
        )   << "Face and pointField are not the same size. " << nl << (*this)
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::referredWallFace::~referredWallFace()
{}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //

bool CML::referredWallFace::operator==(const referredWallFace& rhs) const
{
    return
    (
        static_cast<const face&>(rhs) == static_cast<face>(*this)
     && rhs.pts_ == pts_
     && rhs.patchI_ == patchI_
    );
}


bool CML::referredWallFace::operator!=(const referredWallFace& rhs) const
{
    return !(*this == rhs);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, referredWallFace& rWF)
{
    is  >> static_cast<face&>(rWF) >> rWF.pts_ >> rWF.patchI_;

    // Check state of Istream
    is.check
    (
        "CML::Istream& "
        "CML::operator>>(CML::Istream&, CML::referredWallFace&)"
    );

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const referredWallFace& rWF)
{
    os  << static_cast<const face&>(rWF) << token::SPACE
        << rWF.pts_ << token::SPACE
        << rWF.patchI_;

    // Check state of Ostream
    os.check
    (
        "CML::Ostream& CML::operator<<(CML::Ostream&, "
        "const CML::referredWallFace&)"
    );

    return os;
}


// ************************************************************************* //
