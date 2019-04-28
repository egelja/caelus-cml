/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
Copyright (C) 2016-2018 OpenCFD Ltd.
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
    CML::cloud

Description
    A cloud is a registry collection of lagrangian particles.

SourceFiles
    cloud_.cpp

\*---------------------------------------------------------------------------*/

#ifndef cloud__H
#define cloud__H

#include "objectRegistry.hpp"
#include "Enum.hpp"
#include "IOField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                            Class cloud Declaration
\*---------------------------------------------------------------------------*/

class cloud
:
    public objectRegistry
{

    // Private Member Functions

        //- Disallow default bitwise copy construct
        cloud(const cloud&);

        //- Disallow default bitwise assignment
        void operator=(const cloud&);


public:

    //- Cloud geometry type (internal or IO representations)
    enum class geometryType
    {
        COORDINATES,    //!< barycentric coordinates
        POSITIONS       //!< positions
    };

    static const Enum<geometryType> geometryTypeNames;


    //- Runtime type information
    TypeName("cloud");

        //- The prefix to local: %lagrangian
        static const word prefix;

        //- The default cloud name: %defaultCloud
        static word defaultName;


    // Constructors

        //- Construct for the given objectRegistry and named cloud instance
        cloud(const objectRegistry&, const word& cloudName = defaultName);


    //- Destructor
    virtual ~cloud()
    {};


    // Member Functions

        // Edit

            //- Remap the cells of particles corresponding to the
            //  mesh topology change
            virtual void autoMap(const mapPolyMesh&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
