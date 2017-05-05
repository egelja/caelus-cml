/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::rigidBodyMeshMotionSolver

Description
    Rigid-body mesh motion solver for fvMesh.

    Applies septernion interpolation of movement as function of distance to the
    object surface.

SourceFiles
    rigidBodyMeshMotionSolver.cpp

\*---------------------------------------------------------------------------*/

#ifndef rigidBodyMeshMotionSolver_HPP
#define rigidBodyMeshMotionSolver_HPP

#include "displacementMotionSolver.hpp"
#include "rigidBodyMotion.hpp"
#include "OFstream.hpp"
#include "outputFilterOutputControl.hpp"
#include "HashSet.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
        Class rigidBodyMeshMotionSolver Declaration
\*---------------------------------------------------------------------------*/

class rigidBodyMeshMotionSolver
:
    public motionSolver
{
    //- Class containing the patches and point motion weighting for each body
    class bodyMesh
    {
        //- Name of the body
        const word name_;

        //- ID of the body in the RBD::rigidBodyMotion
        const label bodyID_;

        //- List of mesh patches associated with this body
        const wordReList patches_;

        //- Patches to integrate forces
        const labelHashSet patchSet_;


    public:

        friend class rigidBodyMeshMotionSolver;

        bodyMesh
        (
            const polyMesh& mesh,
            const word& name,
            const label bodyID,
            const dictionary& dict
        );
    };


    // Private data

        //- Dictionary 
        dictionary coeffDict_;

        //- Rigid-body model
        RBD::rigidBodyMotion model_;

        //- List of the bodyMeshes containing the patches and point motion
        //  weighting for each body
        PtrList<bodyMesh> bodyMeshes_;

        //- Reference density required by the forces object for
        //  incompressible calculations, required if rho == rhoInf
        scalar rhoInf_;

        //- Name of density field, optional unless used for an
        //  incompressible simulation, when this needs to be specified
        //  as rhoInf
        word rhoName_;

        //- Current time index (used for updating)
        label curTimeIndex_;

        //- Output controls
        outputFilterOutputControl motionOutputControl_;

        // Solver for the mesh motion
        autoPtr<motionSolver> meshSolverPtr_;

        displacementMotionSolver& meshSolver_;
        
        //- List of stateFiles for bodies
		List<autoPtr<OFstream> > bodyStateFilePtrs_; 
		
		//- List of stateFiles for restraints
		List<autoPtr<OFstream> > restraintStateFilePtrs_; 


    // Private Member Functions

        //- Disallow default bitwise copy construct
        rigidBodyMeshMotionSolver
        (
            const rigidBodyMeshMotionSolver&
        );

        //- Disallow default bitwise assignment
        void operator=(const rigidBodyMeshMotionSolver&);


public:

    //- Runtime type information
    TypeName("rigidBodyMeshMotionSolver");


    // Constructors

        //- Construct from polyMesh and IOdictionary
        rigidBodyMeshMotionSolver
        (
            const polyMesh&,
            const IOdictionary& dict
        );


    //- Destructor
    ~rigidBodyMeshMotionSolver();


    // Member Functions

        void initialiseStateFile();
        
        //- Return point location obtained from the current motion field
        virtual tmp<pointField> curPoints() const;

        //- Solve for motion
        virtual void solve();

        //- Write state using given format, version and compression
        virtual bool writeObject
        (
            IOstream::streamFormat fmt,
            IOstream::versionNumber ver,
            IOstream::compressionType cmp
        ) const;

        //- Read dynamicMeshDict dictionary
        virtual bool read();

        //- Update local data for geometry changes
        virtual void movePoints(const pointField&);

        //-  Update local data for topology changes
        virtual void updateMesh(const mapPolyMesh&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
