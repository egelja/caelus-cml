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
    CML::RBD::rigidBodySolver

Description

SourceFiles
    rigidBodySolver.cpp
    newSixDoFSolver.cpp

\*---------------------------------------------------------------------------*/

#ifndef RBD_rigidBodySolver_HPP
#define RBD_rigidBodySolver_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "rigidBodyMotion.hpp"
#include "runTimeSelectionTables.hpp"

namespace CML
{
namespace RBD
{

/*---------------------------------------------------------------------------*\
                         Class rigidBodySolver Declaration
\*---------------------------------------------------------------------------*/

class rigidBodySolver
{
protected:

    // Protected data

        //- The rigid-body model
        rigidBodyMotion& model_;


        //- Return the motion state
        inline rigidBodyModelState& state();

        //- Return the current joint position and orientation
        inline scalarField& q();

        //- Return the current joint quaternion

        //- Return the current joint velocity
        inline scalarField& qDot();

        //- Return the current joint acceleration
        inline scalarField& qDdot();

        //- Return the current time-step
        inline scalar deltaT() const;


        //- Return the previous motion state
        inline const rigidBodyModelState& state0() const;

        //- Return the current joint position and orientation
        inline const scalarField& q0() const;

        //- Return the current joint quaternion

        //- Return the current joint velocity
        inline const scalarField& qDot0() const;

        //- Return the current joint acceleration
        inline const scalarField& qDdot0() const;

        //- Return the previous time-step
        inline scalar deltaT0() const;

        //- Correct the quaternion joints based on the current change in q
        void correctQuaternionJoints();


public:

    //- Runtime type information
    TypeName("rigidBodySolver");


    // Declare runtime construction

        declareRunTimeSelectionTable
        (
            autoPtr,
            rigidBodySolver,
            dictionary,
            (
                rigidBodyMotion& body,
                const dictionary& dict
            ),
            (body, dict)
        );


    // Constructors

        // Construct for given body
        rigidBodySolver(rigidBodyMotion& body);


    //- Destructor
    virtual ~rigidBodySolver();


    // Selectors

        static autoPtr<rigidBodySolver> New
        (
            rigidBodyMotion& body,
            const dictionary& dict
        );


    // Member Functions

        //- Integrate the rigid-body motion for one time-step
        virtual void solve
        (
            const scalarField& tau,
            const Field<spatialVector>& fx
        ) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RBD
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "rigidBodySolverI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
