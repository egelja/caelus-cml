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

Namespace
    CML::sixDoFRigidBodyMotionConstraints

Description
    Namespace for six DoF motion constraints


Class
    CML::sixDoFRigidBodyMotionConstraint

Description
    Base class for defining constraints for sixDoF motions

SourceFiles
    sixDoFRigidBodyMotionConstraint.cpp
    sixDoFRigidBodyMotionConstraintNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef sixDoFRigidBodyMotionConstraint_H
#define sixDoFRigidBodyMotionConstraint_H

#include "Time.hpp"
#include "dictionary.hpp"
#include "autoPtr.hpp"
#include "vector.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class sixDoFRigidBodyMotion;


/*---------------------------------------------------------------------------*\
                Class sixDoFRigidBodyMotionConstraint Declaration
\*---------------------------------------------------------------------------*/

class sixDoFRigidBodyMotionConstraint
{

protected:

    // Protected data

        //- Constraint model specific coefficient dictionary
        dictionary sDoFRBMCCoeffs_;

        //- Solution tolerance.  Meaning depends on model, usually an
        //  absolute distance or angle.
        scalar tolerance_;

        //- Relaxation factor for solution, default to one
        scalar relaxationFactor_;


public:

    //- Runtime type information
    TypeName("sixDoFRigidBodyMotionConstraint");


    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            sixDoFRigidBodyMotionConstraint,
            dictionary,
            (const dictionary& sDoFRBMCDict),
            (sDoFRBMCDict)
        );


    // Constructors

        //- Construct from the sDoFRBMCDict dictionary and Time
        sixDoFRigidBodyMotionConstraint
        (
            const dictionary& sDoFRBMCDict
        );

        //- Construct and return a clone
        virtual autoPtr<sixDoFRigidBodyMotionConstraint> clone() const = 0;


    // Selectors

        //- Select constructed from the sDoFRBMCDict dictionary and Time
        static autoPtr<sixDoFRigidBodyMotionConstraint> New
        (
            const dictionary& sDoFRBMCDict
        );


    //- Destructor
    virtual ~sixDoFRigidBodyMotionConstraint();


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
        ) const = 0;

        //- Update properties from given dictionary
        virtual bool read(const dictionary& sDoFRBMCDict);

        // Access

            // Return access to sDoFRBMCCoeffs
            inline const dictionary& coeffDict() const
            {
                return sDoFRBMCCoeffs_;
            }

            //- Return access to the tolerance
            inline scalar tolerance() const
            {
                return tolerance_;
            }

            //- Return access to the relaxationFactor
            inline scalar relaxationFactor() const
            {
                return relaxationFactor_;
            }

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
