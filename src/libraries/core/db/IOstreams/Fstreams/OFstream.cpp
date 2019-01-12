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

#include "OFstream.hpp"
#include "OSspecific.hpp"
#include "gzstream.h"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(OFstream, 0);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::OFstreamAllocator::OFstreamAllocator
(
    const fileName& pathname,
    IOstream::compressionType compression
)
:
    ofPtr_(nullptr)
{
    if (pathname.empty())
    {
        if (OFstream::debug)
        {
            Info<< "OFstreamAllocator::OFstreamAllocator(const fileName&) : "
                   "cannot open null file " << endl;
        }
    }

    if (compression == IOstream::COMPRESSED)
    {
        // get identically named uncompressed version out of the way
        if (isFile(pathname, false))
        {
            rm(pathname);
        }

        ofPtr_ = new ogzstream((pathname + ".gz").c_str());
    }
    else
    {
        // get identically named compressed version out of the way
        if (isFile(pathname + ".gz", false))
        {
            rm(pathname + ".gz");
        }
#ifdef windows
        // Use binary mode in case we write binary.
        // Causes windows reading to fail if we don't
        ofPtr_ = new ofstream(pathname.c_str(), ios_base::out|ios_base::binary);
#else
        ofPtr_ = new ofstream(pathname.c_str());
#endif
    }
}


CML::OFstreamAllocator::~OFstreamAllocator()
{
    delete ofPtr_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::OFstream::OFstream
(
    const fileName& pathname,
    streamFormat format,
    versionNumber version,
    compressionType compression
)
:
    OFstreamAllocator(pathname, compression),
    OSstream(*ofPtr_, "OFstream.sinkFile_", format, version, compression),
    pathname_(pathname)
{
    setClosed();
    setState(ofPtr_->rdstate());

    if (!good())
    {
        if (debug)
        {
            Info<< "IFstream::IFstream(const fileName&,"
                   "streamFormat format=ASCII,"
                   "versionNumber version=currentVersion) : "
                   "could not open file for input\n"
                   "in stream " << info() << CML::endl;
        }

        setBad();
    }
    else
    {
        setOpened();
    }

    lineNumber_ = 1;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::OFstream::~OFstream()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

std::ostream& CML::OFstream::stdStream()
{
    if (!ofPtr_)
    {
        FatalErrorInFunction
            << "No stream allocated." << abort(FatalError);
    }
    return *ofPtr_;
}


const std::ostream& CML::OFstream::stdStream() const
{
    if (!ofPtr_)
    {
        FatalErrorInFunction
            << "No stream allocated." << abort(FatalError);
    }
    return *ofPtr_;
}


void CML::OFstream::print(Ostream& os) const
{
    os  << "    OFstream: ";
    OSstream::print(os);
}


// ************************************************************************* //
