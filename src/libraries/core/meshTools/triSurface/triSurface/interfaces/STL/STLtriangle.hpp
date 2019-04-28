/*---------------------------------------------------------------------------*\
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
    CML::STLtriangle

Description
    A triangle representation for STL files.

SourceFiles
    STLtriangleI.hpp

\*---------------------------------------------------------------------------*/

#ifndef STLtriangle_H
#define STLtriangle_H

#include "STLpoint.hpp"
#include "Istream.hpp"
#include "Ostream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class STLtriangle Declaration
\*---------------------------------------------------------------------------*/

class STLtriangle
{
    // Private data

    //- Attribute is 16-bit
    typedef unsigned short STLattrib;

    //- The face normal, many programs write zore or other junk
    STLpoint normal_;

    //- The three points defining the triangle
    STLpoint a_, b_, c_;

    //- The attribute information could for colour or solid id, etc
    STLattrib attrib_;


public:

    // Constructors

    //- Construct null
    inline STLtriangle()
    {}

    //- Construct from components
    inline STLtriangle
    (
        const STLpoint& normal,
        const STLpoint& a,
        const STLpoint& b,
        const STLpoint& c,
        unsigned short attrib
    )
    :
        normal_(normal),
        a_(a),
        b_(b),
        c_(c),
        attrib_(attrib)
        {}

    //- Construct from istream (read binary)
    inline STLtriangle(istream& is)
    {
        read(is);
    }


    // Member Functions

    // Access

    inline const STLpoint& normal() const
    {
        return normal_;
    }

    inline const STLpoint& a() const
    {
        return a_;
    }

    inline const STLpoint& b() const
    {
        return b_;
    }

    inline const STLpoint& c() const
    {
        return c_;
    }

    inline unsigned short attrib() const
    {
        return attrib_;
    }

    // Read

    //- Read from istream (binary)
    inline void read(istream& is)
    {
        is.read(reinterpret_cast<char*>(this), 4*sizeof(STLpoint));
        is.read(reinterpret_cast<char*>(&attrib_), sizeof(STLattrib));
    }


    // Write

    //- Write to ostream (binary)
    inline void write(ostream& os)
    {
        os.write(reinterpret_cast<char*>(this), 4*sizeof(STLpoint));
        os.write(reinterpret_cast<char*>(&attrib_), sizeof(STLattrib));
    }


    // Ostream operator

    inline friend Ostream& operator<<(Ostream& os, const STLtriangle& stlt)
    {
        os  << stlt.normal_ << token::SPACE
            << stlt.a_ << token::SPACE
            << stlt.b_ << token::SPACE
            << stlt.c_ << token::SPACE
            << stlt.attrib_;

        return os;
    }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#endif

// ************************************************************************* //
