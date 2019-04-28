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

#include "mpi.h"

#include "UPstream.hpp"
#include "PstreamReduceOps.hpp"
#include "OSspecific.hpp"
#include "PstreamGlobalsMpi.hpp"
#include "SubList.hpp"

#include <cstring>
#include <cstdlib>
#include <csignal>

#if defined(WM_SP)
#   define MPI_SCALAR MPI_FLOAT
#elif defined(WM_DP)
#   define MPI_SCALAR MPI_DOUBLE
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// NOTE:
// valid parallel options vary between implementations, but flag common ones.
// if they are not removed by MPI_Init(), the subsequent argument processing
// will notice that they are wrong
void CML::UPstream::addValidParOptions(HashTable<string>& validParOptions)
{
    validParOptions.insert("np", "");
    validParOptions.insert("p4pg", "PI file");
    validParOptions.insert("p4wd", "directory");
    validParOptions.insert("p4amslave", "");
    validParOptions.insert("p4yourname", "hostname");
    validParOptions.insert("machinefile", "machine file");
}


bool CML::UPstream::init(int& argc, char**& argv)
{

//    int n_user_procs = 0;

//    string npString("-np_user");

//    for (label i = 0; i < argc; i++)
//    {
//        if (argv[i] == npString)
//        {
//            if (i+1 < argc)
//            {
//                n_user_procs = atoi(argv[i+1]);
//                break;
//            }
//        }
//    }

    MPI_Init(&argc, &argv);

    int numprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myProcNo_);

    if (debug)
    {
        Pout<< "UPstream::init : initialised with numProcs:" << numprocs
            << " myProcNo:" << myProcNo_ << endl;
    }

    if (numprocs <= 1)
    {
        FatalErrorInFunction
            << "bool IPstream::init(int& argc, char**& argv) : "
               "attempt to run parallel on 1 processor"
            << CML::abort(FatalError);
    }

	procIDs_.setSize(numprocs);

	forAll(procIDs_, procNo)
	{
	    procIDs_[procNo] = procNo;
	}

    setParRun();

//    int numProcsWorld;
//    MPI_Comm_size(MPI_COMM_WORLD, &numProcsWorld);
//    MPI_Comm_rank(MPI_COMM_WORLD, &myProcNo_);

//    // Set first communicator as  COMM_WORLD
//    PstreamGlobals::MPICommunicators_.setSize(2);
//    PstreamGlobals::MPICommunicators_[0] = MPI_COMM_WORLD;

//	int n_caelus_procs = numProcsWorld - n_user_procs;

//    if (n_caelus_procs < 2)
//    {
//        FatalErrorInFunction
//            << "bool IPstream::init(int& argc, char**& argv) : "
//               "not enough Caelus processors"
//            << CML::abort(FatalError);
//    }

//	MPI_Comm MPI_COMM_CAELUS;

//	int colour = 0;
//	if(myProcNo_ >= n_caelus_procs)
//	{
//		// User processors
//		colour = 1;
//	}

//    int numprocs;	
//	MPI_Comm_split(MPI_COMM_WORLD, colour, myProcNo_, &MPI_COMM_CAELUS);
//    MPI_Comm_size(MPI_COMM_CAELUS, &numprocs);
    // Set first communicator as  COMM_WORLD
//    PstreamGlobals::MPICommunicators_[1] = MPI_COMM_CAELUS;


#   ifndef SGIMPI
	string bufferSizeName = getEnv("MPI_BUFFER_SIZE");

    if (bufferSizeName.size())
    {
        int bufferSize = atoi(bufferSizeName.c_str());

        if (bufferSize)
        {
            MPI_Buffer_attach(new char[bufferSize], bufferSize);
        }
    }
    else
    {
        FatalErrorInFunction
            << "UPstream::init(int& argc, char**& argv) : "
            << "environment variable MPI_BUFFER_SIZE not defined"
            << CML::abort(FatalError);
    }
#   endif

	int processorNameLen;
    char processorName[MPI_MAX_PROCESSOR_NAME];

    MPI_Get_processor_name(processorName, &processorNameLen);

    // Now that nprocs is known construct communication tables.	
    initCommunicationSchedule();

    return true;
}


void CML::UPstream::exit(int errnum)
{
    if (debug)
    {
        Pout<< "UPstream::exit." << endl;
    }

#   ifndef SGIMPI
    int size;
    char* buff;
    MPI_Buffer_detach(&buff, &size);
    delete[] buff;
#   endif

    if (PstreamGlobals::outstandingRequests_.size())
    {
        label n = PstreamGlobals::outstandingRequests_.size();
        PstreamGlobals::outstandingRequests_.clear();

        WarningInFunction
            << "There are still " << n << " outstanding MPI_Requests." << endl
            << "This means that your code exited before doing a"
            << " UPstream::waitRequests()." << endl
            << "This should not happen for a normal code exit."
            << endl;
    }

    if (errnum == 0)
    {
        MPI_Finalize();
        ::exit(errnum);
    }
    else
    {
        MPI_Abort(MPI_COMM_WORLD, errnum);
//        MPI_Abort(PstreamGlobals::MPICommunicators_[1], errnum);
    }
}


void CML::UPstream::abort()
{
    MPI_Abort(MPI_COMM_WORLD, 1);
//    MPI_Abort(PstreamGlobals::MPICommunicators_[1], 1);
}


void CML::reduce(scalar& Value, const sumOp<scalar>& bop, const int tag)
{
    if (Pstream::debug)
    {
        Pout<< "CML::reduce : value:" << Value << endl;
    }

    if (!UPstream::parRun())
    {
        return;
    }

    if (UPstream::nProcs() <= UPstream::nProcsSimpleSum)
    {
        if (UPstream::master())
        {
            for
            (
                int slave=UPstream::firstSlave();
                slave<=UPstream::lastSlave();
                slave++
            )
            {
                scalar value;

                if
                (
                    MPI_Recv
                    (
                        &value,
                        1,
                        MPI_SCALAR,
                        UPstream::procID(slave),
                        tag,
                        MPI_COMM_WORLD,
//                        PstreamGlobals::MPICommunicators_[1],
                        MPI_STATUS_IGNORE
                    )
                )
                {
                    FatalErrorInFunction
                        << "MPI_Recv failed"
                        << CML::abort(FatalError);
                }

                Value = bop(Value, value);
            }
        }
        else
        {
            if
            (
                MPI_Send
                (
                    &Value,
                    1,
                    MPI_SCALAR,
                    UPstream::procID(UPstream::masterNo()),
                    tag,
                    MPI_COMM_WORLD
//                    PstreamGlobals::MPICommunicators_[1]
                )
            )
            {
                FatalErrorInFunction
                    << "MPI_Send failed"
                    << CML::abort(FatalError);
            }
        }


        if (UPstream::master())
        {
            for
            (
                int slave=UPstream::firstSlave();
                slave<=UPstream::lastSlave();
                slave++
            )
            {
                if
                (
                    MPI_Send
                    (
                        &Value,
                        1,
                        MPI_SCALAR,
                        UPstream::procID(slave),
                        tag,
                        MPI_COMM_WORLD
//                        PstreamGlobals::MPICommunicators_[1]
                    )
                )
                {
                    FatalErrorInFunction
                        << "MPI_Send failed"
                        << CML::abort(FatalError);
                }
            }
        }
        else
        {
            if
            (
                MPI_Recv
                (
                    &Value,
                    1,
                    MPI_SCALAR,
                    UPstream::procID(UPstream::masterNo()),
                    tag,
                    MPI_COMM_WORLD,
//                    PstreamGlobals::MPICommunicators_[1],
                    MPI_STATUS_IGNORE
                )
            )
            {
                FatalErrorInFunction
                    << "MPI_Recv failed"
                    << CML::abort(FatalError);
            }
        }
    }
    else
    {
        scalar sum;
        MPI_Allreduce(&Value, &sum, 1, MPI_SCALAR, MPI_SUM, MPI_COMM_WORLD);
//        MPI_Allreduce(&Value, &sum, 1, MPI_SCALAR, MPI_SUM, PstreamGlobals::MPICommunicators_[1]);
        Value = sum;

        /*
        int myProcNo = UPstream::myProcNo();
        int nProcs = UPstream::nProcs();

        //
        // receive from children
        //
        int level = 1;
        int thisLevelOffset = 2;
        int childLevelOffset = thisLevelOffset/2;
        int childProcId = 0;

        while
        (
            (childLevelOffset < nProcs)
         && (myProcNo % thisLevelOffset) == 0
        )
        {
            childProcId = myProcNo + childLevelOffset;

            scalar value;

            if (childProcId < nProcs)
            {
                if
                (
                    MPI_Recv
                    (
                        &value,
                        1,
                        MPI_SCALAR,
                        UPstream::procID(childProcId),
                        tag,
                        MPI_COMM_WORLD,
                        MPI_STATUS_IGNORE
                    )
                )
                {
                    FatalErrorInFunction
                        << "MPI_Recv failed"
                        << CML::abort(FatalError);
                }

                Value = bop(Value, value);
            }

            level++;
            thisLevelOffset <<= 1;
            childLevelOffset = thisLevelOffset/2;
        }

        //
        // send and receive from parent
        //
        if (!UPstream::master())
        {
            int parentId = myProcNo - (myProcNo % thisLevelOffset);

            if
            (
                MPI_Send
                (
                    &Value,
                    1,
                    MPI_SCALAR,
                    UPstream::procID(parentId),
                    tag,
                    MPI_COMM_WORLD
                )
            )
            {
                FatalErrorInFunction
                    << "MPI_Send failed"
                    << CML::abort(FatalError);
            }

            if
            (
                MPI_Recv
                (
                    &Value,
                    1,
                    MPI_SCALAR,
                    UPstream::procID(parentId),
                    tag,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE
                )
            )
            {
                FatalErrorInFunction
                    << "MPI_Recv failed"
                    << CML::abort(FatalError);
            }
        }


        //
        // distribute to my children
        //
        level--;
        thisLevelOffset >>= 1;
        childLevelOffset = thisLevelOffset/2;

        while (level > 0)
        {
            childProcId = myProcNo + childLevelOffset;

            if (childProcId < nProcs)
            {
                if
                (
                    MPI_Send
                    (
                        &Value,
                        1,
                        MPI_SCALAR,
                        UPstream::procID(childProcId),
                        tag,
                        MPI_COMM_WORLD
                    )
                )
                {
                    FatalErrorInFunction
                        << "MPI_Send failed"
                        << CML::abort(FatalError);
                }
            }

            level--;
            thisLevelOffset >>= 1;
            childLevelOffset = thisLevelOffset/2;
        }
        */
    }

    if (Pstream::debug)
    {
        Pout<< "CML::reduce : reduced value:" << Value << endl;
    }
}

void CML::UPstream::allToAll
(
    const labelUList& sendData,
    labelUList& recvData
)
{
    label np = nProcs();

    if (sendData.size() != np || recvData.size() != np)
    {
        FatalErrorInFunction
            << "Size of sendData " << sendData.size()
            << " or size of recvData " << recvData.size()
            << " is not equal to the number of processors in the domain "
            << np
            << CML::abort(FatalError);
    }

    if (!UPstream::parRun())
    {
        recvData.assign(sendData);
    }
    else
    {
        if
        (
            MPI_Alltoall
            (
                const_cast<label*>(sendData.begin()),
                sizeof(label),
                MPI_BYTE,
                recvData.begin(),
                sizeof(label),
                MPI_BYTE,
                MPI_COMM_WORLD
            )
        )
        {
            FatalErrorInFunction
                << "MPI_Alltoall failed for " << sendData
                << " on communicator " << MPI_COMM_WORLD
                << CML::abort(FatalError);
        }
    }
}

CML::label CML::UPstream::nRequests()
{
    return PstreamGlobals::outstandingRequests_.size();
}


void CML::UPstream::resetRequests(const label i)
{
    if (i < PstreamGlobals::outstandingRequests_.size())
    {
        PstreamGlobals::outstandingRequests_.setSize(i);
    }
}


void CML::UPstream::waitRequests(const label start)
{
    if (debug)
    {
        Pout<< "UPstream::waitRequests : starting wait for "
            << PstreamGlobals::outstandingRequests_.size()-start
            << " outstanding requests starting at " << start << endl;
    }

    if (PstreamGlobals::outstandingRequests_.size())
    {
        SubList<MPI_Request> waitRequests
        (
            PstreamGlobals::outstandingRequests_,
            PstreamGlobals::outstandingRequests_.size() - start,
            start
        );

        if
        (
            MPI_Waitall
            (
                waitRequests.size(),
                waitRequests.begin(),
                MPI_STATUSES_IGNORE
            )
        )
        {
            FatalErrorInFunction
                << "MPI_Waitall returned with error" << CML::endl;
        }

        resetRequests(start);
    }

    if (debug)
    {
        Pout<< "UPstream::waitRequests : finished wait." << endl;
    }
}


bool CML::UPstream::finishedRequest(const label i)
{
    if (debug)
    {
        Pout<< "UPstream::waitRequests : starting wait for request:" << i
            << endl;
    }

    if (i >= PstreamGlobals::outstandingRequests_.size())
    {
        FatalErrorInFunction
            << "There are " << PstreamGlobals::outstandingRequests_.size()
            << " outstanding send requests and you are asking for i=" << i
            << nl
            << "Maybe you are mixing blocking/non-blocking comms?"
            << CML::abort(FatalError);
    }

    int flag;
    MPI_Test
    (
       &PstreamGlobals::outstandingRequests_[i],
       &flag,
        MPI_STATUS_IGNORE
    );

    if (debug)
    {
        Pout<< "UPstream::waitRequests : finished wait for request:" << i
            << endl;
    }

    return flag != 0;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// ************************************************************************* //
