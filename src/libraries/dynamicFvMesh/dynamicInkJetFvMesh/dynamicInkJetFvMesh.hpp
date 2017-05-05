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
    CML::dynamicInkJetFvMesh

Description
    Mesh motion specifically for the "pumping" system of an ink-jet
    injector.

    The set of points in the "pumping" region are compressed and expanded
    sinusoidally to impose a sinusoidal variation of the flow at the
    nozzle exit.

SourceFiles
    dynamicInkJetFvMesh.cpp

\*---------------------------------------------------------------------------*/

#ifndef dynamicInkJetFvMesh_H
#define dynamicInkJetFvMesh_H

#include "dynamicFvMesh.hpp"
#include "dictionary.hpp"
#include "pointIOField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class dynamicInkJetFvMesh Declaration
\*---------------------------------------------------------------------------*/

class dynamicInkJetFvMesh
:
    public dynamicFvMesh
{
    // Private data

        dictionary dynamicMeshCoeffs_;

        scalar amplitude_;
        scalar frequency_;
        scalar refPlaneX_;

        pointIOField stationaryPoints_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        dynamicInkJetFvMesh(const dynamicInkJetFvMesh&);

        //- Disallow default bitwise assignment
        void operator=(const dynamicInkJetFvMesh&);


public:

    //- Runtime type information
    TypeName("dynamicInkJetFvMesh");


    // Constructors

        //- Construct from IOobject
        dynamicInkJetFvMesh(const IOobject& io);


    //- Destructor
    ~dynamicInkJetFvMesh();


    // Member Functions

        //- Update the mesh for both mesh motion and topology change
        virtual bool update();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
