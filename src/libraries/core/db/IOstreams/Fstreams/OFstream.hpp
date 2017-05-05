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
    CML::OFstream

Description
    Output to file stream.

SourceFiles
    OFstream.cpp

\*---------------------------------------------------------------------------*/

#ifndef OFstream_H
#define OFstream_H

#include "OSstream.hpp"
#include "fileName.hpp"
#include "className.hpp"

#include <fstream>
using std::ofstream;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class OFstream;

/*---------------------------------------------------------------------------*\
                      Class OFstreamAllocator Declaration
\*---------------------------------------------------------------------------*/

//- A std::ostream with ability to handle compressed files
class OFstreamAllocator
{
    friend class OFstream;

    ostream* ofPtr_;

    // Constructors

        //- Construct from pathname
        OFstreamAllocator
        (
            const fileName& pathname,
            IOstream::compressionType compression=IOstream::UNCOMPRESSED
        );

    //- Destructor
    ~OFstreamAllocator();

};


/*---------------------------------------------------------------------------*\
                          Class OFstream Declaration
\*---------------------------------------------------------------------------*/

class OFstream
:
    private OFstreamAllocator,
    public OSstream
{
    // Private data

        fileName pathname_;


public:

    // Declare name of the class and its debug switch
    ClassName("OFstream");


    // Constructors

        //- Construct from pathname
        OFstream
        (
            const fileName& pathname,
            streamFormat format=ASCII,
            versionNumber version=currentVersion,
            compressionType compression=UNCOMPRESSED
        );


    //- Destructor
    ~OFstream();


    // Member functions

        // Access

            //- Return the name of the stream
            const fileName& name() const
            {
                return pathname_;
            }

            //- Return non-const access to the name of the stream
            fileName& name()
            {
                return pathname_;
            }


        // STL stream

            //- Access to underlying std::ostream
            virtual ostream& stdStream();

            //- Const access to underlying std::ostream
            virtual const ostream& stdStream() const;


        // Print

            //- Print description of IOstream to Ostream
            void print(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Global predefined null output stream "/dev/null"
extern OFstream Snull;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
