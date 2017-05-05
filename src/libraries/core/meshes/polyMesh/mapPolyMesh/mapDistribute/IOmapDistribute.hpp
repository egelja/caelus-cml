/*---------------------------------------------------------------------------*\
Copyright (C) 2014 OpenFOAM Foundation
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
    CML::IOmapDistribute

Description
    IOmapDistribute is derived from mapDistribute and
    IOobject to give the mapDistribute
    automatic IO functionality via the objectRegistry.

SourceFiles
    IOmapDistribute.C

\*---------------------------------------------------------------------------*/

#ifndef IOmapDistribute_H
#define IOmapDistribute_H

#include "mapDistribute.hpp"
#include "regIOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class IOmapDistribute Declaration
\*---------------------------------------------------------------------------*/

class IOmapDistribute
:
    public regIOobject,
    public mapDistribute
{

public:

    //- Runtime type information
    TypeName("mapDistribute");

    // Constructors

        //- Construct given an IOobject
        IOmapDistribute(const IOobject&);

        //- Construct given an IOobject and mapDistribute
        IOmapDistribute(const IOobject&, const mapDistribute&);

        //- Construct by transferring the mapDistribute contents
        IOmapDistribute(const IOobject&, const Xfer<mapDistribute>&);


    //- Destructor
    virtual ~IOmapDistribute();


    // Member functions

        //- ReadData function required for regIOobject read operation
        virtual bool readData(Istream&);

        //- WriteData function required for regIOobject write operation
        virtual bool writeData(Ostream&) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
