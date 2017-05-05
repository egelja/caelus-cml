/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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
    CML::DebugOStream

Description
    Stream that prefixes a class name and an adress to the output to make
    distinguishing output from different objects easier. Usually used through
    two preprocessor macros:

    Dbug : for parallel runs only outputs on the master processor
    Pbug : for parallel runs outputs on all processors and prefixes the
           processor number

SourceFiles
    DebugOStream.cpp

Contributors/Copyright:
    2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef DebugOStream_H
#define DebugOStream_H

#include "Pstream.hpp"

#include "prefixOSstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class DebugOStream Declaration
\*---------------------------------------------------------------------------*/

class DebugOStream
:
    public prefixOSstream
{
public:
    //- Constructor
    DebugOStream(
        ostream &o,
        const word &typeName,
        const void *object,
        const bool parallel=false
    );
    DebugOStream &operator()() { return *this; }

};

// to be used as "streams"
#define Dbug if(debug && Pstream::master()) DebugOStream(cout,typeName,this)()
#define Pbug if(debug) DebugOStream(cout,typeName,this,true)()
    // for static methods
#define Sbug if(debug) DebugOStream(cout,typeName,reinterpret_cast<const void *>(NULL),true)()

//- get the value of the pointer as a word
word getHex(const void *ptr);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
