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
    CML::repatchPolyTopoChanger

Description
    A mesh which allows changes in the patch distribution of the
    boundary faces.  The change in patching is set using changePatchID. For a
    boundary face, a new patch ID is given.

SourceFiles
    repatchPolyTopoChanger.cpp

\*---------------------------------------------------------------------------*/

#ifndef repatchPolyTopoChanger_H
#define repatchPolyTopoChanger_H

#include "polyMesh.hpp"
#include "polyTopoChange.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class repatchPolyTopoChanger Declaration
\*---------------------------------------------------------------------------*/

class repatchPolyTopoChanger
{
    // Private data

        //- The polyMesh to be repatched
        polyMesh& mesh_;

        //- Topological change to accumulated all mesh changes
        autoPtr<polyTopoChange> meshModPtr_;


    // Private Member Functions

        //- Demand driven access to polyTopoChange
        polyTopoChange& meshMod();

        //- Disallow default bitwise copy construct
        repatchPolyTopoChanger(const repatchPolyTopoChanger&);

        //- Disallow default bitwise assignment
        void operator=(const repatchPolyTopoChanger&);


public:

    // Constructors

        //- Construct for given mesh
        explicit repatchPolyTopoChanger(polyMesh& mesh);


    //- Destructor
    virtual ~repatchPolyTopoChanger()
    {}


    // Member Functions

        //- Change patches.
        void changePatches(const List<polyPatch*>& patches);

        //- Change patch ID for a boundary face. Note: patchID should be in new
        //  numbering.
        void changePatchID
        (
            const label faceID,
            const label patchID
        );

        //- Set zone ID for a face
        void setFaceZone
        (
            const label faceID,
            const label zoneID,
            const bool zoneFlip
        );

        //- Change anchor point (zero'th point of face) for a boundary face.
        void changeAnchorPoint
        (
            const label faceID,
            const label fp
        );


        //- Re-patch the mesh
        void repatch();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
