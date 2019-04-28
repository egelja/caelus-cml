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

#include "referredWallFace.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::referredWallFace::referredWallFace()
:
    face(),
    pts_(),
    patchi_()
{}


CML::referredWallFace::referredWallFace
(
    const face& f,
    const pointField& pts,
    label patchi
)
:
    face(f),
    pts_(pts),
    patchi_(patchi)
{
    if (this->size() != pts_.size())
    {
        FatalErrorInFunction
            << "Face and pointField are not the same size. " << nl << (*this)
            << abort(FatalError);
    }
}


CML::referredWallFace::referredWallFace(const referredWallFace& rWF)
:
    face(rWF),
    pts_(rWF.pts_),
    patchi_(rWF.patchi_)
{
    if (this->size() != pts_.size())
    {
        FatalErrorInFunction
            << "Face and pointField are not the same size. " << nl << (*this)
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
     && rhs.patchi_ == patchi_
    );
}


bool CML::referredWallFace::operator!=(const referredWallFace& rhs) const
{
    return !(*this == rhs);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, referredWallFace& rWF)
{
    is  >> static_cast<face&>(rWF) >> rWF.pts_ >> rWF.patchi_;

    // Check state of Istream
    is.check
    (
        "Foam::Istream& "
        "Foam::operator>>(Foam::Istream&, Foam::referredWallFace&)"
    );

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const referredWallFace& rWF)
{
    os  << static_cast<const face&>(rWF) << token::SPACE
        << rWF.pts_ << token::SPACE
        << rWF.patchi_;

    // Check state of Ostream
    os.check
    (
        "Foam::Ostream& Foam::operator<<(Foam::Ostream&, "
        "const Foam::referredWallFace&)"
    );

    return os;
}


// ************************************************************************* //
