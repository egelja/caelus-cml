/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Author: Franjo Juretic (franjo.juretic@c-fields.com)

\*---------------------------------------------------------------------------*/

#include "VRWGraphList.hpp"
#include "token.hpp"
#include "labelList.hpp"

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<
(
    CML::Ostream& os,
    const CML::VRWGraphList& DL
)
{
    os << DL.size() << nl << token::BEGIN_LIST;

    for(label i=0;i<DL.size();++i)
    {
        os << nl << DL[i];
    }

    os << nl << token::END_LIST;

    // Check state of IOstream
    os.check
    (
        "CML::Ostream& CML::operator<<"
        "(CML::Ostream&, const CML::VRWGraphList&)"
    );

    return os;
}

/*
template<class T, CML::label width>
CML::Istream& CML::operator>>
(
    CML::Istream& is,
    CML::VRWGraphList<T, width>& DL
)
{
    label size;
    T e;
    is >> size;
    DL.setSize(size);
    for(IndexType i=0;i<size;++i)
    {
        is >> e;
        DL[i] = e;
    }

    return is;
}
*/

// ************************************************************************* //
