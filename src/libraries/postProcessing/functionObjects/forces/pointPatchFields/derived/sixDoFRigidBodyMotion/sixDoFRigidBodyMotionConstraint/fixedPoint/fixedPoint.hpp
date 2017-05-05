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
    CML::sixDoFRigidBodyMotionConstraints::fixedPoint

Description
    sixDoFRigidBodyMotionConstraint.  Point fixed in space.

SourceFiles
    fixedPoint.cpp

\*---------------------------------------------------------------------------*/

#ifndef fixedPoint_H
#define fixedPoint_H

#include "sixDoFRigidBodyMotionConstraint.hpp"
#include "point.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace sixDoFRigidBodyMotionConstraints
{

/*---------------------------------------------------------------------------*\
                           Class fixedPoint Declaration
\*---------------------------------------------------------------------------*/

class fixedPoint
:
    public sixDoFRigidBodyMotionConstraint
{
    // Private data

        //- Point which is rigidly attached to the body and constrains
        //  it so that this point remains fixed.  This serves as the
        //  reference point for displacements as well as the target
        //  position.
        point fixedPoint_;


public:

    //- Runtime type information
    TypeName("fixedPoint");


    // Constructors

        //- Construct from components
        fixedPoint
        (
            const dictionary& sDoFRBMCDict
        );

        //- Construct and return a clone
        virtual autoPtr<sixDoFRigidBodyMotionConstraint> clone() const
        {
            return autoPtr<sixDoFRigidBodyMotionConstraint>
            (
                new fixedPoint(*this)
            );
        }


    //- Destructor
    virtual ~fixedPoint();


    // Member Functions

        //- Calculate the constraint position, force and moment.
        //  Global reference frame vectors.  Returns boolean stating
        //  whether the constraint been converged to tolerance.
        virtual bool constrain
        (
            const sixDoFRigidBodyMotion& motion,
            const vector& existingConstraintForce,
            const vector& existingConstraintMoment,
            scalar deltaT,
            vector& constraintPosition,
            vector& constraintForceIncrement,
            vector& constraintMomentIncrement
        ) const;

        //- Update properties from given dictionary
        virtual bool read(const dictionary& sDoFRBMCCoeff);

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace solidBodyMotionFunctions
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
