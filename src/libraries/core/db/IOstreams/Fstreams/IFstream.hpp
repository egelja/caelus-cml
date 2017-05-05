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
    CML::IFstream

Description
    Input from file stream.

SourceFiles
    IFstream.cpp

\*---------------------------------------------------------------------------*/

#ifndef IFstream_H
#define IFstream_H

#include "ISstream.hpp"
#include "fileName.hpp"
#include "className.hpp"

#include <fstream>
using std::ifstream;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class IFstream;

/*---------------------------------------------------------------------------*\
                      Class IFstreamAllocator Declaration
\*---------------------------------------------------------------------------*/

//- A std::istream with ability to handle compressed files
class IFstreamAllocator
{
    friend class IFstream;

    // Private data

        istream* ifPtr_;
        IOstream::compressionType compression_;


    // Constructors

        //- Construct from pathname
        IFstreamAllocator(const fileName& pathname);


    //- Destructor
    ~IFstreamAllocator();
};


/*---------------------------------------------------------------------------*\
                          Class IFstream Declaration
\*---------------------------------------------------------------------------*/

class IFstream
:
    public IFstreamAllocator,
    public ISstream
{
    // Private data

        fileName pathname_;

public:

    // Declare name of the class and its debug switch
    ClassName("IFstream");


    // Constructors

        //- Construct from pathname
        IFstream
        (
            const fileName& pathname,
            streamFormat format=ASCII,
            versionNumber version=currentVersion
        );


    //- Destructor
    ~IFstream();


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

            //- Access to underlying std::istream
            virtual istream& stdStream();

            //- Const access to underlying std::istream
            virtual const istream& stdStream() const;


        // Print

            //- Print description of IOstream to Ostream
            virtual void print(Ostream&) const;


    // Member operators

        //- Return a non-const reference to const IFstream
        //  Needed for read-constructors where the stream argument is temporary:
        //  e.g. thing thisThing(IFstream("thingFileName")());
        IFstream& operator()() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
