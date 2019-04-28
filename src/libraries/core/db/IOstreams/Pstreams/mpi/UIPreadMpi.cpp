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
    Read from UIPstream

\*---------------------------------------------------------------------------*/

#include "mpi.h"

#include "UIPstream.hpp"
#include "PstreamGlobalsMpi.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * //

CML::UIPstream::UIPstream
(
    const commsTypes commsType,
    const int fromProcNo,
    DynamicList<char>& externalBuf,
    label& externalBufPosition,
    const int tag,
    const bool clearAtEnd,
    streamFormat format,
    versionNumber version
)
:
    UPstream(commsType),
    Istream(format, version),
    fromProcNo_(fromProcNo),
    externalBuf_(externalBuf),
    externalBufPosition_(externalBufPosition),
    tag_(tag),
    clearAtEnd_(clearAtEnd),
    messageSize_(0)
{
    setOpened();
    setGood();

    if (commsType == UPstream::nonBlocking)
    {
        // Message is already received into externalBuf
    }
    else
    {
        MPI_Status status;

        label wantedSize = externalBuf_.capacity();

        if (debug)
        {
            Pout<< "UIPstream::UIPstream : read from:" << fromProcNo
                << " tag:" << tag << " wanted size:" << wantedSize
                << CML::endl;
        }


        // If the buffer size is not specified, probe the incoming message
        // and set it
        if (!wantedSize)
        {
//            MPI_Probe(procID(fromProcNo_), tag_, PstreamGlobals::MPICommunicators_[1], &status);
			MPI_Probe(procID(fromProcNo_), tag_, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_BYTE, &messageSize_);

            externalBuf_.setCapacity(messageSize_);
            wantedSize = messageSize_;

            if (debug)
            {
                Pout<< "UIPstream::UIPstream : probed size:" << wantedSize
                    << CML::endl;
            }
        }

        messageSize_ = UIPstream::read
        (
            commsType,
            fromProcNo_,
            externalBuf_.begin(),
            wantedSize,
            tag_
        );

        // Set addressed size. Leave actual allocated memory intact.
        externalBuf_.setSize(messageSize_);

        if (!messageSize_)
        {
            setEof();
        }
    }
}


CML::UIPstream::UIPstream(const int fromProcNo, PstreamBuffers& buffers)
:
    UPstream(buffers.commsType_),
    Istream(buffers.format_, buffers.version_),
    fromProcNo_(fromProcNo),
    externalBuf_(buffers.recvBuf_[fromProcNo]),
    externalBufPosition_(buffers.recvBufPos_[fromProcNo]),
    tag_(buffers.tag_),
    clearAtEnd_(true),
    messageSize_(0)
{
    if (commsType() != UPstream::scheduled && !buffers.finishedSendsCalled_)
    {
        FatalErrorInFunction
            << "PstreamBuffers::finishedSends() never called." << endl
            << "Please call PstreamBuffers::finishedSends() after doing"
            << " all your sends (using UOPstream) and before doing any"
            << " receives (using UIPstream)" << CML::exit(FatalError);
    }

    setOpened();
    setGood();

    if (commsType() == UPstream::nonBlocking)
    {
        // Message is already received into externalBuf
        messageSize_ = buffers.recvBuf_[fromProcNo].size();

        if (debug)
        {
            Pout<< "UIPstream::UIPstream PstreamBuffers :"
                << " fromProcNo:" << fromProcNo
                << " tag:" << tag_ << " receive buffer size:" << messageSize_
                << CML::endl;
        }
    }
    else
    {
        MPI_Status status;

        label wantedSize = externalBuf_.capacity();

        if (debug)
        {
            Pout<< "UIPstream::UIPstream PstreamBuffers :"
                << " read from:" << fromProcNo
                << " tag:" << tag_ << " wanted size:" << wantedSize
                << CML::endl;
        }

        // If the buffer size is not specified, probe the incoming message
        // and set it
        if (!wantedSize)
        {
//            MPI_Probe(procID(fromProcNo_), tag_, PstreamGlobals::MPICommunicators_[1], &status);
			MPI_Probe(procID(fromProcNo_), tag_, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_BYTE, &messageSize_);

            externalBuf_.setCapacity(messageSize_);
            wantedSize = messageSize_;

            if (debug)
            {
                Pout<< "UIPstream::UIPstream PstreamBuffers : probed size:"
                    << wantedSize << CML::endl;
            }
        }

        messageSize_ = UIPstream::read
        (
            commsType(),
            fromProcNo_,
            externalBuf_.begin(),
            wantedSize,
            tag_
        );

        // Set addressed size. Leave actual allocated memory intact.
        externalBuf_.setSize(messageSize_);

        if (!messageSize_)
        {
            setEof();
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::label CML::UIPstream::read
(
    const commsTypes commsType,
    const int fromProcNo,
    char* buf,
    const std::streamsize bufSize,
    const int tag
)
{
    if (debug)
    {
        Pout<< "UIPstream::read : starting read from:" << fromProcNo
            << " tag:" << tag << " wanted size:" << label(bufSize)
            << " commsType:" << UPstream::commsTypeNames[commsType]
            << CML::endl;
    }

    if (commsType == blocking || commsType == scheduled)
    {
        MPI_Status status;

        if
        (
            MPI_Recv
            (
                buf,
                bufSize,
                MPI_BYTE,
                procID(fromProcNo),
                tag,
                MPI_COMM_WORLD,
//                PstreamGlobals::MPICommunicators_[1],
                &status
            )
        )
        {
            FatalErrorInFunction
                << "MPI_Recv cannot receive incoming message"
                << CML::abort(FatalError);

            return 0;
        }


        // Check size of message read

        int messageSize;
        MPI_Get_count(&status, MPI_BYTE, &messageSize);

        if (debug)
        {
            Pout<< "UIPstream::read : finished read from:" << fromProcNo
                << " tag:" << tag << " read size:" << label(bufSize)
                << " commsType:" << UPstream::commsTypeNames[commsType]
                << CML::endl;
        }

        if (messageSize > bufSize)
        {
            FatalErrorInFunction
                << "buffer (" << label(bufSize)
                << ") not large enough for incoming message ("
                << messageSize << ')'
                << CML::abort(FatalError);
        }

        return messageSize;
    }
    else if (commsType == nonBlocking)
    {
        MPI_Request request;

        if
        (
            MPI_Irecv
            (
                buf,
                bufSize,
                MPI_BYTE,
                procID(fromProcNo),
                tag,
                MPI_COMM_WORLD,
//                PstreamGlobals::MPICommunicators_[1],
                &request
            )
        )
        {
            FatalErrorInFunction
                << "MPI_Recv cannot start non-blocking receive"
                << CML::abort(FatalError);

            return 0;
        }

        if (debug)
        {
            Pout<< "UIPstream::read : started read from:" << fromProcNo
                << " tag:" << tag << " read size:" << label(bufSize)
                << " commsType:" << UPstream::commsTypeNames[commsType]
                << " request:" << PstreamGlobals::outstandingRequests_.size()
                << CML::endl;
        }

        PstreamGlobals::outstandingRequests_.append(request);

        // Assume the message is completely received.
        return bufSize;
    }
    else
    {
        FatalErrorInFunction
            << "Unsupported communications type "
            << commsType
            << CML::abort(FatalError);

        return 0;
    }
}


// ************************************************************************* //
