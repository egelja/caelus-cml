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

#include "tetIndices.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::tetIndices::tetIndices()
:
    celli_(-1),
    facei_(-1),
    tetPti_(-1)
{}


CML::tetIndices::tetIndices
(
    label celli,
    label facei,
    label tetPtI
)
:
    celli_(celli),
    facei_(facei),
    tetPti_(tetPtI)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::tetIndices::~tetIndices()
{}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, tetIndices& tI)
{
    is  >> tI.cell() >> tI.face() >> tI.tetPt();

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
