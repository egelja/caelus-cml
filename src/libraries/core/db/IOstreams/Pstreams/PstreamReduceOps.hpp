/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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

#ifndef PstreamReduceOps_H
#define PstreamReduceOps_H

#include "Pstream.hpp"
#include "ops.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Reduce operation with user specified communication schedule
template <class T, class BinaryOp>
void reduce
(
    const List<UPstream::commsStruct>& comms,
    T& Value,
    const BinaryOp& bop,
    const int tag
)
{
    Pstream::gather(comms, Value, bop, tag);
    Pstream::scatter(comms, Value, tag);
}


// Reduce using either linear or tree communication schedule
template <class T, class BinaryOp>
void reduce
(
    T& Value,
    const BinaryOp& bop,
    const int tag = Pstream::msgType()
)
{
    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        reduce(UPstream::linearCommunication(), Value, bop, tag);
    }
    else
    {
        reduce(UPstream::treeCommunication(), Value, bop, tag);
    }
}


// Reduce using either linear or tree communication schedule
template <class T, class BinaryOp>
T returnReduce
(
    const T& Value,
    const BinaryOp& bop,
    const int tag = Pstream::msgType()
)
{
    T WorkValue(Value);

    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        reduce(UPstream::linearCommunication(), WorkValue, bop, tag);
    }
    else
    {
        reduce(UPstream::treeCommunication(), WorkValue, bop, tag);
    }

    return WorkValue;
}


// Insist there is a specialisation for the reduction of a scalar
void reduce
(
    scalar& Value,
    const sumOp<scalar>& bop,
    const int tag = Pstream::msgType()
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
