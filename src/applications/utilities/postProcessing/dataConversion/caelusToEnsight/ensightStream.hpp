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
    CML::ensightStream

Description
    Abstract base class for writing Ensight data

SourceFiles
    ensightStream.cpp

\*---------------------------------------------------------------------------*/

#ifndef ensightStream_H
#define ensightStream_H

#include "fileName.hpp"
#include "scalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{


/*---------------------------------------------------------------------------*\
                         Class ensightStream Declaration
\*---------------------------------------------------------------------------*/

class ensightStream
{
    // Private data

        const fileName name_;

    // Private Member Functions

        //- Disallow default bitwise copy construct
        ensightStream(const ensightStream&);

        //- Disallow default bitwise assignment
        void operator=(const ensightStream&);


public:

    // Constructors

        //- Construct from components
        ensightStream(const fileName& f)
        :
            name_(f)
        {}


    //- Destructor
    virtual ~ensightStream()
    {}


    // Member Functions

        const fileName& name() const
        {
            return name_;
        }

        virtual bool ascii() const = 0;

        virtual void write(const char*) = 0;

        virtual void write(const int) = 0;

        virtual void write(const scalarField&) = 0;

        virtual void write(const List<int>&) = 0;

        virtual void writePartHeader(const label) = 0;



};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
