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

#include "VRWGraph.hpp"
#include "token.hpp"
#include "labelList.hpp"

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<
(
    CML::Ostream& os,
    const CML::VRWGraph& DL
)
{
    os << DL.size() << nl << token::BEGIN_LIST << nl;

    for(label i=0;i<DL.size();++i)
    {
        os << DL.sizeOfRow(i) << token::BEGIN_LIST;
        for(label j=0;j<DL.sizeOfRow(i);++j)
        {
            if( j ) os << token::SPACE;

            os << DL(i, j);
        }

        os << token::END_LIST << nl;
    }

    os << token::END_LIST;

    // Check state of IOstream
    os.check
    (
        "CML::Ostream& CML::operator<<(CML::Ostream&, const CML::VRWGraph&)"
    );

    return os;
}

/*
template<class T, CML::label width>
CML::Istream& CML::operator>>
(
    CML::Istream& is,
    CML::VRWGraph<T, width>& DL
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

void CML::VRWGraph::optimizeMemoryUsage()
{
    labelLongList newPosForNode(data_.size());
    label pos(0), nElements;
    nElements = data_.size();
    for(label elI=0;elI<nElements;++elI)
        if( data_[elI] != FREEENTRY )
        {
            newPosForNode[elI] = pos++;
        }
        else
        {
            newPosForNode[elI] = -1;
        }

    //- create new data
    for(label elI=0;elI<nElements;++elI)
        if( (newPosForNode[elI] != -1) && (newPosForNode[elI] < elI) )
            data_[newPosForNode[elI]] = data_[elI];

    data_.setSize(pos);

    //- renumber rows
    nElements = rows_.size();
    for(label rowI=0;rowI<nElements;++rowI)
        if( rows_[rowI].start() != INVALIDROW )
            rows_[rowI].start() = newPosForNode[rows_[rowI].start()];
}

// ************************************************************************* //
