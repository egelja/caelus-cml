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
    CML::meshWriters::STARCD

Description
    Writes polyMesh in pro-STAR (v4) bnd/cel/vrt format

    The cellTableId and cellTable information are used (if available).
    Otherwise the cellZones are used (if available).

SourceFiles
    STARCDMeshWriter.cpp

\*---------------------------------------------------------------------------*/

#ifndef STARCDMeshWriter_H
#define STARCDMeshWriter_H

#include "meshWriter.hpp"
#include "IOstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace meshWriters
{

/*---------------------------------------------------------------------------*\
                    Class meshWriters::STARCD Declaration
\*---------------------------------------------------------------------------*/

class STARCD
:
    public meshWriter
{
    // Private Data

        static const char* defaultBoundaryName;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        STARCD(const STARCD&);

        //- Disallow default bitwise assignment
        void operator=(const STARCD&);

        //- Pro-STAR 4+ header format
        static void writeHeader(Ostream&, const char* filetype);

        //- Write points
        void writePoints(const fileName& baseName) const;

        //- Write cells
        void writeCells(const fileName& baseName) const;

        //- Write boundary
        void writeBoundary(const fileName& baseName) const;

        void getCellTable();

        label findDefaultBoundary() const;


public:

    // Static data members

        //- Face addressing from CAELUS faces -> pro-STAR faces
        static const label caelusToStarFaceAddr[4][6];


    // Constructors

        //- Open a file for writing
        STARCD
        (
            const polyMesh&,
            const scalar scaleFactor = 1.0
        );


    //- Destructor
    virtual ~STARCD();


    // Member Functions

        // Edit

            //- Remove STAR-CD files for the baseName
            void rmFiles(const fileName& baseName) const;


        // Write

            //- Write volume mesh
            virtual bool write
            (
                const fileName& meshName = fileName::null
            ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace meshWriters
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
