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
    CML::sixDoFRigidBodyMotionConstraints::fixedAxis

Description
    sixDoFRigidBodyMotionConstraint.  Body may only rotate around
    an axis fixed in global space.

SourceFiles
    fixedAxis.cpp

\*---------------------------------------------------------------------------*/

#ifndef fixedAxis_H
#define fixedAxis_H

#include "sixDoFRigidBodyMotionConstraint.hpp"
#include "point.hpp"
#include "tensor.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace sixDoFRigidBodyMotionConstraints
{

/*---------------------------------------------------------------------------*\
                            Class fixedAxis Declaration
\*---------------------------------------------------------------------------*/

class fixedAxis
:
    public sixDoFRigidBodyMotionConstraint
{

    // Private data

        //- Reference axis in global space
        vector fixedAxis_;


public:

    //- Runtime type information
    TypeName("fixedAxis");


    // Constructors

        //- Construct from components
        fixedAxis
        (
            const dictionary& sDoFRBMCDict
        );

        //- Construct and return a clone
        virtual autoPtr<sixDoFRigidBodyMotionConstraint> clone() const
        {
            return autoPtr<sixDoFRigidBodyMotionConstraint>
            (
                new fixedAxis(*this)
            );
        }


    //- Destructor
    virtual ~fixedAxis();


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
