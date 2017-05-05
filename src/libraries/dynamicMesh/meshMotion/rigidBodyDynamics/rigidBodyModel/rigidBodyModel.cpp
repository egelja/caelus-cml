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

#include "rigidBodyModel.hpp"
#include "masslessBody.hpp"
#include "compositeBody.hpp"
#include "jointBody.hpp"
#include "nullJoint.hpp"
#include "rigidBodyRestraint.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
    defineTypeNameAndDebug(rigidBodyModel, 0);
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::RBD::rigidBodyModel::initializeRootBody()
{
    bodies_.append(new masslessBody("root"));
    lambda_.append(0);
    bodyIDs_.insert("root", 0);
    joints_.append(new joints::null());
    XT_.append(spatialTransform());

    nDoF_ = 0;
    unitQuaternions_ = false;

    resizeState();
}


void CML::RBD::rigidBodyModel::resizeState()
{
    Xlambda_.append(spatialTransform());
    X0_.append(spatialTransform());

    v_.append(Zero);
    a_.append(Zero);
    c_.append(Zero);

    IA_.append(spatialTensor::I);
    pA_.append(Zero);

    S_.append(Zero);
    S1_.append(Zero);
    U_.append(Zero);
    U1_.append(Zero);
    Dinv_.append(Zero);
    u_.append(Zero);
}


void CML::RBD::rigidBodyModel::addRestraints
(
    const dictionary& dict
)
{
    if (dict.found("restraints"))
    {
        const dictionary& restraintDict = dict.subDict("restraints");

        label i = 0;

        restraints_.setSize(restraintDict.size());

        forAllConstIter(IDLList<entry>, restraintDict, iter)
        {
            if (iter().isDict())
            {
                restraints_.set
                (
                    i++,
                    restraint::New
                    (
                        iter().keyword(),
                        iter().dict(),
                        *this
                    )
                );
            }
        }

        restraints_.setSize(i);
    }
}


// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * * //

CML::RBD::rigidBodyModel::rigidBodyModel(const Time& time)
:
    time_(time),
    g_(Zero)
{
    initializeRootBody();
}


CML::RBD::rigidBodyModel::rigidBodyModel(const Time& time, const dictionary& dict)
:
    time_(time),
    g_(Zero)
{
    initializeRootBody();

    const dictionary& bodiesDict = dict.subDict("bodies");

    forAllConstIter(IDLList<entry>, bodiesDict, iter)
    {
        const dictionary& bodyDict = iter().dict();

        if (bodyDict.found("mergeWith"))
        {
            merge
            (
                bodyID(bodyDict.lookup("mergeWith")),
                bodyDict.lookup("transform"),
                rigidBody::New(iter().keyword(), bodyDict)
            );
        }
        else
        {
            join
            (
                bodyID(bodyDict.lookup("parent")),
                bodyDict.lookup("transform"),
                joint::New(bodyDict.subDict("joint")),
                rigidBody::New(iter().keyword(), bodyDict)
            );
        }
    }

    // Read the restraints and any other re-readable settings.
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::RBD::rigidBodyModel::~rigidBodyModel()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::label CML::RBD::rigidBodyModel::join_
(
    const label parentID,
    const spatialTransform& XT,
    autoPtr<joint> jointPtr,
    autoPtr<rigidBody> bodyPtr
)
{
    // Append the body
    const rigidBody& body = bodyPtr();
    bodies_.append(bodyPtr);
    const label bodyID = nBodies()-1;
    bodyIDs_.insert(body.name(), bodyID);

    // If the parentID refers to a merged body find the parent into which it has
    // been merged and set lambda and XT accordingly
    if (merged(parentID))
    {
        const subBody& sBody = mergedBody(parentID);
        lambda_.append(sBody.masterID());
        XT_.append(XT & sBody.masterXT());
    }
    else
    {
        lambda_.append(parentID);
        XT_.append(XT);
    }

    // Append the joint
    const joint& prevJoint = joints_[joints_.size() - 1];
    joints_.append(jointPtr);
    joint& curJoint = joints_[joints_.size() - 1];
    curJoint.index() = joints_.size() - 1;
    curJoint.qIndex() = prevJoint.qIndex() + prevJoint.nDoF();

    // Increment the degrees of freedom
    nDoF_ += curJoint.nDoF();
    unitQuaternions_ = unitQuaternions_ || curJoint.unitQuaternion();

    resizeState();

    return bodyID;
}


CML::label CML::RBD::rigidBodyModel::join
(
    const label parentID,
    const spatialTransform& XT,
    autoPtr<joint> jointPtr,
    autoPtr<rigidBody> bodyPtr
)
{
    if (isA<joints::composite>(jointPtr()))
    {
        return join
        (
            parentID,
            XT,
            autoPtr<joints::composite>
            (
                dynamic_cast<joints::composite*>(jointPtr.ptr())
            ),
            bodyPtr
        );
    }
    else
    {
        return join_
        (
            parentID,
            XT,
            jointPtr,
            bodyPtr
        );
    }
}


CML::label CML::RBD::rigidBodyModel::join
(
    const label parentID,
    const spatialTransform& XT,
    autoPtr<joints::composite> cJointPtr,
    autoPtr<rigidBody> bodyPtr
)
{
    label parent = parentID;
    joints::composite& cJoint = cJointPtr();

    // For all but the final joint in the set add a jointBody with the
    // joint and transform
    for (label j=0; j<cJoint.size()-1; j++)
    {
        parent = join_
        (
            parent,
            j == 0 ? XT : spatialTransform(),
            cJoint[j].clone(),
            autoPtr<rigidBody>(new jointBody)
        );
    }

    // For the final joint in the set add the real body
    parent = join_
    (
        parent,
        cJoint.size() == 1 ? XT : spatialTransform(),
        autoPtr<joint>(cJointPtr.ptr()),
        bodyPtr
    );

    // Set the properties of the last joint in the list to those set
    // by rigidBodyModel
    cJoint.setLastJoint();

    return parent;
}


void CML::RBD::rigidBodyModel::makeComposite(const label bodyID)
{
    if (!isA<compositeBody>(bodies_[bodyID]))
    {
        // Retrieve the un-merged body
        autoPtr<rigidBody> bodyPtr = bodies_.set(bodyID, NULL);

        // Insert the compositeBody containing the original body
        bodies_.set
        (
            bodyID,
            new compositeBody(bodyPtr)
        );
    }
}


CML::label CML::RBD::rigidBodyModel::merge
(
    const label parentID,
    const spatialTransform& XT,
    autoPtr<rigidBody> bodyPtr
)
{
    autoPtr<subBody> sBodyPtr;

    // If the parentID refers to a merged body find the parent into which it has
    // been merged and merge this on into the same parent with the appropriate
    // transform
    if (merged(parentID))
    {
        const subBody& sBody = mergedBody(parentID);

        makeComposite(sBody.masterID());

        sBodyPtr.set
        (
            new subBody
            (
                bodyPtr,
                bodies_[sBody.masterID()].name(),
                sBody.masterID(),
                XT & sBody.masterXT()
            )
        );
    }
    else
    {
        makeComposite(parentID);

        sBodyPtr.set
        (
            new subBody
            (
                bodyPtr,
                bodies_[parentID].name(),
                parentID,
                XT
            )
        );
    }

    const subBody& sBody = sBodyPtr();
    mergedBodies_.append(sBodyPtr);

    // Merge the sub-body with the parent
    bodies_[sBody.masterID()].merge(sBody);

    const label sBodyID = mergedBodyID(mergedBodies_.size() - 1);
    bodyIDs_.insert(sBody.name(), sBodyID);

    return sBodyID;
}


CML::spatialTransform CML::RBD::rigidBodyModel::X0
(
    const label bodyId
) const
{
    if (merged(bodyId))
    {
        const subBody& mBody = mergedBody(bodyId);
        return mBody.masterXT() & X0_[mBody.masterID()];
    }
    else
    {
        return X0_[bodyId];
    }
}


void CML::RBD::rigidBodyModel::write(Ostream& os) const
{
    os  << indent << "bodies" << nl
        << indent << token::BEGIN_BLOCK << incrIndent << nl;

    // Write the moving bodies
    for (label i=1; i<nBodies(); i++)
    {
        // Do not write joint-bodies created automatically to support elements
        // of composite joints
        if (!isType<jointBody>(bodies_[i]))
        {
            os  << indent << bodies_[i].name() << nl
                << indent << token::BEGIN_BLOCK << incrIndent << endl;

            bodies_[i].write(os);

            os.writeKeyword("parent")
                << bodies_[lambda_[i]].name() << token::END_STATEMENT << nl;

            os.writeKeyword("transform")
                << XT_[i] << token::END_STATEMENT << nl;

            os  << indent << "joint" << nl << joints_[i] << endl;

            os  << decrIndent << indent << token::END_BLOCK << endl;
        }
    }

    // Write the bodies merged into the parent bodies for efficiency
    forAll(mergedBodies_, i)
    {
        os  << indent << mergedBodies_[i].name() << nl
            << indent << token::BEGIN_BLOCK << incrIndent << endl;

        mergedBodies_[i].body().write(os);

        os.writeKeyword("transform")
            << mergedBodies_[i].masterXT() << token::END_STATEMENT << nl;

        os.writeKeyword("mergeWith")
            << mergedBodies_[i].masterName() << token::END_STATEMENT << nl;

        os  << decrIndent << indent << token::END_BLOCK << endl;
    }

    os  << decrIndent << indent << token::END_BLOCK << nl;


    if (!restraints_.empty())
    {
        os  << indent << "restraints" << nl
            << indent << token::BEGIN_BLOCK << incrIndent << nl;

        forAll(restraints_, ri)
        {
            word restraintType = restraints_[ri].type();

            os  << indent << restraints_[ri].name() << nl
                << indent << token::BEGIN_BLOCK << incrIndent << endl;

            restraints_[ri].write(os);

            os  << decrIndent << indent << token::END_BLOCK << endl;
        }

        os  << decrIndent << indent << token::END_BLOCK << nl;
    }
}


bool CML::RBD::rigidBodyModel::read(const dictionary& dict)
{
    restraints_.clear();
    addRestraints(dict);

    return true;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

CML::Ostream& CML::RBD::operator<<(Ostream& os, const rigidBodyModel& rbm)
{
    rbm.write(os);
    return os;
}


// ************************************************************************* //
