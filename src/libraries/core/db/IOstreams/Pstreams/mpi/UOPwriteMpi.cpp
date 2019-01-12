/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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
    Write primitive and binary block from OPstream

\*---------------------------------------------------------------------------*/

#include "mpi.h"

#include "UOPstream.hpp"
#include "PstreamGlobalsMpi.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::UOPstream::write
(
    const commsTypes commsType,
    const int toProcNo,
    const char* buf,
    const std::streamsize bufSize,
    const int tag
)
{
    if (debug)
    {
        Pout<< "UOPstream::write : starting write to:" << toProcNo
            << " tag:" << tag << " size:" << label(bufSize)
            << " commsType:" << UPstream::commsTypeNames[commsType]
            << CML::endl;
    }

    bool transferFailed = true;

    if (commsType == blocking)
    {
        transferFailed = MPI_Bsend
        (
            const_cast<char*>(buf),
            bufSize,
            MPI_PACKED,
            procID(toProcNo),
            tag,
			MPI_COMM_WORLD
//            PstreamGlobals::MPICommunicators_[1]
        );

        if (debug)
        {
            Pout<< "UOPstream::write : finished write to:" << toProcNo
                << " tag:" << tag << " size:" << label(bufSize)
                << " commsType:" << UPstream::commsTypeNames[commsType]
                << CML::endl;
        }
    }
    else if (commsType == scheduled)
    {
        transferFailed = MPI_Send
        (
            const_cast<char*>(buf),
            bufSize,
            MPI_PACKED,
            procID(toProcNo),
            tag,
			MPI_COMM_WORLD
//            PstreamGlobals::MPICommunicators_[1]
        );

        if (debug)
        {
            Pout<< "UOPstream::write : finished write to:" << toProcNo
                << " tag:" << tag << " size:" << label(bufSize)
                << " commsType:" << UPstream::commsTypeNames[commsType]
                << CML::endl;
        }
    }
    else if (commsType == nonBlocking)
    {
        MPI_Request request;

        transferFailed = MPI_Isend
        (
            const_cast<char*>(buf),
            bufSize,
            MPI_PACKED,
            procID(toProcNo),
            tag,
			MPI_COMM_WORLD,
//            PstreamGlobals::MPICommunicators_[1],
            &request
        );

        if (debug)
        {
            Pout<< "UOPstream::write : started write to:" << toProcNo
                << " tag:" << tag << " size:" << label(bufSize)
                << " commsType:" << UPstream::commsTypeNames[commsType]
                << " request:" << PstreamGlobals::outstandingRequests_.size()
                << CML::endl;
        }

        PstreamGlobals::outstandingRequests_.append(request);
    }
    else
    {
        FatalErrorInFunction
            << "Unsupported communications type "
            << UPstream::commsTypeNames[commsType]
            << CML::abort(FatalError);
    }

    return !transferFailed;
}


// ************************************************************************* //
