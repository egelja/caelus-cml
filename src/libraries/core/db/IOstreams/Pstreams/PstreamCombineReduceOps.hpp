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

InClass
    CML

Description
    Combination-Reduction operation for a parallel run.  The
    information from all nodes is collected on the master node,
    combined using the given combination function and the result is
    broadcast to all nodes


\*---------------------------------------------------------------------------*/

#ifndef PstreamCombineReduceOps_H
#define PstreamCombineReduceOps_H

#include "UPstream.hpp"
#include "Pstream.hpp"
#include "ops.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template <class T, class CombineOp>
void combineReduce
(
    const List<UPstream::commsStruct>& comms,
    T& Value,
    const CombineOp& cop,
    const int tag
)
{
    Pstream::combineGather(comms, Value, cop, tag);
    Pstream::combineScatter(comms, Value, tag);
}


template <class T, class CombineOp>
void combineReduce
(
    T& Value,
    const CombineOp& cop,
    const int tag = Pstream::msgType()
)
{
    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        Pstream::combineGather
        (
            UPstream::linearCommunication(),
            Value,
            cop,
            tag
        );
        Pstream::combineScatter(UPstream::linearCommunication(), Value, tag);
    }
    else
    {
        Pstream::combineGather(UPstream::treeCommunication(), Value, cop, tag);
        Pstream::combineScatter(UPstream::treeCommunication(), Value, tag);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
