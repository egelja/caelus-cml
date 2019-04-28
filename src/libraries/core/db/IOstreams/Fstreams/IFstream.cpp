/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
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

#include "IFstream.hpp"
#include "OSspecific.hpp"
#include "gzstream.h"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(IFstream, 0);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::IFstreamAllocator::IFstreamAllocator(const fileName& pathname)
:
    ifPtr_(nullptr),
    compression_(IOstream::UNCOMPRESSED)
{
    if (pathname.empty())
    {
        if (IFstream::debug)
        {
            Info<< "IFstreamAllocator::IFstreamAllocator(const fileName&) : "
                    "cannot open null file " << endl;
        }
    }
#ifdef windows
    // Use binary mode in case we read binary.
    // Causes windows reading to fail if we don't.
    ifPtr_ = new ifstream(pathname.c_str(), ios_base::in|ios_base::binary);
#else
    ifPtr_ = new ifstream(pathname.c_str());
#endif

    // If the file is compressed, decompress it before reading.
    if (!ifPtr_->good() && isFile(pathname + ".gz", false))
    {
        if (IFstream::debug)
        {
            Info<< "IFstreamAllocator::IFstreamAllocator(const fileName&) : "
                    "decompressing " << pathname + ".gz" << endl;
        }

        delete ifPtr_;

        ifPtr_ = new igzstream((pathname + ".gz").c_str());

        if (ifPtr_->good())
        {
            compression_ = IOstream::COMPRESSED;
        }
    }
}


CML::IFstreamAllocator::~IFstreamAllocator()
{
    delete ifPtr_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::IFstream::IFstream
(
    const fileName& pathname,
    streamFormat format,
    versionNumber version
)
:
    IFstreamAllocator(pathname),
    ISstream
    (
        *ifPtr_,
        "IFstream.sourceFile_",
        format,
        version,
        IFstreamAllocator::compression_
    ),
    pathname_(pathname)
{
    setClosed();

    setState(ifPtr_->rdstate());

    if (!good())
    {
        if (debug)
        {
            Info<< "IFstream::IFstream(const fileName&,"
                   "streamFormat=ASCII,"
                   "versionNumber=currentVersion) : "
                   "could not open file for input"
                << endl << info() << endl;
        }

        setBad();
    }
    else
    {
        setOpened();
    }

    lineNumber_ = 1;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::IFstream::~IFstream()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

std::istream& CML::IFstream::stdStream()
{
    if (!ifPtr_)
    {
        FatalErrorInFunction
            << "No stream allocated" << abort(FatalError);
    }
    return *ifPtr_;
}


const std::istream& CML::IFstream::stdStream() const
{
    if (!ifPtr_)
    {
        FatalErrorInFunction
            << "No stream allocated" << abort(FatalError);
    }
    return *ifPtr_;
}


void CML::IFstream::print(Ostream& os) const
{
    // Print File data
    os  << "IFstream: ";
    ISstream::print(os);
}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //

CML::IFstream& CML::IFstream::operator()() const
{
    if (!good())
    {
        // also checks .gz file
        if (isFile(pathname_, true))
        {
            check("IFstream::operator()");
            FatalIOError.exit();
        }
        else
        {
            FatalIOErrorInFunction(*this)
                << "file " << pathname_ << " does not exist"
                << exit(FatalIOError);
        }
    }

    return const_cast<IFstream&>(*this);
}


// ************************************************************************* //
