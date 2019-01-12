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

Description
    Exchange data.

\*---------------------------------------------------------------------------*/

#include "Pstream.hpp"
#include "contiguous.hpp"
#include "PstreamCombineReduceOps.hpp"
#include "UPstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//template <template<class> class ListType, class T>
template <class Container, class T>
void Pstream::exchange
(
    const List<Container>& sendBufs,
    List<Container>& recvBufs,
    labelListList& sizes,
    const int tag,
    const bool block
)
{
    if (!contiguous<T>())
    {
        FatalErrorInFunction
            << "Continuous data only." << CML::abort(FatalError);
    }

    if (sendBufs.size() != UPstream::nProcs())
    {
        FatalErrorInFunction
            << "Size of list " << sendBufs.size()
            << " does not equal the number of processors "
            << UPstream::nProcs()
            << CML::abort(FatalError);
    }

    sizes.setSize(UPstream::nProcs());
    labelList& nsTransPs = sizes[UPstream::myProcNo()];
    nsTransPs.setSize(UPstream::nProcs());

    forAll(sendBufs, procI)
    {
        nsTransPs[procI] = sendBufs[procI].size();
    }

    // Send sizes across. Note: blocks.
    combineReduce(sizes, UPstream::listEq(), tag);

    if (Pstream::parRun())
    {
        label startOfRequests = Pstream::nRequests();

        // Set up receives
        // ~~~~~~~~~~~~~~~

        recvBufs.setSize(sendBufs.size());
        forAll(sizes, proci)
        {
            label nRecv = sizes[procI][UPstream::myProcNo()];

            if (proci != Pstream::myProcNo() && nRecv > 0)
            {
                recvBufs[proci].setSize(nRecv);
                UIPstream::read
                (
                    UPstream::nonBlocking,
                    proci,
                    reinterpret_cast<char*>(recvBufs[proci].begin()),
                    nRecv*sizeof(T),
                    tag
                );
            }
        }


        // Set up sends
        // ~~~~~~~~~~~~

        forAll(sendBufs, proci)
        {
            if (proci != Pstream::myProcNo() && sendBufs[proci].size() > 0)
            {
                if
                (
                   !UOPstream::write
                    (
                        UPstream::nonBlocking,
                        proci,
                        reinterpret_cast<const char*>(sendBufs[proci].begin()),
                        sendBufs[proci].size()*sizeof(T),
                        tag
                    )
                )
                {
                    FatalErrorInFunction
                        << "Cannot send outgoing message. "
                        << "to:" << proci << " nBytes:"
                        << label(sendBufs[proci].size()*sizeof(T))
                        << CML::abort(FatalError);
                }
            }
        }


        // Wait for all to finish
        // ~~~~~~~~~~~~~~~~~~~~~~

        if (block)
        {
            Pstream::waitRequests(startOfRequests);
        }
    }

    // Do myself
    recvBufs[Pstream::myProcNo()] = sendBufs[Pstream::myProcNo()];
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
