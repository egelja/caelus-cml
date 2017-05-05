/*---------------------------------------------------------------------------*\
Copyright (C) 2009 Frank Bos
Copyright (C) 2016 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Class
    RBFMotionSolver

Description
    Radial basis function motion solver

Author
    Frank Bos, TU Delft.
    Darrin Stephens, Applied CCM Pty Ltd

SourceFiles
    RBFMotionSolver.cpp

\*---------------------------------------------------------------------------*/

#ifndef RBFMotionSolver_HPP
#define RBFMotionSolver_HPP

#include "motionSolver.hpp"
#include "polyMesh.hpp"
#include "RBFInterpolation.hpp"
#include "solidBodyMotionFunction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class RBFMotionSolver Declaration
\*---------------------------------------------------------------------------*/

class RBFMotionSolver
:
    public motionSolver
{
    // Private data

        //- The motion control function
        autoPtr<solidBodyMotionFunction> SBMFPtr_;

        //- Moving patch names
        wordList movingPatches_;

        //- Static patch names
        wordList staticPatches_;

        //- Coarsening ratio
        label coarseningRatio_;

        //- Include zero motion of static patches in RBF interpolation
        Switch includeStaticPatches_;

        //- Frozen interpolation
        Switch frozenInterpolation_;

        //- Moving point IDs
        labelList movingIDs_;

        //- Moving points on the boundary
        mutable vectorField movingPoints_;

        //- Static point IDs
        labelList staticIDs_;

        //- Control point IDs
        labelList controlIDs_;

        //- Control points on the boundary
        mutable vectorField controlPoints_;

        //- Internal point IDs
        labelList internalIDs_;

        //- Internal points
        vectorField internalPoints_;

        //- Prescribed motion on control points
        vectorField motion_;

        //- RBF interpolation
        RBFInterpolation interpolation_;

        //- The reference points which are transformed
        vectorField undisplacedPoints_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        RBFMotionSolver(const RBFMotionSolver&);

        //- Disallow default bitwise assignment
        void operator=(const RBFMotionSolver&);


        //- Make control point IDs.  Constructor helper
        void makeControlIDs();

        //- Set location of points
        void setMovingPoints() const;


public:

    //- Runtime type information
    TypeName("RBFMotionSolver");


    // Constructors

        //- Construct from polyMesh
        RBFMotionSolver
        (
            const polyMesh& mesh,
            const IOdictionary& dict
        );


    // Destructor

        virtual ~RBFMotionSolver();


    // Member Functions

        //- Return moving point indices in mesh points
        const labelList& movingIDs() const
        {
            return movingIDs_;
        }

        //- Return moving points
        const vectorField& movingPoints() const;

        //- Set motion of moving points
        void setMotion(const vectorField&);

        //- Return point location obtained from the current motion field
        virtual tmp<pointField> curPoints() const;

        //- Solve for motion
        virtual void solve();

	    //- Update local data for geometry changes
	    virtual void movePoints(const pointField&);
	
        //- Update the mesh corresponding to given map
        virtual void updateMesh(const mapPolyMesh&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
