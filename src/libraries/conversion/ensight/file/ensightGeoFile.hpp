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
    CML::ensightGeoFile

Description
    Specialized Ensight output with extra geometry file header

\*---------------------------------------------------------------------------*/

#ifndef ensightGeoFile_H
#define ensightGeoFile_H

#include "ensightFile.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class ensightGeoFile Declaration
\*---------------------------------------------------------------------------*/

class ensightGeoFile
:
    public ensightFile
{
    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const ensightGeoFile&);

        //- Disallow default copy constructor
        ensightGeoFile(const ensightGeoFile&);


public:

    // Constructors

        //- Construct from pathname
        ensightGeoFile
        (
            const fileName& pathname,
            IOstream::streamFormat format=IOstream::BINARY
        );


    //- Destructor
    ~ensightGeoFile();


    // Output

        //- write keyword with trailing newline
        virtual Ostream& writeKeyword(const string& key);
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
