/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    CML::multiSolidBodyMotionFvMesh

Description
    Solid-body motion of the mesh specified by a run-time selectable
    motion function.

SourceFiles
    multiSolidBodyMotionFvMesh.cpp

\*---------------------------------------------------------------------------*/

#ifndef multiSolidBodyMotionFvMesh_H
#define multiSolidBodyMotionFvMesh_H

#include "dynamicFvMesh.hpp"
#include "dictionary.hpp"
#include "pointIOField.hpp"
#include "solidBodyMotionFunction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class multiSolidBodyMotionFvMesh Declaration
\*---------------------------------------------------------------------------*/

class multiSolidBodyMotionFvMesh
:
    public dynamicFvMesh
{
    // Private data

        //- Dictionary of motion control parameters
        const dictionary dynamicMeshCoeffs_;

        //- The motion control function
        PtrList<solidBodyMotionFunction> SBMFs_;

        //- The reference points which are transformed
        pointIOField undisplacedPoints_;

        //- Specified cellZones
        labelList zoneIDs_;

        //- Points to move per cellZone
        labelListList pointIDs_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        multiSolidBodyMotionFvMesh(const multiSolidBodyMotionFvMesh&);

        //- Disallow default bitwise assignment
        void operator=(const multiSolidBodyMotionFvMesh&);


public:

    //- Runtime type information
    TypeName("multiSolidBodyMotionFvMesh");


    // Constructors

        //- Construct from IOobject
        multiSolidBodyMotionFvMesh(const IOobject& io);


    //- Destructor
    ~multiSolidBodyMotionFvMesh();


    // Member Functions

        //- Update the mesh for both mesh motion and topology change
        virtual bool update();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
