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

Namespace
    CML::RBD::joints

Description
    Namespace for rigid-body joints

Class
    CML::RBD::joint

Description
    Abstract base-class for all rigid-body joints.

    Reference:
    \verbatim
        Featherstone, R. (2008).
        Rigid body dynamics algorithms.
        Springer.
        Chapter 4.
    \endverbatim

SourceFiles
    jointI.hpp
    joint.cpp

\*---------------------------------------------------------------------------*/

#ifndef RBD_joint_HPP
#define RBD_joint_HPP

#include "List.hpp"
#include "spatialVector.hpp"
#include "compactSpatialTensor.hpp"
#include "CompactSpatialTensorT_.hpp"
#include "spatialTransform.hpp"
#include "quaternion.hpp"
#include "scalarField.hpp"
#include "runTimeSelectionTables.hpp"

namespace CML
{
namespace RBD
{

// Forward declaration of classes
class rigidBodyModel;

// Forward declaration of friend functions and operators
class joint;

inline Ostream& operator<<(Ostream&, const joint&);


/*---------------------------------------------------------------------------*\
                         Class joint Declaration
\*---------------------------------------------------------------------------*/

class joint
{

protected:

    // Protected data

    //- Joint motion sub-space
    List<spatialVector> S_;

    //- Index of this joint in the rigidBodyModel
    label index_;

    //- Index of this joints data in the rigidBodyModel state
    label qIndex_;


private:

    // Private member functions to be used by rigidBodyModel

        //- Allow the rigidBodyModel to set the index for this joint
        label& index()
        {
            return index_;
        }

        //- Allow the rigidBodyModel to set the qIndex for this joint
        label& qIndex()
        {
            return qIndex_;
        }


public:

    //- Allow the rigidBodyModel class to set the joint indices
    friend class rigidBodyModel;

    //- Joint state returned by jcalc
    class XSvc
    {
    public:

        //- The joint transformation
        spatialTransform X;

        //- The joint motion sub-space (3-DoF)
        compactSpatialTensor S;

        //- The joint motion sub-space (1-DoF)
        spatialVector S1;

        //- The constrained joint velocity
        spatialVector v;

        //- The constrained joint acceleration correction
        //  due to changes in the motion sub-space S
        spatialVector c;

        //- Null constructor
        XSvc()
        :
            X(),
            v(Zero),
            c(Zero)
        {}
    };


public:

    //- Runtime type information
    TypeName("joint");


    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            joint,
            dictionary,
            (const dictionary& dict),
            (dict)
        );


    // Constructors

        //- Construct joint setting the size of the motion sub-space
        //  to the given degrees of freedom of the joint
        inline joint(const label nDoF);

        //- Clone this joint (needed by PtrList)
        virtual autoPtr<joint> clone() const = 0;

        class iNew
        {

        public:

            iNew()
            {}

            inline autoPtr<joint> operator()(Istream& is) const;
        };


    //- Destructor
    virtual ~joint();


    // Selectors

        //- Simple selector to return an autoPtr<joint> of the given joint*
        static autoPtr<joint> New(joint* jointPtr);

        //- Select from dictionary
        static autoPtr<joint> New(const dictionary& dict);


    // Member Functions

        //- Return the number of degrees of freedom in this joint
        inline label nDoF() const;

        //- Return true if this joint describes rotation using a quaternion
        inline virtual bool unitQuaternion() const;

        //- Return the index of this joint in the model
        inline label index() const;

        //- Return start index for the state variables for this joint
        //  in the rigidBodyModel state fields
        inline label qIndex() const;

        //- Return the joint motion sub-space
        inline const List<spatialVector>& S() const;

        //- Update the rigidBodyModel state for the joint given
        //  the joint state q, w and velocity qDot
        virtual void jcalc
        (
            XSvc& J,
            const scalarField& q,
            const scalarField& qDot
        ) const = 0;

        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        //- Return the unit quaternion for this joint
        //  if it uses a quaternion representation for rotation
        inline quaternion unitQuaternion
        (
            const scalarField& q
        ) const;

        //- Set the unit quaternion for this joint
        //  if it uses a quaternion representation for rotation
        inline void unitQuaternion
        (
            const quaternion& quat,
            scalarField& q
        ) const;


    // Ostream Operator

        friend Ostream& operator<<(Ostream&, const joint&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RBD
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "jointI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
