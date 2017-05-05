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

Class
    CML::IPstream

Description
    Input inter-processor communications stream.

SourceFiles
    IPstream.cpp

\*---------------------------------------------------------------------------*/

#include "Pstream.hpp"

#ifndef IPstream_H
#define IPstream_H

#include "UIPstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class IPstream Declaration
\*---------------------------------------------------------------------------*/

class IPstream
:
    public Pstream,
    public UIPstream
{

    //- Receive index
    label externalBufPosition_;

public:

    // Constructors

        //- Construct given process index to read from and optional buffer size,
        //  read format and IO version
        IPstream
        (
            const commsTypes commsType,
            const int fromProcNo,
            const label bufSize = 0,
            const int tag = UPstream::msgType(),
            streamFormat format=BINARY,
            versionNumber version=currentVersion
        );

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
