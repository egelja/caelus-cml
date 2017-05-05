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
    CML::OPstream

Description
    Output inter-processor communications stream.

SourceFiles
    OPstream.cpp

\*---------------------------------------------------------------------------*/

#include "Pstream.hpp"

#ifndef OPstream_H
#define OPstream_H

#include "UOPstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                          Class OPstream Declaration
\*---------------------------------------------------------------------------*/

class OPstream
:
    public Pstream,
    public UOPstream
{

public:

    // Constructors

        //- Construct given process index to send to and optional buffer size,
        //  write format and IO version
        OPstream
        (
            const commsTypes commsType,
            const int toProcNo,
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
