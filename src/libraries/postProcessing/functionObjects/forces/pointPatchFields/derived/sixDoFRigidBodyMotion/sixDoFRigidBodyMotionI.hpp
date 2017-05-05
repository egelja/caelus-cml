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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline CML::tensor
CML::sixDoFRigidBodyMotion::rotationTensorX(scalar phi) const
{
    return tensor
    (
        1, 0, 0,
        0, CML::cos(phi), -CML::sin(phi),
        0, CML::sin(phi), CML::cos(phi)
    );
}


inline CML::tensor
CML::sixDoFRigidBodyMotion::rotationTensorY(scalar phi) const
{
    return tensor
    (
        CML::cos(phi), 0, CML::sin(phi),
        0, 1, 0,
        -CML::sin(phi), 0, CML::cos(phi)
    );
}


inline CML::tensor
CML::sixDoFRigidBodyMotion::rotationTensorZ(scalar phi) const
{
    return tensor
    (
        CML::cos(phi), -CML::sin(phi), 0,
        CML::sin(phi), CML::cos(phi), 0,
        0, 0, 1
    );
}


inline void CML::sixDoFRigidBodyMotion::rotate
(
    tensor& Q,
    vector& pi,
    scalar deltaT
) const
{
    tensor R;

    R = rotationTensorX(0.5*deltaT*pi.x()/momentOfInertia_.xx());
    pi = pi & R;
    Q = Q & R;

    R = rotationTensorY(0.5*deltaT*pi.y()/momentOfInertia_.yy());
    pi = pi & R;
    Q = Q & R;

    R = rotationTensorZ(deltaT*pi.z()/momentOfInertia_.zz());
    pi = pi & R;
    Q = Q & R;

    R = rotationTensorY(0.5*deltaT*pi.y()/momentOfInertia_.yy());
    pi = pi & R;
    Q = Q & R;

    R = rotationTensorX(0.5*deltaT*pi.x()/momentOfInertia_.xx());
    pi = pi & R;
    Q = Q & R;
}


inline const CML::sixDoFRigidBodyMotionState&
CML::sixDoFRigidBodyMotion::motionState() const
{
    return motionState_;
}


inline const CML::PtrList<CML::sixDoFRigidBodyMotionRestraint>&
CML::sixDoFRigidBodyMotion::restraints() const
{
    return restraints_;
}


inline const CML::wordList& CML::sixDoFRigidBodyMotion::restraintNames() const
{
    return restraintNames_;
}


inline const CML::PtrList<CML::sixDoFRigidBodyMotionConstraint>&
CML::sixDoFRigidBodyMotion::constraints() const
{
    return constraints_;
}


inline const CML::wordList&
CML::sixDoFRigidBodyMotion::constraintNames() const
{
    return constraintNames_;
}


inline CML::label CML::sixDoFRigidBodyMotion::maxConstraintIterations() const
{
    return maxConstraintIterations_;
}


inline const CML::point&
CML::sixDoFRigidBodyMotion::initialCentreOfMass() const
{
    return initialCentreOfMass_;
}


inline const CML::tensor&
CML::sixDoFRigidBodyMotion::initialQ() const
{
    return initialQ_;
}


inline const CML::tensor& CML::sixDoFRigidBodyMotion::Q() const
{
    return motionState_.Q();
}


inline const CML::vector& CML::sixDoFRigidBodyMotion::v() const
{
    return motionState_.v();
}


inline const CML::vector& CML::sixDoFRigidBodyMotion::a() const
{
    return motionState_.a();
}


inline const CML::vector& CML::sixDoFRigidBodyMotion::pi() const
{
    return motionState_.pi();
}


inline const CML::vector& CML::sixDoFRigidBodyMotion::tau() const
{
    return motionState_.tau();
}


inline CML::point& CML::sixDoFRigidBodyMotion::initialCentreOfMass()
{
    return initialCentreOfMass_;
}


inline CML::tensor& CML::sixDoFRigidBodyMotion::initialQ()
{
    return initialQ_;
}


inline CML::tensor& CML::sixDoFRigidBodyMotion::Q()
{
    return motionState_.Q();
}


inline CML::vector& CML::sixDoFRigidBodyMotion::v()
{
    return motionState_.v();
}


inline CML::vector& CML::sixDoFRigidBodyMotion::a()
{
    return motionState_.a();
}


inline CML::vector& CML::sixDoFRigidBodyMotion::pi()
{
    return motionState_.pi();
}


inline CML::vector& CML::sixDoFRigidBodyMotion::tau()
{
    return motionState_.tau();
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

inline CML::tmp<CML::pointField>
CML::sixDoFRigidBodyMotion::currentPosition(const pointField& pInitial) const
{
    return
    (
        centreOfMass()
      + (Q() & initialQ_.T() & (pInitial - initialCentreOfMass_)))
    ;
}


inline CML::point CML::sixDoFRigidBodyMotion::currentPosition
(
    const point& pInitial
) const
{
    return
    (
        centreOfMass()
      + (Q() & initialQ_.T() & (pInitial - initialCentreOfMass_))
    );
}


inline CML::vector CML::sixDoFRigidBodyMotion::currentOrientation
(
    const vector& vInitial
) const
{
    return (Q() & initialQ_.T() & vInitial);
}


inline const CML::tensor&
CML::sixDoFRigidBodyMotion::orientation() const
{
    return Q();
}


inline CML::vector CML::sixDoFRigidBodyMotion::omega() const
{
    return  Q() & (inv(momentOfInertia_) & pi());
}


inline CML::point CML::sixDoFRigidBodyMotion::currentVelocity
(
    const point& pt
) const
{
    return (omega() ^ (pt - centreOfMass())) + v();
}


inline const CML::point& CML::sixDoFRigidBodyMotion::centreOfMass() const
{
    return motionState_.centreOfMass();
}


inline const CML::diagTensor&
CML::sixDoFRigidBodyMotion::momentOfInertia() const
{
    return momentOfInertia_;
}


inline CML::scalar CML::sixDoFRigidBodyMotion::mass() const
{
    return mass_;
}


inline bool CML::sixDoFRigidBodyMotion::report() const
{
    return report_;
}


inline CML::point& CML::sixDoFRigidBodyMotion::centreOfMass()
{
    return motionState_.centreOfMass();
}


inline CML::diagTensor& CML::sixDoFRigidBodyMotion::momentOfInertia()
{
    return momentOfInertia_;
}


inline CML::scalar& CML::sixDoFRigidBodyMotion::mass()
{
    return mass_;
}

// ************************************************************************* //
