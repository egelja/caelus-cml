/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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

Class
    CML::Pstream

Description
    Inter-processor communications stream

SourceFiles
    Pstream.cpp
    gatherScatter.cpp
    combineGatherScatter.cpp
    gatherScatterList.cpp
    exchange.cpp

\*---------------------------------------------------------------------------*/

#ifndef Pstream_H
#define Pstream_H

#include "UPstream.hpp"
#include "DynamicList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class Pstream Declaration
\*---------------------------------------------------------------------------*/

class Pstream
:
    public UPstream
{

protected:

    // Protected data

        //- Transfer buffer
        DynamicList<char> buf_;

public:

    // Declare name of the class and its debug switch
    ClassName("Pstream");


    // Constructors

        //- Construct given optional buffer size
        Pstream
        (
            const commsTypes commsType,
            const label bufSize = 0
        )
        :
            UPstream(commsType),
            buf_(0)
        {
            if (bufSize)
            {
                buf_.setCapacity(bufSize + 2*sizeof(scalar) + 1);
            }
        }


        // Gather and scatter

            //- Gather data. Apply bop to combine Value
            //  from different processors
            template <class T, class BinaryOp>
            static void gather
            (
                const List<commsStruct>& comms,
                T& Value,
                const BinaryOp& bop,
                const int tag
            );

            //- Like above but switches between linear/tree communication
            template<class T, class BinaryOp>
            static void gather
            (
                T& Value,
                const BinaryOp& bop,
                const int tag = Pstream::msgType()
            );

            //- Scatter data. Distribute without modification. Reverse of gather
            template<class T>
            static void scatter
            (
                const List<commsStruct>& comms,
                T& Value,
                const int tag
            );

            //- Like above but switches between linear/tree communication
            template<class T>
            static void scatter
            (
                T& Value,
                const int tag = Pstream::msgType()
            );

        // Combine variants. Inplace combine values from processors.
        // (Uses construct from Istream instead of <<)

            template<class T, class CombineOp>
            static void combineGather
            (
                const List<commsStruct>& comms,
                T& Value,
                const CombineOp& cop,
                const int tag
            );

            //- Like above but switches between linear/tree communication
            template<class T, class CombineOp>
            static void combineGather
            (
                T& Value,
                const CombineOp& cop,
                const int tag = Pstream::msgType()
            );

            //- Scatter data. Reverse of combineGather
            template<class T>
            static void combineScatter
            (
                const List<commsStruct>& comms,
                T& Value,
                const int tag
            );

            //- Like above but switches between linear/tree communication
            template<class T>
            static void combineScatter
            (
                T& Value,
                const int tag = Pstream::msgType()
            );

        // Combine variants working on whole List at a time.

            template<class T, class CombineOp>
            static void listCombineGather
            (
                const List<commsStruct>& comms,
                List<T>& Value,
                const CombineOp& cop,
                const int tag
            );

            //- Like above but switches between linear/tree communication
            template<class T, class CombineOp>
            static void listCombineGather
            (
                List<T>& Value,
                const CombineOp& cop,
                const int tag = Pstream::msgType()
            );

            //- Scatter data. Reverse of combineGather
            template<class T>
            static void listCombineScatter
            (
                const List<commsStruct>& comms,
                List<T>& Value,
                const int tag
            );

            //- Like above but switches between linear/tree communication
            template<class T>
            static void listCombineScatter
            (
                List<T>& Value,
                const int tag = Pstream::msgType()
            );

        // Combine variants working on whole map at a time. Container needs to
        // have iterators and find() defined.

            template<class Container, class CombineOp>
            static void mapCombineGather
            (
                const List<commsStruct>& comms,
                Container& Values,
                const CombineOp& cop,
                const int tag
            );

            //- Like above but switches between linear/tree communication
            template<class Container, class CombineOp>
            static void mapCombineGather
            (
                Container& Values,
                const CombineOp& cop,
                const int tag = Pstream::msgType()
            );

            //- Scatter data. Reverse of combineGather
            template<class Container>
            static void mapCombineScatter
            (
                const List<commsStruct>& comms,
                Container& Values,
                const int tag
            );

            //- Like above but switches between linear/tree communication
            template<class Container>
            static void mapCombineScatter
            (
                Container& Values,
                const int tag = Pstream::msgType()
            );



        // Gather/scatter keeping the individual processor data separate.
        // Values is a List of size UPstream::nProcs() where
        // Values[UPstream::myProcNo()] is the data for the current processor.

            //- Gather data but keep individual values separate
            template<class T>
            static void gatherList
            (
                const List<commsStruct>& comms,
                List<T>& Values,
                const int tag
            );

            //- Like above but switches between linear/tree communication
            template<class T>
            static void gatherList
            (
                List<T>& Values,
                const int tag = Pstream::msgType()
            );

            //- Scatter data. Reverse of gatherList
            template<class T>
            static void scatterList
            (
                const List<commsStruct>& comms,
                List<T>& Values,
                const int tag
            );

            //- Like above but switches between linear/tree communication
            template<class T>
            static void scatterList
            (
                List<T>& Values,
                const int tag = Pstream::msgType()
            );


        // Exchange

            //- Helper: exchange contiguous data. Sends sendData, receives into
            //  recvData. If block=true will wait for all transfers to finish.
            template<class Container, class T>
            static void exchange
            (
                const UList<Container>& sendData,
                const labelUList& recvSizes,
                List<Container>& recvData,
                const int tag = UPstream::msgType(),
                const bool block = true
            );

            //- Helper: exchange sizes of sendData. sendData is the data per
            //  processor (in the communicator). Returns sizes of sendData
            //  on the sending processor.
            template<class Container>
            static void exchangeSizes
            (
                const Container& sendData,
                labelList& sizes
            );

            //- Exchange contiguous data. Sends sendData, receives into
            //  recvData. Determines sizes to receive.
            //  If block=true will wait for all transfers to finish.
            template<class Container, class T>
            static void exchange
            (
                const UList<Container>& sendData,
                List<Container>& recvData,
                const int tag = UPstream::msgType(),
                const bool block = true
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "UOPstream.hpp"
#include "OPstream.hpp"
#include "UIPstream.hpp"
#include "IPstream.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class T, class BinaryOp>
void Pstream::gather
(
    const List<UPstream::commsStruct>& comms,
    T& Value,
    const BinaryOp& bop,
    const int tag
)
{
    if (UPstream::parRun())
    {
        // Get my communication order
        const commsStruct& myComm = comms[UPstream::myProcNo()];

        // Receive from my downstairs neighbours
        forAll(myComm.below(), belowI)
        {
            T value;

            if (contiguous<T>())
            {
                UIPstream::read
                (
                    UPstream::scheduled,
                    myComm.below()[belowI],
                    reinterpret_cast<char*>(&value),
                    sizeof(T),
                    tag
                );
            }
            else
            {
                IPstream fromBelow
                (
                    UPstream::scheduled,
                    myComm.below()[belowI],
                    0,
                    tag
                );
                fromBelow >> value;
            }

            Value = bop(Value, value);
        }

        // Send up Value
        if (myComm.above() != -1)
        {
            if (contiguous<T>())
            {
                UOPstream::write
                (
                    UPstream::scheduled,
                    myComm.above(),
                    reinterpret_cast<const char*>(&Value),
                    sizeof(T),
                    tag
                );
            }
            else
            {
                OPstream toAbove
                (
                    UPstream::scheduled,
                    myComm.above(),
                    0,
                    tag
                );
                toAbove << Value;
            }
        }
    }
}


template <class T, class BinaryOp>
void Pstream::gather
(
    T& Value,
    const BinaryOp& bop,
    const int tag
)
{
    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        gather(UPstream::linearCommunication(), Value, bop, tag);
    }
    else
    {
        gather(UPstream::treeCommunication(), Value, bop, tag);
    }
}


template <class T>
void Pstream::scatter
(
    const List<UPstream::commsStruct>& comms,
    T& Value,
    const int tag
)
{
    if (UPstream::parRun())
    {
        // Get my communication order
        const commsStruct& myComm = comms[UPstream::myProcNo()];

        // Receive from up
        if (myComm.above() != -1)
        {
            if (contiguous<T>())
            {
                UIPstream::read
                (
                    UPstream::scheduled,
                    myComm.above(),
                    reinterpret_cast<char*>(&Value),
                    sizeof(T),
                    tag
                );
            }
            else
            {
                IPstream fromAbove
                (
                    UPstream::scheduled,
                    myComm.above(),
                    0,
                    tag
                );
                fromAbove >> Value;
            }
        }

        // Send to my downstairs neighbours. Note reverse order (compared to
        // receiving). This is to make sure to send to the critical path
        // (only when using a tree schedule!) first.
        forAllReverse(myComm.below(), belowI)
        {
            if (contiguous<T>())
            {
                UOPstream::write
                (
                    UPstream::scheduled,
                    myComm.below()[belowI],
                    reinterpret_cast<const char*>(&Value),
                    sizeof(T),
                    tag
                );
            }
            else
            {
                OPstream toBelow
                (
                    UPstream::scheduled,
                    myComm.below()[belowI],
                    0,
                    tag
                );
                toBelow << Value;
            }
        }
    }
}


template <class T>
void Pstream::scatter(T& Value, const int tag)
{
    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        scatter(UPstream::linearCommunication(), Value, tag);
    }
    else
    {
        scatter(UPstream::treeCommunication(), Value, tag);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML
#include "OPstream.hpp"
#include "IPstream.hpp"
#include "IOstreams.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class T, class CombineOp>
void Pstream::combineGather
(
    const List<UPstream::commsStruct>& comms,
    T& Value,
    const CombineOp& cop,
    const int tag
)
{
    if (UPstream::parRun())
    {
        // Get my communication order
        const commsStruct& myComm = comms[UPstream::myProcNo()];

        // Receive from my downstairs neighbours
        forAll(myComm.below(), belowI)
        {
            label belowID = myComm.below()[belowI];

            if (contiguous<T>())
            {
                T value;
                UIPstream::read
                (
                    UPstream::scheduled,
                    belowID,
                    reinterpret_cast<char*>(&value),
                    sizeof(T),
                    tag
                );

                if (debug & 2)
                {
                    Pout<< " received from "
                        << belowID << " data:" << value << endl;
                }

                cop(Value, value);
            }
            else
            {
                IPstream fromBelow
                (
                    UPstream::scheduled,
                    belowID,
                    0,
                    tag
                );
                T value(fromBelow);

                if (debug & 2)
                {
                    Pout<< " received from "
                        << belowID << " data:" << value << endl;
                }

                cop(Value, value);
            }
        }

        // Send up Value
        if (myComm.above() != -1)
        {
            if (debug & 2)
            {
                Pout<< " sending to " << myComm.above()
                    << " data:" << Value << endl;
            }

            if (contiguous<T>())
            {
                UOPstream::write
                (
                    UPstream::scheduled,
                    myComm.above(),
                    reinterpret_cast<const char*>(&Value),
                    sizeof(T),
                    tag
                );
            }
            else
            {
                OPstream toAbove
                (
                    UPstream::scheduled,
                    myComm.above(),
                    0,
                    tag
                );
                toAbove << Value;
            }
        }
    }
}


template <class T, class CombineOp>
void Pstream::combineGather
(
    T& Value,
    const CombineOp& cop,
    const int tag
)
{
    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        combineGather
        (
            UPstream::linearCommunication(),
            Value,
            cop,
            tag
        );
    }
    else
    {
        combineGather
        (
            UPstream::treeCommunication(),
            Value,
            cop,
            tag
        );
    }
}


template <class T>
void Pstream::combineScatter
(
    const List<UPstream::commsStruct>& comms,
    T& Value,
    const int tag
)
{
    if (UPstream::parRun())
    {
        // Get my communication order
        const UPstream::commsStruct& myComm = comms[UPstream::myProcNo()];

        // Receive from up
        if (myComm.above() != -1)
        {
            if (contiguous<T>())
            {
                UIPstream::read
                (
                    UPstream::scheduled,
                    myComm.above(),
                    reinterpret_cast<char*>(&Value),
                    sizeof(T),
                    tag
                );
            }
            else
            {
                IPstream fromAbove
                (
                    UPstream::scheduled,
                    myComm.above(),
                    0,
                    tag
                );
                Value = T(fromAbove);
            }

            if (debug & 2)
            {
                Pout<< " received from "
                    << myComm.above() << " data:" << Value << endl;
            }
        }

        // Send to my downstairs neighbours
        forAllReverse(myComm.below(), belowI)
        {
            label belowID = myComm.below()[belowI];

            if (debug & 2)
            {
                Pout<< " sending to " << belowID << " data:" << Value << endl;
            }

            if (contiguous<T>())
            {
                UOPstream::write
                (
                    UPstream::scheduled,
                    belowID,
                    reinterpret_cast<const char*>(&Value),
                    sizeof(T),
                    tag
                );
            }
            else
            {
                OPstream toBelow
                (
                    UPstream::scheduled,
                    belowID,
                    0,
                    tag
                );
                toBelow << Value;
            }
        }
    }
}


template <class T>
void Pstream::combineScatter
(
    T& Value,
    const int tag
)
{
    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        combineScatter(UPstream::linearCommunication(), Value, tag);
    }
    else
    {
        combineScatter(UPstream::treeCommunication(), Value, tag);
    }
}


template <class T, class CombineOp>
void Pstream::listCombineGather
(
    const List<UPstream::commsStruct>& comms,
    List<T>& Values,
    const CombineOp& cop,
    const int tag
)
{
    if (UPstream::parRun())
    {
        // Get my communication order
        const commsStruct& myComm = comms[UPstream::myProcNo()];

        // Receive from my downstairs neighbours
        forAll(myComm.below(), belowI)
        {
            label belowID = myComm.below()[belowI];

            if (contiguous<T>())
            {
                List<T> receivedValues(Values.size());

                UIPstream::read
                (
                    UPstream::scheduled,
                    belowID,
                    reinterpret_cast<char*>(receivedValues.begin()),
                    receivedValues.byteSize(),
                    tag
                );

                if (debug & 2)
                {
                    Pout<< " received from "
                        << belowID << " data:" << receivedValues << endl;
                }

                forAll(Values, i)
                {
                    cop(Values[i], receivedValues[i]);
                }
            }
            else
            {
                IPstream fromBelow
                (
                    UPstream::scheduled,
                    belowID,
                    0,
                    tag
                );
                List<T> receivedValues(fromBelow);

                if (debug & 2)
                {
                    Pout<< " received from "
                        << belowID << " data:" << receivedValues << endl;
                }

                forAll(Values, i)
                {
                    cop(Values[i], receivedValues[i]);
                }
            }
        }

        // Send up Value
        if (myComm.above() != -1)
        {
            if (debug & 2)
            {
                Pout<< " sending to " << myComm.above()
                    << " data:" << Values << endl;
            }

            if (contiguous<T>())
            {
                UOPstream::write
                (
                    UPstream::scheduled,
                    myComm.above(),
                    reinterpret_cast<const char*>(Values.begin()),
                    Values.byteSize(),
                    tag
                );
            }
            else
            {
                OPstream toAbove
                (
                    UPstream::scheduled,
                    myComm.above(),
                    0,
                    tag
                );
                toAbove << Values;
            }
        }
    }
}


template <class T, class CombineOp>
void Pstream::listCombineGather
(
    List<T>& Values,
    const CombineOp& cop,
    const int tag
)
{
    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        listCombineGather
        (
            UPstream::linearCommunication(),
            Values,
            cop,
            tag
        );
    }
    else
    {
        listCombineGather
        (
            UPstream::treeCommunication(),
            Values,
            cop,
            tag
        );
    }
}


template <class T>
void Pstream::listCombineScatter
(
    const List<UPstream::commsStruct>& comms,
    List<T>& Values,
    const int tag
)
{
    if (UPstream::parRun())
    {
        // Get my communication order
        const UPstream::commsStruct& myComm = comms[UPstream::myProcNo()];

        // Receive from up
        if (myComm.above() != -1)
        {
            if (contiguous<T>())
            {
                UIPstream::read
                (
                    UPstream::scheduled,
                    myComm.above(),
                    reinterpret_cast<char*>(Values.begin()),
                    Values.byteSize(),
                    tag
                );
            }
            else
            {
                IPstream fromAbove
                (
                    UPstream::scheduled,
                    myComm.above(),
                    0,
                    tag
                );
                fromAbove >> Values;
            }

            if (debug & 2)
            {
                Pout<< " received from "
                    << myComm.above() << " data:" << Values << endl;
            }
        }

        // Send to my downstairs neighbours
        forAllReverse(myComm.below(), belowI)
        {
            label belowID = myComm.below()[belowI];

            if (debug & 2)
            {
                Pout<< " sending to " << belowID << " data:" << Values << endl;
            }

            if (contiguous<T>())
            {
                UOPstream::write
                (
                    UPstream::scheduled,
                    belowID,
                    reinterpret_cast<const char*>(Values.begin()),
                    Values.byteSize(),
                    tag
                );
            }
            else
            {
                OPstream toBelow
                (
                    UPstream::scheduled,
                    belowID,
                    0,
                    tag
                );
                toBelow << Values;
            }
        }
    }
}


template <class T>
void Pstream::listCombineScatter
(
    List<T>& Values,
    const int tag
)
{
    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        listCombineScatter
        (
            UPstream::linearCommunication(),
            Values,
            tag
        );
    }
    else
    {
        listCombineScatter
        (
            UPstream::treeCommunication(),
            Values,
            tag
        );
    }
}


template <class Container, class CombineOp>
void Pstream::mapCombineGather
(
    const List<UPstream::commsStruct>& comms,
    Container& Values,
    const CombineOp& cop,
    const int tag
)
{
    if (UPstream::parRun())
    {
        // Get my communication order
        const commsStruct& myComm = comms[UPstream::myProcNo()];

        // Receive from my downstairs neighbours
        forAll(myComm.below(), belowI)
        {
            label belowID = myComm.below()[belowI];

            IPstream fromBelow
            (
                UPstream::scheduled,
                belowID,
                0,
                tag
            );
            Container receivedValues(fromBelow);

            if (debug & 2)
            {
                Pout<< " received from "
                    << belowID << " data:" << receivedValues << endl;
            }

            for
            (
                typename Container::const_iterator slaveIter =
                    receivedValues.begin();
                slaveIter != receivedValues.end();
                ++slaveIter
            )
            {
                typename Container::iterator
                    masterIter = Values.find(slaveIter.key());

                if (masterIter != Values.end())
                {
                    cop(masterIter(), slaveIter());
                }
                else
                {
                    Values.insert(slaveIter.key(), slaveIter());
                }
            }
        }

        // Send up Value
        if (myComm.above() != -1)
        {
            if (debug & 2)
            {
                Pout<< " sending to " << myComm.above()
                    << " data:" << Values << endl;
            }

            OPstream toAbove
            (
                UPstream::scheduled,
                myComm.above(),
                0,
                tag
            );
            toAbove << Values;
        }
    }
}


template <class Container, class CombineOp>
void Pstream::mapCombineGather
(
    Container& Values,
    const CombineOp& cop,
    const int tag
)
{
    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        mapCombineGather
        (
            UPstream::linearCommunication(),
            Values,
            cop,
            tag
        );
    }
    else
    {
        mapCombineGather
        (
            UPstream::treeCommunication(),
            Values,
            cop,
            tag
        );
    }
}


template <class Container>
void Pstream::mapCombineScatter
(
    const List<UPstream::commsStruct>& comms,
    Container& Values,
    const int tag
)
{
    if (UPstream::parRun())
    {
        // Get my communication order
        const UPstream::commsStruct& myComm = comms[UPstream::myProcNo()];

        // Receive from up
        if (myComm.above() != -1)
        {
            IPstream fromAbove
            (
                UPstream::scheduled,
                myComm.above(),
                0,
                tag
            );
            fromAbove >> Values;

            if (debug & 2)
            {
                Pout<< " received from "
                    << myComm.above() << " data:" << Values << endl;
            }
        }

        // Send to my downstairs neighbours
        forAllReverse(myComm.below(), belowI)
        {
            label belowID = myComm.below()[belowI];

            if (debug & 2)
            {
                Pout<< " sending to " << belowID << " data:" << Values << endl;
            }

            OPstream toBelow
            (
                UPstream::scheduled,
                belowID,
                0,
                tag
            );
            toBelow << Values;
        }
    }
}


template <class Container>
void Pstream::mapCombineScatter
(
    Container& Values,
    const int tag
)
{
    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        mapCombineScatter
        (
            UPstream::linearCommunication(),
            Values,
            tag
        );
    }
    else
    {
        mapCombineScatter
        (
            UPstream::treeCommunication(),
            Values,
            tag
        );
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML
#include "IPstream.hpp"
#include "OPstream.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class T>
void Pstream::gatherList
(
    const List<UPstream::commsStruct>& comms,
    List<T>& Values,
    const int tag
)
{
    if (UPstream::parRun())
    {
        if (Values.size() != UPstream::nProcs())
        {
            FatalErrorInFunction
                << "Size of list:" << Values.size()
                << " does not equal the number of processors:"
                << UPstream::nProcs()
                << CML::abort(FatalError);
        }

        // Get my communication order
        const commsStruct& myComm = comms[UPstream::myProcNo()];

        // Receive from my downstairs neighbours
        forAll(myComm.below(), belowI)
        {
            label belowID = myComm.below()[belowI];
            const labelList& belowLeaves = comms[belowID].allBelow();

            if (contiguous<T>())
            {
                List<T> receivedValues(belowLeaves.size() + 1);

                UIPstream::read
                (
                    UPstream::scheduled,
                    belowID,
                    reinterpret_cast<char*>(receivedValues.begin()),
                    receivedValues.byteSize(),
                    tag
                );

                Values[belowID] = receivedValues[0];

                forAll(belowLeaves, leafI)
                {
                    Values[belowLeaves[leafI]] = receivedValues[leafI + 1];
                }
            }
            else
            {
                IPstream fromBelow
                (
                    UPstream::scheduled,
                    belowID,
                    0,
                    tag
                );
                fromBelow >> Values[belowID];

                if (debug & 2)
                {
                    Pout<< " received through "
                        << belowID << " data from:" << belowID
                        << " data:" << Values[belowID] << endl;
                }

                // Receive from all other processors below belowID
                forAll(belowLeaves, leafI)
                {
                    label leafID = belowLeaves[leafI];
                    fromBelow >> Values[leafID];

                    if (debug & 2)
                    {
                        Pout<< " received through "
                            << belowID << " data from:" << leafID
                            << " data:" << Values[leafID] << endl;
                    }
                }
            }
        }

        // Send up from Values:
        // - my own value first
        // - all belowLeaves next
        if (myComm.above() != -1)
        {
            const labelList& belowLeaves = myComm.allBelow();

            if (debug & 2)
            {
                Pout<< " sending to " << myComm.above()
                    << " data from me:" << UPstream::myProcNo()
                    << " data:" << Values[UPstream::myProcNo()] << endl;
            }

            if (contiguous<T>())
            {
                List<T> sendingValues(belowLeaves.size() + 1);
                sendingValues[0] = Values[UPstream::myProcNo()];

                forAll(belowLeaves, leafI)
                {
                    sendingValues[leafI + 1] = Values[belowLeaves[leafI]];
                }

                OPstream::write
                (
                    UPstream::scheduled,
                    myComm.above(),
                    reinterpret_cast<const char*>(sendingValues.begin()),
                    sendingValues.byteSize(),
                    tag
                );
            }
            else
            {
                OPstream toAbove
                (
                    UPstream::scheduled,
                    myComm.above(),
                    0,
                    tag
                );
                toAbove << Values[UPstream::myProcNo()];

                forAll(belowLeaves, leafI)
                {
                    label leafID = belowLeaves[leafI];

                    if (debug & 2)
                    {
                        Pout<< " sending to "
                            << myComm.above() << " data from:" << leafID
                            << " data:" << Values[leafID] << endl;
                    }
                    toAbove << Values[leafID];
                }
            }
        }
    }
}


template <class T>
void Pstream::gatherList(List<T>& Values, const int tag)
{
    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        gatherList(UPstream::linearCommunication(), Values, tag);
    }
    else
    {
        gatherList(UPstream::treeCommunication(), Values, tag);
    }
}


template <class T>
void Pstream::scatterList
(
    const List<UPstream::commsStruct>& comms,
    List<T>& Values,
    const int tag
)
{
    if (UPstream::parRun())
    {
        if (Values.size() != UPstream::nProcs())
        {
            FatalErrorInFunction
                << "Size of list:" << Values.size()
                << " does not equal the number of processors:"
                << UPstream::nProcs()
                << CML::abort(FatalError);
        }

        // Get my communication order
        const commsStruct& myComm = comms[UPstream::myProcNo()];

        // Receive from up
        if (myComm.above() != -1)
        {
            const labelList& notBelowLeaves = myComm.allNotBelow();

            if (contiguous<T>())
            {
                List<T> receivedValues(notBelowLeaves.size());

                UIPstream::read
                (
                    UPstream::scheduled,
                    myComm.above(),
                    reinterpret_cast<char*>(receivedValues.begin()),
                    receivedValues.byteSize(),
                    tag
                );

                forAll(notBelowLeaves, leafI)
                {
                    Values[notBelowLeaves[leafI]] = receivedValues[leafI];
                }
            }
            else
            {
                IPstream fromAbove
                (
                    UPstream::scheduled,
                    myComm.above(),
                    0,
                    tag
                );

                forAll(notBelowLeaves, leafI)
                {
                    label leafID = notBelowLeaves[leafI];
                    fromAbove >> Values[leafID];

                    if (debug)
                    {
                        Pout<< " received through "
                            << myComm.above() << " data for:" << leafID
                            << " data:" << Values[leafID] << endl;
                    }
                }
            }
        }

        // Send to my downstairs neighbours
        forAllReverse(myComm.below(), belowI)
        {
            label belowID = myComm.below()[belowI];
            const labelList& notBelowLeaves = comms[belowID].allNotBelow();

            if (contiguous<T>())
            {
                List<T> sendingValues(notBelowLeaves.size());

                forAll(notBelowLeaves, leafI)
                {
                    sendingValues[leafI] = Values[notBelowLeaves[leafI]];
                }

                OPstream::write
                (
                    UPstream::scheduled,
                    belowID,
                    reinterpret_cast<const char*>(sendingValues.begin()),
                    sendingValues.byteSize(),
                    tag
                );
            }
            else
            {
                OPstream toBelow
                (
                    UPstream::scheduled,
                    belowID,
                    0,
                    tag
                );

                // Send data destined for all other processors below belowID
                forAll(notBelowLeaves, leafI)
                {
                    label leafID = notBelowLeaves[leafI];
                    toBelow << Values[leafID];

                    if (debug)
                    {
                        Pout<< " sent through "
                            << belowID << " data for:" << leafID
                            << " data:" << Values[leafID] << endl;
                    }
                }
            }
        }
    }
}


template <class T>
void Pstream::scatterList(List<T>& Values, const int tag)
{
    if (UPstream::nProcs() < UPstream::nProcsSimpleSum)
    {
        scatterList(UPstream::linearCommunication(), Values, tag);
    }
    else
    {
        scatterList(UPstream::treeCommunication(), Values, tag);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "contiguous.hpp"
#include "PstreamCombineReduceOps.hpp"
#include "UPstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class Container, class T>
void Pstream::exchange
(
    const UList<Container>& sendBufs,
    const labelUList& recvSizes,
    List<Container>& recvBufs,
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

    recvBufs.setSize(sendBufs.size());

    recvBufs.setSize(sendBufs.size());

    if (Pstream::parRun())
    {
        label startOfRequests = Pstream::nRequests();

        // Set up receives
        // ~~~~~~~~~~~~~~~

        forAll(recvSizes, proci)
        {
            label nRecv = recvSizes[proci];

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


template<class Container>
void Pstream::exchangeSizes
(
    const Container& sendBufs,
    labelList& recvSizes
)
{
    if (sendBufs.size() != UPstream::nProcs())
    {
        FatalErrorInFunction
            << "Size of container " << sendBufs.size()
            << " does not equal the number of processors "
            << UPstream::nProcs()
            << CML::abort(FatalError);
    }

    labelList sendSizes(sendBufs.size());
    forAll(sendBufs, proci)
    {
        sendSizes[proci] = sendBufs[proci].size();
    }
    recvSizes.setSize(sendSizes.size());
    allToAll(sendSizes, recvSizes);
}


template<class Container, class T>
void Pstream::exchange
(
    const UList<Container>& sendBufs,
    List<Container>& recvBufs,
    const int tag,
    const bool block
)
{
    labelList recvSizes;
    exchangeSizes(sendBufs, recvSizes);

    exchange<Container, T>(sendBufs, recvSizes, recvBufs, tag, block);
}


} // End namespace CML

// ************************************************************************* //
#endif
