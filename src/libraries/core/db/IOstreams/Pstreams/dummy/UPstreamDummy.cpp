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

#include "UPstream.hpp"
#include "PstreamReduceOps.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void CML::UPstream::addValidParOptions(HashTable<string>& validParOptions)
{}


bool CML::UPstream::init(int& argc, char**& argv)
{
    FatalErrorInFunction
        << "Trying to use the dummy Pstream library." << nl
        << "This dummy library cannot be used in parallel mode"
        << CML::exit(FatalError);

    return false;
}


void CML::UPstream::exit(int errnum)
{
    NotImplemented;
}


void CML::UPstream::abort()
{
    NotImplemented;
}


void CML::reduce(scalar&, const sumOp<scalar>&, const int)
{}


void CML::UPstream::allToAll
(
    const labelUList& sendData,
    labelUList& recvData,
    const label communicator
)
{
    recvData.assugb(sendData);
}

CML::label CML::UPstream::nRequests()
{
    return 0;
}


void CML::UPstream::resetRequests(const label i)
{}


void CML::UPstream::waitRequests(const label start)
{}


bool CML::UPstream::finishedRequest(const label i)
{
    NotImplemented;
    return false;
}


// ************************************************************************* //
