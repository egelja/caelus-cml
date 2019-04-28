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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::Time& CML::RBD::rigidBodyModel::time() const
{
    return time_;
}

inline CML::label CML::RBD::rigidBodyModel::nRests() const
{
    return restraints_.size();
}

inline CML::label CML::RBD::rigidBodyModel::nBodies() const
{
    return bodies_.size();
}

inline const CML::PtrList<CML::RBD::restraint>&
CML::RBD::rigidBodyModel::restraints() const
{
    return restraints_;
}

inline CML::PtrList<CML::RBD::rigidBody>
CML::RBD::rigidBodyModel::bodies() const
{
    return bodies_;
}


inline const CML::DynamicList<CML::label>&
CML::RBD::rigidBodyModel::lambda() const
{
    return lambda_;
}


inline const CML::PtrList<CML::RBD::joint>&
CML::RBD::rigidBodyModel::joints() const
{
    return joints_;
}


inline CML::label CML::RBD::rigidBodyModel::nDoF() const
{
    return nDoF_;
}


inline bool CML::RBD::rigidBodyModel::unitQuaternions() const
{
    return unitQuaternions_;
}


inline const CML::vector& CML::RBD::rigidBodyModel::g() const
{
    return g_;
}


inline CML::vector& CML::RBD::rigidBodyModel::g()
{
    return g_;
}


inline const CML::word& CML::RBD::rigidBodyModel::name
(
    const label bodyID
) const
{
    if (merged(bodyID))
    {
        return mergedBody(bodyID).name();
    }
    else
    {
        return bodies_[bodyID].name();
    }
}


inline const CML::RBD::rigidBodyInertia&
CML::RBD::rigidBodyModel::I(const label i) const
{
    return bodies_[i];
}


inline const CML::spatialVector&
CML::RBD::rigidBodyModel::v(const label i) const
{
    return v_[i];
}


inline bool CML::RBD::rigidBodyModel::merged(label bodyID) const
{
    return bodyID < 0;
}


inline CML::label CML::RBD::rigidBodyModel::master(label bodyID) const
{
    if (bodyID < 0)
    {
        return mergedBody(bodyID).masterID();
    }
    else
    {
        return bodyID;
    }
}


inline CML::label
CML::RBD::rigidBodyModel::mergedBodyID(const label mergedBodyIndex) const
{
    return -1 - mergedBodyIndex;
}


inline CML::label
CML::RBD::rigidBodyModel::mergedBodyIndex(const label mergedBodyID) const
{
    return -1 - mergedBodyID;
}


inline const CML::RBD::subBody&
CML::RBD::rigidBodyModel::mergedBody(label mergedBodyID) const
{
    if (!merged(mergedBodyID))
    {
        FatalErrorInFunction
            << "Body " << mergedBodyID << " has not been merged"
            << abort(FatalError);
    }

    return mergedBodies_[mergedBodyIndex(mergedBodyID)];
}


inline CML::label CML::RBD::rigidBodyModel::bodyID(const word& name) const
{
    return bodyIDs_[name];
}


inline CML::vector CML::RBD::rigidBodyModel::masterPoint
(
    const label bodyID,
    const vector& p
) const
{
    if (merged(bodyID))
    {
        return
        (
            mergedBody(bodyID).masterXT().inv()
         && spatialVector(Zero, p)
        ).l();
    }
    else
    {
        return p;
    }
}


inline CML::spatialVector CML::RBD::rigidBodyModel::v
(
    const label bodyID,
    const vector& p
) const
{
    return
    (
        spatialTransform
        (
            X0_[master(bodyID)].E().T(),
            masterPoint(bodyID, p)
        )
      & v_[master(bodyID)]
    );
}


// ************************************************************************* //
