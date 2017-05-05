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
    CML::displacementLayeredMotionFvMotionSolver

Description
    Mesh motion solver for an (multi-block) extruded fvMesh. Gets given the
    structure of the mesh blocks and boundary conditions on these blocks.

    The displacementLayeredMotionCoeffs subdict of dynamicMeshDict specifies
    per region (=cellZone) the boundary conditions on two opposing patches
    (=faceZones). It then interpolates the boundary values using topological
    walking so requires the cellZone to be a layered mesh.

    The boundary conditions on faceZones are currently:

    follow: the faceZone follows the overall mesh displacement.
            Use this for faceZones on boundary faces (so it uses the
            proper boundary conditions on the pointDisplacement).

    uniformFollow: like 'follow' but takes the average value of
            a specified 'patch' (which is not necessarily colocated)

    fixedValue: fixed value.

    timeVaryingUniformFixedValue: table-driven fixed value.

    slip: the second of a pair of faceZones follows the tangential movement
          specified by the first faceZone. (= removes the normal component).

SourceFiles
    displacementLayeredMotionMotionSolver.cpp

\*---------------------------------------------------------------------------*/

#ifndef displacementLayeredMotionMotionSolver_HPP
#define displacementLayeredMotionMotionSolver_HPP

#include "displacementMotionSolver.hpp"
#include "PackedBoolList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward class declarations

/*---------------------------------------------------------------------------*\
             Class displacementLayeredMotionMotionSolver Declaration
\*---------------------------------------------------------------------------*/

class displacementLayeredMotionMotionSolver
:
    public displacementMotionSolver
{
    // Private Member Functions

        void calcZoneMask
        (
            const label cellZoneI,
            PackedBoolList& isZonePoint,
            PackedBoolList& isZoneEdge
        ) const;

        void walkStructured
        (
            const label cellZoneI,
            const PackedBoolList& isZonePoint,
            const PackedBoolList& isZoneEdge,
            const labelList& seedPoints,
            const vectorField& seedData,
            scalarField& distance,
            vectorField& data
        ) const;

        tmp<vectorField> faceZoneEvaluate
        (
            const faceZone& fz,
            const labelList& meshPoints,
            const dictionary& dict,
            const PtrList<pointVectorField>& patchDisp,
            const label patchI
        ) const;

        void cellZoneSolve
        (
            const label cellZoneI,
            const dictionary& zoneDict
        );


        //- Disallow default bitwise copy construct
        displacementLayeredMotionMotionSolver
        (
            const displacementLayeredMotionMotionSolver&
        );

        //- Disallow default bitwise assignment
        void operator=(const displacementLayeredMotionMotionSolver&);


public:

    //- Runtime type information
    TypeName("displacementLayeredMotion");


    // Constructors

        //- Construct from polyMesh and IOdictionary
        displacementLayeredMotionMotionSolver
        (
            const polyMesh&,
            const IOdictionary&
        );


    //- Destructor
    ~displacementLayeredMotionMotionSolver();


    // Member Functions

        //- Return point location obtained from the current motion field
        virtual tmp<pointField> curPoints() const;

        //- Solve for motion
        virtual void solve();

        //- Update topology
        virtual void updateMesh(const mapPolyMesh&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
